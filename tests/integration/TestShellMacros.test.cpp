#include "mutiny/test/Shell.hpp"
#include "mutiny/test/TestingFixture.hpp"

#include "mutiny/test.hpp"

#include <limits.h>
#include <math.h>

#define CHECK_TEST_FAILS_PROPER_WITH_TEXT(text)                                \
  fixture.check_test_fails_with_proper_test_location(text, __FILE__, __LINE__)

// Mainly this is for Visual C++, but we'll define it for any system that has
// the same behavior

TEST_GROUP(TestShellMacros)
{
  mu::tiny::test::TestingFixture fixture;
};

namespace {

void failing_test_method_with_fail()
{
  FAIL("This test fails");
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_check()
{
  CHECK(false);
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_check_text()
{
  CHECK_TEXT(false, "Failed because it failed");
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_check_true()
{
  CHECK_TRUE(false);
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_check_true_text()
{
  CHECK_TRUE_TEXT(false, "Failed because it failed");
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_check_false()
{
  CHECK_FALSE(true);
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_check_false_text()
{
  CHECK_FALSE_TEXT(true, "Failed because it failed");
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_check_equal()
{
  CHECK_EQUAL(1, 2);
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_check_compare()
{
  double small = 0.5, big = 0.8;
  CHECK_COMPARE(small, >=, big);
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_check_compare_text()
{
  double small = 0.5, big = 0.8;
  CHECK_COMPARE_TEXT(small, >=, big, "small bigger than big");
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

int count_in_counting_method;
int counting_method()
{
  return count_in_counting_method++;
}

void failing_test_method_with_check_equal_text()
{
  CHECK_EQUAL_TEXT(1, 2, "Failed because it failed");
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_check_equal_zero()
{
  CHECK_EQUAL_ZERO(1);
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_check_equal_zero_text()
{
  CHECK_EQUAL_ZERO_TEXT(1, "Failed because it failed");
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_bytes_equal()
{
  BYTES_EQUAL('a', 'b');
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_bytes_equal_text()
{
  BYTES_EQUAL_TEXT('a', 'b', "Failed because it failed");
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_signed_bytes_equal()
{
  SIGNED_BYTES_EQUAL(-1, -2);
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_signed_bytes_equal_text()
{
  SIGNED_BYTES_EQUAL_TEXT(-127, -126, "Failed because it failed");
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_pointers_equal()
{
  POINTERS_EQUAL(
      reinterpret_cast<void*>(0xa5a5), reinterpret_cast<void*>(0xf0f0)
  );
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_pointers_equal_text()
{
  POINTERS_EQUAL_TEXT(
      reinterpret_cast<void*>(0xa5a5),
      reinterpret_cast<void*>(0xf0f0),
      "Failed because it failed"
  );
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_functionpointers_equal()
{
  FUNCTIONPOINTERS_EQUAL(
      reinterpret_cast<void (*)()>(0xa5a5), reinterpret_cast<void (*)()>(0xf0f0)
  );
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_functionpointers_equal_text()
{
  FUNCTIONPOINTERS_EQUAL_TEXT(
      reinterpret_cast<void (*)()>(0xa5a5),
      reinterpret_cast<void (*)()>(0xf0f0),
      "Failed because it failed"
  );
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_doubles_equal()
{
  DOUBLES_EQUAL(0.12, 44.1, 0.3);
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_doubles_equal_text()
{
  DOUBLES_EQUAL_TEXT(0.12, 44.1, 0.3, "Failed because it failed");
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

bool line_of_code_executed_after_check = false;

void passing_test_method()
{
  CHECK(true);
  line_of_code_executed_after_check = true;
}

int function_that_returns_a_value()
{
  CHECK(0 == 0);
  CHECK_TEXT(0 == 0, "Shouldn't fail");
  CHECK_TRUE(0 == 0);
  CHECK_TRUE_TEXT(0 == 0, "Shouldn't fail");
  CHECK_FALSE(0 != 0);
  CHECK_FALSE_TEXT(0 != 0, "Shouldn't fail");
  CHECK_EQUAL(1, 1);
  CHECK_EQUAL_TEXT(1, 1, "Shouldn't fail");
  BYTES_EQUAL(0xab, 0xab);
  BYTES_EQUAL_TEXT(0xab, 0xab, "Shouldn't fail");
  CHECK_EQUAL(100, 100);
  CHECK_EQUAL_TEXT(100, 100, "Shouldn't fail");
  CHECK_EQUAL_ZERO(0);
  CHECK_EQUAL_ZERO_TEXT(0, "Shouldn't fail");
  STRCMP_EQUAL("THIS", "THIS");
  STRCMP_EQUAL_TEXT("THIS", "THIS", "Shouldn't fail");
  DOUBLES_EQUAL(1.0, 1.0, .01);
  DOUBLES_EQUAL_TEXT(1.0, 1.0, .01, "Shouldn't fail");
  POINTERS_EQUAL(nullptr, nullptr);
  POINTERS_EQUAL_TEXT(nullptr, nullptr, "Shouldn't fail");
  MEMCMP_EQUAL("THIS", "THIS", 5);
  MEMCMP_EQUAL_TEXT("THIS", "THIS", 5, "Shouldn't fail");
  return 0;
}

void memcmp_equal_failing_test_method_with_unequal_input()
{
  unsigned char expected_data[] = { 0x00, 0x01, 0x02, 0x03 };
  unsigned char actual_data[] = { 0x00, 0x01, 0x03, 0x03 };

  MEMCMP_EQUAL(expected_data, actual_data, sizeof(expected_data));
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void memcmp_equal_failing_test_method_with_null_expected()
{
  unsigned char actual_data[] = { 0x00, 0x01, 0x02, 0x03 };

  MEMCMP_EQUAL(nullptr, actual_data, sizeof(actual_data));
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void memcmp_equal_failing_test_method_with_null_actual()
{
  unsigned char expected_data[] = { 0x00, 0x01, 0x02, 0x03 };

  MEMCMP_EQUAL(expected_data, nullptr, sizeof(expected_data));
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_memcmp_equal_text()
{
  unsigned char expected_data[] = { 0x00, 0x01, 0x02, 0x03 };
  unsigned char actual_data[] = { 0x00, 0x01, 0x03, 0x03 };

  MEMCMP_EQUAL_TEXT(
      expected_data,
      actual_data,
      sizeof(expected_data),
      "Failed because it failed"
  );
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

enum class ScopedIntEnum
{
  a,
  b
};

void enums_equal_int_with_scoped_int_enum_test_method()
{
  ENUMS_EQUAL_INT(ScopedIntEnum::b, ScopedIntEnum::b);
  ENUMS_EQUAL_INT(ScopedIntEnum::b, ScopedIntEnum::a);
}

void enums_equal_int_text_with_scoped_int_enum_test_method()
{
  ENUMS_EQUAL_INT_TEXT(
      ScopedIntEnum::b, ScopedIntEnum::a, "Failed because it failed"
  );
}

enum class ScopedLongEnum : long
{
  a,
  b
};

void enums_equal_type_with_scoped_long_enum_test_method()
{
  ENUMS_EQUAL_TYPE(long, ScopedLongEnum::b, ScopedLongEnum::b);
  ENUMS_EQUAL_TYPE(long, ScopedLongEnum::b, ScopedLongEnum::a);
}

void enums_equal_type_text_with_scoped_long_enum_test_method()
{
  ENUMS_EQUAL_TYPE_TEXT(
      long, ScopedLongEnum::b, ScopedLongEnum::a, "Failed because it failed"
  );
}

enum UnscopedEnum
{
  unscoped_enum_a,
  unscoped_enum_b
};

void enums_equal_int_with_unscoped_enum_test_method()
{
  ENUMS_EQUAL_INT(unscoped_enum_b, unscoped_enum_b);
  ENUMS_EQUAL_INT(unscoped_enum_b, unscoped_enum_a);
}

void enums_equal_int_text_with_unscoped_enum_test_method()
{
  ENUMS_EQUAL_INT_TEXT(
      unscoped_enum_b, unscoped_enum_a, "Failed because it failed"
  );
}

#if MUTINY_HAVE_EXCEPTIONS
void failing_test_method_no_throw_with_check_throws()
{
  CHECK_THROWS(int, (void)(1 + 2));
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void succeeding_test_method_correct_throw_with_check_throws()
{
  CHECK_THROWS(int, throw 4);
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_wrong_throw_with_check_throws()
{
  CHECK_THROWS(int, throw 4.3);
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

#endif

} // namespace

TEST(TestShellMacros, FAILMakesTheTestFailPrintsTheRightResultAndStopsExecuting)
{
  fixture.run_test_with_method(failing_test_method_with_fail);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("This test fails");
}

TEST(TestShellMacros, FAILWillPrintTheFileThatItFailed)
{
  fixture.run_test_with_method(failing_test_method_with_fail);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT(__FILE__);
}

TEST(TestShellMacros, FAILBehavesAsAProperMacro)
{
  if (false)
    FAIL("");
  else
    CHECK(true);

  if (true)
    CHECK(true);
  else
    FAIL("");
}

IGNORE_TEST(TestShellMacros, FAILworksInAnIgnoredTest)
{
  FAIL("die!");
}

TEST(TestShellMacros, FailureWithCHECK)
{
  fixture.run_test_with_method(failing_test_method_with_check);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK(false) failed");
}

TEST(TestShellMacros, CHECKBehavesAsProperMacro)
{
  if (false)
    CHECK(false);
  else
    CHECK(true);
}

IGNORE_TEST(TestShellMacros, CHECKWorksInAnIgnoredTest)
{
  CHECK(false);
}

TEST(TestShellMacros, FailureWithCHECK_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_check_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK(false) failed");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(TestShellMacros, CHECK_TEXTBehavesAsProperMacro)
{
  if (false)
    CHECK_TEXT(false, "false");
  else
    CHECK_TEXT(true, "true");
}

IGNORE_TEST(TestShellMacros, CHECK_TEXTWorksInAnIgnoredTest)
{
  CHECK_TEXT(false, "false");
}

TEST(TestShellMacros, FailureWithCHECK_TRUE)
{
  fixture.run_test_with_method(failing_test_method_with_check_true);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_TRUE(false) failed");
}

TEST(TestShellMacros, CHECK_TRUEBehavesAsProperMacro)
{
  if (false)
    CHECK_TRUE(false);
  else
    CHECK_TRUE(true);
}

IGNORE_TEST(TestShellMacros, CHECK_TRUEWorksInAnIgnoredTest)
{
  CHECK_TRUE(false);
}

TEST(TestShellMacros, FailureWithCHECK_TRUE_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_check_true_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_TRUE(false) failed");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(TestShellMacros, CHECK_TRUE_TEXTBehavesAsProperMacro)
{
  if (false)
    CHECK_TRUE_TEXT(false, "Failed because it failed");
  else
    CHECK_TRUE_TEXT(true, "Failed because it failed");
}

IGNORE_TEST(TestShellMacros, CHECK_TRUE_TEXTWorksInAnIgnoredTest)
{
  CHECK_TRUE_TEXT(false, "Failed because it failed");
}

TEST(TestShellMacros, FailureWithCHECK_FALSE)
{
  fixture.run_test_with_method(failing_test_method_with_check_false);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_FALSE(true) failed");
}

TEST(TestShellMacros, CHECK_FALSEBehavesAsProperMacro)
{
  if (false)
    CHECK_FALSE(true);
  else
    CHECK_FALSE(false);
}

IGNORE_TEST(TestShellMacros, CHECK_FALSEWorksInAnIgnoredTest)
{
  CHECK_FALSE(true);
}

TEST(TestShellMacros, FailureWithCHECK_FALSE_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_check_false_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_FALSE(true)");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(TestShellMacros, CHECK_FALSE_TEXTBehavesAsProperMacro)
{
  if (false)
    CHECK_FALSE_TEXT(true, "Failed because it failed");
  else
    CHECK_FALSE_TEXT(false, "Failed because it failed");
}

IGNORE_TEST(TestShellMacros, CHECK_FALSE_TEXTWorksInAnIgnoredTest)
{
  CHECK_FALSE_TEXT(true, "Failed because it failed");
}

TEST(TestShellMacros, FailureWithCHECK_EQUAL)
{
  fixture.run_test_with_method(failing_test_method_with_check_equal);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <2>");
}

TEST(TestShellMacros, FailureWithCHECK_COMPARE)
{
  fixture.run_test_with_method(failing_test_method_with_check_compare);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_COMPARE(0.5 >= 0.8)");
}

TEST(TestShellMacros, CHECK_COMPAREBehavesAsProperMacro)
{
  if (false)
    CHECK_COMPARE(1, >, 2);
  else
    CHECK_COMPARE(1, <, 2);
}

IGNORE_TEST(TestShellMacros, CHECK_COMPAREWorksInAnIgnoredTest)
{
  CHECK_COMPARE(1, >, 2);
}

TEST(TestShellMacros, FailureWithCHECK_COMPARE_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_check_compare_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_COMPARE(0.5 >= 0.8)");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("small bigger than big");
}

TEST(TestShellMacros, CHECK_COMPARE_TEXTBehavesAsProperMacro)
{
  if (false)
    CHECK_COMPARE_TEXT(1, >, 2, "1 bigger than 2");
  else
    CHECK_COMPARE_TEXT(1, <, 2, "1 smaller than 2");
}

IGNORE_TEST(TestShellMacros, CHECK_COMPARE_TEXTWorksInAnIgnoredTest)
{
  CHECK_COMPARE_TEXT(1, >, 2, "1 smaller than 2");
}

TEST(TestShellMacros, passingCheckEqualWillNotBeEvaluatedMultipleTimesWithCHECK_EQUAL)
{
  count_in_counting_method = 0;
  CHECK_EQUAL(0, counting_method());

  CHECK_EQUAL(1, count_in_counting_method);
}

TEST(TestShellMacros, failing_CHECK_EQUAL_withParamatersThatDontChangeWillNotGiveAnyWarning)
{
  fixture.run_test_with_method(failing_test_method_with_check_equal);
  fixture.assert_print_contains_not("WARNING");
}

TEST(TestShellMacros, CHECK_EQUALBehavesAsProperMacro)
{
  if (false)
    CHECK_EQUAL(1, 2);
  else
    CHECK_EQUAL(1, 1);
}

IGNORE_TEST(TestShellMacros, CHECK_EQUALWorksInAnIgnoredTest)
{
  CHECK_EQUAL(1, 2);
}

TEST(TestShellMacros, FailureWithCHECK_EQUAL_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_check_equal_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <2>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(TestShellMacros, CHECK_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    CHECK_EQUAL_TEXT(1, 2, "Failed because it failed");
  else
    CHECK_EQUAL_TEXT(1, 1, "Failed because it failed");
}

IGNORE_TEST(TestShellMacros, CHECK_EQUAL_TEXTWorksInAnIgnoredTest)
{
  CHECK_EQUAL_TEXT(1, 2, "Failed because it failed");
}

TEST(TestShellMacros, FailureWithCHECK_EQUAL_ZERO)
{
  fixture.run_test_with_method(failing_test_method_with_check_equal_zero);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <1>");
}

TEST(TestShellMacros, passingCheckEqualWillNotBeEvaluatedMultipleTimesWithCHECK_EQUAL_ZERO)
{
  count_in_counting_method = 0;
  CHECK_EQUAL_ZERO(counting_method());

  CHECK_EQUAL(1, count_in_counting_method);
}

TEST(TestShellMacros, failing_CHECK_EQUAL_ZERO_withParamatersThatDontChangeWillNotGiveAnyWarning)
{
  fixture.run_test_with_method(failing_test_method_with_check_equal_zero);
  fixture.assert_print_contains_not("WARNING");
}

IGNORE_TEST(TestShellMacros, CHECK_EQUAL_ZERO_WorksInAnIgnoredTest)
{
  CHECK_EQUAL_ZERO(1);
}

TEST(TestShellMacros, CHECK_EQUAL_ZERO_BehavesAsProperMacro)
{
  if (false)
    CHECK_EQUAL_ZERO(1);
  else
    CHECK_EQUAL_ZERO(0);
}

TEST(TestShellMacros, FailureWithCHECK_EQUAL_ZERO_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_check_equal_zero_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(TestShellMacros, CHECK_EQUAL_ZERO_TEXTBehavesAsProperMacro)
{
  if (false)
    CHECK_EQUAL_ZERO_TEXT(1, "Failed because it failed");
  else
    CHECK_EQUAL_ZERO_TEXT(0, "Failed because it failed");
}

IGNORE_TEST(TestShellMacros, CHECK_EQUAL_ZERO_TEXTWorksInAnIgnoredTest)
{
  CHECK_EQUAL_ZERO_TEXT(1, "Failed because it failed");
}

TEST(TestShellMacros, FailureWithBYTES_EQUAL)
{
  fixture.run_test_with_method(failing_test_method_with_bytes_equal);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <97>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <98>");
}

TEST(TestShellMacros, BYTES_EQUALBehavesAsProperMacro)
{
  if (false)
    BYTES_EQUAL('a', 'b');
  else
    BYTES_EQUAL('c', 'c');
}

IGNORE_TEST(TestShellMacros, BYTES_EQUALWorksInAnIgnoredTest)
{
  BYTES_EQUAL('q', 'w');
}

TEST(TestShellMacros, FailureWithBYTES_EQUAL_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_bytes_equal_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <97>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <98>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(TestShellMacros, BYTES_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    BYTES_EQUAL_TEXT('a', 'b', "Failed because it failed");
  else
    BYTES_EQUAL_TEXT('c', 'c', "Failed because it failed");
}

IGNORE_TEST(TestShellMacros, BYTES_EQUAL_TEXTWorksInAnIgnoredTest)
{
  BYTES_EQUAL_TEXT('q', 'w', "Failed because it failed");
}

TEST(TestShellMacros, FailureWithSIGNED_BYTES_EQUAL)
{
  fixture.run_test_with_method(failing_test_method_with_signed_bytes_equal);
#if CHAR_BIT == 16
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <-1 (0xffff)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <-2 (0xfffe)>");
#elif CHAR_BIT == 8
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <-1 (0xff)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <-2 (0xfe)>");
#endif
}

TEST(TestShellMacros, CHARS_EQUALBehavesAsProperMacro)
{
  if (false)
    SIGNED_BYTES_EQUAL(-1, -2);
  else
    SIGNED_BYTES_EQUAL(-3, -3);
}

IGNORE_TEST(TestShellMacros, CHARS_EQUALWorksInAnIgnoredTest)
{
  SIGNED_BYTES_EQUAL(-7, 19);
}

TEST(TestShellMacros, FailureWithSIGNED_BYTES_EQUAL_TEXT)
{
  fixture.run_test_with_method(
      failing_test_method_with_signed_bytes_equal_text
  );
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <-127 (0x81)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <-126 (0x82)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(TestShellMacros, CHARS_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    SIGNED_BYTES_EQUAL_TEXT(-1, -2, "Failed because it failed");
  else
    SIGNED_BYTES_EQUAL_TEXT(-3, -3, "Failed because it failed");
}

IGNORE_TEST(TestShellMacros, SIGNED_BYTES_EQUAL_TEXTWorksInAnIgnoredTest)
{
  SIGNED_BYTES_EQUAL_TEXT(-7, 19, "Failed because it failed");
}

TEST(TestShellMacros, FailureWithPOINTERS_EQUAL)
{
  fixture.run_test_with_method(failing_test_method_with_pointers_equal);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0xa5a5>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0xf0f0>");
}

TEST(TestShellMacros, POINTERS_EQUALBehavesAsProperMacro)
{
  if (false)
    POINTERS_EQUAL(nullptr, nullptr);
  else
    POINTERS_EQUAL(
        reinterpret_cast<void*>(0xdeadbeefULL),
        reinterpret_cast<void*>(0xdeadbeefULL)
    );
}

IGNORE_TEST(TestShellMacros, POINTERS_EQUALWorksInAnIgnoredTest)
{
  POINTERS_EQUAL(
      reinterpret_cast<void*>(0xbeef), reinterpret_cast<void*>(0xdead)
  );
}

TEST(TestShellMacros, FailureWithPOINTERS_EQUAL_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_pointers_equal_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0xa5a5>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0xf0f0>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(TestShellMacros, POINTERS_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    POINTERS_EQUAL_TEXT(nullptr, nullptr, "Failed because it failed");
  else
    POINTERS_EQUAL_TEXT(
        reinterpret_cast<void*>(0xdeadbeefULL),
        reinterpret_cast<void*>(0xdeadbeefULL),
        "Failed because it failed"
    );
}

IGNORE_TEST(TestShellMacros, POINTERS_EQUAL_TEXTWorksInAnIgnoredTest)
{
  POINTERS_EQUAL_TEXT(
      reinterpret_cast<void*>(0xbeef),
      reinterpret_cast<void*>(0xdead),
      "Failed because it failed"
  );
}

TEST(TestShellMacros, FailureWithFUNCTIONPOINTERS_EQUAL)
{
  fixture.run_test_with_method(failing_test_method_with_functionpointers_equal);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0xa5a5>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0xf0f0>");
}

TEST(TestShellMacros, FUNCTIONPOINTERS_EQUALBehavesAsProperMacro)
{
  if (false)
    FUNCTIONPOINTERS_EQUAL(
        static_cast<void (*)()>(nullptr), static_cast<void (*)()>(nullptr)
    );
  else
    FUNCTIONPOINTERS_EQUAL(
        reinterpret_cast<void (*)()>(0xdeadbeefULL),
        reinterpret_cast<void (*)()>(0xdeadbeefULL)
    );
}

IGNORE_TEST(TestShellMacros, FUNCTIONPOINTERS_EQUALWorksInAnIgnoredTest)
{
  FUNCTIONPOINTERS_EQUAL(
      reinterpret_cast<void (*)()>(0xbeef), reinterpret_cast<void (*)()>(0xdead)
  );
}

TEST(TestShellMacros, FailureWithFUNCTIONPOINTERS_EQUAL_TEXT)
{
  fixture.run_test_with_method(
      failing_test_method_with_functionpointers_equal_text
  );
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0xa5a5>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0xf0f0>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(TestShellMacros, FUNCTIONPOINTERS_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    FUNCTIONPOINTERS_EQUAL_TEXT(
        static_cast<void (*)()>(nullptr),
        static_cast<void (*)()>(nullptr),
        "Failed because it failed"
    );
  else
    FUNCTIONPOINTERS_EQUAL_TEXT(
        reinterpret_cast<void (*)()>(0xdeadbeefULL),
        reinterpret_cast<void (*)()>(0xdeadbeefULL),
        "Failed because it failed"
    );
}

IGNORE_TEST(TestShellMacros, FUNCTIONPOINTERS_EQUAL_TEXTWorksInAnIgnoredTest)
{
  FUNCTIONPOINTERS_EQUAL_TEXT(
      reinterpret_cast<void (*)()>(0xbeef),
      reinterpret_cast<void (*)()>(0xdead),
      "Failed because it failed"
  );
}

TEST(TestShellMacros, FailureWithDOUBLES_EQUAL)
{
  fixture.run_test_with_method(failing_test_method_with_doubles_equal);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0.12>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <44.1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("threshold used was <0.3>");
}

TEST(TestShellMacros, DOUBLES_EQUALBehavesAsProperMacro)
{
  if (false)
    DOUBLES_EQUAL(0.0, 1.1, 0.0005);
  else
    DOUBLES_EQUAL(0.1, 0.2, 0.2);
}

IGNORE_TEST(TestShellMacros, DOUBLES_EQUALWorksInAnIgnoredTest)
{
  DOUBLES_EQUAL(100.0, 0.0, 0.2);
}

TEST(TestShellMacros, FailureWithDOUBLES_EQUAL_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_doubles_equal_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0.12>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <44.1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("threshold used was <0.3>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(TestShellMacros, DOUBLES_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    DOUBLES_EQUAL_TEXT(0.0, 1.1, 0.0005, "Failed because it failed");
  else
    DOUBLES_EQUAL_TEXT(0.1, 0.2, 0.2, "Failed because it failed");
}

IGNORE_TEST(TestShellMacros, DOUBLES_EQUAL_TEXTWorksInAnIgnoredTest)
{
  DOUBLES_EQUAL_TEXT(100.0, 0.0, 0.2, "Failed because it failed");
}

TEST(TestShellMacros, SuccessPrintsNothing)
{
  fixture.run_test_with_method(passing_test_method);

  CHECK_EQUAL(size_t{ 0 }, fixture.get_failure_count());
  fixture.assert_print_contains(".\nOK (1 tests");
  CHECK(line_of_code_executed_after_check);
}

TEST(TestShellMacros, allMacrosFromFunctionThatReturnsAValue)
{
  function_that_returns_a_value();
}

TEST(TestShellMacros, MEMCMP_EQUALBehavesAsAProperMacro)
{
  if (false)
    MEMCMP_EQUAL("TEST", "test", 5);
  else
    MEMCMP_EQUAL("TEST", "TEST", 5);
}

IGNORE_TEST(TestShellMacros, MEMCMP_EQUALWorksInAnIgnoredTest)
{
  MEMCMP_EQUAL("TEST", "test", 5);
}

TEST(TestShellMacros, MEMCMP_EQUALFailureWithUnequalInput)
{
  fixture.run_test_with_method(
      memcmp_equal_failing_test_method_with_unequal_input
  );
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <00 01 02 03>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <00 01 03 03>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 2");
}

TEST(TestShellMacros, MEMCMP_EQUALFailureWithNullExpected)
{
  fixture.run_test_with_method(
      memcmp_equal_failing_test_method_with_null_expected
  );
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <(null)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <00 01 02 03>");
}

TEST(TestShellMacros, MEMCMP_EQUALFailureWithNullActual)
{
  fixture.run_test_with_method(
      memcmp_equal_failing_test_method_with_null_actual
  );
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <00 01 02 03>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <(null)>");
}

TEST(TestShellMacros, MEMCMP_EQUALNullExpectedNullActual)
{
  MEMCMP_EQUAL(nullptr, nullptr, 0);
  MEMCMP_EQUAL(nullptr, nullptr, 1024);
}

TEST(TestShellMacros, MEMCMP_EQUALNullPointerIgnoredInExpectationWhenSize0)
{
  unsigned char actual_data[] = { 0x00, 0x01, 0x03, 0x03 };
  MEMCMP_EQUAL(nullptr, actual_data, 0);
}

TEST(TestShellMacros, MEMCMP_EQUALNullPointerIgnoredInActualWhenSize0)
{
  unsigned char expected_data[] = { 0x00, 0x01, 0x02, 0x03 };
  MEMCMP_EQUAL(expected_data, nullptr, 0);
}

TEST(TestShellMacros, FailureWithMEMCMP_EQUAL_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_memcmp_equal_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <00 01 02 03>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <00 01 03 03>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 2");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(TestShellMacros, MEMCMP_EQUAL_TEXTBehavesAsAProperMacro)
{
  if (false)
    MEMCMP_EQUAL_TEXT("TEST", "test", 5, "Failed because it failed");
  else
    MEMCMP_EQUAL_TEXT("TEST", "TEST", 5, "Failed because it failed");
}

IGNORE_TEST(TestShellMacros, MEMCMP_EQUAL_TEXTWorksInAnIgnoredTest)
{
  MEMCMP_EQUAL_TEXT("TEST", "test", 5, "Failed because it failed");
}

TEST(TestShellMacros, TestENUMS_EQUAL_INTWithScopedIntEnum)
{
  fixture.run_test_with_method(
      enums_equal_int_with_scoped_int_enum_test_method
  );
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0>");
}

TEST(TestShellMacros, ENUMS_EQUAL_INTWithScopedIntEnumBehavesAsProperMacro)
{
  if (false)
    ENUMS_EQUAL_INT(ScopedIntEnum::b, ScopedIntEnum::a);
  else
    ENUMS_EQUAL_INT(ScopedIntEnum::b, ScopedIntEnum::b);
}

IGNORE_TEST(
    TestShellMacros,
    ENUMS_EQUAL_INTWithScopedIntEnumWorksInAnIgnoredTest
)
{
  ENUMS_EQUAL_INT(ScopedIntEnum::b, ScopedIntEnum::a);
}

TEST(TestShellMacros, TestENUMS_EQUAL_INT_TEXTWithScopedIntEnum)
{
  fixture.run_test_with_method(
      enums_equal_int_text_with_scoped_int_enum_test_method
  );
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(TestShellMacros, ENUMS_EQUAL_INT_TEXTWithScopedIntEnumBehavesAsProperMacro)
{
  if (false)
    ENUMS_EQUAL_INT_TEXT(
        ScopedIntEnum::b, ScopedIntEnum::a, "Failed because it failed"
    );
  else
    ENUMS_EQUAL_INT_TEXT(
        ScopedIntEnum::b, ScopedIntEnum::b, "Failed because it failed"
    );
}

IGNORE_TEST(
    TestShellMacros,
    ENUMS_EQUAL_EQUAL_INT_TEXTWithScopedIntEnumWorksInAnIgnoredTest
)
{
  ENUMS_EQUAL_INT_TEXT(
      ScopedIntEnum::b, ScopedIntEnum::a, "Failed because it failed"
  );
}

TEST(TestShellMacros, TestENUMS_EQUAL_TYPEWithScopedLongEnum)
{
  fixture.run_test_with_method(
      enums_equal_type_with_scoped_long_enum_test_method
  );
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0>");
}

TEST(TestShellMacros, ENUMS_EQUAL_TYPEWithScopedLongEnumBehavesAsProperMacro)
{
  if (false)
    ENUMS_EQUAL_TYPE(long, ScopedLongEnum::b, ScopedLongEnum::a);
  else
    ENUMS_EQUAL_TYPE(long, ScopedLongEnum::b, ScopedLongEnum::b);
}

IGNORE_TEST(
    TestShellMacros,
    ENUMS_EQUAL_TYPEWithScopedLongEnumWorksInAnIgnoredTest
)
{
  ENUMS_EQUAL_TYPE(long, ScopedLongEnum::b, ScopedLongEnum::a);
}

TEST(TestShellMacros, TestENUMS_EQUAL_TYPE_TEXTWithScopedLongEnum)
{
  fixture.run_test_with_method(
      enums_equal_type_text_with_scoped_long_enum_test_method
  );
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(TestShellMacros, ENUMS_EQUAL_TYPE_TEXTWithScopedLongEnumBehavesAsProperMacro)
{
  if (false)
    ENUMS_EQUAL_TYPE_TEXT(
        long, ScopedLongEnum::b, ScopedLongEnum::a, "Failed because it failed"
    );
  else
    ENUMS_EQUAL_TYPE_TEXT(
        long, ScopedLongEnum::b, ScopedLongEnum::b, "Failed because it failed"
    );
}

IGNORE_TEST(
    TestShellMacros,
    ENUMS_EQUAL_EQUAL_TYPE_TEXTWithScopedLongEnumWorksInAnIgnoredTest
)
{
  ENUMS_EQUAL_TYPE_TEXT(
      long, ScopedLongEnum::b, ScopedLongEnum::a, "Failed because it failed"
  );
}

TEST(TestShellMacros, TestENUMS_EQUAL_INTWithUnscopedEnum)
{
  fixture.run_test_with_method(enums_equal_int_with_unscoped_enum_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0>");
}

TEST(TestShellMacros, ENUMS_EQUAL_INTWithUnscopedEnumBehavesAsProperMacro)
{
  if (false)
    ENUMS_EQUAL_INT(unscoped_enum_b, unscoped_enum_a);
  else
    ENUMS_EQUAL_INT(unscoped_enum_b, unscoped_enum_b);
}

IGNORE_TEST(
    TestShellMacros,
    ENUMS_EQUAL_INTWithUnscopedEnumWorksInAnIgnoredTest
)
{
  ENUMS_EQUAL_INT(unscoped_enum_b, unscoped_enum_a);
}

TEST(TestShellMacros, TestENUMS_EQUAL_INT_TEXTWithUnscopedEnum)
{
  fixture.run_test_with_method(
      enums_equal_int_text_with_unscoped_enum_test_method
  );
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(TestShellMacros, ENUMS_EQUAL_INT_TEXTWithUnscopedEnumBehavesAsProperMacro)
{
  if (false)
    ENUMS_EQUAL_INT_TEXT(
        unscoped_enum_b, unscoped_enum_a, "Failed because it failed"
    );
  else
    ENUMS_EQUAL_INT_TEXT(
        unscoped_enum_b, unscoped_enum_b, "Failed because it failed"
    );
}

IGNORE_TEST(
    TestShellMacros,
    ENUMS_EQUAL_EQUAL_INT_TEXTWithUnscopedEnumWorksInAnIgnoredTest
)
{
  ENUMS_EQUAL_INT_TEXT(
      unscoped_enum_b, unscoped_enum_a, "Failed because it failed"
  );
}

#if MUTINY_HAVE_EXCEPTIONS
TEST(
TestShellMacros, FailureWithCHECK_THROWS_whenDoesntThrow)
{
  fixture.run_test_with_method(failing_test_method_no_throw_with_check_throws);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected to throw int");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but threw nothing");
  CHECK_EQUAL(size_t{ 1 }, fixture.get_check_count());
}

TEST( TestShellMacros, SuccessWithCHECK_THROWS)
{
  fixture.run_test_with_method(
      succeeding_test_method_correct_throw_with_check_throws
  );
  CHECK_EQUAL(size_t{ 1 }, fixture.get_check_count());
}

TEST( TestShellMacros, FailureWithCHECK_THROWS_whenWrongThrow)
{
  fixture.run_test_with_method(
      failing_test_method_wrong_throw_with_check_throws
  );
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected to throw int");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but threw a different type");
  CHECK_EQUAL(size_t{ 1 }, fixture.get_check_count());
}

TEST( TestShellMacros, MultipleCHECK_THROWS_inOneScope)
{
  CHECK_THROWS(int, throw 4);
  CHECK_THROWS(int, throw 4);
}
#endif
