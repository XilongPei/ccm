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

#ifndef __CCM_IO_BYTEBUFFERASLONGBUFFER_H__
#define __CCM_IO_BYTEBUFFERASLONGBUFFER_H__

#include "ccm/io/ByteBuffer.h"
#include "ccm/io/LongBuffer.h"
#include "ccm.io.IByteOrder.h"

namespace ccm {
namespace io {

class ByteBufferAsLongBuffer
    : public LongBuffer
{
public:
    ECode Constructor(
        /* [in] */ ByteBuffer* bb,
        /* [in] */ Integer mark,
        /* [in] */ Integer pos,
        /* [in] */ Integer lim,
        /* [in] */ Integer cap,
        /* [in] */ Integer off,
        /* [in] */ IByteOrder* order);

    ECode Slice(
        /* [out] */ ILongBuffer** buffer) override;

    ECode Duplicate(
        /* [out] */ ILongBuffer** buffer) override;

    ECode AsReadOnlyBuffer(
        /* [out] */ ILongBuffer** buffer) override;

    ECode Get(
        /* [out] */ Long* l) override;

    ECode Get(
        /* [in] */ Integer index,
        /* [out] */ Long* l) override;

    ECode Get(
        /* [out] */ Array<Long>& dst,
        /* [in] */ Integer offset,
        /* [in] */ Integer length) override;

    ECode Put(
        /* [in] */ Long l) override;

    ECode Put(
        /* [in] */ Integer index,
        /* [in] */ Long l) override;

    ECode Put(
        /* [in] */ const Array<Long>& src,
        /* [in] */ Integer offset,
        /* [in] */ Integer length) override;

    ECode Compact() override;

    ECode IsDirect(
        /* [out] */ Boolean* direct) override;

    ECode IsReadOnly(
        /* [out] */ Boolean* readOnly) override;

    ECode GetOrder(
        /* [out] */ IByteOrder** bo) override;

protected:
    Integer Ix(
        /* [in] */ Integer i);

protected:
    AutoPtr<ByteBuffer> mBB;
    Integer mOffset;

private:
    AutoPtr<IByteOrder> mOrder;
};

inline Integer ByteBufferAsLongBuffer::Ix(
    /* [in] */ Integer i)
{
    return (i << 3) + mOffset;
}

}
}

#endif // __CCM_IO_BYTEBUFFERASLONGBUFFER_H__
