/**
 * @file
 * @brief Entry point for running all registered tests from a main() function.
 *
 * Supported command-line flags:
 * - @c -v          verbose output
 * - @c -g name     run only tests in the named group
 * - @c -n name     run only tests with the given name
 * - @c -r count    repeat the test run @c count times
 * - @c -s          run each test in a separate subprocess (crash isolation)
 */

#ifndef INCLUDED_MU_TINY_TEST_COMMANDLINERUNNER_HPP
#define INCLUDED_MU_TINY_TEST_COMMANDLINERUNNER_HPP

#include "mu/tiny/test/CommandLineArguments.hpp"
#include "mu/tiny/test/Output.hpp"

#include "mu/tiny/export.h"

namespace mu {
namespace tiny {
namespace test {

class Plugin;
class Registry;

/**
 * @brief Parses command-line arguments and drives the test run.
 *
 * Construct one, then call @ref run_all_tests_main(), or use the static helper
 * @ref run_all_tests() which does both in one call. Subclass and override
 * @ref create_console_output() or @ref create_composite_output() to supply a
 * custom output formatter.
 */
class MUTINY_EXPORT CommandLineRunner
{
public:
  /**
   * @brief Parse @p argv and run all registered tests; return a process exit
   * code.
   *
   * This overload accepts @c const char* const* and is suitable for use with
   * main() signatures that take @c const argv.
   *
   * @param argc  Argument count from main().
   * @param argv  Argument vector from main().
   * @return 0 if all tests passed, non-zero otherwise.
   */
  static int run_all_tests(int argc, const char* const* argv);

  /**
   * @brief Parse @p argv and run all registered tests; return a process exit
   * code.
   *
   * Overload for the common @c int main(int argc, char** argv) signature.
   *
   * @param argc  Argument count from main().
   * @param argv  Argument vector from main().
   * @return 0 if all tests passed, non-zero otherwise.
   */
  static int run_all_tests(int argc, char** argv);

  /**
   * @brief Install @p plugin into the current registry before running tests.
   *
   * Equivalent to
   * @code
   * Registry::get_current_registry()->install_plugin(&plugin);
   * @endcode
   * but does not require including @ref mu/tiny/test/Registry.hpp.
   * Call this before @ref run_all_tests().
   *
   * @param plugin  Plugin to install; must outlive the test run.
   */
  static void install_plugin(Plugin& plugin);

  /**
   * @brief Construct a runner bound to a specific registry.
   *
   * Prefer the static run_all_tests() helpers unless you need to supply a
   * custom Registry (e.g. in framework self-tests).
   *
   * @param argc      Argument count.
   * @param argv      Argument vector.
   * @param registry  Registry of tests to run; must outlive this object.
   */
  CommandLineRunner(int argc, const char* const* argv, Registry* registry);
  virtual ~CommandLineRunner();

  /**
   * @brief Parse arguments and run the tests registered in this runner's
   * registry.
   *
   * @return 0 if all tests passed, non-zero otherwise.
   */
  int run_all_tests_main();

protected:
  /**
   * @brief Factory: create the console @ref Output object. Override to replace
   * it.
   */
  virtual Output* create_console_output();

  /**
   * @brief Factory: combine two outputs into a composite output.
   *
   * Called when both console and file output are active simultaneously.
   *
   * @param output_one  First output sink.
   * @param output_two  Second output sink.
   * @return A composite Output that writes to both.
   */
  virtual Output* create_composite_output(
      Output* output_one,
      Output* output_two
  );

  /** @brief The active output object (owned by this runner). */
  Output* output_{ nullptr };

private:
  CommandLineArguments* arguments_{ nullptr };
  Registry* registry_;
  String program_name_;

  bool parse_arguments(Plugin* plugin);
  int run_all_tests();
  void initialize_test_run();
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
