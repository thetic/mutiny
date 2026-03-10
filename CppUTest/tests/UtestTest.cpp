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

#include "CppUTest/Utest.h"

#include "CppUTest/PlatformSpecificFunctions.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/TestOutput.h"
#include "CppUTest/TestTestingFixture.h"

#include <math.h>

TEST_GROUP(UtestShell) {
cpputest::TestTestingFixture fixture;
};

static void
failMethod_()
{
  FAIL("This test fails");
}

static void
passingTestMethod_()
{
  CHECK(true);
}

static void
passingCheckEqualTestMethod_()
{
  CHECK_EQUAL(1, 1);
}

static void
exitTestMethod_()
{
  TEST_EXIT;
  FAIL("Should not get here");
}

TEST(UtestShell, compareDoubles)
{
  CHECK(cpputest::doubles_equal(1.0, 1.001, 0.01));
  CHECK(!cpputest::doubles_equal(1.0, 1.1, 0.05));
  double a = 1.2345678;
  CHECK(cpputest::doubles_equal(a, a, 0.000000001));
}

#ifdef NAN
TEST(UtestShell, compareDoublesNaN)
{
  CHECK(!cpputest::doubles_equal(static_cast<double>(NAN), 1.001, 0.01));
  CHECK(!cpputest::doubles_equal(1.0, static_cast<double>(NAN), 0.01));
  CHECK(!cpputest::doubles_equal(1.0, 1.001, static_cast<double>(NAN)));
}
#endif

#ifdef INFINITY
TEST(UtestShell, compareDoublesInf)
{
  CHECK(!cpputest::doubles_equal(static_cast<double>(INFINITY), 1.0, 0.01));
  CHECK(!cpputest::doubles_equal(1.0, static_cast<double>(INFINITY), 0.01));
  CHECK(cpputest::doubles_equal(1.0, -1.0, static_cast<double>(INFINITY)));
  CHECK(cpputest::doubles_equal(
      static_cast<double>(INFINITY), static_cast<double>(INFINITY), 0.01));
  CHECK(cpputest::doubles_equal(static_cast<double>(INFINITY),
      static_cast<double>(INFINITY),
      static_cast<double>(INFINITY)));
}
#endif

TEST(UtestShell, FailWillIncreaseTheAmountOfChecks)
{
  fixture.setTestFunction(failMethod_);
  fixture.runAllTests();
  LONGS_EQUAL(1, fixture.getCheckCount());
}

TEST(UtestShell, PassedCheckEqualWillIncreaseTheAmountOfChecks)
{
  fixture.setTestFunction(passingCheckEqualTestMethod_);
  fixture.runAllTests();
  LONGS_EQUAL(1, fixture.getCheckCount());
}

IGNORE_TEST(UtestShell, IgnoreTestAccessingFixture)
{
  CHECK(fixture.getCheckCount() == 0);
}

TEST(UtestShell, MacrosUsedInSetup)
{
  fixture.setSetup(failMethod_);
  fixture.setTestFunction(passingTestMethod_);
  fixture.runAllTests();
  LONGS_EQUAL(1, fixture.getFailureCount());
}

TEST(UtestShell, MacrosUsedInTearDown)
{
  fixture.setTeardown(failMethod_);
  fixture.setTestFunction(passingTestMethod_);
  fixture.runAllTests();
  LONGS_EQUAL(1, fixture.getFailureCount());
}

TEST(UtestShell, ExitLeavesQuietly)
{
  fixture.setTestFunction(exitTestMethod_);
  fixture.runAllTests();
  LONGS_EQUAL(0, fixture.getFailureCount());
}

static bool cpputestHasCrashed;

static void
crashMethod()
{
  cpputestHasCrashed = true;
}

TEST(UtestShell, FailWillNotCrashIfNotEnabled)
{
  cpputestHasCrashed = false;
  cpputest::TestShell::setCrashMethod(crashMethod);

  fixture.setTestFunction(failMethod_);
  fixture.runAllTests();

  CHECK_FALSE(cpputestHasCrashed);
  LONGS_EQUAL(1, fixture.getFailureCount());

  cpputest::TestShell::resetCrashMethod();
}

TEST(UtestShell, FailWillCrashIfEnabled)
{
  cpputestHasCrashed = false;
  cpputest::TestShell::setCrashOnFail();
  cpputest::TestShell::setCrashMethod(crashMethod);

  fixture.setTestFunction(failMethod_);
  fixture.runAllTests();

  CHECK(cpputestHasCrashed);

  cpputest::TestShell::restoreDefaultTestTerminator();
  cpputest::TestShell::resetCrashMethod();
}

static int teardownCalled = 0;

static void
teardownMethod_()
{
  teardownCalled++;
}

TEST(UtestShell, TeardownCalledAfterTestFailure)
{
  teardownCalled = 0;
  fixture.setTeardown(teardownMethod_);
  fixture.setTestFunction(failMethod_);
  fixture.runAllTests();
  LONGS_EQUAL(1, fixture.getFailureCount());
  LONGS_EQUAL(1, teardownCalled);
}

static int stopAfterFailure = 0;
static void
stopAfterFailureMethod_()
{
  FAIL("fail");
  stopAfterFailure++;
}

TEST(UtestShell, TestStopsAfterTestFailure)
{
  stopAfterFailure = 0;
  fixture.setTestFunction(stopAfterFailureMethod_);
  fixture.runAllTests();
  CHECK(fixture.hasTestFailed());
  LONGS_EQUAL(1, fixture.getFailureCount());
  LONGS_EQUAL(0, stopAfterFailure);
}

TEST(UtestShell, TestStopsAfterSetupFailure)
{
  stopAfterFailure = 0;
  fixture.setSetup(stopAfterFailureMethod_);
  fixture.setTeardown(stopAfterFailureMethod_);
  fixture.setTestFunction(failMethod_);
  fixture.runAllTests();
  LONGS_EQUAL(2, fixture.getFailureCount());
  LONGS_EQUAL(0, stopAfterFailure);
}

#if CPPUTEST_HAVE_EXCEPTIONS

// Prevents -Wunreachable-code; should always be 'true'
static bool shouldThrowException = true;

static void
thrownUnknownExceptionMethod_()
{
  if (shouldThrowException) {
    throw 33;
  }
  stopAfterFailure++;
}

TEST(UtestShell, TestStopsAfterUnknownExceptionIsThrown)
{
  bool initialRethrowExceptions = cpputest::TestShell::isRethrowingExceptions();
  cpputest::TestShell::setRethrowExceptions(false);
  stopAfterFailure = 0;
  shouldThrowException = true;
  fixture.setTestFunction(thrownUnknownExceptionMethod_);
  fixture.runAllTests();
  LONGS_EQUAL(1, fixture.getFailureCount());
  fixture.assertPrintContains(
      "Unexpected exception of unknown type was thrown");
  LONGS_EQUAL(0, stopAfterFailure);
  cpputest::TestShell::setRethrowExceptions(initialRethrowExceptions);
}

TEST(UtestShell, NoExceptionIsRethrownIfEnabledButNotThrown)
{
  bool initialRethrowExceptions = cpputest::TestShell::isRethrowingExceptions();
  bool exceptionRethrown = false;
  stopAfterFailure = 0;
  cpputest::TestShell::setRethrowExceptions(true);
  shouldThrowException = false;
  fixture.setTestFunction(thrownUnknownExceptionMethod_);
  try {
    fixture.runAllTests();
  } catch (...) {
    exceptionRethrown = true;
  }
  CHECK_FALSE(exceptionRethrown);
  LONGS_EQUAL(0, fixture.getFailureCount());
  LONGS_EQUAL(1, stopAfterFailure);
  cpputest::TestShell::setRethrowExceptions(initialRethrowExceptions);
}

TEST(UtestShell, UnknownExceptionIsRethrownIfEnabled)
{
  bool initialRethrowExceptions = cpputest::TestShell::isRethrowingExceptions();
  bool exceptionRethrown = false;
  stopAfterFailure = 0;
  cpputest::TestShell::setRethrowExceptions(true);
  shouldThrowException = true;
  fixture.setTestFunction(thrownUnknownExceptionMethod_);
  try {
    fixture.runAllTests();
    stopAfterFailure++;
  } catch (...) {
    exceptionRethrown = true;
  }
  CHECK_TRUE(exceptionRethrown);
  LONGS_EQUAL(1, fixture.getFailureCount());
  fixture.assertPrintContains(
      "Unexpected exception of unknown type was thrown");
  LONGS_EQUAL(0, stopAfterFailure);
  cpputest::TestShell::setRethrowExceptions(initialRethrowExceptions);
}

#if CPPUTEST_USE_STD_CPP_LIB
static void
thrownStandardExceptionMethod_()
{
  if (shouldThrowException) {
    throw std::runtime_error("exception text");
  }
  stopAfterFailure++;
}

TEST(UtestShell, TestStopsAfterStandardExceptionIsThrown)
{
  bool initialRethrowExceptions = cpputest::TestShell::isRethrowingExceptions();
  cpputest::TestShell::setRethrowExceptions(false);
  stopAfterFailure = 0;
  shouldThrowException = true;
  fixture.setTestFunction(thrownStandardExceptionMethod_);
  fixture.runAllTests();
  LONGS_EQUAL(1, fixture.getFailureCount());
#if CPPUTEST_HAVE_RTTI
  fixture.assertPrintContains("Unexpected exception of type '");
  fixture.assertPrintContains("runtime_error");
  fixture.assertPrintContains("' was thrown: exception text");
#else
  fixture.assertPrintContains(
      "Unexpected exception of unknown type was thrown");
#endif
  LONGS_EQUAL(0, stopAfterFailure);
  cpputest::TestShell::setRethrowExceptions(initialRethrowExceptions);
}

TEST(UtestShell, StandardExceptionIsRethrownIfEnabled)
{
  bool initialRethrowExceptions = cpputest::TestShell::isRethrowingExceptions();
  bool exceptionRethrown = false;
  stopAfterFailure = 0;
  cpputest::TestShell::setRethrowExceptions(true);
  shouldThrowException = true;
  fixture.setTestFunction(thrownStandardExceptionMethod_);
  try {
    fixture.runAllTests();
    stopAfterFailure++;
  } catch (const std::exception&) {
    exceptionRethrown = true;
  }
  CHECK_TRUE(exceptionRethrown);
  LONGS_EQUAL(1, fixture.getFailureCount());
  fixture.assertPrintContains("Unexpected exception of type '");
  fixture.assertPrintContains("runtime_error");
  fixture.assertPrintContains("' was thrown: exception text");
  LONGS_EQUAL(0, stopAfterFailure);
  cpputest::TestShell::setRethrowExceptions(initialRethrowExceptions);
}
#endif // CPPUTEST_USE_STD_CPP_LIB
#endif // CPPUTEST_HAVE_EXCEPTIONS

TEST(UtestShell, veryVebose)
{
  cpputest::TestShell shell("Group", "name", __FILE__, __LINE__);
  cpputest::StringBufferTestOutput normalOutput;
  normalOutput.verbose(cpputest::TestOutput::level_veryVerbose);
  cpputest::NullTestPlugin plugin;

  cpputest::TestResult result(normalOutput);
  shell.runOneTestInCurrentProcess(&plugin, result);
  STRCMP_CONTAINS(
      "\n------ before runTest", normalOutput.getOutput().asCharString());
}

class defaultUtestShell : public cpputest::TestShell
{};

TEST(UtestShell,
    this_test_covers_the_UtestShell_createTest_and_Utest_testBody_methods)
{
  defaultUtestShell shell;
  fixture.addTest(&shell);
  fixture.runAllTests();
  LONGS_EQUAL(2, fixture.getTestCount());
}

#if CPPUTEST_HAVE_EXCEPTIONS

static bool destructorWasCalledOnFailedTest = false;

static void
destructorCalledForLocalObjects_()
{
  struct SetBoolOnDestruct
  {
    bool& b_;
    ~SetBoolOnDestruct() { b_ = true; }
  } pleaseCallTheDestructor{ destructorWasCalledOnFailedTest };
  destructorWasCalledOnFailedTest = false;
  FAIL("fail");
}

TEST(UtestShell, DestructorIsCalledForLocalObjectsWhenTheTestFails)
{
  fixture.setTestFunction(destructorCalledForLocalObjects_);
  fixture.runAllTests();
  CHECK(destructorWasCalledOnFailedTest);
}

#endif

TEST_GROUP(IgnoredUtestShell) {
cpputest::TestTestingFixture fixture;
cpputest::IgnoredTestShell ignoredTest;
cpputest::ExecFunctionTestShell normalUtestShell;

void
setup() override
{
  fixture.addTest(&ignoredTest);
  fixture.addTest(&normalUtestShell);
}
};

TEST(IgnoredUtestShell, doesIgnoreCount)
{
  fixture.runAllTests();
  LONGS_EQUAL(1, fixture.getIgnoreCount());
}

TEST(IgnoredUtestShell, printsIGNORE_TESTwhenVerbose)
{
  fixture.setOutputVerbose();
  fixture.runAllTests();
  fixture.assertPrintContains("IGNORE_TEST");
}

TEST(IgnoredUtestShell, runIgnoredOptionSpecifiedThenIncreaseRunCount)
{
  ignoredTest.setRunIgnored();
  fixture.runAllTests();
  LONGS_EQUAL(3, fixture.getRunCount());
  LONGS_EQUAL(0, fixture.getIgnoreCount());
}

TEST(IgnoredUtestShell, runIgnoredOptionNotSpecifiedThenIncreaseIgnoredCount)
{
  fixture.runAllTests();
  LONGS_EQUAL(2, fixture.getRunCount());
  LONGS_EQUAL(1, fixture.getIgnoreCount());
}

TEST(IgnoredUtestShell,
    runIgnoredOptionSpecifiedWillNotInfluenceNormalTestCount)
{
  normalUtestShell.setRunIgnored();
  fixture.runAllTests();
  LONGS_EQUAL(2, fixture.getRunCount());
  LONGS_EQUAL(1, fixture.getIgnoreCount());
}

TEST(IgnoredUtestShell, runIgnoredOptionSpecifiedThenReturnTESTInFormattedName)
{
  ignoredTest.setGroupName("TestGroup");
  ignoredTest.setTestName("TestName");
  ignoredTest.setRunIgnored();
  fixture.runAllTests();
  STRCMP_EQUAL("TEST(TestGroup, TestName)",
      ignoredTest.getFormattedName().asCharString());
}

TEST(IgnoredUtestShell,
    runIgnoredOptionNotSpecifiedThenReturnIGNORETESTInFormattedName)
{
  ignoredTest.setGroupName("TestGroup");
  ignoredTest.setTestName("TestName");
  fixture.runAllTests();
  STRCMP_EQUAL("IGNORE_TEST(TestGroup, TestName)",
      ignoredTest.getFormattedName().asCharString());
}

TEST(IgnoredUtestShell, runIgnoredOptionNotSpecifiedThenWillRunReturnFalse)
{
  CHECK_FALSE(ignoredTest.willRun());
}

TEST(IgnoredUtestShell, runIgnoredOptionSpecifiedThenWillRunReturnTrue)
{
  ignoredTest.setRunIgnored();
  CHECK_TRUE(ignoredTest.willRun());
}

TEST_BASE(MyOwnTest)
{
  MyOwnTest()
    : inTest(false)
  {
  }
  bool inTest;

  void setup() override
  {
    CHECK(!inTest);
    inTest = true;
  }
  void teardown() override
  {
    CHECK(inTest);
    inTest = false;
  }
};

TEST_GROUP_BASE(UtestMyOwn, MyOwnTest){};

TEST(UtestMyOwn, test)
{
  CHECK(inTest);
}

class NullParameterTest : public cpputest::TestShell
{};

TEST(UtestMyOwn, NullParameters)
{
  NullParameterTest nullTest; /* Bug fix tests for creating a test without a
                                 name, fix in String */
  cpputest::TestFilter emptyFilter;
  CHECK(nullTest.shouldRun(&emptyFilter, &emptyFilter));
}

class AllocateAndDeallocateInConstructorAndDestructor
{
  char* memory_;
  char* morememory_;

public:
  AllocateAndDeallocateInConstructorAndDestructor()
  {
    memory_ = new char[100];
    morememory_ = nullptr;
  }
  void allocateMoreMemory() { morememory_ = new char[123]; }

  ~AllocateAndDeallocateInConstructorAndDestructor()
  {
    delete[] memory_;
    delete[] morememory_;
  }
};

TEST_GROUP(
    CanHaveMemberVariablesInTestGroupThatAllocateMemoryWithoutCausingMemoryLeaks) {
AllocateAndDeallocateInConstructorAndDestructor dummy;
};

TEST(
    CanHaveMemberVariablesInTestGroupThatAllocateMemoryWithoutCausingMemoryLeaks,
    testInTestGroupName)
{
  dummy.allocateMoreMemory();
}

static int
getZero()
{
  return 0;
}

static int
getOne()
{
  return 1;
}

TEST_GROUP(UtestShellPointerArrayTest) {
cpputest::TestShell* test0;
cpputest::TestShell* test1;
cpputest::TestShell* test2;

void
setup() override
{
  test0 = new cpputest::IgnoredTestShell();
  test1 = new cpputest::IgnoredTestShell();
  test2 = new cpputest::IgnoredTestShell();

  test0->addTest(test1);
  test1->addTest(test2);
}

void
teardown() override
{
  delete test0;
  delete test1;
  delete test2;
}
};

TEST(UtestShellPointerArrayTest, empty)
{
  cpputest::TestShellPointerArray tests(nullptr);
  tests.shuffle(0);
  CHECK(nullptr == tests.getFirstTest());
}

TEST(UtestShellPointerArrayTest, testsAreInOrder)
{
  cpputest::TestShellPointerArray tests(test0);
  CHECK(tests.get(0) == test0);
  CHECK(tests.get(1) == test1);
  CHECK(tests.get(2) == test2);
}

TEST(UtestShellPointerArrayTest,
    relinkingTestsWillKeepThemTheSameWhenNothingWasDone)
{
  cpputest::TestShellPointerArray tests(test0);
  tests.relinkTestsInOrder();
  CHECK(tests.get(0) == test0);
  CHECK(tests.get(1) == test1);
  CHECK(tests.get(2) == test2);
}

TEST(UtestShellPointerArrayTest, firstTestisNotTheFirstTestWithSeed1234)
{
  UT_PTR_SET(PlatformSpecificRand, getZero);

  cpputest::TestShellPointerArray tests(test0);
  tests.shuffle(1234);
  CHECK(tests.getFirstTest() != test0);
}

TEST(UtestShellPointerArrayTest, ShuffleListTestWithRandomAlwaysReturningZero)
{
  UT_PTR_SET(PlatformSpecificRand, getZero);

  cpputest::TestShellPointerArray tests(test0);
  tests.shuffle(3);
  CHECK(tests.get(0) == test1);
  CHECK(tests.get(1) == test2);
  CHECK(tests.get(2) == test0);
}

// swaps with 4 mod 3 (1) then 4 mod 2 (0): 1, [2], [0] --> [1], [0], 2 --> 0,
// 1, 2
TEST(UtestShellPointerArrayTest, ShuffleListTestWithRandomAlwaysReturningOne)
{
  UT_PTR_SET(PlatformSpecificRand, getOne);

  cpputest::TestShellPointerArray tests(test0);
  tests.shuffle(3);
  CHECK(tests.get(0) == test0);
  CHECK(tests.get(1) == test2);
  CHECK(tests.get(2) == test1);
}

TEST(UtestShellPointerArrayTest, reverse)
{
  UT_PTR_SET(PlatformSpecificRand, getOne);

  cpputest::TestShellPointerArray tests(test0);
  tests.reverse();
  CHECK(tests.get(0) == test2);
  CHECK(tests.get(1) == test1);
  CHECK(tests.get(2) == test0);
}
