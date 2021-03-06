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

#include "RPCTestUnit.h"
#include <ccmapi.h>
#include <ccmautoptr.h>
#include <ServiceManager.h>

#include <stdio.h>

using ccm::test::rpc::CID_CService;
using ccm::test::rpc::IService;
using pisces::ServiceManager;

int main(int argv, char** argc)
{
    AutoPtr<IInterface> obj;
    ServiceManager::GetInstance()->GetService(String("rpcservice"), &obj);
    IService* srv = IService::Probe(obj);
    printf("==== srv: %p ====\n", srv);

    printf("==== call IService::TestMethod1 ====\n");
    srv->TestMethod1(9);
    // printf("==== call IService::TestMethod2 ====\n");
    // svc->TestMethod2(1, 2, 3, 4, 5, 6, 7, 8, 9.9,
    //         10.9, 11.9, 12.9, 13.9, 14.9, 15.9, 16.9, 17.9, 18.9);

    // AutoPtr<IParcel> parcel;
    // CoCreateParcel(&parcel);
    // parcel->WriteInteger(9);
    // parcel->WriteLong(0xffffffffff);
    // parcel->SetDataPosition(0);
    // Integer iValue;
    // parcel->ReadInteger(&iValue);
    // Long lValue;
    // parcel->ReadLong(&lValue);
    // printf("==== iValue: %d, lValue: %llx ====\n", iValue, lValue);

    printf("==== return ====\n");
    return 0;
}
