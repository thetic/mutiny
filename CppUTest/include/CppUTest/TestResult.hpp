///////////////////////////////////////////////////////////////////////////////
//
// A TestResult is a collection of the history of some test runs.  Right now
// it just collects failures.  Really it just prints the failures.
//

#ifndef INCLUDED_CPPUTEST_TESTRESULT_HPP
#define INCLUDED_CPPUTEST_TESTRESULT_HPP

#include <stddef.h>

namespace cpputest {

class TestFailure;
class TestOutput;
class TestShell;

class TestResult
{
public:
  TestResult(TestOutput&);
  TestResult(const TestResult&) = default;
  virtual ~TestResult();

  virtual void testsStarted();
  virtual void testsEnded();
  virtual void currentGroupStarted(TestShell* test);
  virtual void currentGroupEnded(TestShell* test);
  virtual void currentTestStarted(TestShell* test);
  virtual void currentTestEnded(TestShell* test);

  virtual void countTest();
  virtual void countRun();
  virtual void countCheck();
  virtual void countFilteredOut();
  virtual void countIgnored();
  virtual void addFailure(const TestFailure& failure);
  virtual void print(const char* text);
  virtual void printVeryVerbose(const char* text);

  size_t getTestCount() const { return testCount_; }
  size_t getRunCount() const { return runCount_; }
  size_t getCheckCount() const { return checkCount_; }
  size_t getFilteredOutCount() const { return filteredOutCount_; }
  size_t getIgnoredCount() const { return ignoredCount_; }
  size_t getFailureCount() const { return failureCount_; }

  bool isFailure() const
  {
    return (getFailureCount() != 0) || (getRunCount() + getIgnoredCount() == 0);
  }

  size_t getTotalExecutionTime() const;
  void setTotalExecutionTime(size_t exTime);

  size_t getCurrentTestTotalExecutionTime() const;
  size_t getCurrentGroupTotalExecutionTime() const;

private:
  TestOutput& output_;
  size_t testCount_;
  size_t runCount_;
  size_t checkCount_;
  size_t failureCount_;
  size_t filteredOutCount_;
  size_t ignoredCount_;
  size_t totalExecutionTime_;
  size_t timeStarted_;
  size_t currentTestTimeStarted_;
  size_t currentTestTotalExecutionTime_;
  size_t currentGroupTimeStarted_;
  size_t currentGroupTotalExecutionTime_;
};

} // namespace cpputest

#endif
