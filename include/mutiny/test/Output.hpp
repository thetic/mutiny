#ifndef INCLUDED_MUTINY_TEST_OUTPUT_HPP
#define INCLUDED_MUTINY_TEST_OUTPUT_HPP

///////////////////////////////////////////////////////////////////////////////
//
//  This is a minimal printer interface.
//  We kept streams out to keep footprint small, and so the test
//  harness could be used with less capable compilers so more
//  platforms could use this test harness
//
///////////////////////////////////////////////////////////////////////////////

#include "mutiny/String.hpp"

namespace mu {
namespace tiny {
namespace test {

class Shell;
class Failure;
class Result;

class Output
{
public:
  enum class VerbosityLevel
  {
    quiet,
    verbose,
    very_verbose
  };

  explicit Output() = default;
  virtual ~Output() = default;

  virtual void print_tests_started();
  virtual void print_tests_ended(const Result& result);
  virtual void print_current_test_started(const Shell& test);
  virtual void print_current_test_ended(const Result& res);
  virtual void print_current_group_started(const Shell& test);
  virtual void print_current_group_ended(const Result& res);

  virtual void verbose(VerbosityLevel level);
  virtual void color();
  virtual void print_buffer(const char*) = 0;
  virtual void print(const char*);
  virtual void print(long);
  virtual void print(size_t);
  virtual void print_double(double);
  virtual void print_failure(const Failure& failure);
  virtual void print_test_property(const char* name, const char* value);
  virtual void print_test_run(size_t number, size_t total);
  virtual void set_progress_indicator(const char*);

  virtual void print_very_verbose(const char*);

  virtual bool needs_console_companion() const { return false; }

  virtual void flush() {}

  using File = void*;
  static File stdout_;

  using FOpenFunc = File (*)(const char* filename, const char* flag);
  static FOpenFunc fopen_;

  using FPutsFunc = void (*)(const char* str, File file);
  static FPutsFunc fputs_;

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
      size_t line_number
  );

  Output(const Output&);
  Output& operator=(const Output&);

  int dot_count_{ 0 };
  VerbosityLevel verbose_{ VerbosityLevel::quiet };
  bool color_{ false };
  const char* progress_indication_{ "." };
};

Output& operator<<(Output&, const char*);
Output& operator<<(Output&, long);

} // namespace test
} // namespace tiny
} // namespace mu

#endif
