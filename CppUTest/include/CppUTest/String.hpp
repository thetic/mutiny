///////////////////////////////////////////////////////////////////////////////
//
// SIMPLESTRING.H
//
// One of the design goals of CppUnitLite is to compilation with very old C++
// compilers.  For that reason, the simple string class that provides
// only the operations needed in CppUnitLite.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_CPPUTEST_STRING_HPP
#define INCLUDED_CPPUTEST_STRING_HPP

#include <stdarg.h>
#include <stddef.h>

#if CPPUTEST_USE_STD_CPP_LIB
#include <string>
#endif

#ifndef __has_attribute
#define CPPUTEST_HAS_ATTRIBUTE(x) 0
#else
#define CPPUTEST_HAS_ATTRIBUTE(x) __has_attribute(x)
#endif

#if defined(__MINGW32__)
#define CPPUTEST_CHECK_FORMAT_TYPE __MINGW_PRINTF_FORMAT
#else
#define CPPUTEST_CHECK_FORMAT_TYPE printf
#endif

namespace cpputest {

class String
{
  friend bool operator==(const String& left, const String& right);
  friend bool operator!=(const String& left, const String& right);

public:
  String(const char* value = "");
  String(const char* value, size_t repeatCount);
  String(const String& other);
  String(String&& other) noexcept;
  ~String();

  String& operator=(const String& other);
  String& operator=(String&& other) noexcept;
  String operator+(const String&) const;
  String& operator+=(const String&);
  String& operator+=(const char*);

  static const size_t npos = static_cast<size_t>(-1);

  char& operator[](size_t pos) { return data()[pos]; }
  const char& operator[](size_t pos) const { return c_str()[pos]; }
  size_t find(char ch) const;

  void replace(char to, char with);
  void replace(const char* to, const char* with);

  String substr(size_t beginPos) const;
  String substr(size_t beginPos, size_t amount) const;
  String subStringFromTill(char startChar, char lastExcludedChar) const;

  const char* c_str() const;
  char* data();
  size_t size() const;
  size_t length() const { return size(); }
  size_t capacity() const { return bufferSize_; }
  bool empty() const;
  void reserve(size_t bufferSize);

private:
  size_t findFrom(size_t starting_position, char ch) const;
  size_t count(const String& str) const;

  void deallocateInternalBuffer();
  void setInternalBufferAsEmptyString();
  void setInternalBufferTo(char* buffer, size_t bufferSize);
  void copyBufferToNewInternalBuffer(const char* otherBuffer);
  void copyBufferToNewInternalBuffer(const char* otherBuffer,
      size_t bufferSize);
  void copyBufferToNewInternalBuffer(const String& otherBuffer);

  char* buffer_;
  size_t bufferSize_;

  char* getEmptyString() const;
};

bool
stringContains(const String& str, const String& substr);
bool
stringStartsWith(const String& str, const String& prefix);
bool
stringEndsWith(const String& str, const String& suffix);

bool
isControl(char ch);
bool
isControlWithShortEscapeSequence(char ch);
char*
StrNCpy(char* s1, const char* s2, size_t n);
const char*
StrStr(const char* s1, const char* s2);

void
padStringsToSameLength(String& str1, String& str2, char ch);
int
AtoI(const char* str);
unsigned
AtoU(const char* str);
int
StrCmp(const char* s1, const char* s2);
int
StrNCmp(const char* s1, const char* s2, size_t n);
char
ToLower(char ch);
int
MemCmp(const void* s1, const void* s2, size_t n);

String
StringFrom(bool value);

String
StringFrom(const void* value);

String
StringFrom(void (*value)());

String
StringFrom(char value);

String
StringFrom(const char* value);

String
StringFromOrNull(const char* value);

String
StringFrom(int value);

String
StringFrom(unsigned int value);

String
StringFrom(long value);

String
StringFrom(unsigned long value);

String
StringFrom(long long value);

String
StringFrom(unsigned long long value);

String
HexStringFrom(unsigned int value);

String
HexStringFrom(int value);

String
HexStringFrom(signed char value);

String
HexStringFrom(long value);

String
HexStringFrom(unsigned long value);

String
HexStringFrom(long long value);

String
HexStringFrom(unsigned long long value);

String
HexStringFrom(const void* value);

String
HexStringFrom(void (*value)());

String
StringFrom(double value, int precision = 6);

String
StringFrom(const String& other);

#if CPPUTEST_HAS_ATTRIBUTE(format)
#define CPPUTEST_CHECK_FORMAT(type, format_parameter, other_parameters)        \
  __attribute__((format(type, format_parameter, other_parameters)))
#else
#define CPPUTEST_CHECK_FORMAT(type, format_parameter,

    other_parameters) /* type, format_parameter, other_parameters */
#endif
String
StringFromFormat(const char* format, ...)
    CPPUTEST_CHECK_FORMAT(CPPUTEST_CHECK_FORMAT_TYPE, 1, 2);

String
VStringFromFormat(const char* format, va_list args);

String
StringFromBinary(const unsigned char* value, size_t size);

String
StringFromBinaryOrNull(const unsigned char* value, size_t size);

String
StringFromBinaryWithSize(const unsigned char* value, size_t size);

String
StringFromBinaryWithSizeOrNull(const unsigned char* value, size_t size);

String
StringFromOrdinalNumber(unsigned int number);

String
BracketsFormattedHexStringFrom(int value);

String
BracketsFormattedHexStringFrom(unsigned int value);

String
BracketsFormattedHexStringFrom(long value);

String
BracketsFormattedHexStringFrom(unsigned long value);

String
BracketsFormattedHexStringFrom(long long value);

String
BracketsFormattedHexStringFrom(unsigned long long value);

String
BracketsFormattedHexStringFrom(signed char value);

String
BracketsFormattedHexString(String hexString);

#if CPPUTEST_USE_STD_CPP_LIB
String
StringFrom(const std::nullptr_t value);

String
StringFrom(const std::string& other);
#endif

} // namespace cpputest

#undef CPPUTEST_HAS_ATTRIBUTE
#undef CPPUTEST_CHECK_FORMAT_TYPE

#endif
