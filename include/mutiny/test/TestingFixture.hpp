#ifndef INCLUDED_MUTINY_TEST_TESTINGFIXTURE_HPP
#define INCLUDED_MUTINY_TEST_TESTINGFIXTURE_HPP

#include "mutiny/test/Registry.hpp"
#include "mutiny/test/Shell.hpp"
#include "mutiny/test/StringBufferOutput.hpp"

#include "mutiny/export.h"

namespace mu {
namespace tiny {
namespace test {

class ExecFunction;
class ExecFunctionShell;

class MUTINY_EXPORT TestingFixture
{
public:
  TestingFixture();
  virtual ~TestingFixture();
  void flush_output_and_reset_result();

  void add_test(Shell* test);
  void install_plugin(Plugin* plugin);

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
  Registry* get_registry();

  bool has_test_failed();
  void assert_print_contains(const String& contains);
  void assert_print_contains_not(const String& contains);
  void check_test_fails_with_proper_test_location(
      const char* text,
      const char* file,
      size_t line
  );

  static void line_executed_after_check();

protected:
  void replace_output(StringBufferOutput* new_output);

private:
  void clear_exec_function();

  static bool line_of_code_executed_after_check_;

  Registry* registry_;
  ExecFunctionShell* gen_test_;
  bool owns_exec_function_;
  StringBufferOutput* output_;
  Result* result_;
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
