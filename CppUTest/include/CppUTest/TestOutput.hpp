#ifndef D_TestOutput_h
#define D_TestOutput_h

///////////////////////////////////////////////////////////////////////////////
//
//  This is a minimal printer interface.
//  We kept streams out to keep footprint small, and so the test
//  harness could be used with less capable compilers so more
//  platforms could use this test harness
//
///////////////////////////////////////////////////////////////////////////////

#include "CppUTest/String.hpp"

namespace cpputest {

class TestShell;
class TestFailure;
class TestResult;

class TestOutput
{
public:
  enum VerbosityLevel
  {
    level_quiet,
    level_verbose,
    level_veryVerbose
  };

  explicit TestOutput();
  virtual ~TestOutput();

  virtual void printTestsStarted();
  virtual void printTestsEnded(const TestResult& result);
  virtual void printCurrentTestStarted(const TestShell& test);
  virtual void printCurrentTestEnded(const TestResult& res);
  virtual void printCurrentGroupStarted(const TestShell& test);
  virtual void printCurrentGroupEnded(const TestResult& res);

  virtual void verbose(VerbosityLevel level);
  virtual void color();
  virtual void printBuffer(const char*) = 0;
  virtual void print(const char*);
  virtual void print(long);
  virtual void print(size_t);
  virtual void printDouble(double);
  virtual void printFailure(const TestFailure& failure);
  virtual void printTestRun(size_t number, size_t total);
  virtual void setProgressIndicator(const char*);

  virtual void printVeryVerbose(const char*);

  virtual void flush() = 0;

protected:
  virtual void printProgressIndicator();
  void printFileAndLineForTestAndFailure(const TestFailure& failure);
  void printFileAndLineForFailure(const TestFailure& failure);
  void printFailureInTest(String testName);
  void printFailureMessage(String reason);
  void printErrorInFileOnLineFormattedForWorkingEnvironment(String testFile,
      size_t lineNumber);

  TestOutput(const TestOutput&);
  TestOutput& operator=(const TestOutput&);

  int dotCount_;
  VerbosityLevel verbose_;
  bool color_;
  const char* progressIndication_;
};

TestOutput&
operator<<(TestOutput&, const char*);
TestOutput&
operator<<(TestOutput&, long);

} // namespace cpputest

#endif
