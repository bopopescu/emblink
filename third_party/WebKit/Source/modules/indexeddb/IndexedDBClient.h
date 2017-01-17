/*
 * Copyright (C) 2010 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef IndexedDBClient_h
#define IndexedDBClient_h

#include "core/frame/LocalFrame.h"
#include "core/workers/WorkerClients.h"
#include "modules/ModulesExport.h"
#include "platform/Supplementable.h"
#include "platform/heap/Handle.h"
#include "wtf/text/WTFString.h"

namespace blink {

class ExecutionContext;
class LocalFrame;
class WorkerClients;

class MODULES_EXPORT IndexedDBClient
    : public GarbageCollectedFinalized<IndexedDBClient>,
      public Supplement<LocalFrame>,
      public Supplement<WorkerClients> {
  USING_GARBAGE_COLLECTED_MIXIN(IndexedDBClient);
  WTF_MAKE_NONCOPYABLE(IndexedDBClient);

 public:
  IndexedDBClient();
  virtual ~IndexedDBClient() {}

  DECLARE_VIRTUAL_TRACE();

  virtual bool allowIndexedDB(ExecutionContext*, const String& name) = 0;

  static IndexedDBClient* from(ExecutionContext*);
  static const char* supplementName();
};

MODULES_EXPORT void provideIndexedDBClientTo(LocalFrame&, IndexedDBClient*);

MODULES_EXPORT void provideIndexedDBClientToWorker(WorkerClients*,
                                                   IndexedDBClient*);

}  // namespace blink

#endif  // IndexedDBClient_h
