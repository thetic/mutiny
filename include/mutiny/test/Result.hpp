/**
 * @file
 * @brief Accumulates pass/fail/skip counters for a test run.
 *
 * Passed through @ref mu::tiny::test::Registry::run_all_tests() and delegates
 * output to an @ref mu::tiny::test::Output sink.
 */

#ifndef INCLUDED_MUTINY_TEST_RESULT_HPP
#define INCLUDED_MUTINY_TEST_RESULT_HPP

#include "mutiny/export.h"

#include <stddef.h>
#include <stdint.h>

namespace mu {
namespace tiny {
namespace test {

class Failure;
class Output;
class Shell;

/**
 * @brief Collects test run statistics and delegates output to an @ref Output
 * sink.
 *
 * A single Result is created per test run and passed through the @ref Registry
 * and @ref Shell to accumulate counts of tests run, checks made, failures
 * encountered, and execution time.
 */
class MUTINY_EXPORT Result
{
public:
  /**
   * @brief Construct a Result that writes through @p output.
   * @param output  The output sink to use for printing.
   */
  Result(Output& output);
  Result(const Result&) = default;
  virtual ~Result() = default;

  /** @brief Signal that the full test suite is starting. */
  virtual void tests_started();
  /** @brief Signal that the full test suite has finished. */
  virtual void tests_ended();

  /**
   * @brief Signal that a new test group is starting.
   * @param test  A test from the group.
   */
  virtual void current_group_started(Shell* test);
  /**
   * @brief Signal that the current test group has finished.
   * @param test  A test from the group.
   */
  virtual void current_group_ended(Shell* test);

  /**
   * @brief Signal that an individual test is starting.
   * @param test  The test about to run.
   */
  virtual void current_test_started(Shell* test);
  /**
   * @brief Signal that an individual test has finished.
   * @param test  The test that just ran.
   */
  virtual void current_test_ended(Shell* test);

  /** @brief Increment the registered-test counter. */
  virtual void count_test();
  /** @brief Increment the ran-test counter. */
  virtual void count_run();
  /** @brief Increment the assertion counter. */
  virtual void count_check();
  /** @brief Increment the filtered-out counter. */
  virtual void count_filtered_out();
  /** @brief Increment the ignored-test counter. */
  virtual void count_ignored();

  /**
   * @brief Mark the current test as skipped with an optional message.
   * @param message  Human-readable reason (may be empty).
   */
  virtual void skip_test(const char* message);

  /**
   * @brief Record a test failure and increment the failure counter.
   * @param failure  The failure to record.
   */
  virtual void add_failure(const Failure& failure);

  /**
   * @brief Record a test property key/value pair.
   * @param name   Property name.
   * @param value  Property value.
   */
  virtual void add_test_property(const char* name, const char* value);

  /**
   * @brief Print @p text to the output sink.
   * @param text  Null-terminated string to print.
   */
  virtual void print(const char* text);

  /**
   * @brief Print a very-verbose detail line.
   * @param text  Null-terminated string to print.
   */
  virtual void print_very_verbose(const char* text);

  /** @return Total number of registered tests. */
  size_t get_test_count() const { return test_count_; }
  /** @return Number of tests that actually ran. */
  size_t get_run_count() const { return run_count_; }
  /** @return Number of assertions checked. */
  size_t get_check_count() const { return check_count_; }
  /** @return Number of tests filtered out. */
  size_t get_filtered_out_count() const { return filtered_out_count_; }
  /** @return Number of ignored tests. */
  size_t get_ignored_count() const { return ignored_count_; }
  /** @return Number of test failures. */
  size_t get_failure_count() const { return failure_count_; }

  /**
   * @return true if there were any failures, or if no tests ran and none were
   *         ignored.
   */
  bool is_failure() const
  {
    return (get_failure_count() != 0) ||
           (get_run_count() + get_ignored_count() == 0);
  }

  /** @return Total elapsed time (ms) for the entire run. */
  uint_least64_t get_total_execution_time() const;
  /**
   * @brief Set the total elapsed time for the run.
   * @param ex_time  Elapsed milliseconds.
   */
  void set_total_execution_time(uint_least64_t ex_time);

  /** @return Elapsed time (ms) for the most recent test. */
  uint_least64_t get_current_test_total_execution_time() const;
  /** @return Elapsed time (ms) for the most recent test group. */
  uint_least64_t get_current_group_total_execution_time() const;

private:
  Output& output_;
  size_t test_count_{ 0 };
  size_t run_count_{ 0 };
  size_t check_count_{ 0 };
  size_t failure_count_{ 0 };
  size_t filtered_out_count_{ 0 };
  size_t ignored_count_{ 0 };
  uint_least64_t total_execution_time_{ 0 };
  uint_least64_t time_started_{ 0 };
  uint_least64_t current_test_time_started_{ 0 };
  uint_least64_t current_test_total_execution_time_{ 0 };
  uint_least64_t current_group_time_started_{ 0 };
  uint_least64_t current_group_total_execution_time_{ 0 };
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
