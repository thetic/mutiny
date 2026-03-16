#include "CppMu/TestOutput.hpp"

#include "CppMu/CppMu.hpp"

namespace cppmu {

namespace {

File
f_open_impl(const char* filename, const char* flag)
{
#if defined(__STDC_LIB_EXT1__) || defined(__STDC_SECURE_LIB__)
  FILE* file;
  fopen_s(&file, filename, flag);
  return file;
#else
  return fopen(filename, flag);
#endif
}

void
f_puts_impl(const char* str, cppmu::File file)
{
  fputs(str, static_cast<FILE*>(file));
}

void
f_close_impl(cppmu::File file)
{
  fclose(static_cast<FILE*>(file));
}

} // namespace

File std_out = stdout;
FOpenFunc f_open = f_open_impl;
FPutsFunc f_puts = f_puts_impl;
FCloseFunc f_close = f_close_impl;

void
TestOutput::verbose(VerbosityLevel level)
{
  verbose_ = level;
}

void
TestOutput::color()
{
  color_ = true;
}

void
TestOutput::print(const char* str)
{
  print_buffer(str);
}

void
TestOutput::print(long n)
{
  print(string_from(n).c_str());
}

void
TestOutput::print(size_t n)
{
  print(string_from(n).c_str());
}

void
TestOutput::print_double(double d)
{
  print(string_from(d).c_str());
}

TestOutput&
operator<<(TestOutput& p, const char* s)
{
  p.print(s);
  return p;
}

TestOutput&
operator<<(TestOutput& p, long int i)
{
  p.print(i);
  return p;
}

void
TestOutput::print_current_test_started(const TestShell& test)
{
  if (verbose_ > VerbosityLevel::quiet)
    print(test.get_formatted_name().c_str());

  if (test.will_run()) {
    set_progress_indicator(".");
  } else {
    set_progress_indicator("!");
  }
}

void
TestOutput::print_current_test_ended(const TestResult& res)
{
  if (verbose_ > VerbosityLevel::quiet) {
    print(" - ");
    print(res.get_current_test_total_execution_time());
    print(" ms\n");
  } else {
    print_progress_indicator();
  }
}

void
TestOutput::print_progress_indicator()
{
  print(progress_indication_);
  if (++dot_count_ % 50 == 0)
    print("\n");
}

void
TestOutput::set_progress_indicator(const char* indicator)
{
  progress_indication_ = indicator;
}

void
TestOutput::print_tests_started()
{
}

void
TestOutput::print_current_group_started(const TestShell& /*test*/)
{
}

void
TestOutput::print_current_group_ended(const TestResult& /*res*/)
{
}

void
TestOutput::print_tests_ended(const TestResult& result)
{
  print("\n");
  const bool is_failure = result.is_failure();
  const size_t failure_count = result.get_failure_count();
  if (is_failure) {
    if (color_) {
      print("\033[31;1m");
    }
    print("Errors (");
    if (failure_count > 0) {
      print(failure_count);
      print(" failures, ");
    } else {
      print("ran nothing, ");
    }
  } else {
    if (color_) {
      print("\033[32;1m");
    }
    print("OK (");
  }
  print(result.get_test_count());
  print(" tests, ");
  print(result.get_run_count());
  print(" ran, ");
  print(result.get_check_count());
  print(" checks, ");
  print(result.get_ignored_count());
  print(" ignored, ");
  print(result.get_filtered_out_count());
  print(" filtered out, ");
  print(result.get_total_execution_time());
  print(" ms)");
  if (color_) {
    print("\033[m");
  }
  if (is_failure && failure_count == 0) {
    print("\nNote: test run failed because no tests were run or ignored. "
          "Assuming "
          "something went wrong. "
          "This often happens because of linking errors or typos in test "
          "filter.");
  }
  print("\n\n");

  dot_count_ = 0;
}

void
TestOutput::print_test_run(size_t number, size_t total)
{
  if (total > 1) {
    print("Test run ");
    print(number);
    print(" of ");
    print(total);
    print("\n");
  }
}

void
TestOutput::print_failure(const TestFailure& failure)
{
  if (failure.is_outside_test_file() || failure.is_in_helper_function())
    print_file_and_line_for_test_and_failure(failure);
  else
    print_file_and_line_for_failure(failure);

  print_failure_message(failure.get_message());
}

void
TestOutput::print_file_and_line_for_test_and_failure(const TestFailure& failure)
{
  print_error_in_file_on_line_formatted_for_working_environment(
      failure.get_test_file_name(), failure.get_test_line_number());
  print_failure_in_test(failure.get_test_name());
  print_error_in_file_on_line_formatted_for_working_environment(
      failure.get_file_name(), failure.get_failure_line_number());
}

void
TestOutput::print_file_and_line_for_failure(const TestFailure& failure)
{
  print_error_in_file_on_line_formatted_for_working_environment(
      failure.get_file_name(), failure.get_failure_line_number());
  print_failure_in_test(failure.get_test_name());
}

void
TestOutput::print_failure_in_test(String test_name)
{
  print(" Failure in ");
  print(test_name.c_str());
}

void
TestOutput::print_failure_message(String reason)
{
  print("\n");
  print("\t");
  print(reason.c_str());
  print("\n\n");
}

void
TestOutput::print_error_in_file_on_line_formatted_for_working_environment(
    String file,
    size_t line_number)
{
  print("\n");
  print(file.c_str());
  print(":");
  print(line_number);
  print(":");
  print(" error:");
}

void
TestOutput::print_very_verbose(const char* str)
{
  if (verbose_ == VerbosityLevel::very_verbose)
    print_buffer(str);
}

} // namespace cppmu
