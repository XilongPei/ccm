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

#ifndef __CCM_UTIL_LOCALE_LOCALEEXTENSIONS_H__
#define __CCM_UTIL_LOCALE_LOCALEEXTENSIONS_H__

#include "ccm/core/SyncObject.h"
#include "ccm.util.ISet.h"
#include <ccmautoptr.h>

using ccm::core::SyncObject;

namespace ccm {
namespace util {
namespace locale {

class LocaleExtensions
    : public SyncObject
{
public:
    static AutoPtr<LocaleExtensions> GetCALENDAR_JAPANESE()
    {
        return nullptr;
    }

    static AutoPtr<LocaleExtensions> GetNUMBER_THAI()
    {
        return nullptr;
    }

    AutoPtr<ISet> GetKeys()
    {
        return nullptr;
    }

    String GetExtensionValue(
        /* [in] */ Char key)
    {
        return String();
    }

    AutoPtr<ISet> GetUnicodeLocaleAttributes()
    {
        return nullptr;
    }

    AutoPtr<ISet> GetUnicodeLocaleKeys()
    {
        return nullptr;
    }

    static Boolean IsValidKey(
        /* [in] */ Char c)
    {
        return false;
    }

    String GetUnicodeLocaleType(
        /* [in] */ const String& unicodeLocaleKey)
    {
        return String();
    }

    String GetID()
    {
        return String();
    }

    ECode GetHashCode(
        /* [out] */ Integer* hash) override;
};

}
}
}

#endif // __CCM_UTIL_LOCALE_LOCALEEXTENSIONS_H__
