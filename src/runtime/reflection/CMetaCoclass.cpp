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

#include "ccmobjectapi.h"
#include "CMetaCoclass.h"
#include "CMetaComponent.h"
#include "CMetaConstructor.h"

using ccm::metadata::MetaInterface;
using ccm::metadata::MetaMethod;

namespace ccm {

CCM_INTERFACE_IMPL_LIGHT_1(CMetaCoclass, IMetaCoclass);

CMetaCoclass::CMetaCoclass(
    /* [in] */ CMetaComponent* mcObj,
    /* [in] */ MetaComponent* mc,
    /* [in] */ MetaCoclass* mk)
    : mMetadata(mk)
    , mOwner(mcObj)
    , mName(mk->mName)
    , mNamespace(mk->mNamespace)
    , mMetaInterfaces(mk->mInterfaceNumber)
{
    mCid.mUuid = mk->mUuid;
    mCid.mCid = &mcObj->mCid;
}

CMetaCoclass::~CMetaCoclass()
{
    mMetadata = nullptr;
    mOwner = nullptr;
}

ECode CMetaCoclass::GetComponent(
    /* [out] */ IMetaComponent** metaComp)
{
    VALIDATE_NOT_NULL(metaComp);

    *metaComp = (IMetaComponent*)mOwner;
    REFCOUNT_ADD(*metaComp);
    return NOERROR;
}

ECode CMetaCoclass::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    *name = mName;
    return NOERROR;
}

ECode CMetaCoclass::GetNamespace(
    /* [out] */ String* ns)
{
    VALIDATE_NOT_NULL(ns);

    *ns = mNamespace;
    return NOERROR;
}

ECode CMetaCoclass::GetCoclassID(
    /* [out] */ CoclassID* cid)
{
    VALIDATE_NOT_NULL(cid);

    cid->mUuid = mCid.mUuid;
    cid->mCid = mCid.mCid;
    return NOERROR;
}

ECode CMetaCoclass::GetConstructorNumber(
    /* [out] */ Integer* number)
{
    VALIDATE_NOT_NULL(number);

    MetaInterface* mi = mOwner->mMetadata->mInterfaces[
            mMetadata->mInterfaceIndexes[mMetadata->mInterfaceNumber - 1]];
    *number = mi->mMethodNumber;
    return NOERROR;
}

ECode CMetaCoclass::GetAllConstructors(
    /* [out] */ Array<IMetaConstructor*>& constrs)
{
    if (mMetaConstructors.IsEmpty()) {
        MetaInterface* mi = mOwner->mMetadata->mInterfaces[
                mMetadata->mInterfaceIndexes[mMetadata->mInterfaceNumber - 1]];
        mMetaConstructors = Array<IMetaConstructor*>(mi->mMethodNumber);
        for (Integer i = 0; i < mi->mMethodNumber; i++) {
            MetaMethod* mm = mi->mMethods[i];
            IMetaConstructor* mcObj = new CMetaConstructor(
                    this, mm);
            mMetaConstructors.Set(i, mcObj);
        }
    }

    Integer N = MIN(mMetaConstructors.GetLength(), constrs.GetLength());
    for (Integer i = 0; i < N; i++) {
        constrs.Set(i, mMetaConstructors[i]);
    }
    return NOERROR;
}

ECode CMetaCoclass::GetConstructor(
    /* [in] */ const String& signature,
    /* [out] */ IMetaConstructor** constr)
{
    return NOERROR;
}

ECode CMetaCoclass::GetInterfaceNumber(
    /* [out] */ Integer* number)
{
    VALIDATE_NOT_NULL(number);

    *number = mMetadata->mInterfaceNumber - 1;
    return NOERROR;
}

ECode CMetaCoclass::GetAllInterfaces(
    /* [out] */ Array<IMetaInterface*>& intfs)
{
    if (mMetaInterfaces.IsEmpty()) {
        return NOERROR;
    }

    BuildAllInterfaces();

    Integer N = MIN(mMetaInterfaces.GetLength(), intfs.GetLength());
    for (Integer i = 0; i < N; i++) {
        IMetaInterface* miObj = mMetaInterfaces[i];
        intfs.Set(i, miObj);
    }

    return NOERROR;
}

ECode CMetaCoclass::GetInterface(
    /* [in] */ const String& fullName,
    /* [out] */ IMetaInterface** intf)
{
    VALIDATE_NOT_NULL(intf);

    if (mMetaInterfaces.IsEmpty()) {
        *intf = nullptr;
        return NOERROR;
    }

    BuildAllInterfaces();

    for (Integer i = 0; i < mMetaInterfaces.GetLength(); i++) {
        IMetaInterface* miObj = mMetaInterfaces[i];
        String name, ns;
        miObj->GetName(&name);
        miObj->GetNamespace(&ns);
        if (fullName.Equals(ns + name)) {
            *intf = miObj;
            REFCOUNT_ADD(*intf);
            return NOERROR;
        }
    }
    *intf = nullptr;
    return NOERROR;
}

ECode CMetaCoclass::GetMethodNumber(
    /* [out] */ Integer* number)
{
    VALIDATE_NOT_NULL(number);

    if (mMetaMethods.IsEmpty()) {
        Integer num = 4;
        for (Integer i = 0; i < mMetadata->mInterfaceNumber - 1; i++) {
            MetaInterface* mi = mOwner->mMetadata->mInterfaces[
                    mMetadata->mInterfaceIndexes[i]];
            if (String::Format("%s%s", mi->mNamespace,
                    mi->mName).Equals("ccm::IInterface")) continue;
            num += mi->mMethodNumber;
        }
        mMetaMethods = Array<IMetaMethod*>(num);
    }

    *number = mMetaMethods.GetLength();
    return NOERROR;
}

ECode CMetaCoclass::GetMethods(
    /* [out] */ Array<IMetaMethod*>& methods)
{
    BuildAllMethod();

    Integer N = MIN(mMetaMethods.GetLength(), methods.GetLength());
    for (Integer i = 0; i < N; i++) {
        methods.Set(i, mMetaMethods[i]);
    }

    return NOERROR;
}

ECode CMetaCoclass::GetMethod(
    /* [in] */ const String& name,
    /* [in] */ const String& signature,
    /* [out] */ IMetaMethod** method)
{
    VALIDATE_NOT_NULL(method);

    BuildAllMethod();

    for (Integer i = 0; i < mMetaMethods.GetLength(); i++) {
        IMetaMethod* mmObj = mMetaMethods[i];
        String mmName, mmSignature;
        mmObj->GetName(&mmName);
        mmObj->GetSignature(&mmSignature);
        if (mmName.Equals(name) && mmSignature.Equals(signature)) {
            *method = mmObj;
            REFCOUNT_ADD(*method);
            return NOERROR;
        }
    }
    *method = nullptr;
    return NOERROR;
}

ECode CMetaCoclass::CreateObject(
    /* [in] */ const InterfaceID& iid,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);

    return CoCreateObjectInstance(mCid, iid, object);
}

void CMetaCoclass::BuildAllInterfaces()
{
    if (mMetaInterfaces[0] == nullptr) {
        for (Integer i = 0; i < mMetadata->mInterfaceNumber; i++) {
            Integer intfIndex = mMetadata->mInterfaceIndexes[i];
            AutoPtr<IMetaInterface> miObj = mOwner->BuildInterface(intfIndex);
            mMetaInterfaces.Set(i, miObj);
        }
    }
}

void CMetaCoclass::BuildAllMethod()
{
    Integer number;
    GetMethodNumber(&number);

    if (mMetaMethods[0] == nullptr) {
        Integer index = 0;
        BuildInterfaceMethod(mOwner->mIInterface, &index);
        for (Integer i = 0; i < mMetadata->mInterfaceNumber; i++) {
            AutoPtr<IMetaInterface> miObj = mOwner->BuildInterface(i);
            String name, ns;
            miObj->GetName(&name);
            miObj->GetNamespace(&ns);
            if (String("ccm::IInterface").Equals(ns + name)) continue;
            BuildInterfaceMethod(miObj, &index);
        }
    }
}

void CMetaCoclass::BuildInterfaceMethod(
    /* [in] */ IMetaInterface* miObj,
    /* [in, out] */ Integer* index)
{
    Integer N;
    miObj->GetMethodNumber(&N);
    for (Integer i = 4; i < N; i++) {
        AutoPtr<IMetaMethod> mmObj;
        miObj->GetMethod(i, (IMetaMethod**)&mmObj);
        mMetaMethods.Set(*index, mmObj);
        (*index)++;
    }
}

}
