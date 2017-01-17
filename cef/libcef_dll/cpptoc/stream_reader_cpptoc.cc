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

#include "libcef_dll/cpptoc/stream_reader_cpptoc.h"
#include "libcef_dll/ctocpp/read_handler_ctocpp.h"


// GLOBAL FUNCTIONS - Body may be edited by hand.

CEF_EXPORT cef_stream_reader_t* cef_stream_reader_create_for_file(
    const cef_string_t* fileName) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: fileName; type: string_byref_const
  DCHECK(fileName);
  if (!fileName)
    return NULL;

  // Execute
  CefRefPtr<CefStreamReader> _retval = CefStreamReader::CreateForFile(
      CefString(fileName));

  // Return type: refptr_same
  return CefStreamReaderCppToC::Wrap(_retval);
}

CEF_EXPORT cef_stream_reader_t* cef_stream_reader_create_for_data(void* data,
    size_t size) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: data; type: simple_byaddr
  DCHECK(data);
  if (!data)
    return NULL;

  // Execute
  CefRefPtr<CefStreamReader> _retval = CefStreamReader::CreateForData(
      data,
      size);

  // Return type: refptr_same
  return CefStreamReaderCppToC::Wrap(_retval);
}

CEF_EXPORT cef_stream_reader_t* cef_stream_reader_create_for_handler(
    cef_read_handler_t* handler) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: handler; type: refptr_diff
  DCHECK(handler);
  if (!handler)
    return NULL;

  // Execute
  CefRefPtr<CefStreamReader> _retval = CefStreamReader::CreateForHandler(
      CefReadHandlerCToCpp::Wrap(handler));

  // Return type: refptr_same
  return CefStreamReaderCppToC::Wrap(_retval);
}


namespace {

// MEMBER FUNCTIONS - Body may be edited by hand.

size_t CEF_CALLBACK stream_reader_read(struct _cef_stream_reader_t* self,
    void* ptr, size_t size, size_t n) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return 0;
  // Verify param: ptr; type: simple_byaddr
  DCHECK(ptr);
  if (!ptr)
    return 0;

  // Execute
  size_t _retval = CefStreamReaderCppToC::Get(self)->Read(
      ptr,
      size,
      n);

  // Return type: simple
  return _retval;
}

int CEF_CALLBACK stream_reader_seek(struct _cef_stream_reader_t* self,
    int64 offset, int whence) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return 0;

  // Execute
  int _retval = CefStreamReaderCppToC::Get(self)->Seek(
      offset,
      whence);

  // Return type: simple
  return _retval;
}

int64 CEF_CALLBACK stream_reader_tell(struct _cef_stream_reader_t* self) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return 0;

  // Execute
  int64 _retval = CefStreamReaderCppToC::Get(self)->Tell();

  // Return type: simple
  return _retval;
}

int CEF_CALLBACK stream_reader_eof(struct _cef_stream_reader_t* self) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return 0;

  // Execute
  int _retval = CefStreamReaderCppToC::Get(self)->Eof();

  // Return type: simple
  return _retval;
}

int CEF_CALLBACK stream_reader_may_block(struct _cef_stream_reader_t* self) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return 0;

  // Execute
  bool _retval = CefStreamReaderCppToC::Get(self)->MayBlock();

  // Return type: bool
  return _retval;
}

}  // namespace


// CONSTRUCTOR - Do not edit by hand.

CefStreamReaderCppToC::CefStreamReaderCppToC() {
  GetStruct()->read = stream_reader_read;
  GetStruct()->seek = stream_reader_seek;
  GetStruct()->tell = stream_reader_tell;
  GetStruct()->eof = stream_reader_eof;
  GetStruct()->may_block = stream_reader_may_block;
}

template<> CefRefPtr<CefStreamReader> CefCppToC<CefStreamReaderCppToC,
    CefStreamReader, cef_stream_reader_t>::UnwrapDerived(CefWrapperType type,
    cef_stream_reader_t* s) {
  NOTREACHED() << "Unexpected class type: " << type;
  return NULL;
}

#if DCHECK_IS_ON()
template<> base::AtomicRefCount CefCppToC<CefStreamReaderCppToC,
    CefStreamReader, cef_stream_reader_t>::DebugObjCt = 0;
#endif

template<> CefWrapperType CefCppToC<CefStreamReaderCppToC, CefStreamReader,
    cef_stream_reader_t>::kWrapperType = WT_STREAM_READER;
