#include "CppUTest/TestResult.hpp"

#include "CppUTest/PlatformSpecificFunctions.h"
#include "CppUTest/TestFailure.hpp"
#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestOutput.hpp"

namespace cpputest {

TestResult::TestResult(TestOutput& p)
  : output_(p)
  , testCount_(0)
  , runCount_(0)
  , checkCount_(0)
  , failureCount_(0)
  , filteredOutCount_(0)
  , ignoredCount_(0)
  , totalExecutionTime_(0)
  , timeStarted_(0)
  , currentTestTimeStarted_(0)
  , currentTestTotalExecutionTime_(0)
  , currentGroupTimeStarted_(0)
  , currentGroupTotalExecutionTime_(0)
{
}

TestResult::~TestResult() {}

void
TestResult::currentGroupStarted(TestShell* test)
{
  output_.printCurrentGroupStarted(*test);
  currentGroupTimeStarted_ =
      static_cast<size_t>(GetPlatformSpecificTimeInMillis());
}

void
TestResult::currentGroupEnded(TestShell* /*test*/)
{
  currentGroupTotalExecutionTime_ =
      static_cast<size_t>(GetPlatformSpecificTimeInMillis()) -
      currentGroupTimeStarted_;
  output_.printCurrentGroupEnded(*this);
}

void
TestResult::currentTestStarted(TestShell* test)
{
  output_.printCurrentTestStarted(*test);
  currentTestTimeStarted_ =
      static_cast<size_t>(GetPlatformSpecificTimeInMillis());
}

void
TestResult::print(const char* text)
{
  output_.print(text);
}

void
TestResult::printVeryVerbose(const char* text)
{
  output_.printVeryVerbose(text);
}

void
TestResult::currentTestEnded(TestShell* /*test*/)
{
  currentTestTotalExecutionTime_ =
      static_cast<size_t>(GetPlatformSpecificTimeInMillis()) -
      currentTestTimeStarted_;
  output_.printCurrentTestEnded(*this);
}

void
TestResult::addFailure(const TestFailure& failure)
{
  output_.printFailure(failure);
  failureCount_++;
}

void
TestResult::countTest()
{
  testCount_++;
}

void
TestResult::countRun()
{
  runCount_++;
}

void
TestResult::countCheck()
{
  checkCount_++;
}

void
TestResult::countFilteredOut()
{
  filteredOutCount_++;
}

void
TestResult::countIgnored()
{
  ignoredCount_++;
}

void
TestResult::testsStarted()
{
  timeStarted_ = static_cast<size_t>(GetPlatformSpecificTimeInMillis());
  output_.printTestsStarted();
}

void
TestResult::testsEnded()
{
  auto timeEnded = static_cast<size_t>(GetPlatformSpecificTimeInMillis());
  totalExecutionTime_ = timeEnded - timeStarted_;
  output_.printTestsEnded(*this);
}

size_t
TestResult::getTotalExecutionTime() const
{
  return totalExecutionTime_;
}

void
TestResult::setTotalExecutionTime(size_t exTime)
{
  totalExecutionTime_ = exTime;
}

size_t
TestResult::getCurrentTestTotalExecutionTime() const
{
  return currentTestTotalExecutionTime_;
}

size_t
TestResult::getCurrentGroupTotalExecutionTime() const
{
  return currentGroupTotalExecutionTime_;
}

} // namespace cpputest
