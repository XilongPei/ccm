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

#ifndef __CCM_TEXT_CSIMPLEDATEFORMAT_H__
#define __CCM_TEXT_CSIMPLEDATEFORMAT_H__

#include "ccm/text/SimpleDateFormat.h"
#include "_ccm_text_CSimpleDateFormat.h"

namespace ccm {
namespace text {

Coclass(CSimpleDateFormat)
    , public SimpleDateFormat
{
public:
    CCM_OBJECT_DECL();

    ECode Clone(
        /* [in] */ const InterfaceID& iid,
        /* [out] */ IInterface** obj) override;

    static ECode New(
        /* [in] */ Integer timeStyle,
        /* [in] */ Integer dateStyle,
        /* [in] */ ILocale* loc,
        /* [in] */ const InterfaceID& iid,
        /* [out] */ ccm::IInterface** object);

    using _CSimpleDateFormat::New;
};

}
}

#endif // __CCM_TEXT_CSIMPLEDATEFORMAT_H__
