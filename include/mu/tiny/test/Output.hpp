/**
 * @file
 * @brief Abstract output interface for test result reporting.
 *
 * Kept stream-free to reduce footprint and support limited compilers. Concrete
 * implementations: @ref mu::tiny::test::ConsoleOutput, @ref
 * mu::tiny::test::StringBufferOutput, and @ref
 * mu::tiny::test::JUnitOutputPlugin.
 */

#ifndef INCLUDED_MU_TINY_TEST_OUTPUT_HPP
#define INCLUDED_MU_TINY_TEST_OUTPUT_HPP

#include "mu/tiny/String.hpp"
#include "mu/tiny/export.h"

#include <stdint.h>

namespace mu {
namespace tiny {
namespace test {

class Shell;
class Failure;
class Result;

/**
 * @brief Abstract base class for all test output sinks.
 *
 * Implement @ref print_buffer() in a subclass to direct output anywhere. All
 * higher-level printing methods (@ref print_failure(), @ref print_test_run(),
 * etc.) are built on top of @ref print_buffer(). The static function pointers
 * (@ref stdout_, @ref fopen_, @ref fputs_, @ref fclose_) allow
 * @ref ConsoleOutput to redirect file I/O without a virtual dispatch.
 */
class MUTINY_EXPORT Output
{
public:
  /**
   * @brief Controls how much detail is printed per test.
   */
  enum class MutinyVerbosityLevel
  {
    quiet,       ///< Only failures and the summary line.
    verbose,     ///< One line per test.
    very_verbose ///< Full per-test detail including test properties.
  };

  explicit Output() = default;
  virtual ~Output() = default;

  /** @brief Called once before any tests run. */
  virtual void print_tests_started();

  /**
   * @brief Called once after all tests have run.
   * @param result  The final result summary.
   */
  virtual void print_tests_ended(const Result& result);

  /**
   * @brief Called before each test body begins.
   * @param test  The test about to run.
   */
  virtual void print_current_test_started(const Shell& test);

  /**
   * @brief Called after each test body completes.
   * @param res  The result state at the end of the test.
   */
  virtual void print_current_test_ended(const Result& res);

  /**
   * @brief Called when execution enters a new test group.
   * @param test  A test from the group that is starting.
   */
  virtual void print_current_group_started(const Shell& test);

  /**
   * @brief Called when execution leaves a test group.
   * @param res  The result state at the end of the group.
   */
  virtual void print_current_group_ended(const Result& res);

  /**
   * @brief Set the verbosity level.
   * @param level  Desired verbosity.
   */
  virtual void verbose(MutinyVerbosityLevel level);

  /** @brief Enable coloured output. */
  virtual void color();

  /**
   * @brief Write @p s directly to the underlying output medium.
   *
   * This is the only pure-virtual method; all other print methods call it.
   *
   * @param s  Null-terminated string to write.
   */
  virtual void print_buffer(const char* s) = 0;

  /**
   * @brief Print a null-terminated string.
   * @param s  String to print.
   */
  virtual void print(const char* s);

  /**
   * @brief Print a signed integer.
   * @param n  Value to print.
   */
  virtual void print(int n);

  /**
   * @brief Print an unsigned integer.
   * @param n  Value to print.
   */
  virtual void print(unsigned int n);

  /**
   * @brief Print a signed integer.
   * @param n  Value to print.
   */
  virtual void print(long n);

  /**
   * @brief Print a floating-point value.
   * @param d  Value to print.
   */
  virtual void print_double(double d);

  /**
   * @brief Print a test failure message.
   * @param failure  The failure to describe.
   */
  virtual void print_failure(const Failure& failure);

  /**
   * @brief Print a test property key/value pair.
   * @param name   Property name.
   * @param value  Property value.
   */
  virtual void print_test_property(const char* name, const char* value);

  /**
   * @brief Mark the current test as skipped.
   * @param message  Human-readable reason (may be empty).
   */
  virtual void print_skipped(const char* message);

  /**
   * @brief Print the current repetition progress.
   * @param number  Current repetition (1-based).
   * @param total   Total number of repetitions.
   */
  virtual void print_test_run(unsigned int number, unsigned int total);

  /**
   * @brief Override the per-test progress indicator character.
   * @param indicator  Null-terminated indicator string (default: `.`).
   */
  virtual void set_progress_indicator(const char* indicator);

  /**
   * @brief Print a very-verbose detail line.
   * @param s  Detail string to print.
   */
  virtual void print_very_verbose(const char* s);

  /**
   * @return true if this output needs a @ref ConsoleOutput companion for
   * interactive output (e.g. @ref JUnitOutputPlugin paired with a console).
   */
  virtual bool needs_console_companion() const { return false; }

  /** @brief Flush any buffered output. */
  virtual void flush() {}

  /** @brief Replaceable stdout handle used by ConsoleOutput. */
  using File = void*;
  static File stdout_;

  /** @brief Replaceable fopen function used by ConsoleOutput. */
  using FOpenFunc = File (*)(const char* filename, const char* flag);
  static FOpenFunc fopen_;

  /** @brief Replaceable fputs function used by ConsoleOutput. */
  using FPutsFunc = void (*)(const char* str, File file);
  static FPutsFunc fputs_;

  /** @brief Replaceable fclose function used by ConsoleOutput. */
  using FCloseFunc = void (*)(File file);
  static FCloseFunc fclose_;

protected:
  virtual void print_progress_indicator();
  void print_file_and_line_for_test_and_failure(const Failure& failure);
  void print_file_and_line_for_failure(const Failure& failure);
  void print_failure_in_test(const String& test_name);
  void print_failure_message(const String& reason);
  void print_error_in_file_on_line_formatted_for_working_environment(
      const String& test_file,
      int_least32_t line_number
  );

  Output(const Output&);
  Output& operator=(const Output&);

  int dot_count_{ 0 };
  MutinyVerbosityLevel verbose_{ MutinyVerbosityLevel::quiet };
  bool color_{ false };
  const char* progress_indication_{ "." };
};

/** @brief Stream-style helper: print a string to an output sink; returns the
 * sink for chaining. */
MUTINY_EXPORT Output& operator<<(Output& lhs, const char* rhs);

/** @brief Stream-style helper: print a long integer to an output sink; returns
 * the sink for chaining. */
MUTINY_EXPORT Output& operator<<(Output& lhs, long rhs);

} // namespace test
} // namespace tiny
} // namespace mu

#endif
