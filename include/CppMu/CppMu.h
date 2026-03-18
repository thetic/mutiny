/******************************************************************************
 *
 * Provides an interface for when working with pure C
 *
 *******************************************************************************/

#ifndef INCLUDED_CPPMU_CPPMU_H
#define INCLUDED_CPPMU_CPPMU_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#else

#define CHECK_EQUAL_BOOL(expected, actual)                                     \
  cppmu_check_equal_bool(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_BOOL_TEXT(expected, actual, text)                          \
  cppmu_check_equal_bool(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_INT(expected, actual)                                      \
  cppmu_check_equal_int(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_INT_TEXT(expected, actual, text)                           \
  cppmu_check_equal_int(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_UINT(expected, actual)                                     \
  cppmu_check_equal_uint(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_UINT_TEXT(expected, actual, text)                          \
  cppmu_check_equal_uint(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_LONG(expected, actual)                                     \
  cppmu_check_equal_long(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_LONG_TEXT(expected, actual, text)                          \
  cppmu_check_equal_long(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_ULONG(expected, actual)                                    \
  cppmu_check_equal_ulong(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_ULONG_TEXT(expected, actual, text)                         \
  cppmu_check_equal_ulong(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_LONGLONG(expected, actual)                                 \
  cppmu_check_equal_longlong(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_LONGLONG_TEXT(expected, actual, text)                      \
  cppmu_check_equal_longlong(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_ULONGLONG(expected, actual)                                \
  cppmu_check_equal_ulonglong(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_ULONGLONG_TEXT(expected, actual, text)                     \
  cppmu_check_equal_ulonglong(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_DOUBLE(expected, actual, threshold)                        \
  cppmu_check_equal_double(expected, actual, threshold, "", __FILE__, __LINE__)

#define CHECK_EQUAL_DOUBLE_TEXT(expected, actual, threshold, text)             \
  cppmu_check_equal_double(                                                    \
      expected, actual, threshold, text, __FILE__, __LINE__                    \
  )

#define CHECK_EQUAL_CHAR(expected, actual)                                     \
  cppmu_check_equal_char(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_CHAR_TEXT(expected, actual, text)                          \
  cppmu_check_equal_char(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_UBYTE(expected, actual)                                    \
  cppmu_check_equal_ubyte(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_UBYTE_TEXT(expected, actual, text)                         \
  cppmu_check_equal_ubyte(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_SBYTE(expected, actual)                                    \
  cppmu_check_equal_sbyte(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_SBYTE_TEXT(expected, actual, text)                         \
  cppmu_check_equal_sbyte(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_STRING(expected, actual)                                   \
  cppmu_check_equal_string(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_STRING_TEXT(expected, actual, text)                        \
  cppmu_check_equal_string(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_POINTER(expected, actual)                                  \
  cppmu_check_equal_pointer(expected, actual, "", __FILE__, __LINE__)

#define CHECK_EQUAL_POINTER_TEXT(expected, actual, text)                       \
  cppmu_check_equal_pointer(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_MEMCMP(expected, actual, size)                             \
  cppmu_check_equal_memcmp(expected, actual, size, "", __FILE__, __LINE__)

#define CHECK_EQUAL_MEMCMP_TEXT(expected, actual, size, text)                  \
  cppmu_check_equal_memcmp(expected, actual, size, text, __FILE__, __LINE__)

#define FAIL_TEXT(text) cppmu_fail(text, __FILE__, __LINE__)

#define FAIL() cppmu_fail("", __FILE__, __LINE__)

#define CHECK(condition)                                                       \
  cppmu_check(condition, #condition, "", __FILE__, __LINE__)

#define CHECK_TEXT(condition, text)                                            \
  cppmu_check(condition, #condition, text, __FILE__, __LINE__)

#define TEST_PROPERTY(name, value) cppmu_add_test_property_c((name), (value))

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

  void cppmu_check_equal_bool(
      bool expected,
      bool actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void cppmu_check_equal_int(
      int expected,
      int actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void cppmu_check_equal_uint(
      unsigned int expected,
      unsigned int actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void cppmu_check_equal_long(
      long expected,
      long actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void cppmu_check_equal_ulong(
      unsigned long expected,
      unsigned long actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void cppmu_check_equal_longlong(
      long long expected,
      long long actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void cppmu_check_equal_ulonglong(
      unsigned long long expected,
      unsigned long long actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void cppmu_check_equal_double(
      double expected,
      double actual,
      double threshold,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void cppmu_check_equal_char(
      char expected,
      char actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void cppmu_check_equal_ubyte(
      unsigned char expected,
      unsigned char actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void cppmu_check_equal_sbyte(
      signed char expected,
      signed char actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void cppmu_check_equal_string(
      const char* expected,
      const char* actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void cppmu_check_equal_pointer(
      const void* expected,
      const void* actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void cppmu_check_equal_memcmp(
      const void* expected,
      const void* actual,
      size_t size,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void cppmu_fail(const char* text, const char* file_name, size_t line_number);
  void cppmu_check(
      int condition,
      const char* condition_string,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  void cppmu_add_test_property(const char* name, const char* value);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_CPPMU_CPPMU_H */
