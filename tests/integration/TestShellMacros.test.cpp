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
  CHECK_EQUAL('a', 'b');
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_bytes_equal_text()
{
  CHECK_EQUAL_TEXT('a', 'b', "Failed because it failed");
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_signed_bytes_equal()
{
  CHECK_EQUAL(-1, -2);
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_signed_bytes_equal_text()
{
  CHECK_EQUAL_TEXT(-127, -126, "Failed because it failed");
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

void failing_test_method_with_check_approx()
{
  CHECK_APPROX(0.12, 44.1, 0.3);
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_check_approx_text()
{
  CHECK_APPROX_TEXT(0.12, 44.1, 0.3, "Failed because it failed");
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
  CHECK_EQUAL(0xab, 0xab);
  CHECK_EQUAL_TEXT(0xab, 0xab, "Shouldn't fail");
  CHECK_EQUAL(100, 100);
  CHECK_EQUAL_TEXT(100, 100, "Shouldn't fail");
  CHECK_EQUAL_ZERO(0);
  CHECK_EQUAL_ZERO_TEXT(0, "Shouldn't fail");
  STRCMP_EQUAL("THIS", "THIS");
  STRCMP_EQUAL_TEXT("THIS", "THIS", "Shouldn't fail");
  CHECK_APPROX(1.0, 1.0, .01);
  CHECK_APPROX_TEXT(1.0, 1.0, .01, "Shouldn't fail");
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

TEST(TestShellMacros, FailureWithCHECK)
{
  fixture.run_test_with_method(failing_test_method_with_check);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK(false) failed");
}

TEST(TestShellMacros, FailureWithCHECK_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_check_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK(false) failed");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(TestShellMacros, FailureWithCHECK_TRUE)
{
  fixture.run_test_with_method(failing_test_method_with_check_true);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_TRUE(false) failed");
}

TEST(TestShellMacros, FailureWithCHECK_TRUE_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_check_true_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_TRUE(false) failed");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(TestShellMacros, FailureWithCHECK_FALSE)
{
  fixture.run_test_with_method(failing_test_method_with_check_false);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_FALSE(true) failed");
}

TEST(TestShellMacros, FailureWithCHECK_FALSE_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_check_false_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_FALSE(true)");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
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

TEST(TestShellMacros, FailureWithCHECK_COMPARE_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_check_compare_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_COMPARE(0.5 >= 0.8)");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("small bigger than big");
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

TEST(TestShellMacros, FailureWithCHECK_EQUAL_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_check_equal_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <2>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
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

TEST(TestShellMacros, FailureWithCHECK_EQUAL_ZERO_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_check_equal_zero_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(TestShellMacros, FailureWithBYTES_EQUAL)
{
  fixture.run_test_with_method(failing_test_method_with_bytes_equal);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <a>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <b>");
}

TEST(TestShellMacros, FailureWithBYTES_EQUAL_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_bytes_equal_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <a>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <b>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(TestShellMacros, FailureWithSIGNED_BYTES_EQUAL)
{
  fixture.run_test_with_method(failing_test_method_with_signed_bytes_equal);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <-1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <-2>");
}

TEST(TestShellMacros, FailureWithSIGNED_BYTES_EQUAL_TEXT)
{
  fixture.run_test_with_method(
      failing_test_method_with_signed_bytes_equal_text
  );
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <-127>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <-126>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(TestShellMacros, FailureWithPOINTERS_EQUAL)
{
  fixture.run_test_with_method(failing_test_method_with_pointers_equal);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0xa5a5>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0xf0f0>");
}

TEST(TestShellMacros, FailureWithPOINTERS_EQUAL_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_pointers_equal_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0xa5a5>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0xf0f0>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(TestShellMacros, FailureWithFUNCTIONPOINTERS_EQUAL)
{
  fixture.run_test_with_method(failing_test_method_with_functionpointers_equal);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0xa5a5>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0xf0f0>");
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

TEST(TestShellMacros, FailureWithCHECK_APPROX)
{
  fixture.run_test_with_method(failing_test_method_with_check_approx);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0.12>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <44.1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("threshold used was <0.3>");
}

TEST(TestShellMacros, FailureWithCHECK_APPROX_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_check_approx_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0.12>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <44.1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("threshold used was <0.3>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
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

TEST(TestShellMacros, TestENUMS_EQUAL_INTWithScopedIntEnum)
{
  fixture.run_test_with_method(
      enums_equal_int_with_scoped_int_enum_test_method
  );
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0>");
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

TEST(TestShellMacros, TestENUMS_EQUAL_TYPEWithScopedLongEnum)
{
  fixture.run_test_with_method(
      enums_equal_type_with_scoped_long_enum_test_method
  );
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0>");
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

TEST(TestShellMacros, TestENUMS_EQUAL_INTWithUnscopedEnum)
{
  fixture.run_test_with_method(enums_equal_int_with_unscoped_enum_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0>");
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

#if MUTINY_HAVE_EXCEPTIONS
TEST(TestShellMacros, FailureWithCHECK_THROWS_whenDoesntThrow)
{
  fixture.run_test_with_method(failing_test_method_no_throw_with_check_throws);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected to throw int");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but threw nothing");
  CHECK_EQUAL(size_t{ 1 }, fixture.get_check_count());
}

TEST(TestShellMacros, SuccessWithCHECK_THROWS)
{
  fixture.run_test_with_method(
      succeeding_test_method_correct_throw_with_check_throws
  );
  CHECK_EQUAL(size_t{ 1 }, fixture.get_check_count());
}

TEST(TestShellMacros, FailureWithCHECK_THROWS_whenWrongThrow)
{
  fixture.run_test_with_method(
      failing_test_method_wrong_throw_with_check_throws
  );
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected to throw int");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but threw a different type");
  CHECK_EQUAL(size_t{ 1 }, fixture.get_check_count());
}

TEST(TestShellMacros, MultipleCHECK_THROWS_inOneScope)
{
  CHECK_THROWS(int, throw 4);
  CHECK_THROWS(int, throw 4);
}
#endif
