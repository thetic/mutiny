#ifndef INCLUDED_CPPUTEST_COMMANDLINETESTRUNNER_HPP
#define INCLUDED_CPPUTEST_COMMANDLINETESTRUNNER_HPP

#include "CppUTest/CommandLineArguments.hpp"
#include "CppUTest/CppUTest.hpp"
#include "CppUTest/TestFilter.hpp"
#include "CppUTest/TestOutput.hpp"

namespace cpputest {

class TestRegistry;

class CommandLineTestRunner
{
public:
  static int run_all_tests(int argc, const char* const* argv);
  static int run_all_tests(int argc, char** argv);

  CommandLineTestRunner(int argc,
      const char* const* argv,
      TestRegistry* registry);
  virtual ~CommandLineTestRunner();

  int run_all_tests_main();

protected:
  virtual TestOutput* create_team_city_output();
  virtual TestOutput* create_j_unit_output(const String& package_name);
  virtual TestOutput* create_console_output();
  virtual TestOutput* create_composite_output(TestOutput* output_one,
      TestOutput* output_two);

  TestOutput* output_;

private:
  CommandLineArguments* arguments_;
  TestRegistry* registry_;

  bool parse_arguments(TestPlugin*);
  int run_all_tests();
  void initialize_test_run();
};

} // namespace cpputest

#define DEF_PLUGIN_SET_POINTER "SetPointerPlugin"

#endif
