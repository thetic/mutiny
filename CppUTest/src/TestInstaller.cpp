#include "CppUTest/TestInstaller.hpp"

#include "CppUTest/TestRegistry.hpp"
#include "CppUTest/TestShell.hpp"

namespace cpputest {

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
