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

#include "ccm/core/System.h"
#include "ccm/io/FileSystem.h"

using ccm::core::System;

namespace ccm {
namespace io {

Boolean FileSystem::sUseCanonCaches      = true;
Boolean FileSystem::sUseCanonPrefixCache = true;

static CONS_PROI_5 void StaticInitializeFileSystem()
{
    FileSystem::StaticInitialize();
}

Boolean FileSystem::GetBooleanProperty(
    /* [in] */ const String& prop,
    /* [in] */ Boolean defaultVal)
{
    String val;
    System::GetProperty(prop, &val);
    if (val.IsNull()) return defaultVal;
    if (val.EqualsIgnoreCase("true")) {
        return true;
    }
    else {
        return false;
    }
}

ECode FileSystem::StaticInitialize()
{
    sUseCanonCaches = GetBooleanProperty(String("ccm.io.useCanonCaches"),
            sUseCanonCaches);
    sUseCanonPrefixCache = GetBooleanProperty(String("ccm.io.useCanonPrefixCache"),
            sUseCanonPrefixCache);
    return NOERROR;
}

}
}
