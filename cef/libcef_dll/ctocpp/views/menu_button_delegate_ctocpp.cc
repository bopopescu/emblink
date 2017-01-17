// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//

#include "libcef_dll/cpptoc/views/button_cpptoc.h"
#include "libcef_dll/cpptoc/views/menu_button_cpptoc.h"
#include "libcef_dll/cpptoc/views/view_cpptoc.h"
#include "libcef_dll/ctocpp/views/menu_button_delegate_ctocpp.h"


// VIRTUAL METHODS - Body may be edited by hand.

void CefMenuButtonDelegateCToCpp::OnMenuButtonPressed(
    CefRefPtr<CefMenuButton> menu_button, const CefPoint& screen_point) {
  cef_menu_button_delegate_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, on_menu_button_pressed))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: menu_button; type: refptr_diff
  DCHECK(menu_button.get());
  if (!menu_button.get())
    return;

  // Execute
  _struct->on_menu_button_pressed(_struct,
      CefMenuButtonCppToC::Wrap(menu_button),
      &screen_point);
}

void CefMenuButtonDelegateCToCpp::OnButtonPressed(CefRefPtr<CefButton> button) {
  cef_button_delegate_t* _struct = reinterpret_cast<cef_button_delegate_t*>(
      GetStruct());
  if (CEF_MEMBER_MISSING(_struct, on_button_pressed))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: button; type: refptr_diff
  DCHECK(button.get());
  if (!button.get())
    return;

  // Execute
  _struct->on_button_pressed(_struct,
      CefButtonCppToC::Wrap(button));
}

CefSize CefMenuButtonDelegateCToCpp::GetPreferredSize(CefRefPtr<CefView> view) {
  cef_view_delegate_t* _struct = reinterpret_cast<cef_view_delegate_t*>(
      GetStruct());
  if (CEF_MEMBER_MISSING(_struct, get_preferred_size))
    return CefSize();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: view; type: refptr_diff
  DCHECK(view.get());
  if (!view.get())
    return CefSize();

  // Execute
  cef_size_t _retval = _struct->get_preferred_size(_struct,
      CefViewCppToC::Wrap(view));

  // Return type: simple
  return _retval;
}

CefSize CefMenuButtonDelegateCToCpp::GetMinimumSize(CefRefPtr<CefView> view) {
  cef_view_delegate_t* _struct = reinterpret_cast<cef_view_delegate_t*>(
      GetStruct());
  if (CEF_MEMBER_MISSING(_struct, get_minimum_size))
    return CefSize();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: view; type: refptr_diff
  DCHECK(view.get());
  if (!view.get())
    return CefSize();

  // Execute
  cef_size_t _retval = _struct->get_minimum_size(_struct,
      CefViewCppToC::Wrap(view));

  // Return type: simple
  return _retval;
}

CefSize CefMenuButtonDelegateCToCpp::GetMaximumSize(CefRefPtr<CefView> view) {
  cef_view_delegate_t* _struct = reinterpret_cast<cef_view_delegate_t*>(
      GetStruct());
  if (CEF_MEMBER_MISSING(_struct, get_maximum_size))
    return CefSize();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: view; type: refptr_diff
  DCHECK(view.get());
  if (!view.get())
    return CefSize();

  // Execute
  cef_size_t _retval = _struct->get_maximum_size(_struct,
      CefViewCppToC::Wrap(view));

  // Return type: simple
  return _retval;
}

int CefMenuButtonDelegateCToCpp::GetHeightForWidth(CefRefPtr<CefView> view,
    int width) {
  cef_view_delegate_t* _struct = reinterpret_cast<cef_view_delegate_t*>(
      GetStruct());
  if (CEF_MEMBER_MISSING(_struct, get_height_for_width))
    return 0;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: view; type: refptr_diff
  DCHECK(view.get());
  if (!view.get())
    return 0;

  // Execute
  int _retval = _struct->get_height_for_width(_struct,
      CefViewCppToC::Wrap(view),
      width);

  // Return type: simple
  return _retval;
}

void CefMenuButtonDelegateCToCpp::OnParentViewChanged(CefRefPtr<CefView> view,
    bool added, CefRefPtr<CefView> parent) {
  cef_view_delegate_t* _struct = reinterpret_cast<cef_view_delegate_t*>(
      GetStruct());
  if (CEF_MEMBER_MISSING(_struct, on_parent_view_changed))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: view; type: refptr_diff
  DCHECK(view.get());
  if (!view.get())
    return;
  // Verify param: parent; type: refptr_diff
  DCHECK(parent.get());
  if (!parent.get())
    return;

  // Execute
  _struct->on_parent_view_changed(_struct,
      CefViewCppToC::Wrap(view),
      added,
      CefViewCppToC::Wrap(parent));
}

void CefMenuButtonDelegateCToCpp::OnChildViewChanged(CefRefPtr<CefView> view,
    bool added, CefRefPtr<CefView> child) {
  cef_view_delegate_t* _struct = reinterpret_cast<cef_view_delegate_t*>(
      GetStruct());
  if (CEF_MEMBER_MISSING(_struct, on_child_view_changed))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: view; type: refptr_diff
  DCHECK(view.get());
  if (!view.get())
    return;
  // Verify param: child; type: refptr_diff
  DCHECK(child.get());
  if (!child.get())
    return;

  // Execute
  _struct->on_child_view_changed(_struct,
      CefViewCppToC::Wrap(view),
      added,
      CefViewCppToC::Wrap(child));
}


// CONSTRUCTOR - Do not edit by hand.

CefMenuButtonDelegateCToCpp::CefMenuButtonDelegateCToCpp() {
}

template<> cef_menu_button_delegate_t* CefCToCpp<CefMenuButtonDelegateCToCpp,
    CefMenuButtonDelegate, cef_menu_button_delegate_t>::UnwrapDerived(
    CefWrapperType type, CefMenuButtonDelegate* c) {
  NOTREACHED() << "Unexpected class type: " << type;
  return NULL;
}

#if DCHECK_IS_ON()
template<> base::AtomicRefCount CefCToCpp<CefMenuButtonDelegateCToCpp,
    CefMenuButtonDelegate, cef_menu_button_delegate_t>::DebugObjCt = 0;
#endif

template<> CefWrapperType CefCToCpp<CefMenuButtonDelegateCToCpp,
    CefMenuButtonDelegate, cef_menu_button_delegate_t>::kWrapperType =
    WT_MENU_BUTTON_DELEGATE;
