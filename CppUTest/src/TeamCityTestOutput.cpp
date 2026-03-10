#include "CppUTest/TeamCityTestOutput.hpp"

#include "CppUTest/TestHarness.hpp"

namespace cpputest {

TeamCityTestOutput::TeamCityTestOutput()
  : currtest_(nullptr)
  , currGroup_()
{
}

TeamCityTestOutput::~TeamCityTestOutput() {}

void
TeamCityTestOutput::printCurrentTestStarted(const TestShell& test)
{
  print("##teamcity[testStarted name='");
  printEscaped(test.getName().c_str());
  print("']\n");
  if (!test.willRun()) {
    print("##teamcity[testIgnored name='");
    printEscaped(test.getName().c_str());
    print("']\n");
  }
  currtest_ = &test;
}

void
TeamCityTestOutput::printCurrentTestEnded(const TestResult& res)
{
  if (!currtest_)
    return;

  print("##teamcity[testFinished name='");
  printEscaped(currtest_->getName().c_str());
  print("' duration='");
  print(res.getCurrentTestTotalExecutionTime());
  print("']\n");
}

void
TeamCityTestOutput::printCurrentGroupStarted(const TestShell& test)
{
  currGroup_ = test.getGroup();
  print("##teamcity[testSuiteStarted name='");
  printEscaped(currGroup_.c_str());
  print("']\n");
}

void
TeamCityTestOutput::printCurrentGroupEnded(const TestResult& /*res*/)
{
  if (currGroup_ == "")
    return;

  print("##teamcity[testSuiteFinished name='");
  printEscaped(currGroup_.c_str());
  print("']\n");
}

void
TeamCityTestOutput::printEscaped(const char* s)
{
  while (*s) {
    char str[3];
    if ((*s == '\'') || (*s == '|') || (*s == '[') || (*s == ']')) {
      str[0] = '|';
      str[1] = *s;
      str[2] = 0;
    } else if (*s == '\r') {
      str[0] = '|';
      str[1] = 'r';
      str[2] = 0;
    } else if (*s == '\n') {
      str[0] = '|';
      str[1] = 'n';
      str[2] = 0;
    } else {
      str[0] = *s;
      str[1] = 0;
    }
    printBuffer(str);
    s++;
  }
}

void
TeamCityTestOutput::printFailure(const TestFailure& failure)
{
  print("##teamcity[testFailed name='");
  printEscaped(failure.getTestNameOnly().c_str());
  print("' message='");
  if (failure.isOutsideTestFile() || failure.isInHelperFunction()) {
    print("TEST failed (");
    print(failure.getTestFileName().c_str());
    print(":");
    print(failure.getTestLineNumber());
    print("): ");
  }

  printEscaped(failure.getFileName().c_str());
  print(":");
  print(failure.getFailureLineNumber());

  print("' details='");
  printEscaped(failure.getMessage().c_str());
  print("']\n");
}

} // namespace cpputest
