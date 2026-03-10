/*
 * Copyright (c) 2007, Michael Feathers, James Grenning and Bas Vodde
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "CppUTest/String.h"

#include "CppUTest/PlatformSpecificFunctions.h"
#include "CppUTest/TestMemoryAllocator.h"

#include <limits.h>
#if CPPUTEST_USE_STD_CPP_LIB
#include <string>
#endif

namespace cpputest {

TestMemoryAllocator* String::stringAllocator_ = nullptr;

TestMemoryAllocator*
String::getStringAllocator()
{
  if (stringAllocator_ == nullptr)
    return defaultNewArrayAllocator();
  return stringAllocator_;
}

void
String::setStringAllocator(TestMemoryAllocator* allocator)
{
  stringAllocator_ = allocator;
}

char*
String::allocStringBuffer(size_t _size, const char* file, size_t line)
{
  return getStringAllocator()->alloc_memory(_size, file, line);
}

void
String::deallocStringBuffer(char* str,
    size_t size,
    const char* file,
    size_t line)
{
  getStringAllocator()->free_memory(str, size, file, line);
}

char*
String::getEmptyString() const
{
  char* empty = allocStringBuffer(1, __FILE__, __LINE__);
  empty[0] = '\0';
  return empty;
}

// does not support + or - prefixes
unsigned
String::AtoU(const char* str)
{
  while (isSpace(*str))
    str++;

  unsigned result = 0;
  for (; isDigit(*str) && *str >= '0'; str++) {
    result *= 10;
    result += static_cast<unsigned>(*str - '0');
  }
  return result;
}

int
String::AtoI(const char* str)
{
  while (isSpace(*str))
    str++;

  char first_char = *str;
  if (first_char == '-' || first_char == '+')
    str++;

  int result = 0;
  for (; isDigit(*str); str++) {
    result *= 10;
    result += *str - '0';
  }
  return (first_char == '-') ? -result : result;
}

int
String::StrCmp(const char* s1, const char* s2)
{
  while (*s1 && *s1 == *s2) {
    ++s1;
    ++s2;
  }
  return *reinterpret_cast<const unsigned char*>(s1) -
         *reinterpret_cast<const unsigned char*>(s2);
}

size_t
String::StrLen(const char* str)
{
  size_t n = static_cast<size_t>(-1);
  do
    n++;
  while (*str++);
  return n;
}

int
String::StrNCmp(const char* s1, const char* s2, size_t n)
{
  while (n && *s1 && *s1 == *s2) {
    --n;
    ++s1;
    ++s2;
  }
  return n ? *reinterpret_cast<const unsigned char*>(s1) -
                 *reinterpret_cast<const unsigned char*>(s2)
           : 0;
}

char*
String::StrNCpy(char* s1, const char* s2, size_t n)
{
  char* result = s1;

  if ((nullptr == s1) || (0 == n))
    return result;

  *s1 = *s2;
  while ((--n != 0) && *s1) {
    *++s1 = *++s2;
  }
  return result;
}

const char*
String::StrStr(const char* s1, const char* s2)
{
  if (!*s2)
    return s1;
  for (; *s1; s1++)
    if (StrNCmp(s1, s2, StrLen(s2)) == 0)
      return s1;
  return nullptr;
}

char
String::ToLower(char ch)
{
  return isUpper(ch) ? static_cast<char>(static_cast<int>(ch) + ('a' - 'A'))
                     : ch;
}

int
String::MemCmp(const void* s1, const void* s2, size_t n)
{
  const unsigned char* p1 = static_cast<const unsigned char*>(s1);
  const unsigned char* p2 = static_cast<const unsigned char*>(s2);

  while (n--)
    if (*p1 != *p2) {
      return *p1 - *p2;
    } else {
      ++p1;
      ++p2;
    }
  return 0;
}

void
String::deallocateInternalBuffer()
{
  if (buffer_) {
    deallocStringBuffer(buffer_, bufferSize_, __FILE__, __LINE__);
    buffer_ = nullptr;
    bufferSize_ = 0;
  }
}

void
String::setInternalBufferAsEmptyString()
{
  deallocateInternalBuffer();

  bufferSize_ = 1;
  buffer_ = getEmptyString();
}

void
String::copyBufferToNewInternalBuffer(const char* otherBuffer,
    size_t bufferSize)
{
  deallocateInternalBuffer();

  bufferSize_ = bufferSize;
  buffer_ = copyToNewBuffer(otherBuffer, bufferSize_);
}

void
String::setInternalBufferToNewBuffer(size_t bufferSize)
{
  deallocateInternalBuffer();

  bufferSize_ = bufferSize;
  buffer_ = allocStringBuffer(bufferSize_, __FILE__, __LINE__);
  buffer_[0] = '\0';
}

void
String::setInternalBufferTo(char* buffer, size_t bufferSize)
{
  deallocateInternalBuffer();

  bufferSize_ = bufferSize;
  buffer_ = buffer;
}

void
String::copyBufferToNewInternalBuffer(const String& otherBuffer)
{
  copyBufferToNewInternalBuffer(otherBuffer.buffer_, otherBuffer.size() + 1);
}

void
String::copyBufferToNewInternalBuffer(const char* otherBuffer)
{
  copyBufferToNewInternalBuffer(otherBuffer, StrLen(otherBuffer) + 1);
}

const char*
String::getBuffer() const
{
  return buffer_;
}

String::String(const char* otherBuffer)
  : buffer_(nullptr)
  , bufferSize_(0)
{
  if (otherBuffer == nullptr)
    setInternalBufferAsEmptyString();
  else
    copyBufferToNewInternalBuffer(otherBuffer);
}

String::String(const char* other, size_t repeatCount)
  : buffer_(nullptr)
  , bufferSize_(0)
{
  size_t otherStringLength = StrLen(other);
  setInternalBufferToNewBuffer(otherStringLength * repeatCount + 1);

  char* next = buffer_;
  for (size_t i = 0; i < repeatCount; i++) {
    StrNCpy(next, other, otherStringLength + 1);
    next += otherStringLength;
  }
  *next = 0;
}

String::String(const String& other)
  : buffer_(nullptr)
  , bufferSize_(0)
{
  copyBufferToNewInternalBuffer(other.getBuffer());
}

String::String(String&& other) noexcept
  : buffer_(other.buffer_)
  , bufferSize_(other.bufferSize_)
{
  other.buffer_ = nullptr;
  other.bufferSize_ = 0;
}

String&
String::operator=(String&& other) noexcept
{
  if (this != &other) {
    deallocateInternalBuffer();
    buffer_ = other.buffer_;
    bufferSize_ = other.bufferSize_;
    other.buffer_ = nullptr;
    other.bufferSize_ = 0;
  }
  return *this;
}

String&
String::operator=(const String& other)
{
  if (this != &other)
    copyBufferToNewInternalBuffer(other);
  return *this;
}

bool
String::contains(const String& other) const
{
  return StrStr(getBuffer(), other.getBuffer()) != nullptr;
}

bool
String::containsNoCase(const String& other) const
{
  return lowerCase().contains(other.lowerCase());
}

bool
String::startsWith(const String& other) const
{
  if (other.size() == 0)
    return true;
  else if (size() == 0)
    return false;
  else
    return StrStr(getBuffer(), other.getBuffer()) == getBuffer();
}

bool
String::endsWith(const String& other) const
{
  size_t length = size();
  size_t other_length = other.size();

  if (other_length == 0)
    return true;
  if (length == 0)
    return false;
  if (length < other_length)
    return false;

  return StrCmp(getBuffer() + length - other_length, other.getBuffer()) == 0;
}

size_t
String::count(const String& substr) const
{
  size_t num = 0;
  const char* str = getBuffer();
  const char* strpart = nullptr;
  if (*str) {
    strpart = StrStr(str, substr.getBuffer());
  }
  while (*str && strpart) {
    str = strpart;
    str++;
    num++;
    strpart = StrStr(str, substr.getBuffer());
  }
  return num;
}

void
String::split(const String& delimiter, StringCollection& col) const
{
  size_t num = count(delimiter);
  size_t extraEndToken = (endsWith(delimiter)) ? 0 : 1U;
  col.allocate(num + extraEndToken);

  const char* str = getBuffer();
  const char* prev;
  for (size_t i = 0; i < num; ++i) {
    prev = str;
    str = StrStr(str, delimiter.getBuffer()) + 1;
    col[i] = String(prev).subString(0, size_t(str - prev));
  }
  if (extraEndToken) {
    col[num] = str;
  }
}

void
String::replace(char to, char with)
{
  size_t s = size();
  for (size_t i = 0; i < s; i++) {
    if (getBuffer()[i] == to)
      buffer_[i] = with;
  }
}

void
String::replace(const char* to, const char* with)
{
  size_t c = count(to);
  if (c == 0) {
    return;
  }
  size_t len = size();
  size_t tolen = StrLen(to);
  size_t withlen = StrLen(with);

  size_t newsize = len + (withlen * c) - (tolen * c) + 1;

  if (newsize > 1) {
    char* newbuf = allocStringBuffer(newsize, __FILE__, __LINE__);
    for (size_t i = 0, j = 0; i < len;) {
      if (StrNCmp(&getBuffer()[i], to, tolen) == 0) {
        StrNCpy(&newbuf[j], with, withlen + 1);
        j += withlen;
        i += tolen;
      } else {
        newbuf[j] = getBuffer()[i];
        j++;
        i++;
      }
    }
    newbuf[newsize - 1] = '\0';
    setInternalBufferTo(newbuf, newsize);
  } else
    setInternalBufferAsEmptyString();
}

String
String::printable() const
{
  static const char* shortEscapeCodes[] = {
    "\\a", "\\b", "\\t", "\\n", "\\v", "\\f", "\\r"
  };

  String result;
  result.setInternalBufferToNewBuffer(getPrintableSize() + 1);

  size_t str_size = size();
  size_t j = 0;
  for (size_t i = 0; i < str_size; i++) {
    char c = buffer_[i];
    if (isControlWithShortEscapeSequence(c)) {
      StrNCpy(&result.buffer_[j],
          shortEscapeCodes[static_cast<unsigned char>(c - '\a')],
          2);
      j += 2;
    } else if (isControl(c)) {
      String hexEscapeCode = StringFromFormat("\\x%02X ", c);
      StrNCpy(&result.buffer_[j], hexEscapeCode.asCharString(), 4);
      j += 4;
    } else {
      result.buffer_[j] = c;
      j++;
    }
  }
  result.buffer_[j] = 0;

  return result;
}

size_t
String::getPrintableSize() const
{
  size_t str_size = size();
  size_t printable_str_size = str_size;

  for (size_t i = 0; i < str_size; i++) {
    char c = buffer_[i];
    if (isControlWithShortEscapeSequence(c)) {
      printable_str_size += 1;
    } else if (isControl(c)) {
      printable_str_size += 3;
    }
  }

  return printable_str_size;
}

String
String::lowerCase() const
{
  String str(*this);

  size_t str_size = str.size();
  for (size_t i = 0; i < str_size; i++)
    str.buffer_[i] = ToLower(str.getBuffer()[i]);

  return str;
}

const char*
String::asCharString() const
{
  return getBuffer();
}

size_t
String::size() const
{
  return StrLen(getBuffer());
}

bool
String::isEmpty() const
{
  return size() == 0;
}

String::~String()
{
  deallocateInternalBuffer();
}

bool
operator==(const String& left, const String& right)
{
  return 0 == String::StrCmp(left.asCharString(), right.asCharString());
}

bool
String::equalsNoCase(const String& str) const
{
  return lowerCase() == str.lowerCase();
}

bool
operator!=(const String& left, const String& right)
{
  return !(left == right);
}

String
String::operator+(const String& rhs) const
{
  String t(getBuffer());
  t += rhs.getBuffer();
  return t;
}

String&
String::operator+=(const String& rhs)
{
  return operator+=(rhs.getBuffer());
}

String&
String::operator+=(const char* rhs)
{
  size_t originalSize = this->size();
  size_t additionalStringSize = StrLen(rhs) + 1;
  size_t sizeOfNewString = originalSize + additionalStringSize;
  char* tbuffer = copyToNewBuffer(this->getBuffer(), sizeOfNewString);
  StrNCpy(tbuffer + originalSize, rhs, additionalStringSize);

  setInternalBufferTo(tbuffer, sizeOfNewString);
  return *this;
}

void
String::padStringsToSameLength(String& str1, String& str2, char padCharacter)
{
  if (str1.size() > str2.size()) {
    padStringsToSameLength(str2, str1, padCharacter);
    return;
  }

  char pad[2];
  pad[0] = padCharacter;
  pad[1] = 0;
  str1 = String(pad, str2.size() - str1.size()) + str1;
}

String
String::subString(size_t beginPos, size_t amount) const
{
  if (beginPos > size() - 1)
    return "";

  String newString = getBuffer() + beginPos;

  if (newString.size() > amount)
    newString.buffer_[amount] = '\0';

  return newString;
}

String
String::subString(size_t beginPos) const
{
  return subString(beginPos, npos);
}

char
String::at(size_t pos) const
{
  return getBuffer()[pos];
}

size_t
String::find(char ch) const
{
  return findFrom(0, ch);
}

size_t
String::findFrom(size_t starting_position, char ch) const
{
  size_t length = size();
  for (size_t i = starting_position; i < length; i++)
    if (at(i) == ch)
      return i;
  return npos;
}

String
String::subStringFromTill(char startChar, char lastExcludedChar) const
{
  size_t beginPos = find(startChar);
  if (beginPos == npos)
    return "";

  size_t endPos = findFrom(beginPos, lastExcludedChar);
  if (endPos == npos)
    return subString(beginPos);

  return subString(beginPos, endPos - beginPos);
}

char*
String::copyToNewBuffer(const char* bufferToCopy, size_t bufferSize)
{
  char* newBuffer = allocStringBuffer(bufferSize, __FILE__, __LINE__);
  StrNCpy(newBuffer, bufferToCopy, bufferSize);
  newBuffer[bufferSize - 1] = '\0';
  return newBuffer;
}

bool
String::isDigit(char ch)
{
  return '0' <= ch && '9' >= ch;
}

bool
String::isSpace(char ch)
{
  return (ch == ' ') || (0x08 < ch && 0x0E > ch);
}

bool
String::isUpper(char ch)
{
  return 'A' <= ch && 'Z' >= ch;
}

bool
String::isControl(char ch)
{
  return ch < ' ' || ch == char(0x7F);
}

bool
String::isControlWithShortEscapeSequence(char ch)
{
  return '\a' <= ch && '\r' >= ch;
}

String
StringFrom(bool value)
{
  return String(StringFromFormat("%s", value ? "true" : "false"));
}

String
StringFrom(const char* value)
{
  return String(value);
}

String
StringFromOrNull(const char* expected)
{
  return (expected) ? StringFrom(expected) : StringFrom("(null)");
}

String
PrintableStringFromOrNull(const char* expected)
{
  return (expected) ? StringFrom(expected).printable() : StringFrom("(null)");
}

String
StringFrom(int value)
{
  return StringFromFormat("%d", value);
}

String
StringFrom(long value)
{
  return StringFromFormat("%ld", value);
}

String
StringFrom(const void* value)
{
  return String("0x") + HexStringFrom(value);
}

String
StringFrom(void (*value)())
{
  return String("0x") + HexStringFrom(value);
}

String
HexStringFrom(long value)
{
  return HexStringFrom(static_cast<unsigned long>(value));
}

String
HexStringFrom(int value)
{
  return HexStringFrom(static_cast<unsigned int>(value));
}

String
HexStringFrom(signed char value)
{
  String result = StringFromFormat("%x", value);
  if (value < 0) {
    size_t size = result.size();
    result = result.subString(size - (CHAR_BIT / 4));
  }
  return result;
}

String
HexStringFrom(unsigned long value)
{
  return StringFromFormat("%lx", value);
}

String
HexStringFrom(unsigned int value)
{
  return StringFromFormat("%x", value);
}

String
BracketsFormattedHexStringFrom(int value)
{
  return BracketsFormattedHexString(HexStringFrom(value));
}

String
BracketsFormattedHexStringFrom(unsigned int value)
{
  return BracketsFormattedHexString(HexStringFrom(value));
}

String
BracketsFormattedHexStringFrom(long value)
{
  return BracketsFormattedHexString(HexStringFrom(value));
}

String
BracketsFormattedHexStringFrom(unsigned long value)
{
  return BracketsFormattedHexString(HexStringFrom(value));
}

String
BracketsFormattedHexStringFrom(signed char value)
{
  return BracketsFormattedHexString(HexStringFrom(value));
}

String
BracketsFormattedHexString(String hexString)
{
  return String("(0x") + hexString + ")";
}

#if CPPUTEST_USE_STD_CPP_LIB
String
StringFrom(const std::nullptr_t value)
{
  (void)value;
  return "(null)";
}
#endif

String
StringFrom(long long value)
{
  return StringFromFormat("%lld", value);
}

String
StringFrom(unsigned long long value)
{
  return StringFromFormat("%llu", value);
}

String
HexStringFrom(long long value)
{
  return HexStringFrom(static_cast<unsigned long long>(value));
}

String
HexStringFrom(unsigned long long value)
{
  return StringFromFormat("%llx", value);
}

String
HexStringFrom(const void* value)
{
  return HexStringFrom(reinterpret_cast<unsigned long long>(value));
}

String
HexStringFrom(void (*value)())
{
  return HexStringFrom(reinterpret_cast<unsigned long long>(value));
}

String
BracketsFormattedHexStringFrom(long long value)
{
  return BracketsFormattedHexString(HexStringFrom(value));
}

String
BracketsFormattedHexStringFrom(unsigned long long value)
{
  return BracketsFormattedHexString(HexStringFrom(value));
}

String
StringFrom(double value, int precision)
{
  if (PlatformSpecificIsNan(value))
    return "Nan - Not a number";
  else if (PlatformSpecificIsInf(value))
    return "Inf - Infinity";
  else
    return StringFromFormat("%.*g", precision, value);
}

String
StringFrom(char value)
{
  return StringFromFormat("%c", value);
}

String
StringFrom(const String& value)
{
  return String(value);
}

String
StringFromFormat(const char* format, ...)
{
  String resultString;
  va_list arguments;
  va_start(arguments, format);

  resultString = VStringFromFormat(format, arguments);
  va_end(arguments);
  return resultString;
}

String
StringFrom(unsigned int i)
{
  return StringFromFormat("%u", i);
}

#if CPPUTEST_USE_STD_CPP_LIB

String
StringFrom(const std::string& value)
{
  return String(value.c_str());
}

#endif

String
StringFrom(unsigned long i)
{
  return StringFromFormat("%lu", i);
}

String
VStringFromFormat(const char* format, va_list args)
{
  va_list argsCopy;
  va_copy(argsCopy, args);
  enum
  {
    sizeOfdefaultBuffer = 100
  };
  char defaultBuffer[sizeOfdefaultBuffer];
  String resultString;

  size_t size = static_cast<size_t>(PlatformSpecificVSNprintf(
      defaultBuffer, sizeOfdefaultBuffer, format, args));
  if (size < sizeOfdefaultBuffer) {
    resultString = String(defaultBuffer);
  } else {
    size_t newBufferSize = size + 1;
    char* newBuffer = String::getStringAllocator()->alloc_memory(
        newBufferSize, __FILE__, __LINE__);
    PlatformSpecificVSNprintf(newBuffer, newBufferSize, format, argsCopy);
    resultString = String(newBuffer);

    String::getStringAllocator()->free_memory(
        newBuffer, newBufferSize, __FILE__, __LINE__);
  }
  va_end(argsCopy);
  return resultString;
}

String
StringFromBinary(const unsigned char* value, size_t size)
{
  String result;

  for (size_t i = 0; i < size; i++) {
    result += StringFromFormat("%02X ", value[i]);
  }
  result = result.subString(0, result.size() - 1);

  return result;
}

String
StringFromBinaryOrNull(const unsigned char* value, size_t size)
{
  return (value) ? StringFromBinary(value, size) : StringFrom("(null)");
}

String
StringFromBinaryWithSize(const unsigned char* value, size_t size)
{
  String result = StringFromFormat(
      "Size = %u | HexContents = ", static_cast<unsigned>(size));
  size_t displayedSize = ((size > 128) ? 128 : size);
  result += StringFromBinaryOrNull(value, displayedSize);
  if (size > displayedSize) {
    result += " ...";
  }
  return result;
}

String
StringFromBinaryWithSizeOrNull(const unsigned char* value, size_t size)
{
  return (value) ? StringFromBinaryWithSize(value, size) : StringFrom("(null)");
}

String
StringFromMaskedBits(unsigned long value, unsigned long mask, size_t byteCount)
{
  String result;
  size_t bitCount = (byteCount > sizeof(unsigned long))
                        ? (sizeof(unsigned long) * CHAR_BIT)
                        : (byteCount * CHAR_BIT);
  const unsigned long msbMask =
      (static_cast<unsigned long>(1) << (bitCount - 1));

  for (size_t i = 0; i < bitCount; i++) {
    if (mask & msbMask) {
      result += (value & msbMask) ? "1" : "0";
    } else {
      result += "x";
    }

    if (((i % 8) == 7) && (i != (bitCount - 1))) {
      result += " ";
    }

    value <<= 1;
    mask <<= 1;
  }

  return result;
}

String
StringFromOrdinalNumber(unsigned int number)
{
  const char* suffix = "th";

  if ((number < 11) || (number > 13)) {
    unsigned int const onesDigit = number % 10;
    if (3 == onesDigit) {
      suffix = "rd";
    } else if (2 == onesDigit) {
      suffix = "nd";
    } else if (1 == onesDigit) {
      suffix = "st";
    }
  }

  return StringFromFormat("%u%s", number, suffix);
}

StringCollection::StringCollection()
{
  collection_ = nullptr;
  size_ = 0;
}

void
StringCollection::allocate(size_t _size)
{
  delete[] collection_;

  size_ = _size;
  collection_ = new String[size_];
}

StringCollection::~StringCollection()
{
  delete[] (collection_);
}

size_t
StringCollection::size() const
{
  return size_;
}

String&
StringCollection::operator[](size_t index)
{
  if (index >= size_) {
    empty_ = "";
    return empty_;
  }

  return collection_[index];
}

} // namespace cpputest
