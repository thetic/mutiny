#include "CppMu/CppMu.h"

#include "CppMu.test.h"

#include "CppMu/CppMu.hpp"
#include "CppMu/TestOutput.hpp"
#include "CppMu/TestRegistry.hpp"
#include "CppMu/TestTestingFixture.hpp"

int setup_teardown_was_called_in_test_group_in_c = 0;
int test_was_called_in_test_group_in_c = 0;

TEST_GROUP_C_WRAPPER(CppMuC)
{
  TEST_GROUP_C_SETUP_WRAPPER(CppMuC)
  TEST_GROUP_C_TEARDOWN_WRAPPER(CppMuC)
};

TEST_C_WRAPPER(CppMuC, checkThatTheTestHasRun)
IGNORE_TEST_C_WRAPPER(CppMuC, ignoreMacroForCFile)

/*
 * This test is a bit strange. They use the fact that you can do -r2 for
 * repeating the same run. When you do so, the same statics will be shared and
 * therefore we can test whether the setup/teardown is run correctly.
 */

TEST(CppMuC, setupHasBeenCalled)
{
  test_was_called_in_test_group_in_c++;
  /* Increased in setup, decreased in teardown. So at this point it must be 1
   * also on a multiple run */
  LONGS_EQUAL(1, setup_teardown_was_called_in_test_group_in_c);
}

namespace {

bool has_destructor_of_the_destructor_checked_been_called;

class HasTheDestructorBeenCalledChecker
{
public:
  HasTheDestructorBeenCalledChecker() = default;
  ~HasTheDestructorBeenCalledChecker()
  {
    has_destructor_of_the_destructor_checked_been_called = true;
  }
};

void
fail_bool_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_BOOL(1, 0);
}

void
fail_bool_text_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_BOOL_TEXT(1, 0, "BoolTestText");
}

void
fail_int_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_INT(1, 2);
}

void
fail_int_text_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_INT_TEXT(1, 2, "IntTestText");
}

void
fail_unsigned_int_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_UINT(1, 2);
}

void
fail_unsigned_int_text_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_UINT_TEXT(1, 2, "UnsignedIntTestText");
}

void
fail_long_int_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_LONG(1, 2);
}

void
fail_long_int_text_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_LONG_TEXT(1, 2, "LongIntTestText");
}

void
fail_unsigned_long_int_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_ULONG(1, 2);
}

void
fail_unsigned_long_int_text_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_ULONG_TEXT(1, 2, "UnsignedLongIntTestText");
}

void
fail_long_long_int_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_LONGLONG(1, 2);
}

void
fail_long_long_int_text_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_LONGLONG_TEXT(1, 2, "LongLongTestText");
}

void
fail_unsigned_long_long_int_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_ULONGLONG(1, 2);
}

void
fail_unsigned_long_long_int_text_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_ULONGLONG_TEXT(1, 2, "UnsignedLongLongTestText");
}

void
fail_real_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_REAL(1.0, 2.0, 0.5);
}

void
fail_real_text_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_REAL_TEXT(1.0, 2.0, 0.5, "RealTestText");
}

void
fail_char_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_CHAR('a', 'c');
}

void
fail_char_text_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_CHAR_TEXT('a', 'c', "CharTestText");
}

void
fail_unsigned_byte_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_UBYTE(254, 253);
}

void
fail_unsigned_byte_text_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_UBYTE_TEXT(254, 253, "UnsignedByteTestText");
}

void
fail_signed_byte_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_SBYTE(-3, -5);
}

void
fail_signed_byte_text_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_SBYTE_TEXT(-3, -5, "SignedByteTestText");
}

void
fail_string_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_STRING("Hello", "Hello World");
}

void
fail_string_text_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_STRING_TEXT("Hello", "Hello World", "StringTestText");
}

void
fail_pointer_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_POINTER(nullptr, reinterpret_cast<void*>(0x1));
}

void
fail_pointer_text_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_POINTER_TEXT(
      nullptr, reinterpret_cast<void*>(0x1), "PointerTestText");
}

void
fail_memcmp_method()
{
  HasTheDestructorBeenCalledChecker checker;
  unsigned char expected_data[] = { 0x00, 0x01, 0x02, 0x03 };
  unsigned char actual_data[] = { 0x00, 0x01, 0x03, 0x03 };

  CHECK_EQUAL_C_MEMCMP(expected_data, actual_data, sizeof(expected_data));
}

void
fail_memcmp_text_method()
{
  HasTheDestructorBeenCalledChecker checker;
  unsigned char expected_data[] = { 0x00, 0x01, 0x02, 0x03 };
  unsigned char actual_data[] = { 0x00, 0x01, 0x03, 0x03 };

  CHECK_EQUAL_C_MEMCMP_TEXT(
      expected_data, actual_data, sizeof(expected_data), "MemcmpTestText");
}

void
fail_text_method()
{
  HasTheDestructorBeenCalledChecker checker;
  FAIL_TEXT_C("Booo");
}

void
fail_method()
{
  HasTheDestructorBeenCalledChecker checker;
  FAIL_C();
}

bool cppmu_has_crashed;

void
crash_method()
{
  cppmu_has_crashed = true;
}

void
check_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_C(false);
}

void
check_text_method()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_C_TEXT(false, "CheckTestText");
}

}

TEST_GROUP(CppMu)
{
  cppmu::TestTestingFixture* fixture;
  TEST_SETUP()
  {
    has_destructor_of_the_destructor_checked_been_called = false;
    fixture = new cppmu::TestTestingFixture();
  }
  TEST_TEARDOWN() { delete fixture; }
};

TEST(CppMu, checkBool)
{
  CHECK_EQUAL_C_BOOL(1, 1);
  CHECK_EQUAL_C_BOOL(1, 2);
  fixture->set_test_function(fail_bool_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <true>\n\tbut was  <false>");
  fixture->assert_print_contains("CppMu");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkBoolText)
{
  CHECK_EQUAL_C_BOOL_TEXT(1, 1, "Text");
  CHECK_EQUAL_C_BOOL_TEXT(1, 2, "Text");
  fixture->set_test_function(fail_bool_text_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <true>\n\tbut was  <false>");
  fixture->assert_print_contains("CppMu");
  fixture->assert_print_contains("Message: BoolTestText");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkInt)
{
  CHECK_EQUAL_C_INT(2, 2);
  fixture->set_test_function(fail_int_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assert_print_contains("CppMu");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkIntText)
{
  CHECK_EQUAL_C_INT_TEXT(2, 2, "Text");
  fixture->set_test_function(fail_int_text_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assert_print_contains("CppMu");
  fixture->assert_print_contains("Message: IntTestText");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkUnsignedInt)
{
  CHECK_EQUAL_C_UINT(2, 2);
  fixture->set_test_function(fail_unsigned_int_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assert_print_contains("CppMu");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkUnsignedIntText)
{
  CHECK_EQUAL_C_UINT_TEXT(2, 2, "Text");
  fixture->set_test_function(fail_unsigned_int_text_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assert_print_contains("CppMu");
  fixture->assert_print_contains("Message: UnsignedIntTestText");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkLongInt)
{
  CHECK_EQUAL_C_LONG(2, 2);
  fixture->set_test_function(fail_long_int_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assert_print_contains("CppMu");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkLongIntText)
{
  CHECK_EQUAL_C_LONG_TEXT(2, 2, "Text");
  fixture->set_test_function(fail_long_int_text_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assert_print_contains("CppMu");
  fixture->assert_print_contains("Message: LongIntTestText");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkUnsignedLongInt)
{
  CHECK_EQUAL_C_ULONG(2, 2);
  fixture->set_test_function(fail_unsigned_long_int_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assert_print_contains("CppMu");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkUnsignedLongIntText)
{
  CHECK_EQUAL_C_ULONG_TEXT(2, 2, "Text");
  fixture->set_test_function(fail_unsigned_long_int_text_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assert_print_contains("CppMu");
  fixture->assert_print_contains("Message: UnsignedLongIntTestText");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkLongLongInt)
{
  CHECK_EQUAL_C_LONGLONG(2, 2);
  fixture->set_test_function(fail_long_long_int_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assert_print_contains("CppMu");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkLongLongIntText)
{
  CHECK_EQUAL_C_LONGLONG_TEXT(2, 2, "Text");
  fixture->set_test_function(fail_long_long_int_text_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assert_print_contains("CppMu");
  fixture->assert_print_contains("Message: LongLongTestText");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkUnsignedLongLongInt)
{
  CHECK_EQUAL_C_ULONGLONG(2, 2);
  fixture->set_test_function(fail_unsigned_long_long_int_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assert_print_contains("CppMu");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkUnsignedLongLongIntText)
{
  CHECK_EQUAL_C_ULONGLONG_TEXT(2, 2, "Text");
  fixture->set_test_function(fail_unsigned_long_long_int_text_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assert_print_contains("CppMu");
  fixture->assert_print_contains("Message: UnsignedLongLongTestText");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkReal)
{
  CHECK_EQUAL_C_REAL(1.0, 1.1, 0.5);
  fixture->set_test_function(fail_real_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <1>\n\tbut was  <2>");
  fixture->assert_print_contains("CppMu");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkRealText)
{
  CHECK_EQUAL_C_REAL_TEXT(1.0, 1.1, 0.5, "Text");
  fixture->set_test_function(fail_real_text_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <1>\n\tbut was  <2>");
  fixture->assert_print_contains("CppMu");
  fixture->assert_print_contains("Message: RealTestText");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkChar)
{
  CHECK_EQUAL_C_CHAR('a', 'a');
  fixture->set_test_function(fail_char_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <a>\n\tbut was  <c>");
  fixture->assert_print_contains("CppMu");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkCharText)
{
  CHECK_EQUAL_C_CHAR_TEXT('a', 'a', "Text");
  fixture->set_test_function(fail_char_text_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <a>\n\tbut was  <c>");
  fixture->assert_print_contains("CppMu");
  fixture->assert_print_contains("Message: CharTestText");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkUnsignedByte)
{
  CHECK_EQUAL_C_UBYTE(254, 254);
  fixture->set_test_function(fail_unsigned_byte_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <254>\n\tbut was  <253>");
  fixture->assert_print_contains("CppMu");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkUnsignedByteText)
{
  CHECK_EQUAL_C_UBYTE_TEXT(254, 254, "Text");
  fixture->set_test_function(fail_unsigned_byte_text_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <254>\n\tbut was  <253>");
  fixture->assert_print_contains("CppMu");
  fixture->assert_print_contains("Message: UnsignedByteTestText");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkSignedByte)
{
  CHECK_EQUAL_C_SBYTE(-3, -3);
  fixture->set_test_function(fail_signed_byte_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <-3>\n\tbut was  <-5>");
  fixture->assert_print_contains("CppMu");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkSignedByteText)
{
  CHECK_EQUAL_C_SBYTE_TEXT(-3, -3, "Text");
  fixture->set_test_function(fail_signed_byte_text_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <-3>\n\tbut was  <-5>");
  fixture->assert_print_contains("CppMu");
  fixture->assert_print_contains("Message: SignedByteTestText");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkString)
{
  CHECK_EQUAL_C_STRING("Hello", "Hello");
  fixture->set_test_function(fail_string_method);
  fixture->run_all_tests();

  cppmu::StringEqualFailure failure(
      cppmu::TestShell::get_current(), "file", 1, "Hello", "Hello World", "");
  fixture->assert_print_contains(failure.get_message());
  fixture->assert_print_contains("CppMu");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkStringText)
{
  CHECK_EQUAL_C_STRING_TEXT("Hello", "Hello", "Text");
  fixture->set_test_function(fail_string_text_method);
  fixture->run_all_tests();

  cppmu::StringEqualFailure failure(
      cppmu::TestShell::get_current(), "file", 1, "Hello", "Hello World", "");
  fixture->assert_print_contains(failure.get_message());
  fixture->assert_print_contains("CppMu");
  fixture->assert_print_contains("Message: StringTestText");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkPointer)
{
  CHECK_EQUAL_C_POINTER(nullptr, nullptr);
  fixture->set_test_function(fail_pointer_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <0x0>\n\tbut was  <0x1>");
  fixture->assert_print_contains("CppMu");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkPointerText)
{
  CHECK_EQUAL_C_POINTER_TEXT(nullptr, nullptr, "Text");
  fixture->set_test_function(fail_pointer_text_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("expected <0x0>\n\tbut was  <0x1>");
  fixture->assert_print_contains("CppMu");
  fixture->assert_print_contains("Message: PointerTestText");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkMemcmp)
{
  CHECK_EQUAL_C_MEMCMP("TEST", "TEST", 5);
  fixture->set_test_function(fail_memcmp_method);
  fixture->run_all_tests();
  fixture->assert_print_contains(
      "expected <00 01 02 03>\n\tbut was  <00 01 03 03>");
  fixture->assert_print_contains("CppMu");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkMemcmpText)
{
  CHECK_EQUAL_C_MEMCMP_TEXT("TEST", "TEST", 5, "Text");
  fixture->set_test_function(fail_memcmp_text_method);
  fixture->run_all_tests();
  fixture->assert_print_contains(
      "expected <00 01 02 03>\n\tbut was  <00 01 03 03>");
  fixture->assert_print_contains("CppMu");
  fixture->assert_print_contains("Message: MemcmpTestText");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkFailText)
{
  fixture->set_test_function(fail_text_method);
  fixture->run_all_tests();
  fixture->assert_print_contains("Booo");
  fixture->assert_print_contains("CppMu");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkFail)
{
  fixture->set_test_function(fail_method);
  fixture->run_all_tests();
  LONGS_EQUAL(1, fixture->get_failure_count());
  fixture->assert_print_contains("CppMu");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, doesNotCrashIfNotSetToCrash)
{
  cppmu_has_crashed = false;
  cppmu::TestShell::set_crash_method(crash_method);
  fixture->set_test_function(fail_method);

  fixture->run_all_tests();

  CHECK_FALSE(cppmu_has_crashed);
  LONGS_EQUAL(1, fixture->get_failure_count());
  CHECK(!has_destructor_of_the_destructor_checked_been_called);

  cppmu::TestShell::reset_crash_method();
}

TEST(CppMu, doesCrashIfSetToCrash)
{
  cppmu_has_crashed = false;
  cppmu::TestShell::set_crash_on_fail();
  cppmu::TestShell::set_crash_method(crash_method);
  fixture->set_test_function(fail_method);

  fixture->run_all_tests();

  CHECK(cppmu_has_crashed);
  CHECK(!has_destructor_of_the_destructor_checked_been_called);

  cppmu::TestShell::restore_default_test_terminator();
  cppmu::TestShell::reset_crash_method();
}

TEST(CppMu, checkCheck)
{
  CHECK_C(true);
  fixture->set_test_function(check_method);
  fixture->run_all_tests();
  LONGS_EQUAL(1, fixture->get_failure_count());
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMu, checkCheckText)
{
  CHECK_C_TEXT(true, "Text");
  fixture->set_test_function(check_text_method);
  fixture->run_all_tests();
  LONGS_EQUAL(1, fixture->get_failure_count());
  fixture->assert_print_contains("Message: CheckTestText");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}
