#include "CppUTest/TestHarness.h"

#include "CppUTest/TestShell.hpp"

void
CHECK_EQUAL_C_BOOL_LOCATION(int expected,
    int actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertEquals(!!expected != !!actual,
      expected ? "true" : "false",
      actual ? "true" : "false",
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_INT_LOCATION(int expected,
    int actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertLongsEqual(
      static_cast<long>(expected),
      static_cast<long>(actual),
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_UINT_LOCATION(unsigned int expected,
    unsigned int actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertUnsignedLongsEqual(
      static_cast<unsigned long>(expected),
      static_cast<unsigned long>(actual),
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_LONG_LOCATION(long expected,
    long actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertLongsEqual(expected,
      actual,
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_ULONG_LOCATION(unsigned long expected,
    unsigned long actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertUnsignedLongsEqual(expected,
      actual,
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_LONGLONG_LOCATION(long long expected,
    long long actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertLongLongsEqual(expected,
      actual,
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_ULONGLONG_LOCATION(unsigned long long expected,
    unsigned long long actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertUnsignedLongLongsEqual(expected,
      actual,
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_REAL_LOCATION(double expected,
    double actual,
    double threshold,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertDoublesEqual(expected,
      actual,
      threshold,
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_CHAR_LOCATION(char expected,
    char actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertEquals(((expected) != (actual)),
      cpputest::StringFrom(expected),
      cpputest::StringFrom(actual),
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_UBYTE_LOCATION(unsigned char expected,
    unsigned char actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertEquals(((expected) != (actual)),
      cpputest::StringFrom(static_cast<int>(expected)),
      cpputest::StringFrom(static_cast<int>(actual)),
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_SBYTE_LOCATION(char signed expected,
    signed char actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertEquals(((expected) != (actual)),
      cpputest::StringFrom(static_cast<int>(expected)),
      cpputest::StringFrom(static_cast<int>(actual)),
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_STRING_LOCATION(const char* expected,
    const char* actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertCstrEqual(expected,
      actual,
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_POINTER_LOCATION(const void* expected,
    const void* actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertPointersEqual(expected,
      actual,
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_MEMCMP_LOCATION(const void* expected,
    const void* actual,
    size_t size,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertBinaryEqual(expected,
      actual,
      size,
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
FAIL_TEXT_C_LOCATION(const char* text, const char* fileName, size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->fail(text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
FAIL_C_LOCATION(const char* fileName, size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->fail("",
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_C_LOCATION(int condition,
    const char* conditionString,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertTrue(condition != 0,
      "CHECK_C",
      conditionString,
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}
