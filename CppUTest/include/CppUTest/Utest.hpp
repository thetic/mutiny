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

// This file contains the Test class along with the macros which make effective
// in the harness.

#ifndef D_UTest_h
#define D_UTest_h

#include "CppUTest/String.hpp"
#include "CppUTest/TestTerminator.hpp"

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

#ifndef CPPUTEST_HAVE_RTTI
#if ((__cplusplus >= 202002L) && !__cpp_rtti) ||                               \
    (defined(_MSC_VER) && !_CPPRTTI) || (defined(__GNUC__) && !__GXX_RTTI) ||  \
    (defined(__ghs__) && !__RTTI) || (defined(__WATCOMC__) && !_CPPRTTI)
#define CPPUTEST_HAVE_RTTI 0
#else
#define CPPUTEST_HAVE_RTTI 1
#endif
#endif
#endif

namespace cpputest {

class TestResult;
class TestPlugin;
class TestFailure;
class TestFilter;

bool
doubles_equal(double d1, double d2, double threshold);

//////////////////// Test

class TestShell;

class Test
{
public:
  Test();
  virtual ~Test();
  virtual void run();

  virtual void setup();
  virtual void teardown();
  virtual void testBody();
};

//////////////////// TestShell

class TestShell
{
public:
  static TestShell* getCurrent();

  static const TestTerminator& getCurrentTestTerminator();
  static const TestTerminator& getCurrentTestTerminatorWithoutExceptions();

  static void setCrashOnFail();
  static void restoreDefaultTestTerminator();

  static void setRethrowExceptions(bool rethrowExceptions);
  static bool isRethrowingExceptions();

public:
  TestShell(const char* groupName,
      const char* testName,
      const char* fileName,
      size_t lineNumber);
  virtual ~TestShell();

  virtual TestShell* addTest(TestShell* test);
  virtual TestShell* getNext() const;
  virtual size_t countTests();

  bool shouldRun(const TestFilter* groupFilters,
      const TestFilter* nameFilters) const;
  const String getName() const;
  const String getGroup() const;
  virtual String getFormattedName() const;
  const String getFile() const;
  size_t getLineNumber() const;
  virtual bool willRun() const;
  virtual bool hasFailed() const;
  void countCheck();

  virtual void assertTrue(bool condition,
      const char* checkString,
      const char* conditionString,
      const char* text,
      const char* fileName,
      size_t lineNumber,
      const TestTerminator& testTerminator = getCurrentTestTerminator());
  virtual void assertCstrEqual(const char* expected,
      const char* actual,
      const char* text,
      const char* fileName,
      size_t lineNumber,
      const TestTerminator& testTerminator = getCurrentTestTerminator());
  virtual void assertCstrNEqual(const char* expected,
      const char* actual,
      size_t length,
      const char* text,
      const char* fileName,
      size_t lineNumber,
      const TestTerminator& testTerminator = getCurrentTestTerminator());
  virtual void assertCstrNoCaseEqual(const char* expected,
      const char* actual,
      const char* text,
      const char* fileName,
      size_t lineNumber);
  virtual void assertCstrContains(const char* expected,
      const char* actual,
      const char* text,
      const char* fileName,
      size_t lineNumber);
  virtual void assertCstrNoCaseContains(const char* expected,
      const char* actual,
      const char* text,
      const char* fileName,
      size_t lineNumber);
  virtual void assertLongsEqual(long expected,
      long actual,
      const char* text,
      const char* fileName,
      size_t lineNumber,
      const TestTerminator& testTerminator = getCurrentTestTerminator());
  virtual void assertUnsignedLongsEqual(unsigned long expected,
      unsigned long actual,
      const char* text,
      const char* fileName,
      size_t lineNumber,
      const TestTerminator& testTerminator = getCurrentTestTerminator());
  virtual void assertLongLongsEqual(long long expected,
      long long actual,
      const char* text,
      const char* fileName,
      size_t lineNumber,
      const TestTerminator& testTerminator = getCurrentTestTerminator());
  virtual void assertUnsignedLongLongsEqual(unsigned long long expected,
      unsigned long long actual,
      const char* text,
      const char* fileName,
      size_t lineNumber,
      const TestTerminator& testTerminator = getCurrentTestTerminator());
  virtual void assertSignedBytesEqual(signed char expected,
      signed char actual,
      const char* text,
      const char* fileName,
      size_t lineNumber,
      const TestTerminator& testTerminator = getCurrentTestTerminator());
  virtual void assertPointersEqual(const void* expected,
      const void* actual,
      const char* text,
      const char* fileName,
      size_t lineNumber,
      const TestTerminator& testTerminator = getCurrentTestTerminator());
  virtual void assertFunctionPointersEqual(void (*expected)(),
      void (*actual)(),
      const char* text,
      const char* fileName,
      size_t lineNumber,
      const TestTerminator& testTerminator = getCurrentTestTerminator());
  virtual void assertDoublesEqual(double expected,
      double actual,
      double threshold,
      const char* text,
      const char* fileName,
      size_t lineNumber,
      const TestTerminator& testTerminator = getCurrentTestTerminator());
  virtual void assertEquals(bool failed,
      const char* expected,
      const char* actual,
      const char* text,
      const char* file,
      size_t lineNumber,
      const TestTerminator& testTerminator = getCurrentTestTerminator());
  virtual void assertEquals(bool failed,
      String expected,
      String actual,
      const char* text,
      const char* file,
      size_t lineNumber,
      const TestTerminator& testTerminator = getCurrentTestTerminator());
  virtual void assertBinaryEqual(const void* expected,
      const void* actual,
      size_t length,
      const char* text,
      const char* fileName,
      size_t lineNumber,
      const TestTerminator& testTerminator = getCurrentTestTerminator());
  virtual void assertCompare(bool comparison,
      const char* checkString,
      const char* comparisonString,
      const char* text,
      const char* fileName,
      size_t lineNumber,
      const TestTerminator& testTerminator = getCurrentTestTerminator());
  virtual void fail(const char* text,
      const char* fileName,
      size_t lineNumber,
      const TestTerminator& testTerminator = getCurrentTestTerminator());
  virtual void exitTest(
      const TestTerminator& testTerminator = getCurrentTestTerminator());

  virtual void print(const char* text, const char* fileName, size_t lineNumber);
  virtual void print(const String& text,
      const char* fileName,
      size_t lineNumber);
  virtual void printVeryVerbose(const char* text);

  void setFileName(const char* fileName);
  void setLineNumber(size_t lineNumber);
  void setGroupName(const char* groupName);
  void setTestName(const char* testName);

  static void crash();
  static void setCrashMethod(void (*crashme)());
  static void resetCrashMethod();

  virtual void setRunIgnored();

  virtual Test* createTest();
  virtual void destroyTest(Test* test);

  virtual void runOneTest(TestPlugin* plugin, TestResult& result);
  virtual void runOneTestInCurrentProcess(TestPlugin* plugin,
      TestResult& result);

  virtual void failWith(const TestFailure& failure);
  virtual void failWith(const TestFailure& failure,
      const TestTerminator& terminator);

  virtual void addFailure(const TestFailure& failure);

protected:
  TestShell();
  TestShell(const char* groupName,
      const char* testName,
      const char* fileName,
      size_t lineNumber,
      TestShell* nextTest);

  virtual String getMacroName() const;
  TestResult* getTestResult();

private:
  const char* group_;
  const char* name_;
  const char* file_;
  size_t lineNumber_;
  TestShell* next_;
  bool hasFailed_;

  void setTestResult(TestResult* result);
  void setCurrentTest(TestShell* test);
  bool match(const char* target, const TestFilter* filters) const;

  static TestShell* currentTest_;
  static TestResult* testResult_;

  static const TestTerminator* currentTestTerminator_;
  static const TestTerminator* currentTestTerminatorWithoutExceptions_;
  static bool rethrowExceptions_;
};

//////////////////// FailedException

class FailedException
{
public:
  int dummy_;
};

//////////////////// IgnoredTestShell

class IgnoredTestShell : public TestShell
{
public:
  IgnoredTestShell();
  virtual ~IgnoredTestShell() override;
  explicit IgnoredTestShell(const char* groupName,
      const char* testName,
      const char* fileName,
      size_t lineNumber);
  virtual bool willRun() const override;
  virtual void setRunIgnored() override;

protected:
  virtual String getMacroName() const override;
  virtual void runOneTest(TestPlugin* plugin, TestResult& result) override;

private:
  IgnoredTestShell(const IgnoredTestShell&);
  IgnoredTestShell& operator=(const IgnoredTestShell&);

  bool runIgnored_;
};

//////////////////// TestShellPointerArray

class TestShellPointerArray
{
public:
  TestShellPointerArray(TestShell* firstTest);
  ~TestShellPointerArray();

  void shuffle(size_t seed);
  void reverse();
  void relinkTestsInOrder();
  TestShell* getFirstTest() const;
  TestShell* get(size_t index) const;

private:
  void swap(size_t index1, size_t index2);

  TestShell** arrayOfTests_;
  size_t count_;
};

} // namespace cpputest

#endif
