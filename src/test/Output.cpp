#include "mu/tiny/test/Output.hpp"

#include "mu/tiny/test/Failure.hpp"
#include "mu/tiny/test/Result.hpp"
#include "mu/tiny/test/Shell.hpp"

#include <stdio.h>

namespace mu {
namespace tiny {
namespace test {

namespace {

Output::File fopen_impl(const char* filename, const char* flag)
{
#if defined(__STDC_LIB_EXT1__) || defined(__STDC_SECURE_LIB__)
  FILE* file;
  ::fopen_s(&file, filename, flag);
  return file;
#else
  return ::fopen(filename, flag);
#endif
}

void fputs_impl(const char* str, Output::File file)
{
  ::fputs(str, static_cast<FILE*>(file));
}

void fclose_impl(Output::File file)
{
  ::fclose(static_cast<FILE*>(file));
}

} // namespace

Output::File Output::stdout_ = stdout;
Output::FOpenFunc Output::fopen_ = fopen_impl;
Output::FPutsFunc Output::fputs_ = fputs_impl;
Output::FCloseFunc Output::fclose_ = fclose_impl;

void Output::verbose(MutinyVerbosityLevel level)
{
  verbose_ = level;
}

void Output::color()
{
  color_ = true;
}

void Output::print(const char* str)
{
  print_buffer(str);
}

void Output::print(long n)
{
  print(string_from(n).c_str());
}

void Output::print(size_t n)
{
  print(string_from(n).c_str());
}

void Output::print_double(double d)
{
  print(string_from(d).c_str());
}

Output& operator<<(Output& lhs, const char* rhs)
{
  lhs.print(rhs);
  return lhs;
}

Output& operator<<(Output& lhs, long int rhs)
{
  lhs.print(rhs);
  return lhs;
}

void Output::print_current_test_started(const Shell& test)
{
  if (verbose_ > MutinyVerbosityLevel::quiet) {
    print(test.get_formatted_name().c_str());
  }

  if (test.will_run()) {
    set_progress_indicator(".");
  } else {
    set_progress_indicator("!");
  }
}

void Output::print_current_test_ended(const Result& res)
{
  if (verbose_ > MutinyVerbosityLevel::quiet) {
    print(" - ");
    print(string_from(res.get_current_test_total_execution_time()).c_str());
    print(" ms\n");
  } else {
    print_progress_indicator();
  }
}

void Output::print_progress_indicator()
{
  constexpr int dots_per_line{ 50 };
  print(progress_indication_);
  if (++dot_count_ % dots_per_line == 0) {
    print("\n");
  }
}

void Output::set_progress_indicator(const char* indicator)
{
  progress_indication_ = indicator;
}

void Output::print_tests_started() {}

void Output::print_current_group_started(const Shell& /*test*/) {}

void Output::print_current_group_ended(const Result& /*res*/) {}

void Output::print_tests_ended(const Result& result)
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
  print(result.get_skipped_count());
  print(" skipped, ");
  print(result.get_filtered_out_count());
  print(" filtered out, ");
  print(string_from(result.get_total_execution_time()).c_str());
  print(" ms)");
  if (color_) {
    print("\033[m");
  }
  if (is_failure && failure_count == 0) {
    print(
        "\nNote: test run failed because no tests were run or skipped. "
        "Assuming "
        "something went wrong. "
        "This often happens because of linking errors or typos in test "
        "filter."
    );
  }
  print("\n\n");

  dot_count_ = 0;
}

void Output::print_test_run(size_t number, size_t total)
{
  if (total > 1) {
    print("Test run ");
    print(number);
    print(" of ");
    print(total);
    print("\n");
  }
}

void Output::print_test_property(const char* /*name*/, const char* /*value*/) {}

void Output::print_skipped(const char* /*message*/) {}

void Output::print_failure(const Failure& failure)
{
  if (failure.is_outside_test_file() || failure.is_in_helper_function()) {
    print_file_and_line_for_test_and_failure(failure);
  } else {
    print_file_and_line_for_failure(failure);
  }

  print_failure_message(failure.get_message());
}

void Output::print_file_and_line_for_test_and_failure(const Failure& failure)
{
  print_error_in_file_on_line_formatted_for_working_environment(
      failure.get_test_file_name(), failure.get_test_line_number()
  );
  print_failure_in_test(failure.get_test_name());
  print_error_in_file_on_line_formatted_for_working_environment(
      failure.get_file_name(), failure.get_failure_line_number()
  );
}

void Output::print_file_and_line_for_failure(const Failure& failure)
{
  print_error_in_file_on_line_formatted_for_working_environment(
      failure.get_file_name(), failure.get_failure_line_number()
  );
  print_failure_in_test(failure.get_test_name());
}

void Output::print_failure_in_test(const String& test_name)
{
  print(" Failure in ");
  print(test_name.c_str());
}

void Output::print_failure_message(const String& reason)
{
  print("\n");
  print("\t");
  print(reason.c_str());
  print("\n\n");
}

void Output::print_error_in_file_on_line_formatted_for_working_environment(
    const String& file,
    size_t line_number
)
{
  print("\n");
  print(file.c_str());
  print(":");
  print(line_number);
  print(":");
  print(" error:");
}

void Output::print_very_verbose(const char* str)
{
  if (verbose_ == MutinyVerbosityLevel::very_verbose) {
    print_buffer(str);
  }
}

} // namespace test
} // namespace tiny
} // namespace mu
