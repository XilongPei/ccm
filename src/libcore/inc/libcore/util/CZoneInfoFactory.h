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

#ifndef __LIBCORE_UTIL_CZONEINFOFACTORY_H__
#define __LIBCORE_UTIL_CZONEINFOFACTORY_H__

#include "libcore.io.IBufferIterator.h"
#include "libcore.util.IZoneInfo.h"
#include "libcore.util.IZoneInfoFactory.h"
#include "_libcore_util_CZoneInfoFactory.h"
#include <ccmobject.h>

using libcore::io::IBufferIterator;

namespace libcore {
namespace util {

class CZoneInfoFactory
    : public Object
    , public IZoneInfoFactory
{
public:
    CCM_INTERFACE_DECL();
    CCM_OBJECT_DECL();

    ECode ReadTimeZone(
        /* [in] */ const String& id,
        /* [in] */ IBufferIterator* it,
        /* [in] */ Long currentTimeMillis,
        /* [out] */ IZoneInfo** zoneInfo) override;
};

}
}

#endif // __LIBCORE_UTIL_CZONEINFOFACTORY_H__
