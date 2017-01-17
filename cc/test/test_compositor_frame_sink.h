// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_TEST_TEST_COMPOSITOR_FRAME_SINK_H_
#define CC_TEST_TEST_COMPOSITOR_FRAME_SINK_H_

#include "base/memory/weak_ptr.h"
#include "cc/output/compositor_frame_sink.h"
#include "cc/output/renderer_settings.h"
#include "cc/scheduler/begin_frame_source.h"
#include "cc/surfaces/display.h"
#include "cc/surfaces/display_client.h"
#include "cc/surfaces/surface_factory.h"
#include "cc/surfaces/surface_factory_client.h"
#include "cc/surfaces/surface_id_allocator.h"
#include "cc/surfaces/surface_manager.h"

namespace cc {
class CopyOutputRequest;
class OutputSurface;

class TestCompositorFrameSinkClient {
 public:
  virtual ~TestCompositorFrameSinkClient() {}

  virtual void DisplayReceivedCompositorFrame(const CompositorFrame& frame) = 0;
  virtual void DisplayWillDrawAndSwap(bool will_draw_and_swap,
                                      const RenderPassList& render_passes) = 0;
  virtual void DisplayDidDrawAndSwap() = 0;
};

// CompositorFrameSink that owns and forwards frames to a Display.
class TestCompositorFrameSink : public CompositorFrameSink,
                                public SurfaceFactoryClient,
                                public DisplayClient {
 public:
  // Pass true for |force_disable_reclaim_resources| to act like the Display
  // is out-of-process and can't return resources synchronously.
  TestCompositorFrameSink(
      scoped_refptr<ContextProvider> compositor_context_provider,
      scoped_refptr<ContextProvider> worker_context_provider,
      std::unique_ptr<OutputSurface> display_output_surface,
      SharedBitmapManager* shared_bitmap_manager,
      gpu::GpuMemoryBufferManager* gpu_memory_buffer_manager,
      const RendererSettings& renderer_settings,
      base::SingleThreadTaskRunner* task_runner,
      bool synchronous_composite,
      bool force_disable_reclaim_resources);
  ~TestCompositorFrameSink() override;

  void SetClient(TestCompositorFrameSinkClient* client) {
    test_client_ = client;
  }
  void SetEnlargePassTextureAmount(const gfx::Size& s) {
    enlarge_pass_texture_amount_ = s;
  }

  Display* display() const { return display_.get(); }

  // Will be submitted with the next SwapBuffers.
  void RequestCopyOfOutput(std::unique_ptr<CopyOutputRequest> request);

  // CompositorFrameSink implementation.
  bool BindToClient(CompositorFrameSinkClient* client) override;
  void DetachFromClient() override;
  void SwapBuffers(CompositorFrame frame) override;
  void ForceReclaimResources() override;

  // SurfaceFactoryClient implementation.
  void ReturnResources(const ReturnedResourceArray& resources) override;
  void SetBeginFrameSource(BeginFrameSource* begin_frame_source) override;

  // DisplayClient implementation.
  void DisplayOutputSurfaceLost() override;
  void DisplayWillDrawAndSwap(bool will_draw_and_swap,
                              const RenderPassList& render_passes) override;
  void DisplayDidDrawAndSwap() override;

 private:
  void DidDrawCallback(bool synchronous);

  FrameSinkId frame_sink_id_;
  // TODO(danakj): These don't need to be stored in unique_ptrs when
  // CompositorFrameSink is owned/destroyed on the compositor thread.
  std::unique_ptr<SurfaceManager> surface_manager_;
  std::unique_ptr<SurfaceIdAllocator> surface_id_allocator_;
  LocalFrameId delegated_local_frame_id_;

  // Uses surface_manager_.
  std::unique_ptr<SurfaceFactory> surface_factory_;

  // Uses surface_manager_.
  std::unique_ptr<Display> display_;

  const bool display_context_shared_with_compositor_;

  bool bound_ = false;
  TestCompositorFrameSinkClient* test_client_ = nullptr;
  gfx::Size enlarge_pass_texture_amount_;

  std::vector<std::unique_ptr<CopyOutputRequest>> copy_requests_;

  base::WeakPtrFactory<TestCompositorFrameSink> weak_ptrs_;
};

}  // namespace cc

#endif  // CC_TEST_TEST_COMPOSITOR_FRAME_SINK_H_
