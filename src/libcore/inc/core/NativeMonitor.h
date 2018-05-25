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

#ifndef __CCM_CORE_NATIVEMONITOR_H__
#define __CCM_CORE_NATIVEMONITOR_H__

#include "core/NativeLockWord.h"
#include "core/NativeMutex.h"
#include <ccmtypes.h>

namespace ccm {
namespace core {

typedef uint32_t MonitorId;

class NativeThread;
class NativeObject;

class NativeMonitor
{
public:
    static ECode MonitorEnter(
        /* [in] */ NativeThread* self,
        /* [in] */ NativeObject* obj,
        /* [in] */ Boolean trylock);

    static ECode MonitorExit(
        /* [in] */ NativeThread* self,
        /* [in] */ NativeObject* obj);

    static ECode Notify(
        /* [in] */ NativeThread* self,
        /* [in] */ NativeObject* obj);

    static ECode NotifyAll(
        /* [in] */ NativeThread* self,
        /* [in] */ NativeObject* obj);

    NativeObject* GetObject();

    NativeThread* GetOwner() const;

    MonitorId GetMonitorId() const;

    // Inflate the lock on obj. May fail to inflate for spurious reasons, always re-check.
    static void InflateThinLocked(
        /* [in] */ NativeThread* self,
        /* [in] */ NativeObject* obj,
        /* [in] */ NativeLockWord lockWord);

private:
    NativeMonitor(
        /* [in] */ NativeThread* self,
        /* [in] */ NativeThread* owner,
        /* [in] */ NativeObject* obj,
        /* [in] */ MonitorId id);

    // Install the monitor into its object, may fail if another thread installs a different monitor
    // first.
    Boolean Install(
        /* [in] */ NativeThread* self);

    // Changes the shape of a monitor from thin to fat, preserving the internal lock state. The
    // calling thread must own the lock or the owner must be suspended. There's a race with other
    // threads inflating the lock, installing hash codes and spurious failures. The caller should
    // re-read the lock word following the call.
    static void Inflate(
        /* [in] */ NativeThread* self,
        /* [in] */ NativeThread* owner,
        /* [in] */ NativeObject* obj);

    static ECode FailedUnlock(
        /* [in] */ NativeObject* obj,
        /* [in] */ uint32_t expectedOwnerThreadId,
        /* [in] */ uint32_t foundOwnerThreadId,
        /* [in] */ NativeMonitor* mon);

    // Try to lock without blocking, returns true if we acquired the lock.
    ECode TryLock(
        /* [in] */ NativeThread* self);

    // Variant for already holding the monitor lock.
    ECode TryLockLocked(
        /* [in] */ NativeThread* self);

    ECode Lock(
        /* [in] */ NativeThread* self);

    ECode Unlock(
        /* [in] */ NativeThread* self);

    static ECode DoNotify(
        /* [in] */ NativeThread* self,
        /* [in] */ NativeObject* obj,
        /* [in] */ Boolean notifyAll);

    ECode Notify(
        /* [in] */ NativeThread* self);

    ECode NotifyAll(
        /* [in] */ NativeThread* self);

private:
    friend class NativeMonitorPool;

    NativeMutex mMonitorLock;

    NativeConditionVariable mMonitorContenders;

    // Number of people waiting on the condition.
    size_t mNumWaiters;

    // Which thread currently owns the lock?
    NativeThread* volatile mOwner;

    // Owner's recursive lock depth.
    Integer mLockCount;

    NativeObject* mObj;

    // Threads currently waiting on this monitor.
    NativeThread* mWaitSet;

    // The denser encoded version of this monitor as stored in the lock word.
    MonitorId mMonitorId;

    // Free list for monitor pool.
    NativeMonitor* mNextFree;
};

inline ECode NativeMonitor::Notify(
    /* [in] */ NativeThread* self,
    /* [in] */ NativeObject* obj)
{
    return DoNotify(self, obj, false);
}

inline ECode NativeMonitor::NotifyAll(
    /* [in] */ NativeThread* self,
    /* [in] */ NativeObject* obj)
{
    return DoNotify(self, obj, true);
}

inline NativeObject* NativeMonitor::GetObject()
{
    return mObj;
}

inline NativeThread* NativeMonitor::GetOwner() const
{
    return mOwner;
}

inline MonitorId NativeMonitor::GetMonitorId() const
{
    return mMonitorId;
}

class NativeMonitorList
{
public:
    void Add(
        /* [in] */ NativeMonitor* m);
};

}
}

#endif // __CCM_CORE_NATIVEMONITOR_H__
