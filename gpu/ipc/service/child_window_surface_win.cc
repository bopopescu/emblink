// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "gpu/ipc/service/child_window_surface_win.h"

#include <memory>

#include "base/compiler_specific.h"
#include "base/memory/ptr_util.h"
#include "base/threading/thread.h"
#include "base/win/scoped_hdc.h"
#include "base/win/wrapped_window_proc.h"
#include "gpu/ipc/common/gpu_messages.h"
#include "gpu/ipc/service/gpu_channel_manager.h"
#include "gpu/ipc/service/gpu_channel_manager_delegate.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/win/hwnd_util.h"
#include "ui/gfx/win/window_impl.h"
#include "ui/gl/egl_util.h"
#include "ui/gl/gl_context.h"
#include "ui/gl/gl_surface_egl.h"
#include "ui/gl/scoped_make_current.h"

namespace gpu {

// This owns the thread and contains data that's shared between the threads.
struct SharedData {
  SharedData() : thread("Window owner thread") {}

  base::Lock rect_lock;
  gfx::Rect rect_to_clear;

  base::Thread thread;
};

namespace {

ATOM g_window_class;

// This runs on the window owner thread.
LRESULT CALLBACK IntermediateWindowProc(HWND window,
                                        UINT message,
                                        WPARAM w_param,
                                        LPARAM l_param) {
  switch (message) {
    case WM_ERASEBKGND:
      // Prevent windows from erasing the background.
      return 1;
    case WM_PAINT:
      PAINTSTRUCT paint;
      if (BeginPaint(window, &paint)) {
        SharedData* shared_data =
            reinterpret_cast<SharedData*>(gfx::GetWindowUserData(window));
        DCHECK(shared_data);
        {
          base::AutoLock lock(shared_data->rect_lock);
          shared_data->rect_to_clear.Union(gfx::Rect(paint.rcPaint));
        }

        EndPaint(window, &paint);
      }
      return 0;
    default:
      return DefWindowProc(window, message, w_param, l_param);
  }
}

// This runs on the window owner thread.
void InitializeWindowClass() {
  if (g_window_class)
    return;

  WNDCLASSEX intermediate_class;
  base::win::InitializeWindowClass(
      L"Intermediate D3D Window",
      &base::win::WrappedWindowProc<IntermediateWindowProc>, CS_OWNDC, 0, 0,
      nullptr, reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)), nullptr,
      nullptr, nullptr, &intermediate_class);
  g_window_class = RegisterClassEx(&intermediate_class);
  if (!g_window_class) {
    LOG(ERROR) << "RegisterClass failed.";
    return;
  }
}

// Hidden popup window  used as a parent for the child surface window.
// Must be created and destroyed on the thread.
class HiddenPopupWindow : public gfx::WindowImpl {
 public:
  static HWND Create() {
    gfx::WindowImpl* window = new HiddenPopupWindow;

    window->set_window_style(WS_POPUP);
    window->set_window_ex_style(WS_EX_TOOLWINDOW);
    window->Init(GetDesktopWindow(), gfx::Rect());
    EnableWindow(window->hwnd(), FALSE);
    // The |window| instance is now owned by the window user data.
    DCHECK_EQ(window, gfx::GetWindowUserData(window->hwnd()));
    return window->hwnd();
  }

  static void Destroy(HWND window) {
    // This uses the fact that the window user data contains a pointer
    // to gfx::WindowImpl instance.
    gfx::WindowImpl* window_data =
        reinterpret_cast<gfx::WindowImpl*>(gfx::GetWindowUserData(window));
    DCHECK_EQ(window, window_data->hwnd());
    DestroyWindow(window);
    delete window_data;
  }

 private:
  // Explicitly do nothing in Close. We do this as some external apps may get a
  // handle to this window and attempt to close it.
  void OnClose() {}

  CR_BEGIN_MSG_MAP_EX(HiddenPopupWindow)
    CR_MSG_WM_CLOSE(OnClose)
  CR_END_MSG_MAP()
};

// This runs on the window owner thread.
void CreateWindowsOnThread(const gfx::Size& size,
                           base::WaitableEvent* event,
                           SharedData* shared_data,
                           HWND* child_window,
                           HWND* parent_window) {
  InitializeWindowClass();
  DCHECK(g_window_class);

  // Create hidden parent window on the current thread.
  *parent_window = HiddenPopupWindow::Create();
  // Create child window.
  HWND window = CreateWindowEx(
      WS_EX_NOPARENTNOTIFY, reinterpret_cast<wchar_t*>(g_window_class), L"",
      WS_CHILDWINDOW | WS_DISABLED | WS_VISIBLE, 0, 0, size.width(),
      size.height(), *parent_window, NULL, NULL, NULL);
  CHECK(window);
  *child_window = window;
  gfx::SetWindowUserData(window, shared_data);
  event->Signal();
}

// This runs on the main thread after the window was destroyed on window owner
// thread.
void DestroySharedData(std::unique_ptr<SharedData> shared_data) {
  shared_data->thread.Stop();
}

// This runs on the window owner thread.
void DestroyWindowsOnThread(HWND child_window, HWND hidden_popup_window) {
  DestroyWindow(child_window);
  HiddenPopupWindow::Destroy(hidden_popup_window);
}

}  // namespace

ChildWindowSurfaceWin::ChildWindowSurfaceWin(GpuChannelManager* manager,
                                             HWND parent_window)
    : gl::NativeViewGLSurfaceEGL(0),
      parent_window_(parent_window),
      manager_(manager),
      alpha_(true),
      first_swap_(true) {
  // Don't use EGL_ANGLE_window_fixed_size so that we can avoid recreating the
  // window surface, which can cause flicker on DirectComposition.
  enable_fixed_size_angle_ = false;
}

EGLConfig ChildWindowSurfaceWin::GetConfig() {
  if (!config_) {
    int alpha_size = alpha_ ? 8 : EGL_DONT_CARE;

    EGLint config_attribs[] = {EGL_ALPHA_SIZE,
                               alpha_size,
                               EGL_BLUE_SIZE,
                               8,
                               EGL_GREEN_SIZE,
                               8,
                               EGL_RED_SIZE,
                               8,
                               EGL_RENDERABLE_TYPE,
                               EGL_OPENGL_ES2_BIT,
                               EGL_SURFACE_TYPE,
                               EGL_WINDOW_BIT | EGL_PBUFFER_BIT,
                               EGL_NONE};

    EGLDisplay display = GetHardwareDisplay();
    EGLint num_configs;
    if (!eglChooseConfig(display, config_attribs, &config_, 1, &num_configs)) {
      LOG(ERROR) << "eglChooseConfig failed with error "
                 << ui::GetLastEGLErrorString();
      return NULL;
    }
  }

  return config_;
}

bool ChildWindowSurfaceWin::InitializeNativeWindow() {
  if (window_)
    return true;

  shared_data_ = base::MakeUnique<SharedData>();

  base::Thread::Options options(base::MessageLoop::TYPE_UI, 0);
  shared_data_->thread.StartWithOptions(options);

  base::WaitableEvent event(base::WaitableEvent::ResetPolicy::AUTOMATIC,
                            base::WaitableEvent::InitialState::NOT_SIGNALED);

  RECT window_rect;
  GetClientRect(parent_window_, &window_rect);

  shared_data_->thread.task_runner()->PostTask(
      FROM_HERE,
      base::Bind(&CreateWindowsOnThread, gfx::Rect(window_rect).size(), &event,
                 shared_data_.get(), &window_, &initial_parent_window_));
  event.Wait();

  manager_->delegate()->SendAcceleratedSurfaceCreatedChildWindow(parent_window_,
                                                                 window_);
  return true;
}

bool ChildWindowSurfaceWin::Resize(const gfx::Size& size,
                                   float scale_factor,
                                   bool has_alpha) {
  if (!SupportsPostSubBuffer()) {
    if (!MoveWindow(window_, 0, 0, size.width(), size.height(), FALSE)) {
      return false;
    }
    alpha_ = has_alpha;
    return gl::NativeViewGLSurfaceEGL::Resize(size, scale_factor, has_alpha);
  } else {
    if (size == GetSize() && has_alpha == alpha_)
      return true;

    // Force a resize and redraw (but not a move, activate, etc.).
    if (!SetWindowPos(window_, nullptr, 0, 0, size.width(), size.height(),
                      SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOCOPYBITS |
                          SWP_NOOWNERZORDER | SWP_NOZORDER)) {
      return false;
    }
    size_ = size;
    if (has_alpha == alpha_) {
      // A 0-size PostSubBuffer doesn't swap but forces the swap chain to resize
      // to match the window.
      PostSubBuffer(0, 0, 0, 0);
    } else {
      alpha_ = has_alpha;
      config_ = nullptr;

      std::unique_ptr<ui::ScopedMakeCurrent> scoped_make_current;
      gl::GLContext* current_context = gl::GLContext::GetCurrent();
      bool was_current = current_context && current_context->IsCurrent(this);
      if (was_current) {
        scoped_make_current.reset(
            new ui::ScopedMakeCurrent(current_context, this));
        current_context->ReleaseCurrent(this);
      }

      Destroy();

      if (!Initialize()) {
        LOG(ERROR) << "Failed to resize window.";
        return false;
      }
    }
    return true;
  }
}

gfx::SwapResult ChildWindowSurfaceWin::SwapBuffers() {
  gfx::SwapResult result = NativeViewGLSurfaceEGL::SwapBuffers();
  // Force the driver to finish drawing before clearing the contents to
  // transparent, to reduce or eliminate the period of time where the contents
  // have flashed black.
  if (first_swap_) {
    glFinish();
    first_swap_ = false;
  }
  ClearInvalidContents();
  return result;
}

gfx::SwapResult ChildWindowSurfaceWin::PostSubBuffer(int x,
                                                     int y,
                                                     int width,
                                                     int height) {
  gfx::SwapResult result =
      NativeViewGLSurfaceEGL::PostSubBuffer(x, y, width, height);
  ClearInvalidContents();
  return result;
}

void ChildWindowSurfaceWin::ClearInvalidContents() {
  base::AutoLock lock(shared_data_->rect_lock);
  if (!shared_data_->rect_to_clear.IsEmpty()) {
    base::win::ScopedGetDC dc(window_);

    RECT rect = shared_data_->rect_to_clear.ToRECT();

    // DirectComposition composites with the contents under the SwapChain,
    // so ensure that's cleared. GDI treats black as transparent.
    FillRect(dc, &rect, reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
    shared_data_->rect_to_clear = gfx::Rect();
  }
}

ChildWindowSurfaceWin::~ChildWindowSurfaceWin() {
  if (shared_data_) {
    scoped_refptr<base::TaskRunner> task_runner =
        shared_data_->thread.task_runner();
    task_runner->PostTaskAndReply(
        FROM_HERE,
        base::Bind(&DestroyWindowsOnThread, window_, initial_parent_window_),
        base::Bind(&DestroySharedData, base::Passed(std::move(shared_data_))));
  }
}

}  // namespace gpu
