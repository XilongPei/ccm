//=========================================================================
// Copyright (C) 2018 The C++ Component Model(COMO) Open Source Project
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

#ifndef __JING_ICU_TEXT_H__
#define __JING_ICU_TEXT_H__

#include "como/core/SyncObject.h"
#include "como.util.ILocale.h"
#include "jing.icu.text.ITimeZoneNames.h"

using como::core::SyncObject;
using como::util::ILocale;

namespace jing {
namespace icu {
namespace text {

class TimeZoneNames
    : public SyncObject
    , public ITimeZoneNames
{
public:
    static AutoPtr<ITimeZoneNames> GetInstance(
        /* [in] */ ILocale* locale)
    {
        return nullptr;
    }
};

}
}
}

#endif // __JING_ICU_TEXT_H__