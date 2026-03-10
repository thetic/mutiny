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
// Failure is a class which holds information for a specific
// test failure. It can be overriden for more complex failure messages
//
///////////////////////////////////////////////////////////////////////////////

#ifndef D_TestFailure_H
#define D_TestFailure_H

#include "CppUTest/String.hpp"

#if CPPUTEST_USE_STD_CPP_LIB
#include <stdexcept>
#endif

#ifdef __cplusplus
#ifndef CPPUTEST_HAVE_EXCEPTIONS
#if ((__cplusplus >= 202002L) && !__cpp_exceptions) ||                         \
    (defined(_MSC_VER) && !_CPPUNWIND) ||                                      \
    (defined(__GNUC__) && !__EXCEPTIONS) ||                                    \
    (defined(__ghs__) && !__EXCEPTION_HANDLING) ||                             \
    (defined(__WATCOMC__) && !_CPPUNWIND)
#define CPPUTEST_HAVE_EXCEPTIONS 0
#else
#define CPPUTEST_HAVE_EXCEPTIONS 1
#endif
#endif
#endif

namespace cpputest {

class TestShell;
class TestOutput;

class TestFailure
{

public:
  TestFailure(TestShell*,
      const char* fileName,
      size_t lineNumber,
      const String& theMessage);
  TestFailure(TestShell*, const String& theMessage);
  TestFailure(TestShell*, const char* fileName, size_t lineNumber);
  TestFailure(const TestFailure&);
  TestFailure(TestFailure&&) noexcept;
  virtual ~TestFailure();

  virtual String getFileName() const;
  virtual String getTestName() const;
  virtual String getTestNameOnly() const;
  virtual size_t getFailureLineNumber() const;
  virtual String getMessage() const;
  virtual String getTestFileName() const;
  virtual size_t getTestLineNumber() const;
  bool isOutsideTestFile() const;
  bool isInHelperFunction() const;

protected:
  String createButWasString(const String& expected, const String& actual);
  String createDifferenceAtPosString(const String& actual,
      size_t offset,
      size_t reportedPosition);
  String createUserText(const String& text);

  String testName_;
  String testNameOnly_;
  String fileName_;
  size_t lineNumber_;
  String testFileName_;
  size_t testLineNumber_;
  String message_;

  TestFailure& operator=(const TestFailure&);
};

class EqualsFailure : public TestFailure
{
public:
  EqualsFailure(TestShell*,
      const char* fileName,
      size_t lineNumber,
      const char* expected,
      const char* actual,
      const String& text);
  EqualsFailure(TestShell*,
      const char* fileName,
      size_t lineNumber,
      const String& expected,
      const String& actual,
      const String& text);
};

class DoublesEqualFailure : public TestFailure
{
public:
  DoublesEqualFailure(TestShell*,
      const char* fileName,
      size_t lineNumber,
      double expected,
      double actual,
      double threshold,
      const String& text);
};

class CheckEqualFailure : public TestFailure
{
public:
  CheckEqualFailure(TestShell* test,
      const char* fileName,
      size_t lineNumber,
      const String& expected,
      const String& actual,
      const String& text);
};

class ComparisonFailure : public TestFailure
{
public:
  ComparisonFailure(TestShell* test,
      const char* fileName,
      size_t lineNumber,
      const String& checkString,
      const String& comparisonString,
      const String& text);
};

class ContainsFailure : public TestFailure
{
public:
  ContainsFailure(TestShell*,
      const char* fileName,
      size_t lineNumber,
      const String& expected,
      const String& actual,
      const String& text);
};

class CheckFailure : public TestFailure
{
public:
  CheckFailure(TestShell* test,
      const char* fileName,
      size_t lineNumber,
      const String& checkString,
      const String& conditionString,
      const String& textString = "");
};

class FailFailure : public TestFailure
{
public:
  FailFailure(TestShell* test,
      const char* fileName,
      size_t lineNumber,
      const String& message);
};

class LongsEqualFailure : public TestFailure
{
public:
  LongsEqualFailure(TestShell* test,
      const char* fileName,
      size_t lineNumber,
      long expected,
      long actual,
      const String& text);
};

class UnsignedLongsEqualFailure : public TestFailure
{
public:
  UnsignedLongsEqualFailure(TestShell* test,
      const char* fileName,
      size_t lineNumber,
      unsigned long expected,
      unsigned long actual,
      const String& text);
};

class LongLongsEqualFailure : public TestFailure
{
public:
  LongLongsEqualFailure(TestShell* test,
      const char* fileName,
      size_t lineNumber,
      long long expected,
      long long actual,
      const String& text);
};

class UnsignedLongLongsEqualFailure : public TestFailure
{
public:
  UnsignedLongLongsEqualFailure(TestShell* test,
      const char* fileName,
      size_t lineNumber,
      unsigned long long expected,
      unsigned long long actual,
      const String& text);
};

class SignedBytesEqualFailure : public TestFailure
{
public:
  SignedBytesEqualFailure(TestShell* test,
      const char* fileName,
      size_t lineNumber,
      signed char expected,
      signed char actual,
      const String& text);
};

class StringEqualFailure : public TestFailure
{
public:
  StringEqualFailure(TestShell* test,
      const char* fileName,
      size_t lineNumber,
      const char* expected,
      const char* actual,
      const String& text);
};

class StringEqualNoCaseFailure : public TestFailure
{
public:
  StringEqualNoCaseFailure(TestShell* test,
      const char* fileName,
      size_t lineNumber,
      const char* expected,
      const char* actual,
      const String& text);
};

class BinaryEqualFailure : public TestFailure
{
public:
  BinaryEqualFailure(TestShell* test,
      const char* fileName,
      size_t lineNumber,
      const unsigned char* expected,
      const unsigned char* actual,
      size_t size,
      const String& text);
};

class FeatureUnsupportedFailure : public TestFailure
{
public:
  FeatureUnsupportedFailure(TestShell* test,
      const char* fileName,
      size_t lineNumber,
      const String& featureName,
      const String& text);
};

#if CPPUTEST_HAVE_EXCEPTIONS
class UnexpectedExceptionFailure : public TestFailure
{
public:
  UnexpectedExceptionFailure(TestShell* test);
#if CPPUTEST_USE_STD_CPP_LIB
  UnexpectedExceptionFailure(TestShell* test, const std::exception& e);
#endif
};
#endif

} // namespace cpputest

#endif
