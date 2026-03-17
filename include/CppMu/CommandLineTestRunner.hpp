#ifndef INCLUDED_CPPMU_COMMANDLINETESTRUNNER_HPP
#define INCLUDED_CPPMU_COMMANDLINETESTRUNNER_HPP

#include "CppMu/CommandLineArguments.hpp"
#include "CppMu/CppMu.hpp"
#include "CppMu/TestFilter.hpp"
#include "CppMu/TestOutput.hpp"

namespace cppmu {

class TestRegistry;

class CommandLineTestRunner
{
public:
  static int run_all_tests(int argc, const char* const* argv);
  static int run_all_tests(int argc, char** argv);

  CommandLineTestRunner(
      int argc,
      const char* const* argv,
      TestRegistry* registry
  );
  virtual ~CommandLineTestRunner();

  int run_all_tests_main();

protected:
  virtual TestOutput* create_console_output();
  virtual TestOutput* create_composite_output(
      TestOutput* output_one,
      TestOutput* output_two
  );

  TestOutput* output_{ nullptr };

private:
  CommandLineArguments* arguments_{ nullptr };
  TestRegistry* registry_;

  bool parse_arguments(TestPlugin*);
  int run_all_tests();
  void initialize_test_run();
};

} // namespace cppmu

#endif
