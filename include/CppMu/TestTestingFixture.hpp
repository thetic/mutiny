#ifndef INCLUDED_CPPMU_TESTTESTINGFIXTURE_HPP
#define INCLUDED_CPPMU_TESTTESTINGFIXTURE_HPP

#include "CppMu/StringBufferTestOutput.hpp"
#include "CppMu/TestRegistry.hpp"
#include "CppMu/TestShell.hpp"

namespace cppmu {

class ExecFunction;
class ExecFunctionTestShell;

class TestTestingFixture
{
public:
  TestTestingFixture();
  virtual ~TestTestingFixture();
  void flush_output_and_reset_result();

  void add_test(TestShell* test);
  void install_plugin(TestPlugin* plugin);

  void set_test_function(void (*test_function)());
  void set_test_function(ExecFunction* test_function);
  void set_setup(void (*setup_function)());
  void set_teardown(void (*teardown_function)());

  void set_output_verbose();

  void run_test_with_method(void (*method)());
  void run_all_tests();

  size_t get_failure_count();
  size_t get_check_count();
  size_t get_ignore_count();
  size_t get_run_count();
  size_t get_test_count();
  const String& get_output();
  TestRegistry* get_registry();

  bool has_test_failed();
  void assert_print_contains(const String& contains);
  void assert_print_contains_not(const String& contains);
  void check_test_fails_with_proper_test_location(const char* text,
      const char* file,
      size_t line);

  static void line_executed_after_check();

private:
  void clear_exec_function();

  static bool line_of_code_executed_after_check_;

  TestRegistry* registry_;
  ExecFunctionTestShell* gen_test_;
  bool owns_exec_function_;
  StringBufferTestOutput* output_;
  TestResult* result_;
};

} // namespace cppmu

#endif
