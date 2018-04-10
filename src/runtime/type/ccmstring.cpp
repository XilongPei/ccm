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

//=========================================================================
// Copyright (C) 2012 The Elastos Open Source Project
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

#include "ccmsharedbuffer.h"
#include "ccmtypes.h"
#include "util/ccmlogger.h"

#include <limits.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

namespace ccm {

char* EMPTY_STRING = nullptr;

extern void Init_EMPTY_STRING()
{
    SharedBuffer* buf = SharedBuffer::Alloc(1);
    EMPTY_STRING = (char*)buf->GetData();
    EMPTY_STRING[0] = '\0';
}

extern void Uninit_EMPTY_STRING()
{
    SharedBuffer::GetBufferFromData(EMPTY_STRING)->Release();
    EMPTY_STRING = NULL;
}

static char* GetEmptyString()
{
    SharedBuffer::GetBufferFromData(EMPTY_STRING)->AddRef();
    return EMPTY_STRING;
}

static char* AllocFromUTF8(
    /* [in] */ const char* string, size_t byteSize)
{
    if (byteSize == 0) return GetEmptyString();
    if (byteSize > INT_MAX) {
        Logger::E("String", "Invalid buffer size %zu", byteSize);
        return nullptr;
    }

    SharedBuffer* buf = SharedBuffer::Alloc(byteSize + 1);
    if (buf == nullptr) {
        Logger::E("String", "Malloc string which size is %zu failed.", byteSize);
        return nullptr;
    }

    char* str = (char*)buf->GetData();
    memcpy(str, string, byteSize);
    str[byteSize] = '\0';
    return str;
}

String::String(
    /* [in] */ const char* string)
    : mString(nullptr)
    , mCharCount(0)
{
    if (string != nullptr) {
        mString = AllocFromUTF8(string, strlen(string));
    }
}

String::String(
    /* [in] */ const char* string,
    /* [in] */ Integer byteSize)
    : mString(nullptr)
    , mCharCount(0)
{
    if (string != nullptr && byteSize >= 0 && byteSize <= strlen(string)) {
        mString = AllocFromUTF8(string, byteSize);
    }
}

String::String(
    /* [in] */ const String& other)
{
    mString = other.mString;
    mCharCount = other.mCharCount;
    SharedBuffer::GetBufferFromData(mString)->AddRef();
}

String::String(
    /* [in] */ String&& other)
{
    mString = other.mString;
    mCharCount = other.mCharCount;
    other.mString = nullptr;
    other.mCharCount = 0;
}

String::~String()
{
    if (mString != nullptr) {
        SharedBuffer::GetBufferFromData(mString)->Release();
        mString = nullptr;
        mCharCount = 0;
    }
}

Integer String::GetLength() const
{
    if (IsNullOrEmpty()) return 0;
    if (IsCounted()) return GetCharCount();

    Integer charCount = 0;
    Integer byteSize;
    const char* p = mString;
    const char* end = mString + GetByteLength() + 1;
    while (*p != '\0' && p < end) {
        byteSize = UTF8SequenceLength(*p);
        if (byteSize == 0 || p + byteSize >= end) break;
        p += byteSize;
        charCount++;
    }

    SetCharCount(charCount);
    return charCount;
}

Integer String::GetUTF16Length(
    /* [in] */ Integer start) const
{
    if (IsNullOrEmpty()) return 0;

    Integer utf16Count = 0, charCount = 0;
    Integer byteSize;
    const char* p = mString;
    const char* end = mString + GetByteLength() + 1;
    while (*p != '\0' && p < end) {
        Char unicode = GetCharInternal(p, &byteSize);
        if (byteSize == 0 || p + byteSize >= end) break;
        p += byteSize;
        if (charCount >= start) {
            utf16Count++;
            if (unicode > 0xFFFF) utf16Count++; // this will be a surrogate pair in utf16
        }
        charCount++;
    }
    return utf16Count;
}

Integer String::GetByteLength() const
{
    if (mString == nullptr || mString[0] == '\0') return 0;

    return (Integer)SharedBuffer::GetBufferFromData(mString)->GetSize() - 1;
}

Char String::GetChar(
    /* [in] */ Integer index) const
{
    if (IsNullOrEmpty() || index < 0) return INVALID_CHAR;

    Integer byteSize;
    const char* p = mString;
    const char* end = mString + GetByteLength() + 1;
    while (*p && p < end) {
        Char unicode = GetCharInternal(p, &byteSize);
        if (byteSize == 0 || p + byteSize >= end) break;
        if (index == 0) {
            return unicode;
        }
        p += byteSize;
        index -= 1;
    }

    return INVALID_CHAR;
}

Array<Char> String::GetChars(
    /* [in] */ Integer start) const
{
    Integer charCount = GetLength();
    if (start >= charCount) {
        return Array<Char>();
    }

    Array<Char> charArray(charCount - start);

    Integer byteSize, i = 0;
    const char* p = mString;
    const char* end = mString + GetByteLength() + 1;
    while (*p && p < end) {
        Char unicode = GetCharInternal(p, &byteSize);
        if (byteSize == 0 || p + byteSize >= end) break;
        if (i >= start) {
            charArray[i - start] = unicode;
        }
        p += byteSize;
        i++;
    }

    return charArray;
}

 Array<Short> String::GetUTF16Chars(
    /* [in] */ Integer start) const
{
    Integer utf16Count = GetUTF16Length(start);
    if (start > utf16Count) {
        return Array<Short>();
    }

    Array<Short> utf16Array(utf16Count);

    Integer byteSize, count = 0, i = 0;
    const char* p = mString;
    const char* end = mString + GetByteLength() + 1;
    while (*p && p < end) {
        Char unicode = GetCharInternal(p, &byteSize);
        if (byteSize == 0 || p + byteSize >= end) break;
        if (count >= start) {
            if (unicode <= 0xFFFF) {
                utf16Array[i++] = unicode;
            }
            else {
                // Multiple UTF16 characters with surrogates
                unicode =  unicode - 0x10000;
                utf16Array[i++] = (Short)((unicode >> 10) + 0xD800);
                utf16Array[i++] = (Short)((unicode & 0x3FF) + 0xDC00);
            }
        }
        p += byteSize;
        count++;
    }

    return utf16Array;
}

Integer String::Compare(
    /* [in] */ const char* string) const
{
    if (mString == string) return 0;
    if (mString == nullptr) return -1;
    if (string == nullptr) return 1;

    if (mString[0] == '\0' && string[0] == '\0') {
        return 0;
    }

    return strcmp(mString, string);
}

Integer String::CompareIgnoreCase(
    /* [in] */ const char* string) const
{
    if (mString == string) return 0;
    if (mString == nullptr) return -1;
    if (string == nullptr) return 1;

    if (mString[0] == '\0' && string[0] == '\0') {
        return 0;
    }

    return strcasecmp(mString, string);
}

String String::Substring(
    /* [in] */ Integer charStart,
    /* [in] */ Integer charEnd) const
{
    if (mString == nullptr) return String();
    if (charStart < 0 || charEnd < 0 ||
        charStart > charEnd || charStart >= GetLength()) {
        return String("");
    }

    Integer charCount = 0;
    Integer byteSize;
    const char* p = mString;
    const char* end = mString + GetByteLength() + 1;
    const char* p1 = p;
    const char* p2 = end;
    while (*p != '\0' && p < end) {
        byteSize = UTF8SequenceLength(*p);
        if (byteSize == 0 || p + byteSize >= end) break;
        if (charCount == charStart) {
            p1 = p;
        }
        p += byteSize;
        if (charCount == charEnd) {
            p2 = p;
            break;
        }
        charCount++;
    }

    return String(p1, (p2 - 1) - p1);
}

Integer String::LastIndexOf(
    /* [in] */ const char* string) const
{
    Integer byteIndex = LastByteIndexOfInternal(
            string, GetByteLength() - 1);
    return ToCharIndex(byteIndex);
}

Integer String::ToByteIndex(
    /* [in] */ Integer charIndex) const
{
    if (charIndex < 0 || charIndex >= GetLength()) {
        return -1;
    }

    Integer charCount = 0;
    Integer byteSize;
    const char* p = mString;
    const char* end = mString + GetByteLength() + 1;
    while (*p != '\0' && p < end) {
        byteSize = UTF8SequenceLength(*p);
        if (byteSize == 0 || p + byteSize >= end) break;
        if (charCount == charIndex) {
            break;
        }
        p += byteSize;
        charCount++;
    }

    return p - mString;
}

Integer String::ToCharIndex(
    /* [in] */ Integer byteIndex) const
{
    if (byteIndex < 0 || byteIndex > GetByteLength()) {
        return -1;
    }

    Integer charIndex = 0;
    Integer byteSize;
    const char* p = mString;
    const char* end = mString + GetByteLength() + 1;
    while (*p != '\0' && p < end) {
        byteSize = UTF8SequenceLength(*p);
        if (byteSize == 0 || p + byteSize >= end) break;
        if (byteIndex >= p - mString && byteIndex < p - mString + byteSize) {
            break;
        }
        p += byteSize;
        charIndex++;
    }

    return charIndex;
}

String& String::operator=(
    /* [in] */ const String& other)
{
    if (mString == other.mString) {
        return *this;
    }

    if (other.mString != nullptr) {
        SharedBuffer::GetBufferFromData(other.mString)->AddRef();
    }
    if (mString != nullptr) {
        SharedBuffer::GetBufferFromData(mString)->Release();
    }
    mString = other.mString;
    mCharCount = other.mCharCount;
    return *this;
}

String& String::operator=(
    /* [in] */ String&& other)
{
    SharedBuffer::GetBufferFromData(mString)->Release();
    mString = other.mString;
    mCharCount = other.mCharCount;
    other.mString = nullptr;
    return *this;
}

String& String::operator=(
    /* [in] */ const char* string)
{
    if (mString != nullptr) {
        SharedBuffer::GetBufferFromData(mString)->Release();
    }
    mString = string != nullptr ?
        AllocFromUTF8(string, strlen(string)) : nullptr;
    mCharCount = 0;
    return *this;
}

String& String::operator+=(
    /* [in] */ const String& other)
{
    if (other.IsNullOrEmpty()) {
        return *this;
    }

    Integer origByteSize = GetByteLength();
    Integer newByteSize = origByteSize + other.GetByteLength();
    char* buf = LockBuffer(newByteSize);
    if (buf == nullptr) {
        return *this;
    }

    memcpy(buf + origByteSize, other.string(), other.GetByteLength());
    buf[newByteSize] = '\0';
    UnlockBuffer(newByteSize);
    return *this;
}

String& String::operator+=(
    /* [in] */ const char* string)
{
    if (string == nullptr || string[0] == '\0') {
        return *this;
    }

    Integer origByteSize = GetByteLength();
    Integer stringByteSize = strlen(string);
    Integer newByteSize = origByteSize + stringByteSize;
    char* buf = LockBuffer(newByteSize);
    if (buf == nullptr) {
        return *this;
    }

    memcpy(buf + origByteSize, string, stringByteSize);
    buf[newByteSize] = '\0';
    UnlockBuffer(newByteSize);
    return *this;
}

String String::Format(
    /* [in] */ const char* format ...)
{
    va_list args, args1;

    va_start(args, format);
    va_copy(args1, args);

    int len = vsnprintf(nullptr, 0, format, args);
    va_end(args);

    String str("");
    char* buf = str.LockBuffer(len);
    if (buf == nullptr) {
        Logger::E("String", "Lock %d bytes buffer failed", len);
        return str;
    }
    vsnprintf(buf, len + 1, format, args1);
    str.UnlockBuffer(len);
    va_end(args1);

    return str;
}

Integer String::UTF8SequenceLengthNonASCII(
    /* [in] */ char b0)
{
    if ((b0 & 0xC0) != 0xC0) {
        return 0;
    }
    if ((b0 & 0xE0) == 0xC0) {
        return 2;
    }
    if ((b0 & 0xF0) == 0xE0) {
        return 3;
    }
    if ((b0 & 0xF8) == 0xF0){
        return 4;
    }
    return 0;
}

Integer String::LastByteIndexOfInternal(
    /* [in] */ const char* string,
    /* [in] */ Integer fromByteIndex) const
{
    Integer fromIndex = fromByteIndex;
    Integer sourceLength = GetByteLength();
    Integer targetLength = strlen(string);
    Integer rightIndex = sourceLength - targetLength;
    if (fromIndex > rightIndex) {
        fromIndex = rightIndex;
    }

    Integer strLastIndex = targetLength - 1;
    char strLastChar = string[strLastIndex];
    Integer min = targetLength - 1;
    Integer i = min + fromIndex;

startSearchForLastChar:
    while (true) {
        while (i >= min && mString[i] != strLastChar) {
            i--;
        }
        if (i < min) {
            return -1;
        }
        Integer j = i - 1;
        Integer start = j - (targetLength - 1);
        Integer k = strLastIndex - 1;

        while (j > start) {
            if (mString[j--] != string[k--]) {
                i--;
                goto startSearchForLastChar;
            }
        }
        return start + 1;
    }
}

char* String::LockBuffer(
    /* [in] */ Integer byteSize)
{
    if (byteSize < 0) return nullptr;

    SharedBuffer* buf;
    if (mString != nullptr) {
        buf = SharedBuffer::GetBufferFromData(mString)->EditResize(
                byteSize + 1);
    }
    else {
        buf = SharedBuffer::Alloc(byteSize + 1);
    }
    if (buf == nullptr) return nullptr;

    mString = (char*)buf->GetData();
    return mString;
}

ECode String::UnlockBuffer(
    /* [in] */ Integer byteSize)
{
    if (byteSize < 0) return E_ILLEGAL_ARGUMENT_EXCEPTION;

    if (byteSize != GetByteLength()) {
        SharedBuffer* buf;
        if (mString != nullptr) {
            buf = SharedBuffer::GetBufferFromData(mString)->EditResize(
                    byteSize + 1);
        }
        else {
            buf = SharedBuffer::Alloc(byteSize + 1);
        }
        if (buf == nullptr) {
            Logger::E("String", "Unlock %d bytes buffer failed",
                    byteSize);
            return E_OUT_OF_MEMORY_ERROR;
        }

        mString = (char*)buf->GetData();
        mString[byteSize] = 0;
    }

    return NOERROR;
}

void String::SetCharCount(
    /* [in] */ Integer charCount) const
{
    mCharCount = (0x7FFFFFFF & charCount);
    mCharCount |= (1 << 31);
}

Integer String::GetCharCount() const
{
    return (0x7FFFFFFF & mCharCount);
}

Boolean String::IsCounted() const
{
    return (mCharCount & (1 << 31)) != 0;
}

void String::ClearCounted()
{
    mCharCount = 0;
}

Char String::GetCharInternal(
    /* [in] */ const char* cur,
    /* [in] */ Integer* byteSize)
{
    if (IsASCII(*cur)) {
        *byteSize = 1;
        return *cur;
    }

    const char first_char = *cur++;
    Char result = first_char;
    Char mask, to_ignore_mask;
    Integer num_to_read = 0;
    for (num_to_read = 1, mask = 0x40, to_ignore_mask = 0xFFFFFF80;
         (first_char & mask);
         num_to_read++, to_ignore_mask |= mask, mask >>= 1) {
        // 0x3F == 00111111
        result = (result << 6) + (*cur++ & 0x3F);
    }
    to_ignore_mask |= mask;
    result &= ~(to_ignore_mask << (6 * (num_to_read - 1)));
    *byteSize = num_to_read;
    return result;
}

}
