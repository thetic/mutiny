/******************************************************************************
 *
 * Provides an interface for when working with pure C
 *
 *******************************************************************************/

#ifndef INCLUDED_CPPUTEST_TESTHARNESS_H
#define INCLUDED_CPPUTEST_TESTHARNESS_H

#include <stddef.h>

#define CHECK_EQUAL_C_BOOL(expected, actual)                                   \
  CHECK_EQUAL_C_BOOL_LOCATION(expected, actual, NULL, __FILE__, __LINE__)

#define CHECK_EQUAL_C_BOOL_TEXT(expected, actual, text)                        \
  CHECK_EQUAL_C_BOOL_LOCATION(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_C_INT(expected, actual)                                    \
  CHECK_EQUAL_C_INT_LOCATION(expected, actual, NULL, __FILE__, __LINE__)

#define CHECK_EQUAL_C_INT_TEXT(expected, actual, text)                         \
  CHECK_EQUAL_C_INT_LOCATION(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_C_UINT(expected, actual)                                   \
  CHECK_EQUAL_C_UINT_LOCATION(expected, actual, NULL, __FILE__, __LINE__)

#define CHECK_EQUAL_C_UINT_TEXT(expected, actual, text)                        \
  CHECK_EQUAL_C_UINT_LOCATION(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_C_LONG(expected, actual)                                   \
  CHECK_EQUAL_C_LONG_LOCATION(expected, actual, NULL, __FILE__, __LINE__)

#define CHECK_EQUAL_C_LONG_TEXT(expected, actual, text)                        \
  CHECK_EQUAL_C_LONG_LOCATION(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_C_ULONG(expected, actual)                                  \
  CHECK_EQUAL_C_ULONG_LOCATION(expected, actual, NULL, __FILE__, __LINE__)

#define CHECK_EQUAL_C_ULONG_TEXT(expected, actual, text)                       \
  CHECK_EQUAL_C_ULONG_LOCATION(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_C_LONGLONG(expected, actual)                               \
  CHECK_EQUAL_C_LONGLONG_LOCATION(expected, actual, NULL, __FILE__, __LINE__)

#define CHECK_EQUAL_C_LONGLONG_TEXT(expected, actual, text)                    \
  CHECK_EQUAL_C_LONGLONG_LOCATION(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_C_ULONGLONG(expected, actual)                              \
  CHECK_EQUAL_C_ULONGLONG_LOCATION(expected, actual, NULL, __FILE__, __LINE__)

#define CHECK_EQUAL_C_ULONGLONG_TEXT(expected, actual, text)                   \
  CHECK_EQUAL_C_ULONGLONG_LOCATION(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_C_REAL(expected, actual, threshold)                        \
  CHECK_EQUAL_C_REAL_LOCATION(                                                 \
      expected, actual, threshold, NULL, __FILE__, __LINE__)

#define CHECK_EQUAL_C_REAL_TEXT(expected, actual, threshold, text)             \
  CHECK_EQUAL_C_REAL_LOCATION(                                                 \
      expected, actual, threshold, text, __FILE__, __LINE__)

#define CHECK_EQUAL_C_CHAR(expected, actual)                                   \
  CHECK_EQUAL_C_CHAR_LOCATION(expected, actual, NULL, __FILE__, __LINE__)

#define CHECK_EQUAL_C_CHAR_TEXT(expected, actual, text)                        \
  CHECK_EQUAL_C_CHAR_LOCATION(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_C_UBYTE(expected, actual)                                  \
  CHECK_EQUAL_C_UBYTE_LOCATION(expected, actual, NULL, __FILE__, __LINE__)

#define CHECK_EQUAL_C_UBYTE_TEXT(expected, actual, text)                       \
  CHECK_EQUAL_C_UBYTE_LOCATION(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_C_SBYTE(expected, actual)                                  \
  CHECK_EQUAL_C_SBYTE_LOCATION(expected, actual, NULL, __FILE__, __LINE__)

#define CHECK_EQUAL_C_SBYTE_TEXT(expected, actual, text)                       \
  CHECK_EQUAL_C_SBYTE_LOCATION(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_C_STRING(expected, actual)                                 \
  CHECK_EQUAL_C_STRING_LOCATION(expected, actual, NULL, __FILE__, __LINE__)

#define CHECK_EQUAL_C_STRING_TEXT(expected, actual, text)                      \
  CHECK_EQUAL_C_STRING_LOCATION(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_C_POINTER(expected, actual)                                \
  CHECK_EQUAL_C_POINTER_LOCATION(expected, actual, NULL, __FILE__, __LINE__)

#define CHECK_EQUAL_C_POINTER_TEXT(expected, actual, text)                     \
  CHECK_EQUAL_C_POINTER_LOCATION(expected, actual, text, __FILE__, __LINE__)

#define CHECK_EQUAL_C_MEMCMP(expected, actual, size)                           \
  CHECK_EQUAL_C_MEMCMP_LOCATION(                                               \
      expected, actual, size, NULL, __FILE__, __LINE__)

#define CHECK_EQUAL_C_MEMCMP_TEXT(expected, actual, size, text)                \
  CHECK_EQUAL_C_MEMCMP_LOCATION(                                               \
      expected, actual, size, text, __FILE__, __LINE__)

#define FAIL_TEXT_C(text) FAIL_TEXT_C_LOCATION(text, __FILE__, __LINE__)

#define FAIL_C() FAIL_C_LOCATION(__FILE__, __LINE__)

#define CHECK_C(condition)                                                     \
  CHECK_C_LOCATION(condition, #condition, NULL, __FILE__, __LINE__)

#define CHECK_C_TEXT(condition, text)                                          \
  CHECK_C_LOCATION(condition, #condition, text, __FILE__, __LINE__)

/******************************************************************************
 *
 * TEST macros for in C.
 *
 *******************************************************************************/

/* For use in C file */
#define TEST_GROUP_C_SETUP(group_name)                                         \
  extern void group_##group_name##_setup_wrapper_c(void);                      \
  void group_##group_name##_setup_wrapper_c(void)

#define TEST_GROUP_C_TEARDOWN(group_name)                                      \
  extern void group_##group_name##_teardown_wrapper_c(void);                   \
  void group_##group_name##_teardown_wrapper_c(void)

#define TEST_C(group_name, test_name)                                          \
  extern void test_##group_name##_##test_name##_wrapper_c(void);               \
  void test_##group_name##_##test_name##_wrapper_c(void)

#define IGNORE_TEST_C(group_name, test_name)                                   \
  extern void ignore_##group_name##_##test_name##_wrapper_c(void);             \
  void ignore_##group_name##_##test_name##_wrapper_c(void)

/* For use in C++ file */

#define TEST_GROUP_C_WRAPPER(group_name)                                       \
  extern "C" void group_##group_name##_setup_wrapper_c(void);                  \
  extern "C" void group_##group_name##_teardown_wrapper_c(void);               \
  TEST_GROUP(group_name)

#define TEST_GROUP_C_SETUP_WRAPPER(group_name)                                 \
  void setup() override                                                        \
  {                                                                            \
    group_##group_name##_setup_wrapper_c();                                    \
  }

#define TEST_GROUP_C_TEARDOWN_WRAPPER(group_name)                              \
  void teardown() override                                                     \
  {                                                                            \
    group_##group_name##_teardown_wrapper_c();                                 \
  }

#define TEST_C_WRAPPER(group_name, test_name)                                  \
  extern "C" void test_##group_name##_##test_name##_wrapper_c();               \
  TEST(group_name, test_name)                                                  \
  {                                                                            \
    test_##group_name##_##test_name##_wrapper_c();                             \
  }

#define IGNORE_TEST_C_WRAPPER(group_name, test_name)                           \
  extern "C" void ignore_##group_name##_##test_name##_wrapper_c();             \
  IGNORE_TEST(group_name, test_name)                                           \
  {                                                                            \
    ignore_##group_name##_##test_name##_wrapper_c();                           \
  }

#ifdef __cplusplus
extern "C"
{
#endif

  /* CHECKS that can be used from C code */
  void CHECK_EQUAL_C_BOOL_LOCATION(int expected,
      int actual,
      const char* text,
      const char* fileName,
      size_t lineNumber);
  void CHECK_EQUAL_C_INT_LOCATION(int expected,
      int actual,
      const char* text,
      const char* fileName,
      size_t lineNumber);
  void CHECK_EQUAL_C_UINT_LOCATION(unsigned int expected,
      unsigned int actual,
      const char* text,
      const char* fileName,
      size_t lineNumber);
  void CHECK_EQUAL_C_LONG_LOCATION(long expected,
      long actual,
      const char* text,
      const char* fileName,
      size_t lineNumber);
  void CHECK_EQUAL_C_ULONG_LOCATION(unsigned long expected,
      unsigned long actual,
      const char* text,
      const char* fileName,
      size_t lineNumber);
  void CHECK_EQUAL_C_LONGLONG_LOCATION(long long expected,
      long long actual,
      const char* text,
      const char* fileName,
      size_t lineNumber);
  void CHECK_EQUAL_C_ULONGLONG_LOCATION(unsigned long long expected,
      unsigned long long actual,
      const char* text,
      const char* fileName,
      size_t lineNumber);
  void CHECK_EQUAL_C_REAL_LOCATION(double expected,
      double actual,
      double threshold,
      const char* text,
      const char* fileName,
      size_t lineNumber);
  void CHECK_EQUAL_C_CHAR_LOCATION(char expected,
      char actual,
      const char* text,
      const char* fileName,
      size_t lineNumber);
  void CHECK_EQUAL_C_UBYTE_LOCATION(unsigned char expected,
      unsigned char actual,
      const char* text,
      const char* fileName,
      size_t lineNumber);
  void CHECK_EQUAL_C_SBYTE_LOCATION(signed char expected,
      signed char actual,
      const char* text,
      const char* fileName,
      size_t lineNumber);
  void CHECK_EQUAL_C_STRING_LOCATION(const char* expected,
      const char* actual,
      const char* text,
      const char* fileName,
      size_t lineNumber);
  void CHECK_EQUAL_C_POINTER_LOCATION(const void* expected,
      const void* actual,
      const char* text,
      const char* fileName,
      size_t lineNumber);
  void CHECK_EQUAL_C_MEMCMP_LOCATION(const void* expected,
      const void* actual,
      size_t size,
      const char* text,
      const char* fileName,
      size_t lineNumber);
  void FAIL_TEXT_C_LOCATION(const char* text,
      const char* fileName,
      size_t lineNumber);
  void FAIL_C_LOCATION(const char* fileName, size_t lineNumber);
  void CHECK_C_LOCATION(int condition,
      const char* conditionString,
      const char* text,
      const char* fileName,
      size_t lineNumber);

#ifdef __cplusplus
}
#endif

#endif
