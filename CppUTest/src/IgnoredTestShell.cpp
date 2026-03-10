#include "CppUTest/IgnoredTestShell.hpp"

#include "CppUTest/TestPlugin.hpp"
#include "CppUTest/TestResult.hpp"

namespace cpputest {

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

} // namespace cpputest
