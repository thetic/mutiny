#include "CppUTest/TestOutput.hpp"

#include "CppUTest/TestHarness.hpp"

namespace cpputest {

TestOutput::TestOutput()
  : dotCount_(0)
  , verbose_(level_quiet)
  , color_(false)
  , progressIndication_(".")
{
}

TestOutput::~TestOutput() {}

void
TestOutput::verbose(VerbosityLevel level)
{
  verbose_ = level;
}

void
TestOutput::color()
{
  color_ = true;
}

void
TestOutput::print(const char* str)
{
  printBuffer(str);
}

void
TestOutput::print(long n)
{
  print(StringFrom(n).c_str());
}

void
TestOutput::print(size_t n)
{
  print(StringFrom(n).c_str());
}

void
TestOutput::printDouble(double d)
{
  print(StringFrom(d).c_str());
}

TestOutput&
operator<<(TestOutput& p, const char* s)
{
  p.print(s);
  return p;
}

TestOutput&
operator<<(TestOutput& p, long int i)
{
  p.print(i);
  return p;
}

void
TestOutput::printCurrentTestStarted(const TestShell& test)
{
  if (verbose_ > level_quiet)
    print(test.getFormattedName().c_str());

  if (test.willRun()) {
    setProgressIndicator(".");
  } else {
    setProgressIndicator("!");
  }
}

void
TestOutput::printCurrentTestEnded(const TestResult& res)
{
  if (verbose_ > level_quiet) {
    print(" - ");
    print(res.getCurrentTestTotalExecutionTime());
    print(" ms\n");
  } else {
    printProgressIndicator();
  }
}

void
TestOutput::printProgressIndicator()
{
  print(progressIndication_);
  if (++dotCount_ % 50 == 0)
    print("\n");
}

void
TestOutput::setProgressIndicator(const char* indicator)
{
  progressIndication_ = indicator;
}

void
TestOutput::printTestsStarted()
{
}

void
TestOutput::printCurrentGroupStarted(const TestShell& /*test*/)
{
}

void
TestOutput::printCurrentGroupEnded(const TestResult& /*res*/)
{
}

void
TestOutput::printTestsEnded(const TestResult& result)
{
  print("\n");
  const bool isFailure = result.isFailure();
  const size_t failureCount = result.getFailureCount();
  if (isFailure) {
    if (color_) {
      print("\033[31;1m");
    }
    print("Errors (");
    if (failureCount > 0) {
      print(failureCount);
      print(" failures, ");
    } else {
      print("ran nothing, ");
    }
  } else {
    if (color_) {
      print("\033[32;1m");
    }
    print("OK (");
  }
  print(result.getTestCount());
  print(" tests, ");
  print(result.getRunCount());
  print(" ran, ");
  print(result.getCheckCount());
  print(" checks, ");
  print(result.getIgnoredCount());
  print(" ignored, ");
  print(result.getFilteredOutCount());
  print(" filtered out, ");
  print(result.getTotalExecutionTime());
  print(" ms)");
  if (color_) {
    print("\033[m");
  }
  if (isFailure && failureCount == 0) {
    print("\nNote: test run failed because no tests were run or ignored. "
          "Assuming "
          "something went wrong. "
          "This often happens because of linking errors or typos in test "
          "filter.");
  }
  print("\n\n");

  dotCount_ = 0;
}

void
TestOutput::printTestRun(size_t number, size_t total)
{
  if (total > 1) {
    print("Test run ");
    print(number);
    print(" of ");
    print(total);
    print("\n");
  }
}

void
TestOutput::printFailure(const TestFailure& failure)
{
  if (failure.isOutsideTestFile() || failure.isInHelperFunction())
    printFileAndLineForTestAndFailure(failure);
  else
    printFileAndLineForFailure(failure);

  printFailureMessage(failure.getMessage());
}

void
TestOutput::printFileAndLineForTestAndFailure(const TestFailure& failure)
{
  printErrorInFileOnLineFormattedForWorkingEnvironment(
      failure.getTestFileName(), failure.getTestLineNumber());
  printFailureInTest(failure.getTestName());
  printErrorInFileOnLineFormattedForWorkingEnvironment(
      failure.getFileName(), failure.getFailureLineNumber());
}

void
TestOutput::printFileAndLineForFailure(const TestFailure& failure)
{
  printErrorInFileOnLineFormattedForWorkingEnvironment(
      failure.getFileName(), failure.getFailureLineNumber());
  printFailureInTest(failure.getTestName());
}

void
TestOutput::printFailureInTest(String testName)
{
  print(" Failure in ");
  print(testName.c_str());
}

void
TestOutput::printFailureMessage(String reason)
{
  print("\n");
  print("\t");
  print(reason.c_str());
  print("\n\n");
}

void
TestOutput::printErrorInFileOnLineFormattedForWorkingEnvironment(String file,
    size_t lineNumber)
{
  print("\n");
  print(file.c_str());
  print(":");
  print(lineNumber);
  print(":");
  print(" error:");
}

void
TestOutput::printVeryVerbose(const char* str)
{
  if (verbose_ == level_veryVerbose)
    printBuffer(str);
}

} // namespace cpputest
