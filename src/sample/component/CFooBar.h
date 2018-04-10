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

#ifndef __CCM_DEMO_CFOOBAR_H__
#define __CCM_DEMO_CFOOBAR_H__

#include <ccmapi.h>
#include <ccmobject.h>
#include "_ccm_demo_CFooBar.h"

namespace ccm {
namespace demo {

Coclass(CFooBar)
    , public Object
    , public IFoo
    , public IBar
{
public:
    CCM_INTERFACE_DECL();

    ECode constructor();

    ECode constructor(
        /* [in] */ Long data);

    ECode Foo(
        /* [in] */ Integer data) override;

    ECode Bar(
        /* [in] */ const String& data) override;
};

}
}

#endif //__CCM_DEMO_CFOOBAR_H__
