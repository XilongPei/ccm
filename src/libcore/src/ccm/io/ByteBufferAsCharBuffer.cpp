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

#include "ccm/io/ByteBufferAsCharBuffer.h"
#include "ccm/io/CDirectByteBuffer.h"
#include "ccm.core.IArrayHolder.h"

using ccm::core::E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
using ccm::core::IArrayHolder;

namespace ccm {
namespace io {

ECode ByteBufferAsCharBuffer::Constructor(
    /* [in] */ ByteBuffer* bb,
    /* [in] */ Integer mark,
    /* [in] */ Integer pos,
    /* [in] */ Integer lim,
    /* [in] */ Integer cap,
    /* [in] */ Integer off,
    /* [in] */ IByteOrder* order)
{
    FAIL_RETURN(CharBuffer::Constructor(mark, pos, lim, cap));
    AutoPtr<IByteBuffer> newBB;
    bb->Duplicate(&newBB);
    mBB = (ByteBuffer*)newBB.Get();
    bb->IsReadOnly(&mIsReadOnly);
    if (Object::InstanceOf(bb, CID_CDirectByteBuffer)) {
        mAddress = bb->mAddress + off;
    }
    mBB->SetOrder(order);
    mOrder = order;
    mOffset = off;
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::Slice(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    Integer pos, lim;
    GetPosition(&pos);
    GetLimit(&lim);
    CHECK(pos <= lim);
    Integer rem = (pos <= lim ? lim - pos : 0);
    Integer off = (pos << 2) + mOffset;
    CHECK(off >= 0);
    AutoPtr<ByteBufferAsCharBuffer> bb = new ByteBufferAsCharBuffer();
    FAIL_RETURN(bb->Constructor(mBB, -1, 0, rem, rem, off, mOrder));
    *buffer = (ICharBuffer*)bb.Get();
    REFCOUNT_ADD(*buffer);
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::Duplicate(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    Integer pos, lim, cap;
    GetPosition(&pos);
    GetLimit(&lim);
    GetCapacity(&cap);
    AutoPtr<ByteBufferAsCharBuffer> bb = new ByteBufferAsCharBuffer();
    FAIL_RETURN(bb->Constructor(
            mBB, MarkValue(), pos, lim, cap, mOffset, mOrder));
    *buffer = (ICharBuffer*)bb.Get();
    REFCOUNT_ADD(*buffer);
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::AsReadOnlyBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    AutoPtr<IByteBuffer> rb;
    mBB->AsReadOnlyBuffer(&rb);
    Integer pos, lim, cap;
    GetPosition(&pos);
    GetLimit(&lim);
    GetCapacity(&cap);
    AutoPtr<ByteBufferAsCharBuffer> bb = new ByteBufferAsCharBuffer();
    FAIL_RETURN(bb->Constructor(
            (ByteBuffer*)rb.Get(), MarkValue(), pos, lim, cap, mOffset, mOrder));
    *buffer = (ICharBuffer*)bb.Get();
    REFCOUNT_ADD(*buffer);
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::Get(
    /* [out] */ Char* c)
{
    VALIDATE_NOT_NULL(c);

    Integer index;
    NextGetIndex(&index);
    return Get(index, c);
}

ECode ByteBufferAsCharBuffer::Get(
    /* [in] */ Integer index,
    /* [out] */ Char* c)
{
    VALIDATE_NOT_NULL(c);

    FAIL_RETURN(CheckIndex(index));
    return mBB->GetCharUnchecked(Ix(index), c);
}

ECode ByteBufferAsCharBuffer::Get(
    /* [out] */ Array<Char>& dst,
    /* [in] */ Integer offset,
    /* [in] */ Integer length)
{
    FAIL_RETURN(CheckBounds(offset, length, dst.GetLength()));
    Integer remaining;
    Remaining(&remaining);
    if (length > remaining){
        return E_BUFFER_UNDERFLOW_EXCEPTION;
    }
    Integer pos;
    GetPosition(&pos);
    mBB->GetUnchecked(Ix(pos), dst, offset, length);
    SetPosition(pos + length);
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::GetUnchecked(
    /* [in] */ Integer index,
    /* [out] */ Char* c)
{
    VALIDATE_NOT_NULL(c);

    return mBB->GetCharUnchecked(Ix(index), c);
}

ECode ByteBufferAsCharBuffer::Put(
    /* [in] */ Char c)
{
    Integer index;
    NextPutIndex(&index);
    return Put(index, c);
}

ECode ByteBufferAsCharBuffer::Put(
    /* [in] */ Integer index,
    /* [in] */ Char c)
{
    if (mIsReadOnly) {
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    FAIL_RETURN(CheckIndex(index));
    return mBB->PutCharUnchecked(Ix(index), c);
}

ECode ByteBufferAsCharBuffer::Put(
    /* [in] */ const Array<Char>& src,
    /* [in] */ Integer offset,
    /* [in] */ Integer length)
{
    if (mIsReadOnly) {
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    FAIL_RETURN(CheckBounds(offset, length, src.GetLength()));
    Integer remaining;
    Remaining(&remaining);
    if (length > remaining) {
        return E_BUFFER_OVERFLOW_EXCEPTION;
    }
    Integer pos;
    GetPosition(&pos);
    mBB->PutUnchecked(Ix(pos), src, offset, length);
    SetPosition(pos + length);
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::Compact()
{
    if (mIsReadOnly) {
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    Integer pos, lim, cap;
    GetPosition(&pos);
    GetLimit(&lim);
    CHECK(pos <= lim);
    Integer rem = (pos <= lim ? lim - pos : 0);
    if (!Object::InstanceOf(mBB, CID_CDirectByteBuffer)) {
        AutoPtr<IArrayHolder> holder;
        mBB->GetArray((IInterface**)&holder);
        Array<Byte> bytes;
        holder->GetArray(&bytes);
        bytes.Copy(Ix(0), bytes, Ix(pos), rem << 2);
    }
    else {
        void* dst = reinterpret_cast<void*>(mBB->mAddress + Ix(0));
        void* src = reinterpret_cast<void*>(mBB->mAddress + Ix(pos));
        memmove(dst, src, rem << 2);
    }
    SetPosition(rem);
    GetCapacity(&cap);
    SetLimit(cap);
    DiscardMark();
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::IsDirect(
    /* [out] */ Boolean* direct)
{
    return mBB->IsDirect(direct);
}

ECode ByteBufferAsCharBuffer::IsReadOnly(
    /* [out] */ Boolean* readOnly)
{
    VALIDATE_NOT_NULL(readOnly);

    *readOnly = mIsReadOnly;
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::ToString(
    /* [in] */ Integer start,
    /* [in] */ Integer end,
    /* [out] */ String* desc)
{
    VALIDATE_NOT_NULL(desc);

    Integer lim;
    GetLimit(&lim);
    if ((end > lim) || (start > end)) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Integer len = end - start;
    Array<Char> ca(len);
    AutoPtr<ICharBuffer> cb;
    CharBuffer::Wrap(ca, &cb);
    AutoPtr<ICharBuffer> db;
    Duplicate(&db);
    IBuffer::Probe(db)->SetPosition(start);
    IBuffer::Probe(db)->SetLimit(end);
    cb->Put(db);
    *desc = String(ca);
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::SubSequence(
    /* [in] */ Integer start,
    /* [in] */ Integer end,
    /* [out] */ ICharSequence** subcsq)
{
    VALIDATE_NOT_NULL(subcsq);

    Integer pos, lim;
    GetPosition(&pos);
    GetLimit(&lim);
    CHECK(pos <= lim);
    pos = (pos <= lim ? pos : lim);
    Integer len = lim - pos;

    if ((start < 0) || (end > len) || (start > end)) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Integer cap;
    GetCapacity(&cap);
    AutoPtr<ByteBufferAsCharBuffer> bb = new ByteBufferAsCharBuffer();
    FAIL_RETURN(bb->Constructor(
            mBB, -1, pos + start, pos + end, cap, mOffset, mOrder));
    *subcsq = (ICharSequence*)bb.Get();
    REFCOUNT_ADD(*subcsq);
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::GetOrder(
    /* [out] */ IByteOrder** bo)
{
    VALIDATE_NOT_NULL(bo);

    *bo = mOrder;
    REFCOUNT_ADD(*bo);
    return NOERROR;
}

}
}
