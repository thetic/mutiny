#include "CppMu/CppMu.h"

#include <stdbool.h>

static int setup_teardown_was_called_in_test_group_in_c = 0;
static int test_was_called_in_test_group_in_c = 0;

TEST_GROUP_SETUP(CppMuC)
{
  setup_teardown_was_called_in_test_group_in_c++;
}

TEST_GROUP_TEARDOWN(CppMuC)
{
  setup_teardown_was_called_in_test_group_in_c--;
  CHECK(test_was_called_in_test_group_in_c == 1);
  test_was_called_in_test_group_in_c--;
}

TEST(CppMuC, checkThatTheTestHasRun)
{
  test_was_called_in_test_group_in_c++;
}

IGNORE_TEST(CppMuC, ignoreMacroForCFile)
{
  test_was_called_in_test_group_in_c++;
}

TEST(CppMuC, bool)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_BOOL(1, 1);
  CHECK_EQUAL_BOOL(1, 2);
}

TEST(CppMuC, boolText)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_BOOL_TEXT(1, 1, "Text");
  CHECK_EQUAL_BOOL_TEXT(1, 2, "Text");
}

TEST(CppMuC, int)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_INT(2, 2);
}

TEST(CppMuC, intText)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_INT_TEXT(2, 2, "Text");
}

TEST(CppMuC, unsignedInt)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_UINT(2, 2);
}

TEST(CppMuC, unsignedIntText)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_UINT_TEXT(2, 2, "Text");
}

TEST(CppMuC, long)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_LONG(2, 2);
}

TEST(CppMuC, longText)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_LONG_TEXT(2, 2, "Text");
}

TEST(CppMuC, ulong)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_ULONG(2, 2);
}

TEST(CppMuC, ulongText)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_ULONG_TEXT(2, 2, "Text");
}

TEST(CppMuC, longLong)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_LONGLONG(2, 2);
}

TEST(CppMuC, longLongText)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_LONGLONG_TEXT(2, 2, "Text");
}

TEST(CppMuC, uLongLong)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_ULONGLONG(2, 2);
}

TEST(CppMuC, uLongLongText)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_ULONGLONG_TEXT(2, 2, "Text");
}

TEST(CppMuC, real)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_REAL(1.0, 1.1, 0.5);
}

TEST(CppMuC, realText)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_REAL_TEXT(1.0, 1.1, 0.5, "Text");
}

TEST(CppMuC, char)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_CHAR('a', 'a');
}

TEST(CppMuC, charText)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_CHAR_TEXT('a', 'a', "Text");
}

TEST(CppMuC, uByte)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_UBYTE(254, 254);
}

TEST(CppMuC, uByteText)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_UBYTE_TEXT(254, 254, "Text");
}

TEST(CppMuC, sByte)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_SBYTE(-3, -3);
}

TEST(CppMuC, sbyteText)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_SBYTE_TEXT(-3, -3, "Text");
}

TEST(CppMuC, string)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_STRING("Hello", "Hello");
}

TEST(CppMuC, stringText)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_STRING_TEXT("Hello", "Hello", "Text");
}

TEST(CppMuC, pointer)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_POINTER(NULL, NULL);
}

TEST(CppMuC, pointerText)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_POINTER_TEXT(NULL, NULL, "Text");
}

TEST(CppMuC, memcmp)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_MEMCMP("TEST", "TEST", 5);
}

TEST(CppMuC, memcmpText)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_MEMCMP_TEXT("TEST", "TEST", 5, "Text");
}

TEST(CppMuC, check)
{
  test_was_called_in_test_group_in_c++;
  CHECK(true);
}

TEST(CppMuC, checkText)
{
  test_was_called_in_test_group_in_c++;
  CHECK_TEXT(true, "Text");
}

EXPECT_FAIL_TEST(CppMuC, boolFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_BOOL(1, 0);
}

EXPECT_FAIL_TEST(CppMuC, boolTextFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_BOOL_TEXT(1, 0, "Text");
}

EXPECT_FAIL_TEST(CppMuC, intFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_INT(2, 3);
}

EXPECT_FAIL_TEST(CppMuC, intTextFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_INT_TEXT(2, 3, "Text");
}

EXPECT_FAIL_TEST(CppMuC, unsignedIntFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_UINT(2, 3);
}

EXPECT_FAIL_TEST(CppMuC, unsignedIntTextFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_UINT_TEXT(2, 3, "Text");
}

EXPECT_FAIL_TEST(CppMuC, longFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_LONG(2, 3);
}

EXPECT_FAIL_TEST(CppMuC, longTextFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_LONG_TEXT(2, 3, "Text");
}

EXPECT_FAIL_TEST(CppMuC, ulongFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_ULONG(2, 3);
}

EXPECT_FAIL_TEST(CppMuC, ulongTextFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_ULONG_TEXT(2, 3, "Text");
}

EXPECT_FAIL_TEST(CppMuC, longLongFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_LONGLONG(2, 3);
}

EXPECT_FAIL_TEST(CppMuC, longLongTextFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_LONGLONG_TEXT(2, 3, "Text");
}

EXPECT_FAIL_TEST(CppMuC, uLongLongFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_ULONGLONG(2, 3);
}

EXPECT_FAIL_TEST(CppMuC, uLongLongTextFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_ULONGLONG_TEXT(2, 3, "Text");
}

EXPECT_FAIL_TEST(CppMuC, realFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_REAL(1.0, 3.0, 0.5);
}

EXPECT_FAIL_TEST(CppMuC, realTextFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_REAL_TEXT(1.0, 3.0, 0.5, "Text");
}

EXPECT_FAIL_TEST(CppMuC, charFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_CHAR('a', 'b');
}

EXPECT_FAIL_TEST(CppMuC, charTextFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_CHAR_TEXT('a', 'b', "Text");
}

EXPECT_FAIL_TEST(CppMuC, uByteFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_UBYTE(254, 253);
}

EXPECT_FAIL_TEST(CppMuC, uByteTextFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_UBYTE_TEXT(254, 253, "Text");
}

EXPECT_FAIL_TEST(CppMuC, sByteFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_SBYTE(-3, -4);
}

EXPECT_FAIL_TEST(CppMuC, sbyteTextFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_SBYTE_TEXT(-3, -4, "Text");
}

EXPECT_FAIL_TEST(CppMuC, stringFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_STRING("Hello", "World");
}

EXPECT_FAIL_TEST(CppMuC, stringTextFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_STRING_TEXT("Hello", "World", "Text");
}

EXPECT_FAIL_TEST(CppMuC, pointerFail)
{
  int x = 0;
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_POINTER(NULL, &x);
}

EXPECT_FAIL_TEST(CppMuC, pointerTextFail)
{
  int x = 0;
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_POINTER_TEXT(NULL, &x, "Text");
}

EXPECT_FAIL_TEST(CppMuC, memcmpFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_MEMCMP("ABCD", "TEST", 4);
}

EXPECT_FAIL_TEST(CppMuC, memcmpTextFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_EQUAL_MEMCMP_TEXT("ABCD", "TEST", 4, "Text");
}

EXPECT_FAIL_TEST(CppMuC, failText)
{
  test_was_called_in_test_group_in_c++;
  FAIL_TEXT("expected failure");
}

EXPECT_FAIL_TEST(CppMuC, fail)
{
  test_was_called_in_test_group_in_c++;
  FAIL();
}

EXPECT_FAIL_TEST(CppMuC, checkFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK(false);
}

EXPECT_FAIL_TEST(CppMuC, checkTextFail)
{
  test_was_called_in_test_group_in_c++;
  CHECK_TEXT(false, "Text");
}
