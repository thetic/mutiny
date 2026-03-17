/******************************************************************************
 *
 * Provides an interface for when working with pure C
 *
 *******************************************************************************/

#ifndef INCLUDED_CPPMU_CPPMU_H
#define INCLUDED_CPPMU_CPPMU_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#else

#define CHECK_EQUAL_BOOL(expected, actual)                                     \
  check_equal_c_bool_location(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_BOOL_TEXT(expected, actual, text)                          \
  check_equal_c_bool_location(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_INT(expected, actual)                                      \
  check_equal_c_int_location(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_INT_TEXT(expected, actual, text)                           \
  check_equal_c_int_location(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_UINT(expected, actual)                                     \
  check_equal_c_uint_location(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_UINT_TEXT(expected, actual, text)                          \
  check_equal_c_uint_location(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_LONG(expected, actual)                                     \
  check_equal_c_long_location(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_LONG_TEXT(expected, actual, text)                          \
  check_equal_c_long_location(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_ULONG(expected, actual)                                    \
  check_equal_c_ulong_location(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_ULONG_TEXT(expected, actual, text)                         \
  check_equal_c_ulong_location(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_LONGLONG(expected, actual)                                 \
  check_equal_c_longlong_location(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_LONGLONG_TEXT(expected, actual, text)                      \
  check_equal_c_longlong_location(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_ULONGLONG(expected, actual)                                \
  check_equal_c_ulonglong_location(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_ULONGLONG_TEXT(expected, actual, text)                     \
  check_equal_c_ulonglong_location(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_REAL(expected, actual, threshold)                          \
  check_equal_c_real_location(                                                 \
      expected, actual, threshold, "", __FILE__, __LINE__                      \
  )

#define CHECK_EQUAL_REAL_TEXT(expected, actual, threshold, text)               \
  check_equal_c_real_location(                                                 \
      expected, actual, threshold, text, __FILE__, __LINE__                    \
  )

#define CHECK_EQUAL_CHAR(expected, actual)                                     \
  check_equal_c_char_location(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_CHAR_TEXT(expected, actual, text)                          \
  check_equal_c_char_location(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_UBYTE(expected, actual)                                    \
  check_equal_c_ubyte_location(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_UBYTE_TEXT(expected, actual, text)                         \
  check_equal_c_ubyte_location(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_SBYTE(expected, actual)                                    \
  check_equal_c_sbyte_location(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_SBYTE_TEXT(expected, actual, text)                         \
  check_equal_c_sbyte_location(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_STRING(expected, actual)                                   \
  check_equal_c_string_location(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_STRING_TEXT(expected, actual, text)                        \
  check_equal_c_string_location(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_POINTER(expected, actual)                                  \
  check_equal_c_pointer_location(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_POINTER_TEXT(expected, actual, text)                       \
  check_equal_c_pointer_location(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_MEMCMP(expected, actual, size)                             \
  check_equal_c_memcmp_location(expected, actual, size, "", __FILE__, __LINE__)

#define CHECK_EQUAL_MEMCMP_TEXT(expected, actual, size, text)                  \
  check_equal_c_memcmp_location(                                               \
      expected, actual, size, text, __FILE__, __LINE__                         \
  )

#define FAIL_TEXT(text) fail_text_c_location(text, __FILE__, __LINE__)

#define FAIL() fail_c_location(__FILE__, __LINE__)

#define CHECK(condition)                                                       \
  check_c_location(condition, #condition, "", __FILE__, __LINE__)

#define CHECK_TEXT(condition, text)                                            \
  check_c_location(condition, #condition, text, __FILE__, __LINE__)

#define TEST_PROPERTY(name, value) add_test_property_c((name), (value))

#define TEST_GROUP_SETUP(group_name)                                           \
  extern void group_##group_name##_setup_wrapper_c(void);                      \
  void group_##group_name##_setup_wrapper_c(void)

#define TEST_GROUP_TEARDOWN(group_name)                                        \
  extern void group_##group_name##_teardown_wrapper_c(void);                   \
  void group_##group_name##_teardown_wrapper_c(void)

#define TEST(group_name, test_name)                                            \
  extern void test_##group_name##_##test_name##_wrapper_c(void);               \
  void test_##group_name##_##test_name##_wrapper_c(void)

#define IGNORE_TEST(group_name, test_name)                                     \
  extern void ignore_##group_name##_##test_name##_wrapper_c(void);             \
  void ignore_##group_name##_##test_name##_wrapper_c(void)

#define EXPECT_FAIL_TEST(group_name, test_name)                                \
  extern void expect_fail_##group_name##_##test_name##_wrapper_c(void);        \
  void expect_fail_##group_name##_##test_name##_wrapper_c(void)
#endif

  /* CHECKS that can be used from C code */
  void check_equal_c_bool_location(
      int expected,
      int actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void check_equal_c_int_location(
      int expected,
      int actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void check_equal_c_uint_location(
      unsigned int expected,
      unsigned int actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void check_equal_c_long_location(
      long expected,
      long actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void check_equal_c_ulong_location(
      unsigned long expected,
      unsigned long actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void check_equal_c_longlong_location(
      long long expected,
      long long actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void check_equal_c_ulonglong_location(
      unsigned long long expected,
      unsigned long long actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void check_equal_c_real_location(
      double expected,
      double actual,
      double threshold,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void check_equal_c_char_location(
      char expected,
      char actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void check_equal_c_ubyte_location(
      unsigned char expected,
      unsigned char actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void check_equal_c_sbyte_location(
      signed char expected,
      signed char actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void check_equal_c_string_location(
      const char* expected,
      const char* actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void check_equal_c_pointer_location(
      const void* expected,
      const void* actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void check_equal_c_memcmp_location(
      const void* expected,
      const void* actual,
      size_t size,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void fail_text_c_location(
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void fail_c_location(const char* file_name, size_t line_number);
  void check_c_location(
      int condition,
      const char* condition_string,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void add_test_property_c(const char* name, const char* value);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_CPPMU_CPPMU_H */
