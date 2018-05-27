//=========================================================================
// Copyright (C) 2018 The C++ Component Model(CCM) Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=========================================================================

#ifndef __CCM_CORE_NATIVEAPI_H__
#define __CCM_CORE_NATIVEAPI_H__

#include "core/globals.h"
#include <ccmtypes.h>

namespace ccm {
namespace core {

size_t GetStackOverflowReservedBytes(
    /* [in] */ InstructionSet isa);

void NativeSetThreadName(
    /* [in] */ const String& name);

// Returns the calling thread's tid. (The C libraries don't expose this.)
pid_t GetTid();

class BacktraceMap;

// Dumps the native stack for thread 'tid' to 'os'.
void DumpNativeStack(
    /* [in] */ String* os,
    /* [in] */ pid_t tid,
    /* [in] */ BacktraceMap* map = nullptr,
    /* [in] */ const char* prefix = "",
    /* [in] */ void* ucontext = nullptr);

ECode NativeObjectLock(
    /* [in] */ HANDLE handle);

ECode NativeObjectUnlock(
    /* [in] */ HANDLE handle);

ECode NativeObjectNotify(
    /* [in] */ HANDLE handle);

ECode NativeObjectNotifyAll(
    /* [in] */ HANDLE handle);

ECode NativeObjectWait(
    /* [in] */ HANDLE handle);

ECode NativeObjectWait(
    /* [in] */ HANDLE handle,
    /* [in] */ Long ms,
    /* [in] */ Integer ns);

}
}

#endif // __CCM_CORE_NATIVEAPI_H__
