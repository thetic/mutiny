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

#include "CppUTest/TestShell.hpp"

#include "CppUTest/ConsoleTestOutput.hpp"
#include "CppUTest/PlatformSpecificFunctions.hpp"
#include "CppUTest/Test.hpp"
#include "CppUTest/TestFailure.hpp"
#include "CppUTest/TestPlugin.hpp"
#include "CppUTest/TestRegistry.hpp"
#include "CppUTest/TestResult.hpp"

namespace cpputest {

bool
doubles_equal(double d1, double d2, double threshold)
{
  if (PlatformSpecificIsNan(d1) || PlatformSpecificIsNan(d2) ||
      PlatformSpecificIsNan(threshold))
    return false;

  if (PlatformSpecificIsInf(d1) && PlatformSpecificIsInf(d2)) {
    return true;
  }

  return PlatformSpecificFabs(d1 - d2) <= threshold;
}

/* Sometimes stubs use the CppUTest assertions.
 * Its not correct to do so, but this small helper class will prevent a
 * segmentation fault and instead will give an error message and also the
 * file/line of the check that was executed outside the tests.
 */
class OutsideTestRunnerUTest : public TestShell
{
public:
  static OutsideTestRunnerUTest& instance();
  virtual TestResult& getTestResult() { return defaultTestResult; }
  virtual ~OutsideTestRunnerUTest() override {}

private:
  OutsideTestRunnerUTest()
    : TestShell("\n\t NOTE: Assertion happened without being in a test run "
                "(perhaps in "
                "main?)",
          "\n\t       Something is very wrong. Check this assertion and fix",
          "unknown file",
          0)
    , defaultTestResult(defaultOutput)
  {
  }
  ConsoleTestOutput defaultOutput;
  TestResult defaultTestResult;
};

OutsideTestRunnerUTest&
OutsideTestRunnerUTest::instance()
{
  static OutsideTestRunnerUTest instance_;
  return instance_;
}

/*
 * Below helpers are used for the PlatformSpecificSetJmp and LongJmp. They pass
 * a method for what needs to happen after the jump, so that the stack stays
 * right.
 *
 */

struct HelperTestRunInfo
{
  HelperTestRunInfo(TestShell* shell, TestPlugin* plugin, TestResult* result)
    : shell_(shell)
    , plugin_(plugin)
    , result_(result)
  {
  }

  TestShell* shell_;
  TestPlugin* plugin_;
  TestResult* result_;
};

static void
helperDoRunOneTestInCurrentProcess(void* data)
{
  HelperTestRunInfo* runInfo = static_cast<HelperTestRunInfo*>(data);

  TestShell* shell = runInfo->shell_;
  TestPlugin* plugin = runInfo->plugin_;
  TestResult* result = runInfo->result_;

  shell->runOneTestInCurrentProcess(plugin, *result);
}

/******************************** */

static const NormalTestTerminator normalTestTerminator = NormalTestTerminator();
static const CrashingTestTerminator crashingTestTerminator =
    CrashingTestTerminator();
static const TestTerminatorWithoutExceptions
    normalTestTerminatorWithoutExceptions = TestTerminatorWithoutExceptions();
static const CrashingTestTerminatorWithoutExceptions
    crashingTestTerminatorWithoutExceptions =
        CrashingTestTerminatorWithoutExceptions();

const TestTerminator* TestShell::currentTestTerminator_ = &normalTestTerminator;
const TestTerminator* TestShell::currentTestTerminatorWithoutExceptions_ =
    &normalTestTerminatorWithoutExceptions;

bool TestShell::rethrowExceptions_ = false;

/******************************** */

TestShell::TestShell()
  : group_("UndefinedTestGroup")
  , name_("UndefinedTest")
  , file_("UndefinedFile")
  , lineNumber_(0)
  , next_(nullptr)
  , hasFailed_(false)
{
}

TestShell::TestShell(const char* groupName,
    const char* testName,
    const char* fileName,
    size_t lineNumber)
  : group_(groupName)
  , name_(testName)
  , file_(fileName)
  , lineNumber_(lineNumber)
  , next_(nullptr)
  , hasFailed_(false)
{
}

TestShell::TestShell(const char* groupName,
    const char* testName,
    const char* fileName,
    size_t lineNumber,
    TestShell* nextTest)
  : group_(groupName)
  , name_(testName)
  , file_(fileName)
  , lineNumber_(lineNumber)
  , next_(nextTest)
  , hasFailed_(false)
{
}

TestShell::~TestShell() {}

static void (*pleaseCrashMeRightNow)() = PlatformSpecificAbort;

void
TestShell::setCrashMethod(void (*crashme)())
{
  pleaseCrashMeRightNow = crashme;
}

void
TestShell::resetCrashMethod()
{
  pleaseCrashMeRightNow = PlatformSpecificAbort;
}

void
TestShell::crash()
{
  pleaseCrashMeRightNow();
}

void
TestShell::runOneTest(TestPlugin* plugin, TestResult& result)
{
  hasFailed_ = false;
  result.countRun();
  HelperTestRunInfo runInfo(this, plugin, &result);
  PlatformSpecificSetJmp(helperDoRunOneTestInCurrentProcess, &runInfo);
}

Test*
TestShell::createTest()
{
  return new Test();
}

void
TestShell::destroyTest(Test* test)
{
  delete test;
}

void
TestShell::runOneTestInCurrentProcess(TestPlugin* plugin, TestResult& result)
{
  result.printVeryVerbose("\n-- before runAllPreTestAction: ");
  plugin->runAllPreTestAction(*this, result);
  result.printVeryVerbose("\n-- after runAllPreTestAction: ");

  // save test context, so that test class can be tested
  TestShell* savedTest = TestShell::getCurrent();
  TestResult* savedResult = TestShell::getTestResult();

  TestShell::setTestResult(&result);
  TestShell::setCurrentTest(this);

  Test* testToRun = nullptr;

#if CPPUTEST_HAVE_EXCEPTIONS
  try {
#endif
    result.printVeryVerbose("\n---- before createTest: ");
    testToRun = createTest();
    result.printVeryVerbose("\n---- after createTest: ");

    result.printVeryVerbose("\n------ before runTest: ");
    testToRun->run();
    result.printVeryVerbose("\n------ after runTest: ");

    TestShell::setCurrentTest(savedTest);
    TestShell::setTestResult(savedResult);
#if CPPUTEST_HAVE_EXCEPTIONS
  } catch (...) {
    destroyTest(testToRun);
    throw;
  }
#endif

  result.printVeryVerbose("\n---- before destroyTest: ");
  destroyTest(testToRun);
  result.printVeryVerbose("\n---- after destroyTest: ");

  result.printVeryVerbose("\n-- before runAllPostTestAction: ");
  plugin->runAllPostTestAction(*this, result);
  result.printVeryVerbose("\n-- after runAllPostTestAction: ");
}

TestShell*
TestShell::getNext() const
{
  return next_;
}

TestShell*
TestShell::addTest(TestShell* test)
{
  next_ = test;
  return this;
}

size_t
TestShell::countTests()
{
  return next_ ? next_->countTests() + 1 : 1;
}

String
TestShell::getMacroName() const
{
  return "TEST";
}

const String
TestShell::getName() const
{
  return String(name_);
}

const String
TestShell::getGroup() const
{
  return String(group_);
}

String
TestShell::getFormattedName() const
{
  String formattedName(getMacroName());
  formattedName += "(";
  formattedName += group_;
  formattedName += ", ";
  formattedName += name_;
  formattedName += ")";

  return formattedName;
}

bool
TestShell::hasFailed() const
{
  return hasFailed_;
}

void
TestShell::countCheck()
{
  getTestResult()->countCheck();
}

bool
TestShell::willRun() const
{
  return true;
}

void
TestShell::setRunIgnored()
{
}

void
TestShell::setFileName(const char* fileName)
{
  file_ = fileName;
}

void
TestShell::setLineNumber(size_t lineNumber)
{
  lineNumber_ = lineNumber;
}

void
TestShell::setGroupName(const char* groupName)
{
  group_ = groupName;
}

void
TestShell::setTestName(const char* testName)
{
  name_ = testName;
}

const String
TestShell::getFile() const
{
  return String(file_);
}

size_t
TestShell::getLineNumber() const
{
  return lineNumber_;
}

bool
TestShell::match(const char* target, const TestFilter* filters) const
{
  if (filters == nullptr)
    return true;

  for (; filters != nullptr; filters = filters->getNext())
    if (filters->match(target))
      return true;

  return false;
}

bool
TestShell::shouldRun(const TestFilter* groupFilters,
    const TestFilter* nameFilters) const
{
  return match(group_, groupFilters) && match(name_, nameFilters);
}

void
TestShell::failWith(const TestFailure& failure)
{
  failWith(failure, getCurrentTestTerminator());
}

void
TestShell::failWith(const TestFailure& failure,
    const TestTerminator& terminator)
{
  addFailure(failure);
  terminator.exitCurrentTest();
}

void
TestShell::addFailure(const TestFailure& failure)
{
  hasFailed_ = true;
  getTestResult()->addFailure(failure);
}

void
TestShell::exitTest(const TestTerminator& terminator)
{
  terminator.exitCurrentTest();
}

void
TestShell::assertTrue(bool condition,
    const char* checkString,
    const char* conditionString,
    const char* text,
    const char* fileName,
    size_t lineNumber,
    const TestTerminator& testTerminator)
{
  getTestResult()->countCheck();
  if (!condition) {
    addFailure(CheckFailure(
        this, fileName, lineNumber, checkString, conditionString, text));
    testTerminator.exitCurrentTest();
  }
}

void
TestShell::fail(const char* text,
    const char* fileName,
    size_t lineNumber,
    const TestTerminator& testTerminator)
{
  getTestResult()->countCheck();
  addFailure(FailFailure(this, fileName, lineNumber, text));
  testTerminator.exitCurrentTest();
}

void
TestShell::assertCstrEqual(const char* expected,
    const char* actual,
    const char* text,
    const char* fileName,
    size_t lineNumber,
    const TestTerminator& testTerminator)
{
  getTestResult()->countCheck();
  if (actual == nullptr && expected == nullptr)
    return;
  if (actual == nullptr || expected == nullptr) {
    addFailure(
        StringEqualFailure(this, fileName, lineNumber, expected, actual, text));
    testTerminator.exitCurrentTest();
  }
  if (String::StrCmp(expected, actual) != 0) {
    addFailure(
        StringEqualFailure(this, fileName, lineNumber, expected, actual, text));
    testTerminator.exitCurrentTest();
  }
}

void
TestShell::assertCstrNEqual(const char* expected,
    const char* actual,
    size_t length,
    const char* text,
    const char* fileName,
    size_t lineNumber,
    const TestTerminator& testTerminator)
{
  getTestResult()->countCheck();
  if (actual == nullptr && expected == nullptr)
    return;
  if (actual == nullptr || expected == nullptr) {
    addFailure(
        StringEqualFailure(this, fileName, lineNumber, expected, actual, text));
    testTerminator.exitCurrentTest();
  }
  if (String::StrNCmp(expected, actual, length) != 0) {
    addFailure(
        StringEqualFailure(this, fileName, lineNumber, expected, actual, text));
    testTerminator.exitCurrentTest();
  }
}

void
TestShell::assertCstrNoCaseEqual(const char* expected,
    const char* actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  getTestResult()->countCheck();
  if (actual == nullptr && expected == nullptr)
    return;
  if (actual == nullptr || expected == nullptr) {
    addFailure(StringEqualNoCaseFailure(
        this, fileName, lineNumber, expected, actual, text));
    exitTest(getCurrentTestTerminator());
  }
  if (!String(expected).equalsNoCase(actual)) {
    addFailure(StringEqualNoCaseFailure(
        this, fileName, lineNumber, expected, actual, text));
    exitTest(getCurrentTestTerminator());
  }
}

void
TestShell::assertCstrContains(const char* expected,
    const char* actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  getTestResult()->countCheck();
  if (actual == nullptr && expected == nullptr)
    return;
  if (actual == nullptr || expected == nullptr) {
    addFailure(
        ContainsFailure(this, fileName, lineNumber, expected, actual, text));
    exitTest(getCurrentTestTerminator());
  }
  if (!String(actual).contains(expected)) {
    addFailure(
        ContainsFailure(this, fileName, lineNumber, expected, actual, text));
    exitTest(getCurrentTestTerminator());
  }
}

void
TestShell::assertCstrNoCaseContains(const char* expected,
    const char* actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  getTestResult()->countCheck();
  if (actual == nullptr && expected == nullptr)
    return;
  if (actual == nullptr || expected == nullptr) {
    addFailure(
        ContainsFailure(this, fileName, lineNumber, expected, actual, text));
    exitTest(getCurrentTestTerminator());
  }
  if (!String(actual).containsNoCase(expected)) {
    addFailure(
        ContainsFailure(this, fileName, lineNumber, expected, actual, text));
    exitTest(getCurrentTestTerminator());
  }
}

void
TestShell::assertLongsEqual(long expected,
    long actual,
    const char* text,
    const char* fileName,
    size_t lineNumber,
    const TestTerminator& testTerminator)
{
  getTestResult()->countCheck();
  if (expected != actual) {
    addFailure(
        LongsEqualFailure(this, fileName, lineNumber, expected, actual, text));
    testTerminator.exitCurrentTest();
  }
}

void
TestShell::assertUnsignedLongsEqual(unsigned long expected,
    unsigned long actual,
    const char* text,
    const char* fileName,
    size_t lineNumber,
    const TestTerminator& testTerminator)
{
  getTestResult()->countCheck();
  if (expected != actual) {
    addFailure(UnsignedLongsEqualFailure(
        this, fileName, lineNumber, expected, actual, text));
    testTerminator.exitCurrentTest();
  }
}

void
TestShell::assertLongLongsEqual(long long expected,
    long long actual,
    const char* text,
    const char* fileName,
    size_t lineNumber,
    const TestTerminator& testTerminator)
{
  getTestResult()->countCheck();
  if (expected != actual) {
    addFailure(LongLongsEqualFailure(
        this, fileName, lineNumber, expected, actual, text));
    testTerminator.exitCurrentTest();
  }
}

void
TestShell::assertUnsignedLongLongsEqual(unsigned long long expected,
    unsigned long long actual,
    const char* text,
    const char* fileName,
    size_t lineNumber,
    const TestTerminator& testTerminator)
{
  getTestResult()->countCheck();
  if (expected != actual) {
    addFailure(UnsignedLongLongsEqualFailure(
        this, fileName, lineNumber, expected, actual, text));
    testTerminator.exitCurrentTest();
  }
}

void
TestShell::assertSignedBytesEqual(signed char expected,
    signed char actual,
    const char* text,
    const char* fileName,
    size_t lineNumber,
    const TestTerminator& testTerminator)
{
  getTestResult()->countCheck();
  if (expected != actual) {
    addFailure(SignedBytesEqualFailure(
        this, fileName, lineNumber, expected, actual, text));
    testTerminator.exitCurrentTest();
  }
}

void
TestShell::assertPointersEqual(const void* expected,
    const void* actual,
    const char* text,
    const char* fileName,
    size_t lineNumber,
    const TestTerminator& testTerminator)
{
  getTestResult()->countCheck();
  if (expected != actual) {
    addFailure(EqualsFailure(this,
        fileName,
        lineNumber,
        StringFrom(expected),
        StringFrom(actual),
        text));
    testTerminator.exitCurrentTest();
  }
}

void
TestShell::assertFunctionPointersEqual(void (*expected)(),
    void (*actual)(),
    const char* text,
    const char* fileName,
    size_t lineNumber,
    const TestTerminator& testTerminator)
{
  getTestResult()->countCheck();
  if (expected != actual) {
    addFailure(EqualsFailure(this,
        fileName,
        lineNumber,
        StringFrom(expected),
        StringFrom(actual),
        text));
    testTerminator.exitCurrentTest();
  }
}

void
TestShell::assertDoublesEqual(double expected,
    double actual,
    double threshold,
    const char* text,
    const char* fileName,
    size_t lineNumber,
    const TestTerminator& testTerminator)
{
  getTestResult()->countCheck();
  if (!doubles_equal(expected, actual, threshold)) {
    addFailure(DoublesEqualFailure(
        this, fileName, lineNumber, expected, actual, threshold, text));
    testTerminator.exitCurrentTest();
  }
}

void
TestShell::assertBinaryEqual(const void* expected,
    const void* actual,
    size_t length,
    const char* text,
    const char* fileName,
    size_t lineNumber,
    const TestTerminator& testTerminator)
{
  getTestResult()->countCheck();
  if (length == 0)
    return;
  if (actual == nullptr && expected == nullptr)
    return;
  if (actual == nullptr || expected == nullptr) {
    addFailure(BinaryEqualFailure(this,
        fileName,
        lineNumber,
        static_cast<const unsigned char*>(expected),
        static_cast<const unsigned char*>(actual),
        length,
        text));
    testTerminator.exitCurrentTest();
  }
  if (String::MemCmp(expected, actual, length) != 0) {
    addFailure(BinaryEqualFailure(this,
        fileName,
        lineNumber,
        static_cast<const unsigned char*>(expected),
        static_cast<const unsigned char*>(actual),
        length,
        text));
    testTerminator.exitCurrentTest();
  }
}

void
TestShell::assertEquals(bool failed,
    const char* expected,
    const char* actual,
    const char* text,
    const char* file,
    size_t line,
    const TestTerminator& testTerminator)
{
  getTestResult()->countCheck();
  if (failed) {
    addFailure(CheckEqualFailure(this, file, line, expected, actual, text));
    testTerminator.exitCurrentTest();
  }
}

void
TestShell::assertEquals(bool failed,
    String expected,
    String actual,
    const char* text,
    const char* file,
    size_t line,
    const TestTerminator& testTerminator)
{
  getTestResult()->countCheck();
  if (failed) {
    addFailure(CheckEqualFailure(
        this, file, line, expected.c_str(), actual.c_str(), text));
    {
      String e(static_cast<String&&>(expected));
    }
    {
      String a(static_cast<String&&>(actual));
    }
    testTerminator.exitCurrentTest();
  }
}

void
TestShell::assertCompare(bool comparison,
    const char* checkString,
    const char* comparisonString,
    const char* text,
    const char* fileName,
    size_t lineNumber,
    const TestTerminator& testTerminator)
{
  getTestResult()->countCheck();
  if (!comparison) {
    addFailure(ComparisonFailure(
        this, fileName, lineNumber, checkString, comparisonString, text));
    testTerminator.exitCurrentTest();
  }
}

void
TestShell::print(const char* text, const char* fileName, size_t lineNumber)
{
  String stringToPrint = "\n";
  stringToPrint += fileName;
  stringToPrint += ":";
  stringToPrint += StringFrom(lineNumber);
  stringToPrint += " ";
  stringToPrint += text;
  getTestResult()->print(stringToPrint.c_str());
}

void
TestShell::print(const String& text, const char* fileName, size_t lineNumber)
{
  print(text.c_str(), fileName, lineNumber);
}

void
TestShell::printVeryVerbose(const char* text)
{
  getTestResult()->printVeryVerbose(text);
}

TestResult* TestShell::testResult_ = nullptr;
TestShell* TestShell::currentTest_ = nullptr;

void
TestShell::setTestResult(TestResult* result)
{
  testResult_ = result;
}

void
TestShell::setCurrentTest(TestShell* test)
{
  currentTest_ = test;
}

TestResult*
TestShell::getTestResult()
{
  if (testResult_ == nullptr)
    return &OutsideTestRunnerUTest::instance().getTestResult();
  return testResult_;
}

TestShell*
TestShell::getCurrent()
{
  if (currentTest_ == nullptr)
    return &OutsideTestRunnerUTest::instance();
  return currentTest_;
}

const TestTerminator&
TestShell::getCurrentTestTerminator()
{
  return *currentTestTerminator_;
}

const TestTerminator&
TestShell::getCurrentTestTerminatorWithoutExceptions()
{
  return *currentTestTerminatorWithoutExceptions_;
}

void
TestShell::setCrashOnFail()
{
  currentTestTerminator_ = &crashingTestTerminator;
  currentTestTerminatorWithoutExceptions_ =
      &crashingTestTerminatorWithoutExceptions;
}

void
TestShell::restoreDefaultTestTerminator()
{
  currentTestTerminator_ = &normalTestTerminator;
  currentTestTerminatorWithoutExceptions_ =
      &normalTestTerminatorWithoutExceptions;
}

void
TestShell::setRethrowExceptions(bool rethrowExceptions)
{
  rethrowExceptions_ = rethrowExceptions;
}

bool
TestShell::isRethrowingExceptions()
{
  return rethrowExceptions_;
}

} // namespace cpputest
