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

#ifndef __CCM_UTIL_LOCALE_LOCALEOBJECTCACHE_H__
#define __CCM_UTIL_LOCALE_LOCALEOBJECTCACHE_H__

#include <ccmautoptr.h>
#include <ccmobject.h>
#include <ccmtypes.h>

namespace ccm {
namespace util {
namespace locale {

class LocaleObjectCache
    : public Object
{
public:
    ECode Put(
        /* [in] */ IInterface* key,
        /* [in] */ IInterface* value);

    ECode Get(
        /* [in] */ IInterface* key,
        /* [out] */ IInterface** value);

protected:
    virtual AutoPtr<IInterface> CreateObject(
        /* [in] */ IInterface* key) = 0;

    virtual AutoPtr<IInterface> NormalizeKey(
        /* [in] */ IInterface* key);
};

}
}
}

#endif // __CCM_UTIL_LOCALE_LOCALEOBJECTCACHE_H__