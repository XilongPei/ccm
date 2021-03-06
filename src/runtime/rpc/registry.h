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

#ifndef __CCM_REGISTRY_H__
#define __CCM_REGISTRY_H__

#include "ccmtypes.h"

namespace ccm {

extern ECode RegisterExportObject(
    /* [in] */ RPCType type,
    /* [in] */ IObject* object,
    /* [in] */ IStub* stub);

extern ECode UnregisterExportObject(
    /* [in] */ RPCType type,
    /* [in] */ IObject* object);

extern ECode FindExportObject(
    /* [in] */ RPCType type,
    /* [in] */ IObject* object,
    /* [out] */ IStub** stub);

extern ECode FindExportObject(
    /* [in] */ RPCType type,
    /* [in] */ IInterfacePack* ipack,
    /* [out] */ IStub** stub);

extern ECode RegisterImportObject(
    /* [in] */ RPCType type,
    /* [in] */ IInterfacePack* ipack,
    /* [in] */ IObject* object);

extern ECode UnregisterImportObject(
    /* [in] */ RPCType type,
    /* [in] */ IInterfacePack* ipack);

extern ECode FindImportObject(
    /* [in] */ RPCType type,
    /* [in] */ IInterfacePack* ipack,
    /* [out] */ IObject** object);

}

#endif // __CCM_REGISTRY_H__
