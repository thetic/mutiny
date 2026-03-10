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

///////////////////////////////////////////////////////////////////////////////
//
// SIMPLESTRING.H
//
// One of the design goals of CppUnitLite is to compilation with very old C++
// compilers.  For that reason, the simple string class that provides
// only the operations needed in CppUnitLite.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef D_String_h
#define D_String_h

#include <stdarg.h>
#include <stddef.h>
#ifdef __cplusplus
#ifndef CPPUTEST_USE_STD_CPP_LIB
#define CPPUTEST_USE_STD_CPP_LIB 1
#endif
#if CPPUTEST_USE_STD_CPP_LIB
#include <cstddef>
#include <string>
#endif
#endif
/* Kludge to get a va_copy in VC++ V6 and in GCC 98 */
#ifndef va_copy
#ifdef __GNUC__
#define va_copy __va_copy
#else
#define va_copy(copy, original) copy = original;
#endif
#endif

namespace cpputest {

class StringCollection;
class TestMemoryAllocator;

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

  char at(size_t pos) const;
  size_t find(char ch) const;
  bool contains(const String& other) const;
  bool containsNoCase(const String& other) const;
  bool startsWith(const String& other) const;
  bool endsWith(const String& other) const;
  void split(const String& split, StringCollection& outCollection) const;
  bool equalsNoCase(const String& str) const;

  void replace(char to, char with);
  void replace(const char* to, const char* with);

  String lowerCase() const;
  String subString(size_t beginPos) const;
  String subString(size_t beginPos, size_t amount) const;
  String subStringFromTill(char startChar, char lastExcludedChar) const;

  String printable() const;

  const char* asCharString() const;
  size_t size() const;
  bool isEmpty() const;

  static void padStringsToSameLength(String& str1, String& str2, char ch);

  static TestMemoryAllocator* getStringAllocator();
  static void setStringAllocator(TestMemoryAllocator* allocator);

  static int AtoI(const char* str);
  static unsigned AtoU(const char* str);
  static int StrCmp(const char* s1, const char* s2);
  static int StrNCmp(const char* s1, const char* s2, size_t n);
  static char ToLower(char ch);
  static int MemCmp(const void* s1, const void* s2, size_t n);

private:
  size_t findFrom(size_t starting_position, char ch) const;
  size_t count(const String& str) const;
  static size_t StrLen(const char*);
  static char* StrNCpy(char* s1, const char* s2, size_t n);
  static const char* StrStr(const char* s1, const char* s2);
  static char* allocStringBuffer(size_t size, const char* file, size_t line);
  static void deallocStringBuffer(char* str,
                                  size_t size,
                                  const char* file,
                                  size_t line);

  const char* getBuffer() const;

  void deallocateInternalBuffer();
  void setInternalBufferAsEmptyString();
  void setInternalBufferToNewBuffer(size_t bufferSize);
  void setInternalBufferTo(char* buffer, size_t bufferSize);
  void copyBufferToNewInternalBuffer(const char* otherBuffer);
  void copyBufferToNewInternalBuffer(const char* otherBuffer,
                                     size_t bufferSize);
  void copyBufferToNewInternalBuffer(const String& otherBuffer);

  char* buffer_;
  size_t bufferSize_;

  static TestMemoryAllocator* stringAllocator_;

  char* getEmptyString() const;
  static char* copyToNewBuffer(const char* bufferToCopy, size_t bufferSize);
  static bool isDigit(char ch);
  static bool isSpace(char ch);
  static bool isUpper(char ch);
  static bool isControl(char ch);
  static bool isControlWithShortEscapeSequence(char ch);

  size_t getPrintableSize() const;
};

class StringCollection
{
public:
  StringCollection();
  ~StringCollection();

  void allocate(size_t size);

  size_t size() const;
  String& operator[](size_t index);

private:
  String* collection_;
  String empty_;
  size_t size_;

  void operator=(StringCollection&);
  StringCollection(StringCollection&);
};

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
#if CPPUTEST_HAS_ATTRIBUTE(format)
#define CPPUTEST_CHECK_FORMAT(type, format_parameter, other_parameters)        \
  __attribute__((format(type, format_parameter, other_parameters)))
#else
#define CPPUTEST_CHECK_FORMAT(                                                 \
  type,                                                                        \
  format_parameter,                                                            \
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
StringFromMaskedBits(unsigned long value, unsigned long mask, size_t byteCount);
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
String
PrintableStringFromOrNull(const char* expected);

#if CPPUTEST_USE_STD_CPP_LIB
String
StringFrom(const std::nullptr_t value);
#endif

#if CPPUTEST_USE_STD_CPP_LIB

String
StringFrom(const std::string& other);

#endif

} // namespace cpputest

#endif
