#include "CppUTest/ExpectFailTestShell.hpp"

#include "CppUTest/StringBufferTestOutput.hpp"
#include "CppUTest/TestFailure.hpp"
#include "CppUTest/TestPlugin.hpp"
#include "CppUTest/TestResult.hpp"

namespace cpputest {

ExpectFailTestShell::ExpectFailTestShell() {}

ExpectFailTestShell::ExpectFailTestShell(const char* groupName,
    const char* testName,
    const char* fileName,
    size_t lineNumber)
  : TestShell(groupName, testName, fileName, lineNumber)
{
}

ExpectFailTestShell::~ExpectFailTestShell() {}

String
ExpectFailTestShell::getMacroName() const
{
  return "EXPECT_FAIL_TEST";
}

void
ExpectFailTestShell::runOneTest(TestPlugin* plugin, TestResult& result)
{
  StringBufferTestOutput shadowOutput;
  TestResult shadowResult(shadowOutput);
  TestShell::runOneTest(plugin, shadowResult);

  result.countRun();
  if (shadowResult.getFailureCount() == 0) {
    result.addFailure(FailFailure(this,
        getFile().c_str(),
        getLineNumber(),
        "EXPECT_FAIL_TEST: test was expected to fail but it passed"));
  }
}

} // namespace cpputest
