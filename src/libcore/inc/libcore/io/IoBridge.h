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

#ifndef __LIBCORE_IO_IOBRIDGE_H__
#define __LIBCORE_IO_IOBRIDGE_H__

#include "ccm.io.IFileDescriptor.h"

using ccm::io::IFileDescriptor;

namespace libcore {
namespace io {

class IoBridge
{
public:
    static ECode CloseAndSignalBlockedThreads(
        /* [in] */ IFileDescriptor* fd);

    static ECode Read(
        /* [in] */ IFileDescriptor* fd,
        /* [out] */ Array<Byte>& bytes,
        /* [in] */ Integer byteOffset,
        /* [in] */ Integer byteCount,
        /* [out] */ Integer* number);

private:
    IoBridge();
};

}
}

#endif // __LIBCORE_IO_IOBRIDGE_H__