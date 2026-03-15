#ifndef INCLUDED_CPPUTEST_TESTSHELL_HPP
#define INCLUDED_CPPUTEST_TESTSHELL_HPP

#include "CppUTest/String.hpp"
#include "CppUTest/TestTerminator.hpp"

#ifdef __cplusplus
#ifndef CPPUTEST_HAVE_EXCEPTIONS
#if ((__cplusplus >= 202002L) && !__cpp_exceptions) ||                         \
    (defined(_MSC_VER) && !_CPPUNWIND) ||                                      \
    (defined(__GNUC__) && !__EXCEPTIONS) ||                                    \
    (defined(__ghs__) && !__EXCEPTION_HANDLING) ||                             \
    (defined(__WATCOMC__) && !_CPPUNWIND)
#define CPPUTEST_HAVE_EXCEPTIONS 0
#else
#define CPPUTEST_HAVE_EXCEPTIONS 1
#endif
#endif

#ifndef CPPUTEST_HAVE_RTTI
#if ((__cplusplus >= 202002L) && !__cpp_rtti) ||                               \
    (defined(_MSC_VER) && !_CPPRTTI) || (defined(__GNUC__) && !__GXX_RTTI) ||  \
    (defined(__ghs__) && !__RTTI) || (defined(__WATCOMC__) && !_CPPRTTI)
#define CPPUTEST_HAVE_RTTI 0
#else
#define CPPUTEST_HAVE_RTTI 1
#endif
#endif
#endif

namespace cpputest {

class TestResult;
class TestPlugin;
class TestFailure;
class TestFilter;

bool
doubles_equal(double d1, double d2, double threshold);

//////////////////// TestShell

class TestShell
{
public:
  static TestShell* get_current();

  static const TestTerminator& get_current_test_terminator();
  static const TestTerminator& get_current_test_terminator_without_exceptions();

  static void set_crash_on_fail();
  static void restore_default_test_terminator();

  static void set_rethrow_exceptions(bool rethrow_exceptions);
  static bool is_rethrowing_exceptions();

public:
  TestShell(const char* group_name,
      const char* test_name,
      const char* file_name,
      size_t line_number);
  virtual ~TestShell();

  virtual TestShell* add_test(TestShell* test);
  virtual TestShell* get_next() const;
  virtual size_t count_tests();

  bool should_run(const TestFilter* group_filters,
      const TestFilter* name_filters) const;
  const String get_name() const;
  const String get_group() const;
  virtual String get_formatted_name() const;
  const String get_file() const;
  size_t get_line_number() const;
  virtual bool will_run() const;
  virtual bool has_failed() const;
  virtual bool is_ordered() const;
  void count_check();

  virtual void assert_true(bool condition,
      const char* check_string,
      const char* condition_string,
      const char* text,
      const char* file_name,
      size_t line_number,
      const TestTerminator& test_terminator = get_current_test_terminator());
  virtual void assert_cstr_equal(const char* expected,
      const char* actual,
      const char* text,
      const char* file_name,
      size_t line_number,
      const TestTerminator& test_terminator = get_current_test_terminator());
  virtual void assert_cstr_n_equal(const char* expected,
      const char* actual,
      size_t length,
      const char* text,
      const char* file_name,
      size_t line_number,
      const TestTerminator& test_terminator = get_current_test_terminator());
  virtual void assert_cstr_contains(const char* expected,
      const char* actual,
      const char* text,
      const char* file_name,
      size_t line_number);
  virtual void assert_longs_equal(long expected,
      long actual,
      const char* text,
      const char* file_name,
      size_t line_number,
      const TestTerminator& test_terminator = get_current_test_terminator());
  virtual void assert_unsigned_longs_equal(unsigned long expected,
      unsigned long actual,
      const char* text,
      const char* file_name,
      size_t line_number,
      const TestTerminator& test_terminator = get_current_test_terminator());
  virtual void assert_long_longs_equal(long long expected,
      long long actual,
      const char* text,
      const char* file_name,
      size_t line_number,
      const TestTerminator& test_terminator = get_current_test_terminator());
  virtual void assert_unsigned_long_longs_equal(unsigned long long expected,
      unsigned long long actual,
      const char* text,
      const char* file_name,
      size_t line_number,
      const TestTerminator& test_terminator = get_current_test_terminator());
  virtual void assert_signed_bytes_equal(signed char expected,
      signed char actual,
      const char* text,
      const char* file_name,
      size_t line_number,
      const TestTerminator& test_terminator = get_current_test_terminator());
  virtual void assert_pointers_equal(const void* expected,
      const void* actual,
      const char* text,
      const char* file_name,
      size_t line_number,
      const TestTerminator& test_terminator = get_current_test_terminator());
  virtual void assert_function_pointers_equal(void (*expected)(),
      void (*actual)(),
      const char* text,
      const char* file_name,
      size_t line_number,
      const TestTerminator& test_terminator = get_current_test_terminator());
  virtual void assert_doubles_equal(double expected,
      double actual,
      double threshold,
      const char* text,
      const char* file_name,
      size_t line_number,
      const TestTerminator& test_terminator = get_current_test_terminator());
  virtual void assert_equals(bool failed,
      const char* expected,
      const char* actual,
      const char* text,
      const char* file,
      size_t line_number,
      const TestTerminator& test_terminator = get_current_test_terminator());
  virtual void assert_equals(bool failed,
      String expected,
      String actual,
      const char* text,
      const char* file,
      size_t line_number,
      const TestTerminator& test_terminator = get_current_test_terminator());
  virtual void assert_binary_equal(const void* expected,
      const void* actual,
      size_t length,
      const char* text,
      const char* file_name,
      size_t line_number,
      const TestTerminator& test_terminator = get_current_test_terminator());
  virtual void assert_compare(bool comparison,
      const char* check_string,
      const char* comparison_string,
      const char* text,
      const char* file_name,
      size_t line_number,
      const TestTerminator& test_terminator = get_current_test_terminator());
  virtual void fail(const char* text,
      const char* file_name,
      size_t line_number,
      const TestTerminator& test_terminator = get_current_test_terminator());
  virtual void exit_test(
      const TestTerminator& test_terminator = get_current_test_terminator());

  virtual void print(const char* text,
      const char* file_name,
      size_t line_number);
  virtual void print(const String& text,
      const char* file_name,
      size_t line_number);
  virtual void print_very_verbose(const char* text);

  void set_file_name(const char* file_name);
  void set_line_number(size_t line_number);
  void set_group_name(const char* group_name);
  void set_test_name(const char* test_name);

  static void crash();
  static void set_crash_method(void (*crashme)());
  static void reset_crash_method();

  virtual void set_run_ignored();

  virtual class Test* create_test();
  virtual void destroy_test(class Test* test);

  virtual void run_one_test(TestPlugin* plugin, TestResult& result);
  virtual void run_one_test_in_current_process(TestPlugin* plugin,
      TestResult& result);

  virtual void fail_with(const TestFailure& failure);
  virtual void fail_with(const TestFailure& failure,
      const TestTerminator& terminator);

  virtual void add_failure(const TestFailure& failure);

protected:
  TestShell();
  TestShell(const char* group_name,
      const char* test_name,
      const char* file_name,
      size_t line_number,
      TestShell* next_test);

  virtual String get_macro_name() const;
  TestResult* get_test_result();

private:
  const char* group_;
  const char* name_;
  const char* file_;
  size_t line_number_;
  TestShell* next_;
  bool has_failed_;

  void set_test_result(TestResult* result);
  void set_current_test(TestShell* test);
  bool match(const char* target, const TestFilter* filters) const;

  static TestShell* current_test_;
  static TestResult* test_result_;

  static const TestTerminator* current_test_terminator_;
  static const TestTerminator* current_test_terminator_without_exceptions_;
  static bool rethrow_exceptions_;
};

//////////////////// FailedException

class FailedException
{
public:
  int dummy;
};

} // namespace cpputest

#endif
