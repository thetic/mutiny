#ifndef D_CommandLineTestRunner_H
#define D_CommandLineTestRunner_H

#include "CppUTest/CommandLineArguments.hpp"
#include "CppUTest/TestFilter.hpp"
#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestOutput.hpp"

namespace cpputest {

class TestRegistry;

class CommandLineTestRunner
{
public:
  static int RunAllTests(int ac, const char* const* av);
  static int RunAllTests(int ac, char** av);

  CommandLineTestRunner(int ac, const char* const* av, TestRegistry* registry);
  virtual ~CommandLineTestRunner();

  int runAllTestsMain();

protected:
  virtual TestOutput* createTeamCityOutput();
  virtual TestOutput* createJUnitOutput(const String& packageName);
  virtual TestOutput* createConsoleOutput();
  virtual TestOutput* createCompositeOutput(TestOutput* outputOne,
      TestOutput* outputTwo);

  TestOutput* output_;

private:
  CommandLineArguments* arguments_;
  TestRegistry* registry_;

  bool parseArguments(TestPlugin*);
  int runAllTests();
  void initializeTestRun();
};

} // namespace cpputest

#define DEF_PLUGIN_SET_POINTER "SetPointerPlugin"

#endif
