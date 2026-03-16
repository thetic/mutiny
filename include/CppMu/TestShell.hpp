#ifndef INCLUDED_CPPMU_TESTSHELL_HPP
#define INCLUDED_CPPMU_TESTSHELL_HPP

#include "CppMu/String.hpp"
#include "CppMu/TestTerminator.hpp"
#include "CppMu/features.hpp"

namespace cppmu {

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
  virtual ~TestShell() = default;

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

  virtual void add_failure(const TestFailure& failure);

protected:
  TestShell();

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

} // namespace cppmu

// Different checking macros

#define CHECK(condition)                                                       \
  CHECK_TRUE_LOCATION(                                                         \
      condition, "CHECK", #condition, nullptr, __FILE__, __LINE__)

#define CHECK_TEXT(condition, text)                                            \
  CHECK_TRUE_LOCATION(static_cast<bool>(condition),                            \
      "CHECK",                                                                 \
      #condition,                                                              \
      text,                                                                    \
      __FILE__,                                                                \
      __LINE__)

#define CHECK_TRUE(condition)                                                  \
  CHECK_TRUE_LOCATION(static_cast<bool>(condition),                            \
      "CHECK_TRUE",                                                            \
      #condition,                                                              \
      nullptr,                                                                 \
      __FILE__,                                                                \
      __LINE__)

#define CHECK_TRUE_TEXT(condition, text)                                       \
  CHECK_TRUE_LOCATION(                                                         \
      condition, "CHECK_TRUE", #condition, text, __FILE__, __LINE__)

#define CHECK_FALSE(condition)                                                 \
  CHECK_FALSE_LOCATION(                                                        \
      condition, "CHECK_FALSE", #condition, nullptr, __FILE__, __LINE__)

#define CHECK_FALSE_TEXT(condition, text)                                      \
  CHECK_FALSE_LOCATION(                                                        \
      condition, "CHECK_FALSE", #condition, text, __FILE__, __LINE__)

#define CHECK_TRUE_LOCATION(                                                   \
    condition, checkString, conditionString, text, file, line)                 \
  do {                                                                         \
    cppmu::TestShell::get_current()->assert_true(                              \
        (condition), checkString, conditionString, text, file, line);          \
  } while (0)

#define CHECK_FALSE_LOCATION(                                                  \
    condition, checkString, conditionString, text, file, line)                 \
  do {                                                                         \
    cppmu::TestShell::get_current()->assert_true(                              \
        !(condition), checkString, conditionString, text, file, line);         \
  } while (0)

// MSVC C4127: suppress "conditional expression is constant" for the
// self-comparison guards (x != x) used to detect NaN-like side-effect cases.
#ifdef _MSC_VER
#define CPPMU_SUPPRESS_C4127_PUSH                                              \
  __pragma(warning(push)) __pragma(warning(disable : 4127))
#define CPPMU_SUPPRESS_C4127_POP __pragma(warning(pop))
#else
#define CPPMU_SUPPRESS_C4127_PUSH
#define CPPMU_SUPPRESS_C4127_POP
#endif

// This check needs the operator!=(), and a string_from(YourType) function
#define CHECK_EQUAL(expected, actual)                                          \
  CHECK_EQUAL_LOCATION(expected, actual, nullptr, __FILE__, __LINE__)

#define CHECK_EQUAL_TEXT(expected, actual, text)                               \
  CHECK_EQUAL_LOCATION(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_LOCATION(expected, actual, text, file, line)               \
  CPPMU_SUPPRESS_C4127_PUSH                                                    \
  do {                                                                         \
    if ((expected) != (actual)) {                                              \
      if ((actual) != (actual))                                                \
        cppmu::TestShell::get_current()->print(                                \
            "WARNING:\n\tThe \"Actual Parameter\" parameter is evaluated "     \
            "multiple times resulting in different values.\n\tThus the value " \
            "in the error message is probably incorrect.",                     \
            file,                                                              \
            line);                                                             \
      if ((expected) != (expected))                                            \
        cppmu::TestShell::get_current()->print(                                \
            "WARNING:\n\tThe \"Expected Parameter\" parameter is evaluated "   \
            "multiple times resulting in different values.\n\tThus the value " \
            "in the error message is probably incorrect.",                     \
            file,                                                              \
            line);                                                             \
      cppmu::TestShell::get_current()->assert_equals(true,                     \
          cppmu::string_from(expected).c_str(),                                \
          cppmu::string_from(actual).c_str(),                                  \
          text,                                                                \
          file,                                                                \
          line);                                                               \
    } else {                                                                   \
      cppmu::TestShell::get_current()->assert_longs_equal(                     \
          static_cast<long>(0), static_cast<long>(0), nullptr, file, line);    \
    }                                                                          \
  } while (0) CPPMU_SUPPRESS_C4127_POP

#define CHECK_EQUAL_ZERO(actual) CHECK_EQUAL(0, (actual))

#define CHECK_EQUAL_ZERO_TEXT(actual, text)                                    \
  CHECK_EQUAL_TEXT(0, (actual), (text))

#define CHECK_COMPARE(first, relop, second)                                    \
  CHECK_COMPARE_TEXT(first, relop, second, nullptr)

#define CHECK_COMPARE_TEXT(first, relop, second, text)                         \
  CHECK_COMPARE_LOCATION(first, relop, second, text, __FILE__, __LINE__)

#define CHECK_COMPARE_LOCATION(first, relop, second, text, file, line)         \
  do {                                                                         \
    bool success = (first)relop(second);                                       \
    if (!success) {                                                            \
      cppmu::String conditionString;                                           \
      conditionString += cppmu::string_from(first);                            \
      conditionString += " ";                                                  \
      conditionString += #relop;                                               \
      conditionString += " ";                                                  \
      conditionString += cppmu::string_from(second);                           \
      cppmu::TestShell::get_current()->assert_compare(false,                   \
          "CHECK_COMPARE",                                                     \
          conditionString.c_str(),                                             \
          text,                                                                \
          __FILE__,                                                            \
          __LINE__);                                                           \
    }                                                                          \
  } while (0)

// This check checks for char* string equality using strcmp.
// This makes up for the fact that CHECK_EQUAL only compares the pointers to
// char*'s
#define STRCMP_EQUAL(expected, actual)                                         \
  STRCMP_EQUAL_LOCATION(expected, actual, nullptr, __FILE__, __LINE__)

#define STRCMP_EQUAL_TEXT(expected, actual, text)                              \
  STRCMP_EQUAL_LOCATION(expected, actual, text, __FILE__, __LINE__)

#define STRCMP_EQUAL_LOCATION(expected, actual, text, file, line)              \
  do {                                                                         \
    cppmu::TestShell::get_current()->assert_cstr_equal(                        \
        expected, actual, text, file, line);                                   \
  } while (0)

#define STRNCMP_EQUAL(expected, actual, length)                                \
  STRNCMP_EQUAL_LOCATION(expected, actual, length, nullptr, __FILE__, __LINE__)

#define STRNCMP_EQUAL_TEXT(expected, actual, length, text)                     \
  STRNCMP_EQUAL_LOCATION(expected, actual, length, text, __FILE__, __LINE__)

#define STRNCMP_EQUAL_LOCATION(expected, actual, length, text, file, line)     \
  do {                                                                         \
    cppmu::TestShell::get_current()->assert_cstr_n_equal(                      \
        expected, actual, length, text, file, line);                           \
  } while (0)

#define STRCMP_CONTAINS(expected, actual)                                      \
  STRCMP_CONTAINS_LOCATION(expected, actual, nullptr, __FILE__, __LINE__)

#define STRCMP_CONTAINS_TEXT(expected, actual, text)                           \
  STRCMP_CONTAINS_LOCATION(expected, actual, text, __FILE__, __LINE__)

#define STRCMP_CONTAINS_LOCATION(expected, actual, text, file, line)           \
  do {                                                                         \
    cppmu::TestShell::get_current()->assert_cstr_contains(                     \
        expected, actual, text, file, line);                                   \
  } while (0)

// Check two long integers for equality
#define LONGS_EQUAL(expected, actual)                                          \
  LONGS_EQUAL_LOCATION((expected),                                             \
      (actual),                                                                \
      "LONGS_EQUAL(" #expected ", " #actual ") failed",                        \
      __FILE__,                                                                \
      __LINE__)

#define LONGS_EQUAL_TEXT(expected, actual, text)                               \
  LONGS_EQUAL_LOCATION((expected), (actual), text, __FILE__, __LINE__)

#define UNSIGNED_LONGS_EQUAL(expected, actual)                                 \
  UNSIGNED_LONGS_EQUAL_LOCATION(                                               \
      (expected), (actual), nullptr, __FILE__, __LINE__)

#define UNSIGNED_LONGS_EQUAL_TEXT(expected, actual, text)                      \
  UNSIGNED_LONGS_EQUAL_LOCATION((expected), (actual), text, __FILE__, __LINE__)

#define LONGS_EQUAL_LOCATION(expected, actual, text, file, line)               \
  do {                                                                         \
    cppmu::TestShell::get_current()->assert_longs_equal(                       \
        static_cast<long>(expected),                                           \
        static_cast<long>(actual),                                             \
        text,                                                                  \
        file,                                                                  \
        line);                                                                 \
  } while (0)

#define UNSIGNED_LONGS_EQUAL_LOCATION(expected, actual, text, file, line)      \
  do {                                                                         \
    cppmu::TestShell::get_current()->assert_unsigned_longs_equal(              \
        static_cast<unsigned long>(expected),                                  \
        static_cast<unsigned long>(actual),                                    \
        text,                                                                  \
        file,                                                                  \
        line);                                                                 \
  } while (0)

#define LONGLONGS_EQUAL(expected, actual)                                      \
  LONGLONGS_EQUAL_LOCATION(expected, actual, nullptr, __FILE__, __LINE__)

#define LONGLONGS_EQUAL_TEXT(expected, actual, text)                           \
  LONGLONGS_EQUAL_LOCATION(expected, actual, text, __FILE__, __LINE__)

#define UNSIGNED_LONGLONGS_EQUAL(expected, actual)                             \
  UNSIGNED_LONGLONGS_EQUAL_LOCATION(                                           \
      expected, actual, nullptr, __FILE__, __LINE__)

#define UNSIGNED_LONGLONGS_EQUAL_TEXT(expected, actual, text)                  \
  UNSIGNED_LONGLONGS_EQUAL_LOCATION(expected, actual, text, __FILE__, __LINE__)

#define LONGLONGS_EQUAL_LOCATION(expected, actual, text, file, line)           \
  do {                                                                         \
    cppmu::TestShell::get_current()->assert_long_longs_equal(                  \
        static_cast<long long>(expected),                                      \
        static_cast<long long>(actual),                                        \
        text,                                                                  \
        file,                                                                  \
        line);                                                                 \
  } while (0)

#define UNSIGNED_LONGLONGS_EQUAL_LOCATION(expected, actual, text, file, line)  \
  do {                                                                         \
    cppmu::TestShell::get_current()->assert_unsigned_long_longs_equal(         \
        static_cast<unsigned long long>(expected),                             \
        static_cast<unsigned long long>(actual),                               \
        text,                                                                  \
        file,                                                                  \
        line);                                                                 \
  } while (0)

#define BYTES_EQUAL(expected, actual)                                          \
  LONGS_EQUAL((expected) & 0xff, (actual) & 0xff)

#define BYTES_EQUAL_TEXT(expected, actual, text)                               \
  LONGS_EQUAL_TEXT((expected) & 0xff, (actual) & 0xff, text)

#define SIGNED_BYTES_EQUAL(expected, actual)                                   \
  SIGNED_BYTES_EQUAL_LOCATION(expected, actual, __FILE__, __LINE__)

#define SIGNED_BYTES_EQUAL_LOCATION(expected, actual, file, line)              \
  do {                                                                         \
    cppmu::TestShell::get_current()->assert_signed_bytes_equal(                \
        expected, actual, nullptr, file, line);                                \
  } while (0)

#define SIGNED_BYTES_EQUAL_TEXT(expected, actual, text)                        \
  SIGNED_BYTES_EQUAL_TEXT_LOCATION(expected, actual, text, __FILE__, __LINE__)

#define SIGNED_BYTES_EQUAL_TEXT_LOCATION(expected, actual, text, file, line)   \
  do {                                                                         \
    cppmu::TestShell::get_current()->assert_signed_bytes_equal(                \
        expected, actual, text, file, line);                                   \
  } while (0)

#define POINTERS_EQUAL(expected, actual)                                       \
  POINTERS_EQUAL_LOCATION((expected), (actual), nullptr, __FILE__, __LINE__)

#define POINTERS_EQUAL_TEXT(expected, actual, text)                            \
  POINTERS_EQUAL_LOCATION((expected), (actual), text, __FILE__, __LINE__)

#define POINTERS_EQUAL_LOCATION(expected, actual, text, file, line)            \
  do {                                                                         \
    cppmu::TestShell::get_current()->assert_pointers_equal(                    \
        static_cast<const void*>(expected),                                    \
        static_cast<const void*>(actual),                                      \
        text,                                                                  \
        file,                                                                  \
        line);                                                                 \
  } while (0)

#define FUNCTIONPOINTERS_EQUAL(expected, actual)                               \
  FUNCTIONPOINTERS_EQUAL_LOCATION(                                             \
      (expected), (actual), nullptr, __FILE__, __LINE__)

#define FUNCTIONPOINTERS_EQUAL_TEXT(expected, actual, text)                    \
  FUNCTIONPOINTERS_EQUAL_LOCATION(                                             \
      (expected), (actual), text, __FILE__, __LINE__)

#define FUNCTIONPOINTERS_EQUAL_LOCATION(expected, actual, text, file, line)    \
  do {                                                                         \
    cppmu::TestShell::get_current()->assert_function_pointers_equal(           \
        reinterpret_cast<void (*)()>(expected),                                \
        reinterpret_cast<void (*)()>(actual),                                  \
        text,                                                                  \
        file,                                                                  \
        line);                                                                 \
  } while (0)

// Check two doubles for equality within a tolerance threshold
#define DOUBLES_EQUAL(expected, actual, threshold)                             \
  DOUBLES_EQUAL_LOCATION(                                                      \
      expected, actual, threshold, nullptr, __FILE__, __LINE__)

#define DOUBLES_EQUAL_TEXT(expected, actual, threshold, text)                  \
  DOUBLES_EQUAL_LOCATION(expected, actual, threshold, text, __FILE__, __LINE__)

#define DOUBLES_EQUAL_LOCATION(expected, actual, threshold, text, file, line)  \
  do {                                                                         \
    cppmu::TestShell::get_current()->assert_doubles_equal(                     \
        expected, actual, threshold, text, file, line);                        \
  } while (0)

#define MEMCMP_EQUAL(expected, actual, size)                                   \
  MEMCMP_EQUAL_LOCATION(expected, actual, size, nullptr, __FILE__, __LINE__)

#define MEMCMP_EQUAL_TEXT(expected, actual, size, text)                        \
  MEMCMP_EQUAL_LOCATION(expected, actual, size, text, __FILE__, __LINE__)

#define MEMCMP_EQUAL_LOCATION(expected, actual, size, text, file, line)        \
  do {                                                                         \
    cppmu::TestShell::get_current()->assert_binary_equal(                      \
        expected, actual, size, text, file, line);                             \
  } while (0)

#define ENUMS_EQUAL_INT(expected, actual)                                      \
  ENUMS_EQUAL_TYPE(int, expected, actual)

#define ENUMS_EQUAL_INT_TEXT(expected, actual, text)                           \
  ENUMS_EQUAL_TYPE_TEXT(int, expected, actual, text)

#define ENUMS_EQUAL_TYPE(underlying_type, expected, actual)                    \
  ENUMS_EQUAL_TYPE_LOCATION(                                                   \
      underlying_type, expected, actual, nullptr, __FILE__, __LINE__)

#define ENUMS_EQUAL_TYPE_TEXT(underlying_type, expected, actual, text)         \
  ENUMS_EQUAL_TYPE_LOCATION(                                                   \
      underlying_type, expected, actual, text, __FILE__, __LINE__)

#define ENUMS_EQUAL_TYPE_LOCATION(                                             \
    underlying_type, expected, actual, text, file, line)                       \
  do {                                                                         \
    underlying_type expected_underlying_value =                                \
        static_cast<underlying_type>(expected);                                \
    underlying_type actual_underlying_value =                                  \
        static_cast<underlying_type>(actual);                                  \
    if (expected_underlying_value != actual_underlying_value) {                \
      cppmu::TestShell::get_current()->assert_equals(true,                     \
          cppmu::string_from(expected_underlying_value).c_str(),               \
          cppmu::string_from(actual_underlying_value).c_str(),                 \
          text,                                                                \
          file,                                                                \
          line);                                                               \
    } else {                                                                   \
      cppmu::TestShell::get_current()->assert_longs_equal(                     \
          static_cast<long>(0), static_cast<long>(0), nullptr, file, line);    \
    }                                                                          \
  } while (0)

// Fail if you get to this macro
// The macro FAIL may already be taken, so allow FAIL_TEST too
#ifndef FAIL
#define FAIL(text) FAIL_LOCATION(text, __FILE__, __LINE__)

#define FAIL_LOCATION(text, file, line)                                        \
  do {                                                                         \
    cppmu::TestShell::get_current()->fail(text, file, line);                   \
  } while (0)
#endif

#define FAIL_TEST(text) FAIL_TEST_LOCATION(text, __FILE__, __LINE__)

#define FAIL_TEST_LOCATION(text, file, line)                                   \
  do {                                                                         \
    cppmu::TestShell::get_current()->fail(text, file, line);                   \
  } while (0)

#define TEST_EXIT                                                              \
  do {                                                                         \
    cppmu::TestShell::get_current()->exit_test();                              \
  } while (0)

#if CPPMU_HAVE_EXCEPTIONS
#define CHECK_THROWS(expected, expression)                                     \
  do {                                                                         \
    cppmu::String failure_msg(                                                 \
        "expected to throw " #expected "\nbut threw nothing");                 \
    bool caught_expected = false;                                              \
    try {                                                                      \
      (expression);                                                            \
    } catch (const expected&) {                                                \
      caught_expected = true;                                                  \
    } catch (...) {                                                            \
      failure_msg =                                                            \
          "expected to throw " #expected "\nbut threw a different type";       \
    }                                                                          \
    if (!caught_expected) {                                                    \
      cppmu::TestShell::get_current()->fail(                                   \
          failure_msg.c_str(), __FILE__, __LINE__);                            \
    } else {                                                                   \
      cppmu::TestShell::get_current()->count_check();                          \
    }                                                                          \
  } while (0)
#endif /* CPPMU_HAVE_EXCEPTIONS */

#endif
