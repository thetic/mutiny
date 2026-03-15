#ifndef INCLUDED_CPPUTEST_COMMANDLINETESTRUNNER_HPP
#define INCLUDED_CPPUTEST_COMMANDLINETESTRUNNER_HPP

#include "CppUTest/CommandLineArguments.hpp"
#include "CppUTest/TestFilter.hpp"
#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestOutput.hpp"

namespace cpputest {

class TestRegistry;

class CommandLineTestRunner
{
public:
  static int RunAllTests(int argc, const char* const* argv);
  static int RunAllTests(int argc, char** argv);

  CommandLineTestRunner(int argc,
      const char* const* argv,
      TestRegistry* registry);
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
