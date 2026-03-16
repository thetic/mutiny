#include "CppMu/TestResult.hpp"

#include "CppMu/CppMu.hpp"
#include "CppMu/TestFailure.hpp"
#include "CppMu/TestOutput.hpp"
#include "CppMu/time.hpp"

namespace cppmu {

TestResult::TestResult(TestOutput& p)
  : output_(p)

{
}

TestResult::~TestResult() {}

void
TestResult::current_group_started(TestShell* test)
{
  output_.print_current_group_started(*test);
  current_group_time_started_ =
      static_cast<size_t>(cppmu::get_time_in_millis());
}

void
TestResult::current_group_ended(TestShell* /*test*/)
{
  current_group_total_execution_time_ =
      static_cast<size_t>(cppmu::get_time_in_millis()) -
      current_group_time_started_;
  output_.print_current_group_ended(*this);
}

void
TestResult::current_test_started(TestShell* test)
{
  output_.print_current_test_started(*test);
  current_test_time_started_ = static_cast<size_t>(cppmu::get_time_in_millis());
}

void
TestResult::print(const char* text)
{
  output_.print(text);
}

void
TestResult::print_very_verbose(const char* text)
{
  output_.print_very_verbose(text);
}

void
TestResult::current_test_ended(TestShell* /*test*/)
{
  current_test_total_execution_time_ =
      static_cast<size_t>(cppmu::get_time_in_millis()) -
      current_test_time_started_;
  output_.print_current_test_ended(*this);
}

void
TestResult::add_failure(const TestFailure& failure)
{
  output_.print_failure(failure);
  failure_count_++;
}

void
TestResult::count_test()
{
  test_count_++;
}

void
TestResult::count_run()
{
  run_count_++;
}

void
TestResult::count_check()
{
  check_count_++;
}

void
TestResult::count_filtered_out()
{
  filtered_out_count_++;
}

void
TestResult::count_ignored()
{
  ignored_count_++;
}

void
TestResult::tests_started()
{
  time_started_ = static_cast<size_t>(cppmu::get_time_in_millis());
  output_.print_tests_started();
}

void
TestResult::tests_ended()
{
  auto time_ended = static_cast<size_t>(cppmu::get_time_in_millis());
  total_execution_time_ = time_ended - time_started_;
  output_.print_tests_ended(*this);
}

size_t
TestResult::get_total_execution_time() const
{
  return total_execution_time_;
}

void
TestResult::set_total_execution_time(size_t ex_time)
{
  total_execution_time_ = ex_time;
}

size_t
TestResult::get_current_test_total_execution_time() const
{
  return current_test_total_execution_time_;
}

size_t
TestResult::get_current_group_total_execution_time() const
{
  return current_group_total_execution_time_;
}

} // namespace cppmu
