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

#ifndef __CCM_CCDL_COCLASS_H__
#define __CCM_CCDL_COCLASS_H__

#include "Attribute.h"
#include "Interface.h"
#include "Method.h"
#include "Type.h"
#include "../util/ArrayList.h"

namespace ccm {
namespace ccdl {

class Coclass : public Type
{
public:
    Coclass()
        : mInterfaces(10, false)
    {}

    void SetNamespace(
        /* [in] */ Namespace* ns) override;

    inline Uuid& GetUuid()
    { return mUuid; }

    void SetAttribute(
        /* [in] */ const Attribute& attr);

    bool IsCoclass() override
    { return true; }

    bool AddConstructor(
        /* [in] */ Method* constructor);

    bool AddInterface(
        /* [in] */ Interface* interface);

    inline int GetInterfaceNumber()
    { return mInterfaces.GetSize(); }

    inline Interface* GetInterface(
        /* [in] */ int index)
    { return mInterfaces.Get(index); }

    String Signature() override;

    String Dump(
        /* [in] */ const String& prefix) override;

private:
    Uuid mUuid;
    String mVersion;
    String mDescription;
    ArrayList<Method*> mConstructors;
    ArrayList<Interface*> mInterfaces;
};

}
}

#endif // __CCM_CCDL_COCLASS_H__
