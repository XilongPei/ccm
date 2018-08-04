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

#ifndef __PISCES_SYSTEM_OSCONSTANTS_H__
#define __PISCES_SYSTEM_OSCONSTANTS_H__

#include <ccmtypes.h>

using namespace ccm;

namespace pisces {
namespace system {

class OsConstants final
{
public:
    static const Integer AF_INET_;
    static const Integer AF_INET6_;
    static const Integer EINVAL_;
    static const Integer MS_SYNC_;
    static const Integer O_ACCMODE_;
    static const Integer O_RDONLY_;
    static const Integer SOL_SOCKET_;
    static const Integer SO_DOMAIN_;
    static const Integer SO_LINGER_;

private:
    OsConstants();
};

}
}

#endif // __PISCES_SYSTEM_OSCONSTANTS_H__