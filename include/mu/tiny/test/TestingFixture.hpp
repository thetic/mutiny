/**
 * @file
 * @brief Self-contained test harness for testing the framework itself.
 *
 * Owns a private @ref mu::tiny::test::Registry, @ref mu::tiny::test::Output,
 * and @ref mu::tiny::test::Result so that tests run through it do not affect
 * global state. See also @ref mu::tiny::test::ExecFunction for the callable
 * interface it accepts.
 */

#ifndef INCLUDED_MU_TINY_TEST_TESTINGFIXTURE_HPP
#define INCLUDED_MU_TINY_TEST_TESTINGFIXTURE_HPP

#include "mu/tiny/test/ExecFunction.hpp"
#include "mu/tiny/test/Registry.hpp"
#include "mu/tiny/test/Shell.hpp"
#include "mu/tiny/test/StringBufferOutput.hpp"

#include "mu/tiny/export.h"

namespace mu {
namespace tiny {
namespace test {

class ExecFunctionShell;

/**
 * @brief Encapsulates an isolated @ref Registry, @ref Output, and @ref Result
 * for running tests inside other tests.
 *
 * TestingFixture is used to test the mutiny framework itself. It owns a
 * private @ref Registry and @ref StringBufferOutput so that tests run through
 * it do not affect the global @ref Registry or console output.
 */
class MUTINY_EXPORT TestingFixture
{
public:
  /** @brief Construct a fixture with its own Registry, Output, and Result. */
  TestingFixture();
  virtual ~TestingFixture();

  /** @brief Flush buffered output and reset the Result counters. */
  void flush_output_and_reset_result();

  /**
   * @brief Add @p test to the private Registry.
   * @param test  Shell to register.
   */
  void add_test(Shell* test);

  /**
   * @brief Install @p plugin into the private Registry.
   * @param plugin  Plugin to install.
   */
  void install_plugin(Plugin* plugin);

  /**
   * @brief Set the test body to a plain function pointer.
   * @param test_function  Function to invoke as the test body.
   */
  void set_test_function(void (*test_function)());

  /**
   * @brief Set the test body to an ExecFunction.
   * @param test_function  ExecFunction whose exec() is the test body.
   */
  void set_test_function(ExecFunction* test_function);

  /**
   * @brief Set the setup function run before each test body.
   * @param setup_function  Function to invoke as setup.
   */
  void set_setup(void (*setup_function)());

  /**
   * @brief Set the teardown function run after each test body.
   * @param teardown_function  Function to invoke as teardown.
   */
  void set_teardown(void (*teardown_function)());

  /** @brief Enable verbose output on the private Output. */
  void set_output_verbose();

  /**
   * @brief Run the single test set by set_test_function() with @p method as
   * its body.
   * @param method  Body to execute inside the test.
   */
  void run_test_with_method(void (*method)());

  /** @brief Run all tests registered in the private Registry. */
  void run_all_tests();

  /** @return Number of failures recorded so far. */
  unsigned int get_failure_count();
  /** @return Number of assertions checked so far. */
  unsigned int get_check_count();
  /** @return Number of ignored tests. */
  unsigned int get_skip_count();
  /** @return Number of tests that actually ran. */
  unsigned int get_run_count();
  /** @return Total number of registered tests. */
  unsigned int get_test_count();
  /** @return The accumulated output string. */
  const String& get_output();
  /** @return The private Registry. */
  Registry* get_registry();

  /** @return true if the last test run recorded at least one failure. */
  bool has_test_failed();

  /**
   * @brief Assert that the output buffer contains @p contains.
   * @param contains  Substring expected in the output.
   */
  void assert_print_contains(const String& contains);

  /**
   * @brief Assert that the output buffer does not contain @p contains.
   * @param contains  Substring that must be absent from the output.
   */
  void assert_print_contains_not(const String& contains);

  /**
   * @brief Run a test that is expected to fail and verify the failure message.
   *
   * @param text  Expected substring of the failure message.
   * @param file  Expected source file in the failure location.
   * @param line  Expected line number in the failure location.
   */
  void check_test_fails_with_proper_test_location(
      const char* text,
      const char* file,
      int_least32_t line
  );

  /**
   * @brief Sentinel that must be called to prove execution continued past a
   * check.
   *
   * Call this immediately after an assertion macro in a test that uses
   * TestingFixture; the fixture verifies it was reached.
   */
  static void line_executed_after_check();

protected:
  /**
   * @brief Swap in a different output object.
   * @param new_output  New output to use; the fixture takes ownership.
   */
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
