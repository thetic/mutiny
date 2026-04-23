#include "mu/tiny/test.h"

#include "mu/tiny/test/Shell.hpp"

void mutiny_check_equal_bool(
    bool expected,
    bool actual,
    const char* text,
    const char* file_name,
    int line_number
)
{
  mu::tiny::test::Shell::get_current()->assert_equals(
      expected != actual,
      expected ? "true" : "false",
      actual ? "true" : "false",
      text,
      file_name,
      line_number,
      mu::tiny::test::Shell::get_current_test_terminator_without_exceptions()
  );
}

void mutiny_check_equal_int(
    int expected,
    int actual,
    const char* text,
    const char* file_name,
    int line_number
)
{
  mu::tiny::test::Shell::get_current()->assert_intmax_equal(
      static_cast<intmax_t>(expected),
      static_cast<intmax_t>(actual),
      text,
      file_name,
      line_number,
      mu::tiny::test::Shell::get_current_test_terminator_without_exceptions()
  );
}

void mutiny_check_equal_uint(
    unsigned int expected,
    unsigned int actual,
    const char* text,
    const char* file_name,
    int line_number
)
{
  mu::tiny::test::Shell::get_current()->assert_uintmax_equal(
      static_cast<uintmax_t>(expected),
      static_cast<uintmax_t>(actual),
      text,
      file_name,
      line_number,
      mu::tiny::test::Shell::get_current_test_terminator_without_exceptions()
  );
}

void mutiny_check_equal_long(
    long expected,
    long actual,
    const char* text,
    const char* file_name,
    int line_number
)
{
  mu::tiny::test::Shell::get_current()->assert_intmax_equal(
      static_cast<intmax_t>(expected),
      static_cast<intmax_t>(actual),
      text,
      file_name,
      line_number,
      mu::tiny::test::Shell::get_current_test_terminator_without_exceptions()
  );
}

void mutiny_check_equal_ulong(
    unsigned long expected,
    unsigned long actual,
    const char* text,
    const char* file_name,
    int line_number
)
{
  mu::tiny::test::Shell::get_current()->assert_uintmax_equal(
      static_cast<uintmax_t>(expected),
      static_cast<uintmax_t>(actual),
      text,
      file_name,
      line_number,
      mu::tiny::test::Shell::get_current_test_terminator_without_exceptions()
  );
}

void mutiny_check_equal_longlong(
    long long expected,
    long long actual,
    const char* text,
    const char* file_name,
    int line_number
)
{
  mu::tiny::test::Shell::get_current()->assert_intmax_equal(
      static_cast<intmax_t>(expected),
      static_cast<intmax_t>(actual),
      text,
      file_name,
      line_number,
      mu::tiny::test::Shell::get_current_test_terminator_without_exceptions()
  );
}

void mutiny_check_equal_ulonglong(
    unsigned long long expected,
    unsigned long long actual,
    const char* text,
    const char* file_name,
    int line_number
)
{
  mu::tiny::test::Shell::get_current()->assert_uintmax_equal(
      static_cast<uintmax_t>(expected),
      static_cast<uintmax_t>(actual),
      text,
      file_name,
      line_number,
      mu::tiny::test::Shell::get_current_test_terminator_without_exceptions()
  );
}

void mutiny_check_equal_double(
    double expected,
    double actual,
    double threshold,
    const char* text,
    const char* file_name,
    int line_number
)
{
  if (!mu::tiny::test::approx_equal(expected, actual, threshold)) {
    mu::tiny::test::Shell::get_current()->assert_approx_equal(
        expected,
        actual,
        threshold,
        text,
        file_name,
        line_number,
        mu::tiny::test::Shell::get_current_test_terminator_without_exceptions()
    );
  } else {
    mu::tiny::test::Shell::get_current()->count_check();
  }
}

void mutiny_check_equal_char(
    char expected,
    char actual,
    const char* text,
    const char* file_name,
    int line_number
)
{
  mu::tiny::test::Shell::get_current()->assert_equals(
      (expected != actual),
      mu::tiny::string_from(expected),
      mu::tiny::string_from(actual),
      text,
      file_name,
      line_number,
      mu::tiny::test::Shell::get_current_test_terminator_without_exceptions()
  );
}

void mutiny_check_equal_ubyte(
    unsigned char expected,
    unsigned char actual,
    const char* text,
    const char* file_name,
    int line_number
)
{
  mu::tiny::test::Shell::get_current()->assert_equals(
      (expected != actual),
      mu::tiny::string_from(static_cast<int>(expected)),
      mu::tiny::string_from(static_cast<int>(actual)),
      text,
      file_name,
      line_number,
      mu::tiny::test::Shell::get_current_test_terminator_without_exceptions()
  );
}

void mutiny_check_equal_sbyte(
    char signed expected,
    signed char actual,
    const char* text,
    const char* file_name,
    int line_number
)
{
  mu::tiny::test::Shell::get_current()->assert_equals(
      (expected != actual),
      mu::tiny::string_from(static_cast<int>(expected)),
      mu::tiny::string_from(static_cast<int>(actual)),
      text,
      file_name,
      line_number,
      mu::tiny::test::Shell::get_current_test_terminator_without_exceptions()
  );
}

void mutiny_check_equal_string(
    const char* expected,
    const char* actual,
    const char* text,
    const char* file_name,
    int line_number
)
{
  mu::tiny::test::Shell::get_current()->assert_cstr_equal(
      expected,
      actual,
      text,
      file_name,
      line_number,
      mu::tiny::test::Shell::get_current_test_terminator_without_exceptions()
  );
}

void mutiny_check_equal_pointer(
    const void* expected,
    const void* actual,
    const char* text,
    const char* file_name,
    int line_number
)
{
  mu::tiny::test::Shell::get_current()->assert_pointers_equal(
      expected,
      actual,
      text,
      file_name,
      line_number,
      mu::tiny::test::Shell::get_current_test_terminator_without_exceptions()
  );
}

void mutiny_check_equal_memcmp(
    const void* expected,
    const void* actual,
    size_t size,
    const char* text,
    const char* file_name,
    int line_number
)
{
  mu::tiny::test::Shell::get_current()->assert_binary_equal(
      expected,
      actual,
      size,
      text,
      file_name,
      line_number,
      mu::tiny::test::Shell::get_current_test_terminator_without_exceptions()
  );
}

void mutiny_fail(const char* text, const char* file_name, int line_number)
{
  mu::tiny::test::Shell::get_current()->fail(
      text,
      file_name,
      line_number,
      mu::tiny::test::Shell::get_current_test_terminator_without_exceptions()
  );
}

void mutiny_check(
    int condition,
    const char* condition_string,
    const char* text,
    const char* file_name,
    int line_number
)
{
  mu::tiny::test::Shell::get_current()->assert_true(
      condition != 0,
      "CHECK",
      condition_string,
      text,
      file_name,
      line_number,
      mu::tiny::test::Shell::get_current_test_terminator_without_exceptions()
  );
}

void mutiny_add_test_property(const char* name, const char* value)
{
  mu::tiny::test::Shell::get_current()->add_test_property(name, value);
}
