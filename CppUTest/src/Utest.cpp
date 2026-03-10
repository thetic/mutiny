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

#include "CppUTest/PlatformSpecificFunctions.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/TestOutput.h"
#include "CppUTest/TestRegistry.h"

namespace cpputest {

#if defined(__GNUC__) && __GNUC__ >= 11
#define NEEDS_DISABLE_NULL_WARNING
#endif /* GCC >= 11 */

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

static void
helperDoTestSetup(void* data)
{
  static_cast<Test*>(data)->setup();
}

static void
helperDoTestBody(void* data)
{
  static_cast<Test*>(data)->testBody();
}

static void
helperDoTestTeardown(void* data)
{
  static_cast<Test*>(data)->teardown();
}

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
TestShell::assertBitsEqual(unsigned long expected,
    unsigned long actual,
    unsigned long mask,
    size_t byteCount,
    const char* text,
    const char* fileName,
    size_t lineNumber,
    const TestTerminator& testTerminator)
{
  getTestResult()->countCheck();
  if ((expected & mask) != (actual & mask)) {
    addFailure(BitsEqualFailure(
        this, fileName, lineNumber, expected, actual, mask, byteCount, text));
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

ExecFunctionTestShell::~ExecFunctionTestShell() {}

////////////// Test ////////////

Test::Test() {}

Test::~Test() {}

#if CPPUTEST_HAVE_EXCEPTIONS

void
Test::run()
{
  TestShell* current = TestShell::getCurrent();
  int jumpResult = 0;
  try {
    current->printVeryVerbose("\n-------- before setup: ");
    jumpResult = PlatformSpecificSetJmp(helperDoTestSetup, this);
    current->printVeryVerbose("\n-------- after  setup: ");

    if (jumpResult) {
      current->printVeryVerbose("\n----------  before body: ");
      PlatformSpecificSetJmp(helperDoTestBody, this);
      current->printVeryVerbose("\n----------  after body: ");
    }
  } catch (FailedException&) {
    PlatformSpecificRestoreJumpBuffer();
  }
#if CPPUTEST_USE_STD_CPP_LIB
  catch (const std::exception& e) {
    current->addFailure(UnexpectedExceptionFailure(current, e));
    PlatformSpecificRestoreJumpBuffer();
    if (current->isRethrowingExceptions()) {
      throw;
    }
  }
#endif
  catch (...) {
    current->addFailure(UnexpectedExceptionFailure(current));
    PlatformSpecificRestoreJumpBuffer();
    if (current->isRethrowingExceptions()) {
      throw;
    }
  }

  try {
    current->printVeryVerbose("\n--------  before teardown: ");
    PlatformSpecificSetJmp(helperDoTestTeardown, this);
    current->printVeryVerbose("\n--------  after teardown: ");
  } catch (FailedException&) {
    PlatformSpecificRestoreJumpBuffer();
  }
#if CPPUTEST_USE_STD_CPP_LIB
  catch (const std::exception& e) {
    current->addFailure(UnexpectedExceptionFailure(current, e));
    PlatformSpecificRestoreJumpBuffer();
    if (current->isRethrowingExceptions()) {
      throw;
    }
  }
#endif
  catch (...) {
    current->addFailure(UnexpectedExceptionFailure(current));
    PlatformSpecificRestoreJumpBuffer();
    if (current->isRethrowingExceptions()) {
      throw;
    }
  }
}
#else

void
Test::run()
{
  if (PlatformSpecificSetJmp(helperDoTestSetup, this)) {
    PlatformSpecificSetJmp(helperDoTestBody, this);
  }
  PlatformSpecificSetJmp(helperDoTestTeardown, this);
}

#endif

void
Test::setup()
{
}

void
Test::testBody()
{
}

void
Test::teardown()
{
}

/////////////////// Terminators

TestTerminator::~TestTerminator() {}

void
NormalTestTerminator::exitCurrentTest() const
{
#if CPPUTEST_HAVE_EXCEPTIONS
  throw FailedException();
#else
  TestTerminatorWithoutExceptions().exitCurrentTest();
#endif
}

NormalTestTerminator::~NormalTestTerminator() {}

void
TestTerminatorWithoutExceptions::exitCurrentTest() const
{
  PlatformSpecificLongJmp();
}

TestTerminatorWithoutExceptions::~TestTerminatorWithoutExceptions() {}

void
CrashingTestTerminator::exitCurrentTest() const
{
  TestShell::crash();
  NormalTestTerminator::exitCurrentTest();
}

CrashingTestTerminator::~CrashingTestTerminator() {}

void
CrashingTestTerminatorWithoutExceptions::exitCurrentTest() const
{
  TestShell::crash();
  TestTerminatorWithoutExceptions::exitCurrentTest();
}

CrashingTestTerminatorWithoutExceptions::
    ~CrashingTestTerminatorWithoutExceptions()
{
}

//////////////////// ExecFunction
//
ExecFunction::ExecFunction() {}

ExecFunction::~ExecFunction() {}

ExecFunctionWithoutParameters::ExecFunctionWithoutParameters(
    void (*testFunction)())
  : testFunction_(testFunction)
{
}

ExecFunctionWithoutParameters::~ExecFunctionWithoutParameters() {}

void
ExecFunctionWithoutParameters::exec()
{
  if (testFunction_)
    testFunction_();
}

//////////////////// ExecFunctionTest

ExecFunctionTest::ExecFunctionTest(ExecFunctionTestShell* shell)
  : shell_(shell)
{
}

void
ExecFunctionTest::testBody()
{
  if (shell_->testFunction_)
    shell_->testFunction_->exec();
}

void
ExecFunctionTest::setup()
{
  if (shell_->setup_)
    shell_->setup_();
}

void
ExecFunctionTest::teardown()
{
  if (shell_->teardown_)
    shell_->teardown_();
}

/////////////// IgnoredTestShell /////////////
IgnoredTestShell::IgnoredTestShell()
  : runIgnored_(false)
{
}

IgnoredTestShell::IgnoredTestShell(const char* groupName,
    const char* testName,
    const char* fileName,
    size_t lineNumber)
  : TestShell(groupName, testName, fileName, lineNumber)
  , runIgnored_(false)
{
}

IgnoredTestShell::~IgnoredTestShell() {}

bool
IgnoredTestShell::willRun() const
{
  if (runIgnored_)
    return TestShell::willRun();

  return false;
}

String
IgnoredTestShell::getMacroName() const
{
  if (runIgnored_)
    return "TEST";

  return "IGNORE_TEST";
}

void
IgnoredTestShell::runOneTest(TestPlugin* plugin, TestResult& result)
{
  if (runIgnored_) {
    TestShell::runOneTest(plugin, result);
    return;
  }

  result.countIgnored();
}

void
IgnoredTestShell::setRunIgnored()
{
  runIgnored_ = true;
}

//////////////////// TestShellPointerArray

TestShellPointerArray::TestShellPointerArray(TestShell* firstTest)
  : arrayOfTests_(nullptr)
  , count_(0)
{
  count_ = (firstTest) ? firstTest->countTests() : 0;
  if (count_ == 0)
    return;

  arrayOfTests_ = new TestShell*[count_];

  TestShell* currentTest = firstTest;
  for (size_t i = 0; i < count_; i++) {
    arrayOfTests_[i] = currentTest;
    currentTest = currentTest->getNext();
  }
}

TestShellPointerArray::~TestShellPointerArray()
{
  delete[] arrayOfTests_;
}

void
TestShellPointerArray::swap(size_t index1, size_t index2)
{
  TestShell* e2 = arrayOfTests_[index2];
  TestShell* e1 = arrayOfTests_[index1];
  arrayOfTests_[index1] = e2;
  arrayOfTests_[index2] = e1;
}

void
TestShellPointerArray::shuffle(size_t seed)
{
  if (count_ == 0)
    return;

  PlatformSpecificSrand(static_cast<unsigned int>(seed));

  for (size_t i = count_ - 1; i >= 1; --i) {
    if (count_ == 0)
      return;

    const size_t j =
        static_cast<size_t>(PlatformSpecificRand()) %
        (i + 1); // distribution biased by modulo, but good enough for shuffling
    swap(i, j);
  }
  relinkTestsInOrder();
}

void
TestShellPointerArray::reverse()
{
  if (count_ == 0)
    return;

  size_t halfCount = count_ / 2;
  for (size_t i = 0; i < halfCount; i++) {
    size_t j = count_ - i - 1;
    swap(i, j);
  }
  relinkTestsInOrder();
}

void
TestShellPointerArray::relinkTestsInOrder()
{
  TestShell* tests = nullptr;
  for (size_t i = 0; i < count_; i++)
    tests = arrayOfTests_[count_ - i - 1]->addTest(tests);
}

TestShell*
TestShellPointerArray::getFirstTest() const
{
  return get(0);
}

TestShell*
TestShellPointerArray::get(size_t index) const
{
  if (index >= count_)
    return nullptr;
  return arrayOfTests_[index];
}

////////////// TestInstaller ////////////

TestInstaller::TestInstaller(TestShell& shell,
    const char* groupName,
    const char* testName,
    const char* fileName,
    size_t lineNumber)
{
  shell.setGroupName(groupName);
  shell.setTestName(testName);
  shell.setFileName(fileName);
  shell.setLineNumber(lineNumber);
  TestRegistry::getCurrentRegistry()->addTest(&shell);
}

TestInstaller::~TestInstaller() {}

void
TestInstaller::unDo()
{
  TestRegistry::getCurrentRegistry()->unDoLastAddTest();
}

} // namespace cpputest
