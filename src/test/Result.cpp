#include "mutiny/test/Result.hpp"

#include "mutiny/test/Failure.hpp"
#include "mutiny/test/Output.hpp"

#include "mutiny/time.hpp"

#include <stdint.h>

namespace mu {
namespace tiny {
namespace test {

Result::Result(Output& p)
  : output_(p)
{
}

void Result::current_group_started(Shell* test)
{
  output_.print_current_group_started(*test);
  current_group_time_started_ = get_time_in_millis();
}

void Result::current_group_ended(Shell* /*test*/)
{
  current_group_total_execution_time_ =
      get_time_in_millis() - current_group_time_started_;
  output_.print_current_group_ended(*this);
}

void Result::current_test_started(Shell* test)
{
  output_.print_current_test_started(*test);
  current_test_time_started_ = get_time_in_millis();
}

void Result::print(const char* text)
{
  output_.print(text);
}

void Result::print_very_verbose(const char* text)
{
  output_.print_very_verbose(text);
}

void Result::current_test_ended(Shell* /*test*/)
{
  current_test_total_execution_time_ =
      get_time_in_millis() - current_test_time_started_;
  output_.print_current_test_ended(*this);
}

void Result::add_failure(const Failure& failure)
{
  output_.print_failure(failure);
  failure_count_++;
}

void Result::add_test_property(const char* name, const char* value)
{
  output_.print_test_property(name, value);
}

void Result::count_test()
{
  test_count_++;
}

void Result::count_run()
{
  run_count_++;
}

void Result::count_check()
{
  check_count_++;
}

void Result::count_filtered_out()
{
  filtered_out_count_++;
}

void Result::count_ignored()
{
  ignored_count_++;
}

void Result::skip_test(const char* message)
{
  output_.print_skipped(message);
}

void Result::tests_started()
{
  time_started_ = get_time_in_millis();
  output_.print_tests_started();
}

void Result::tests_ended()
{
  auto time_ended = get_time_in_millis();
  total_execution_time_ = time_ended - time_started_;
  output_.print_tests_ended(*this);
}

uint_least64_t Result::get_total_execution_time() const
{
  return total_execution_time_;
}

void Result::set_total_execution_time(uint_least64_t ex_time)
{
  total_execution_time_ = ex_time;
}

uint_least64_t Result::get_current_test_total_execution_time() const
{
  return current_test_total_execution_time_;
}

uint_least64_t Result::get_current_group_total_execution_time() const
{
  return current_group_total_execution_time_;
}

} // namespace test
} // namespace tiny
} // namespace mu
