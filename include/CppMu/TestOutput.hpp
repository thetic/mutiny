#ifndef INCLUDED_CPPMU_TESTOUTPUT_HPP
#define INCLUDED_CPPMU_TESTOUTPUT_HPP

///////////////////////////////////////////////////////////////////////////////
//
//  This is a minimal printer interface.
//  We kept streams out to keep footprint small, and so the test
//  harness could be used with less capable compilers so more
//  platforms could use this test harness
//
///////////////////////////////////////////////////////////////////////////////

#include "CppMu/String.hpp"

#include <stdio.h>

namespace cppmu {

using File = void*;
using FOpenFunc = File (*)(const char* filename, const char* flag);
using FPutsFunc = void (*)(const char* str, File file);
using FCloseFunc = void (*)(File file);

extern File std_out;
extern FOpenFunc f_open;
extern FPutsFunc f_puts;
extern FCloseFunc f_close;

class TestShell;
class TestFailure;
class TestResult;

class TestOutput
{
public:
  enum class VerbosityLevel
  {
    quiet,
    verbose,
    very_verbose
  };

  explicit TestOutput() = default;
  virtual ~TestOutput() = default;

  virtual void print_tests_started();
  virtual void print_tests_ended(const TestResult& result);
  virtual void print_current_test_started(const TestShell& test);
  virtual void print_current_test_ended(const TestResult& res);
  virtual void print_current_group_started(const TestShell& test);
  virtual void print_current_group_ended(const TestResult& res);

  virtual void verbose(VerbosityLevel level);
  virtual void color();
  virtual void print_buffer(const char*) = 0;
  virtual void print(const char*);
  virtual void print(long);
  virtual void print(size_t);
  virtual void print_double(double);
  virtual void print_failure(const TestFailure& failure);
  virtual void print_test_property(const char* name, const char* value);
  virtual void print_test_run(size_t number, size_t total);
  virtual void set_progress_indicator(const char*);

  virtual void print_very_verbose(const char*);

  virtual bool needs_console_companion() const { return false; }

  virtual void flush() {}

protected:
  virtual void print_progress_indicator();
  void print_file_and_line_for_test_and_failure(const TestFailure& failure);
  void print_file_and_line_for_failure(const TestFailure& failure);
  void print_failure_in_test(String test_name);
  void print_failure_message(String reason);
  void print_error_in_file_on_line_formatted_for_working_environment(
      String test_file,
      size_t line_number
  );

  TestOutput(const TestOutput&);
  TestOutput& operator=(const TestOutput&);

  int dot_count_{ 0 };
  VerbosityLevel verbose_{ VerbosityLevel::quiet };
  bool color_{ false };
  const char* progress_indication_{ "." };
};

TestOutput& operator<<(TestOutput&, const char*);
TestOutput& operator<<(TestOutput&, long);

} // namespace cppmu

#endif
