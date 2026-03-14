#include "CppUTest/String.hpp"

#include "CppUTest/PlatformSpecificFunctions.h"

#include <limits.h>
#if CPPUTEST_USE_STD_CPP_LIB
#include <string>
#endif

namespace cpputest {

static char*
copyToNewBuffer(const char* bufferToCopy, size_t bufferSize);
static bool
isDigit(char ch);
static bool
isSpace(char ch);
static bool
isUpper(char ch);

size_t
StrLen(const char*);
char*
allocStringBuffer(size_t size, const char* file, size_t line);
void
deallocStringBuffer(char* str, size_t size, const char* file, size_t line);

char*
allocStringBuffer(size_t _size, const char*, size_t)
{
  return new char[_size];
}

void
deallocStringBuffer(char* str, size_t, const char*, size_t)
{
  delete[] str;
}

char*
String::getEmptyString() const
{
  char* buf = allocStringBuffer(1, __FILE__, __LINE__);
  buf[0] = '\0';
  return buf;
}

// does not support + or - prefixes
unsigned
AtoU(const char* str)
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
AtoI(const char* str)
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
StrCmp(const char* s1, const char* s2)
{
  while (*s1 && *s1 == *s2) {
    ++s1;
    ++s2;
  }
  return *reinterpret_cast<const unsigned char*>(s1) -
         *reinterpret_cast<const unsigned char*>(s2);
}

size_t
StrLen(const char* str)
{
  size_t n = static_cast<size_t>(-1);
  do
    n++;
  while (*str++);
  return n;
}

int
StrNCmp(const char* s1, const char* s2, size_t n)
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
StrNCpy(char* s1, const char* s2, size_t n)
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
StrStr(const char* s1, const char* s2)
{
  if (!*s2)
    return s1;
  for (; *s1; s1++)
    if (StrNCmp(s1, s2, StrLen(s2)) == 0)
      return s1;
  return nullptr;
}

char
ToLower(char ch)
{
  return isUpper(ch) ? static_cast<char>(static_cast<int>(ch) + ('a' - 'A'))
                     : ch;
}

int
MemCmp(const void* s1, const void* s2, size_t n)
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
String::reserve(size_t bufferSize)
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
  reserve(otherStringLength * repeatCount + 1);

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
  copyBufferToNewInternalBuffer(other.c_str());
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
stringContains(const String& str, const String& substr)
{
  return StrStr(str.c_str(), substr.c_str()) != nullptr;
}

bool
stringStartsWith(const String& str, const String& prefix)
{
  if (prefix.size() == 0)
    return true;
  else if (str.size() == 0)
    return false;
  else
    return StrStr(str.c_str(), prefix.c_str()) == str.c_str();
}

bool
stringEndsWith(const String& str, const String& suffix)
{
  size_t len = str.size();
  size_t other_len = suffix.size();

  if (other_len == 0)
    return true;
  if (len == 0)
    return false;
  if (len < other_len)
    return false;

  return StrCmp(str.c_str() + len - other_len, suffix.c_str()) == 0;
}

size_t
String::count(const String& substr) const
{
  size_t num = 0;
  const char* str = c_str();
  const char* strpart = nullptr;
  if (*str) {
    strpart = StrStr(str, substr.c_str());
  }
  while (*str && strpart) {
    str = strpart;
    str++;
    num++;
    strpart = StrStr(str, substr.c_str());
  }
  return num;
}

void
String::replace(char to, char with)
{
  size_t s = size();
  for (size_t i = 0; i < s; i++) {
    if (c_str()[i] == to)
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
      if (StrNCmp(&c_str()[i], to, tolen) == 0) {
        StrNCpy(&newbuf[j], with, withlen + 1);
        j += withlen;
        i += tolen;
      } else {
        newbuf[j] = c_str()[i];
        j++;
        i++;
      }
    }
    newbuf[newsize - 1] = '\0';
    setInternalBufferTo(newbuf, newsize);
  } else
    setInternalBufferAsEmptyString();
}

const char*
String::c_str() const
{
  return buffer_;
}

char*
String::data()
{
  return buffer_;
}

size_t
String::size() const
{
  return StrLen(c_str());
}

bool
String::empty() const
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
  return 0 == StrCmp(left.c_str(), right.c_str());
}

bool
operator!=(const String& left, const String& right)
{
  return !(left == right);
}

String
String::operator+(const String& rhs) const
{
  String t(c_str());
  t += rhs.c_str();
  return t;
}

String&
String::operator+=(const String& rhs)
{
  return operator+=(rhs.c_str());
}

String&
String::operator+=(const char* rhs)
{
  size_t originalSize = this->size();
  size_t additionalStringSize = StrLen(rhs) + 1;
  size_t sizeOfNewString = originalSize + additionalStringSize;
  char* tbuffer = copyToNewBuffer(this->c_str(), sizeOfNewString);
  StrNCpy(tbuffer + originalSize, rhs, additionalStringSize);

  setInternalBufferTo(tbuffer, sizeOfNewString);
  return *this;
}

void
padStringsToSameLength(String& str1, String& str2, char padCharacter)
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
String::substr(size_t beginPos, size_t amount) const
{
  if (beginPos > size() - 1)
    return "";

  String newString = c_str() + beginPos;

  if (newString.size() > amount)
    newString.buffer_[amount] = '\0';

  return newString;
}

String
String::substr(size_t beginPos) const
{
  return substr(beginPos, npos);
}

size_t
String::find(char ch) const
{
  return findFrom(0, ch);
}

size_t
String::findFrom(size_t starting_position, char ch) const
{
  size_t len = size();
  for (size_t i = starting_position; i < len; i++)
    if (c_str()[i] == ch)
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
    return substr(beginPos);

  return substr(beginPos, endPos - beginPos);
}

char*
copyToNewBuffer(const char* bufferToCopy, size_t bufferSize)
{
  char* newBuffer = allocStringBuffer(bufferSize, __FILE__, __LINE__);
  StrNCpy(newBuffer, bufferToCopy, bufferSize);
  newBuffer[bufferSize - 1] = '\0';
  return newBuffer;
}

bool
isDigit(char ch)
{
  return '0' <= ch && '9' >= ch;
}

bool
isSpace(char ch)
{
  return (ch == ' ') || (0x08 < ch && 0x0E > ch);
}

bool
isUpper(char ch)
{
  return 'A' <= ch && 'Z' >= ch;
}

bool
isControl(char ch)
{
  return ch < ' ' || ch == char(0x7F);
}

bool
isControlWithShortEscapeSequence(char ch)
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
    result = result.substr(size - (CHAR_BIT / 4));
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
  constexpr size_t sizeOfdefaultBuffer = 100;
  char defaultBuffer[sizeOfdefaultBuffer];
  String resultString;

  size_t size = static_cast<size_t>(PlatformSpecificVSNprintf(
      defaultBuffer, sizeOfdefaultBuffer, format, args));
  if (size < sizeOfdefaultBuffer) {
    resultString = String(defaultBuffer);
  } else {
    size_t newBufferSize = size + 1;
    char* newBuffer = new char[newBufferSize];
    PlatformSpecificVSNprintf(newBuffer, newBufferSize, format, argsCopy);
    resultString = String(newBuffer);

    delete[] newBuffer;
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
  result = result.substr(0, result.size() - 1);

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

} // namespace cpputest
