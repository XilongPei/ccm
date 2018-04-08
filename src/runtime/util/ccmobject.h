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

#ifndef __CCM_OBJECT_H__
#define __CCM_OBJECT_H__

#include "ccmtypes.h"

namespace ccm {

class Object
    : public IObject
{
public:
    IInterface* Probe(
        /* [in] */ const InterfaceID& iid) override;

    Integer AddRef(
        /* [in] */ HANDLE id = 0) override;

    Integer Release(
        /* [in] */ HANDLE id = 0) override;

    ECode GetInterfaceID(
        /* [in] */ IInterface* object,
        /* [out] */ InterfaceID* iid) override;

    ECode GetCoclassID(
        /* [out] */ CoclassID* cid) override;
};

}

#endif // __CCM_OBJECT_H__
