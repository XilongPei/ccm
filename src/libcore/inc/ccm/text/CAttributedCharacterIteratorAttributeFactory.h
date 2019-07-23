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

#include "ccm.text.IAttributedCharacterIterator.h"
#include "_ccm_text_CAttributedCharacterIteratorAttributeFactory.h"
#include <ccmobject.h>

namespace ccm {
namespace text {

Coclass(CAttributedCharacterIteratorAttributeFactory)
    , public Object
    , public IAttributedCharacterIterator::IAttributeFactory
{
public:
    CCM_INTERFACE_DECL();

    CCM_OBJECT_DECL();

    ECode GetLANGUAGE(
        /* [out] */ IAttributedCharacterIterator::IAttribute** attribute) override;

    ECode GetREADING(
        /* [out] */ IAttributedCharacterIterator::IAttribute** attribute) override;

    ECode GetINPUT_METHOD_SEGMENT(
        /* [out] */ IAttributedCharacterIterator::IAttribute** attribute) override;
};

}
}
