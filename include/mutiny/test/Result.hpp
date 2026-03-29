///////////////////////////////////////////////////////////////////////////////
//
// A Result is a collection of the history of some test runs.  Right now
// it just collects failures.  Really it just prints the failures.
//

#ifndef INCLUDED_MUTINY_TEST_RESULT_HPP
#define INCLUDED_MUTINY_TEST_RESULT_HPP

#include "mutiny/export.h"

#include <stddef.h>

namespace mu {
namespace tiny {
namespace test {

class Failure;
class Output;
class Shell;

class MUTINY_EXPORT Result
{
public:
  Result(Output&);
  Result(const Result&) = default;
  virtual ~Result() = default;

  virtual void tests_started();
  virtual void tests_ended();
  virtual void current_group_started(Shell* test);
  virtual void current_group_ended(Shell* test);
  virtual void current_test_started(Shell* test);
  virtual void current_test_ended(Shell* test);

  virtual void count_test();
  virtual void count_run();
  virtual void count_check();
  virtual void count_filtered_out();
  virtual void count_ignored();
  virtual void add_failure(const Failure& failure);
  virtual void add_test_property(const char* name, const char* value);
  virtual void print(const char* text);
  virtual void print_very_verbose(const char* text);

  size_t get_test_count() const { return test_count_; }
  size_t get_run_count() const { return run_count_; }
  size_t get_check_count() const { return check_count_; }
  size_t get_filtered_out_count() const { return filtered_out_count_; }
  size_t get_ignored_count() const { return ignored_count_; }
  size_t get_failure_count() const { return failure_count_; }

  bool is_failure() const
  {
    return (get_failure_count() != 0) ||
           (get_run_count() + get_ignored_count() == 0);
  }

  size_t get_total_execution_time() const;
  void set_total_execution_time(size_t ex_time);

  size_t get_current_test_total_execution_time() const;
  size_t get_current_group_total_execution_time() const;

private:
  Output& output_;
  size_t test_count_{ 0 };
  size_t run_count_{ 0 };
  size_t check_count_{ 0 };
  size_t failure_count_{ 0 };
  size_t filtered_out_count_{ 0 };
  size_t ignored_count_{ 0 };
  size_t total_execution_time_{ 0 };
  size_t time_started_{ 0 };
  size_t current_test_time_started_{ 0 };
  size_t current_test_total_execution_time_{ 0 };
  size_t current_group_time_started_{ 0 };
  size_t current_group_total_execution_time_{ 0 };
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
