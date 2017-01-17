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

#include "libcef_dll/ctocpp/get_geolocation_callback_ctocpp.h"


// VIRTUAL METHODS - Body may be edited by hand.

void CefGetGeolocationCallbackCToCpp::OnLocationUpdate(
    const CefGeoposition& position) {
  cef_get_geolocation_callback_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, on_location_update))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->on_location_update(_struct,
      &position);
}


// CONSTRUCTOR - Do not edit by hand.

CefGetGeolocationCallbackCToCpp::CefGetGeolocationCallbackCToCpp() {
}

template<> cef_get_geolocation_callback_t* CefCToCpp<CefGetGeolocationCallbackCToCpp,
    CefGetGeolocationCallback, cef_get_geolocation_callback_t>::UnwrapDerived(
    CefWrapperType type, CefGetGeolocationCallback* c) {
  NOTREACHED() << "Unexpected class type: " << type;
  return NULL;
}

#if DCHECK_IS_ON()
template<> base::AtomicRefCount CefCToCpp<CefGetGeolocationCallbackCToCpp,
    CefGetGeolocationCallback, cef_get_geolocation_callback_t>::DebugObjCt =
    0;
#endif

template<> CefWrapperType CefCToCpp<CefGetGeolocationCallbackCToCpp,
    CefGetGeolocationCallback, cef_get_geolocation_callback_t>::kWrapperType =
    WT_GET_GEOLOCATION_CALLBACK;
