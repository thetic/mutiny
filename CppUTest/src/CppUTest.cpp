#include "CppUTest/CppUTest.h"

#include "CppUTest/TestShell.hpp"

void
check_equal_c_bool_location(int expected,
    int actual,
    const char* text,
    const char* file_name,
    size_t line_number)
{
  cpputest::TestShell::get_current()->assert_equals(!!expected != !!actual,
      expected ? "true" : "false",
      actual ? "true" : "false",
      text,
      file_name,
      line_number,
      cpputest::TestShell::get_current_test_terminator_without_exceptions());
}

void
check_equal_c_int_location(int expected,
    int actual,
    const char* text,
    const char* file_name,
    size_t line_number)
{
  cpputest::TestShell::get_current()->assert_longs_equal(
      static_cast<long>(expected),
      static_cast<long>(actual),
      text,
      file_name,
      line_number,
      cpputest::TestShell::get_current_test_terminator_without_exceptions());
}

void
check_equal_c_uint_location(unsigned int expected,
    unsigned int actual,
    const char* text,
    const char* file_name,
    size_t line_number)
{
  cpputest::TestShell::get_current()->assert_unsigned_longs_equal(
      static_cast<unsigned long>(expected),
      static_cast<unsigned long>(actual),
      text,
      file_name,
      line_number,
      cpputest::TestShell::get_current_test_terminator_without_exceptions());
}

void
check_equal_c_long_location(long expected,
    long actual,
    const char* text,
    const char* file_name,
    size_t line_number)
{
  cpputest::TestShell::get_current()->assert_longs_equal(expected,
      actual,
      text,
      file_name,
      line_number,
      cpputest::TestShell::get_current_test_terminator_without_exceptions());
}

void
check_equal_c_ulong_location(unsigned long expected,
    unsigned long actual,
    const char* text,
    const char* file_name,
    size_t line_number)
{
  cpputest::TestShell::get_current()->assert_unsigned_longs_equal(expected,
      actual,
      text,
      file_name,
      line_number,
      cpputest::TestShell::get_current_test_terminator_without_exceptions());
}

void
check_equal_c_longlong_location(long long expected,
    long long actual,
    const char* text,
    const char* file_name,
    size_t line_number)
{
  cpputest::TestShell::get_current()->assert_long_longs_equal(expected,
      actual,
      text,
      file_name,
      line_number,
      cpputest::TestShell::get_current_test_terminator_without_exceptions());
}

void
check_equal_c_ulonglong_location(unsigned long long expected,
    unsigned long long actual,
    const char* text,
    const char* file_name,
    size_t line_number)
{
  cpputest::TestShell::get_current()->assert_unsigned_long_longs_equal(expected,
      actual,
      text,
      file_name,
      line_number,
      cpputest::TestShell::get_current_test_terminator_without_exceptions());
}

void
check_equal_c_real_location(double expected,
    double actual,
    double threshold,
    const char* text,
    const char* file_name,
    size_t line_number)
{
  cpputest::TestShell::get_current()->assert_doubles_equal(expected,
      actual,
      threshold,
      text,
      file_name,
      line_number,
      cpputest::TestShell::get_current_test_terminator_without_exceptions());
}

void
check_equal_c_char_location(char expected,
    char actual,
    const char* text,
    const char* file_name,
    size_t line_number)
{
  cpputest::TestShell::get_current()->assert_equals(((expected) != (actual)),
      cpputest::string_from(expected),
      cpputest::string_from(actual),
      text,
      file_name,
      line_number,
      cpputest::TestShell::get_current_test_terminator_without_exceptions());
}

void
check_equal_c_ubyte_location(unsigned char expected,
    unsigned char actual,
    const char* text,
    const char* file_name,
    size_t line_number)
{
  cpputest::TestShell::get_current()->assert_equals(((expected) != (actual)),
      cpputest::string_from(static_cast<int>(expected)),
      cpputest::string_from(static_cast<int>(actual)),
      text,
      file_name,
      line_number,
      cpputest::TestShell::get_current_test_terminator_without_exceptions());
}

void
check_equal_c_sbyte_location(char signed expected,
    signed char actual,
    const char* text,
    const char* file_name,
    size_t line_number)
{
  cpputest::TestShell::get_current()->assert_equals(((expected) != (actual)),
      cpputest::string_from(static_cast<int>(expected)),
      cpputest::string_from(static_cast<int>(actual)),
      text,
      file_name,
      line_number,
      cpputest::TestShell::get_current_test_terminator_without_exceptions());
}

void
check_equal_c_string_location(const char* expected,
    const char* actual,
    const char* text,
    const char* file_name,
    size_t line_number)
{
  cpputest::TestShell::get_current()->assert_cstr_equal(expected,
      actual,
      text,
      file_name,
      line_number,
      cpputest::TestShell::get_current_test_terminator_without_exceptions());
}

void
check_equal_c_pointer_location(const void* expected,
    const void* actual,
    const char* text,
    const char* file_name,
    size_t line_number)
{
  cpputest::TestShell::get_current()->assert_pointers_equal(expected,
      actual,
      text,
      file_name,
      line_number,
      cpputest::TestShell::get_current_test_terminator_without_exceptions());
}

void
check_equal_c_memcmp_location(const void* expected,
    const void* actual,
    size_t size,
    const char* text,
    const char* file_name,
    size_t line_number)
{
  cpputest::TestShell::get_current()->assert_binary_equal(expected,
      actual,
      size,
      text,
      file_name,
      line_number,
      cpputest::TestShell::get_current_test_terminator_without_exceptions());
}

void
fail_text_c_location(const char* text,
    const char* file_name,
    size_t line_number)
{
  cpputest::TestShell::get_current()->fail(text,
      file_name,
      line_number,
      cpputest::TestShell::get_current_test_terminator_without_exceptions());
}

void
fail_c_location(const char* file_name, size_t line_number)
{
  cpputest::TestShell::get_current()->fail("",
      file_name,
      line_number,
      cpputest::TestShell::get_current_test_terminator_without_exceptions());
}

void
check_c_location(int condition,
    const char* condition_string,
    const char* text,
    const char* file_name,
    size_t line_number)
{
  cpputest::TestShell::get_current()->assert_true(condition != 0,
      "CHECK_C",
      condition_string,
      text,
      file_name,
      line_number,
      cpputest::TestShell::get_current_test_terminator_without_exceptions());
}
