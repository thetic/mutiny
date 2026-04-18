#include "mu/tiny/test/Shell.hpp"
#include "mu/tiny/test/TestingFixture.hpp"

#include "mu/tiny/test.hpp"

#define CHECK_TEST_FAILS_PROPER_WITH_TEXT(text)                                \
  fixture.check_test_fails_with_proper_test_location(text, __FILE__, __LINE__)

TEST_GROUP(TestShellStringMacros)
{
  mu::tiny::test::TestingFixture fixture;
};

namespace {

void strcmp_equal_with_actual_is_null_test_method()
{
  STRCMP_EQUAL("ok", nullptr);
}

void strcmp_equal_with_expected_is_null_test_method()
{
  STRCMP_EQUAL(nullptr, "ok");
}

void strcmp_contains_with_actual_is_null_test_method()
{
  STRCMP_CONTAINS("ok", nullptr);
}

void strcmp_contains_with_expected_is_null_test_method()
{
  STRCMP_CONTAINS(nullptr, "ok");
}

void strncmp_equal_with_actual_is_null_test_method()
{
  STRNCMP_EQUAL("ok", nullptr, 2);
}

void strncmp_equal_with_expected_is_null_test_method()
{
  STRNCMP_EQUAL(nullptr, "ok", 2);
}

void failing_test_method_with_strcmp_equal()
{
  STRCMP_EQUAL("hello", "hell");
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_strcmp_equal_text()
{
  STRCMP_EQUAL_TEXT("hello", "hell", "Failed because it failed");
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_strncmp_equal()
{
  STRNCMP_EQUAL("hello", "hallo", 5);
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_strncmp_equal_text()
{
  STRNCMP_EQUAL_TEXT("hello", "hallo", 5, "Failed because it failed");
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_strcmp_contains()
{
  STRCMP_CONTAINS("hello", "world");
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void failing_test_method_with_strcmp_contains_text()
{
  STRCMP_CONTAINS_TEXT("hello", "world", "Failed because it failed");
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void compare_n_first_chars_with_upper_and_lowercase()
{
  STRNCMP_EQUAL("hello world!", "HELLO WORLD!", 12);
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void compare_n_first_chars_with_difference_in_the_middle()
{
  STRNCMP_EQUAL("Hello World!", "Hello Peter!", 12);
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void compare_n_first_chars_with_empty_string()
{
  STRNCMP_EQUAL("", "Not empty string", 5);
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

void compare_n_first_chars_with_last_char_different()
{
  STRNCMP_EQUAL("Not empty string?", "Not empty string!", 17);
  mu::tiny::test::TestingFixture::line_executed_after_check();
}

} // namespace

TEST(TestShellStringMacros, FailureWithSTRCMP_EQUALAndActualIsNULL)
{
  fixture.run_test_with_method(strcmp_equal_with_actual_is_null_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <(null)>");
}

TEST(TestShellStringMacros, FailureWithSTRCMP_EQUALAndExpectedIsNULL)
{
  fixture.run_test_with_method(strcmp_equal_with_expected_is_null_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <(null)>");
}

TEST(TestShellStringMacros, FailureWithSTRCMP_CONTAINSAndActualIsNULL)
{
  fixture.run_test_with_method(strcmp_contains_with_actual_is_null_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <ok>");
}

TEST(TestShellStringMacros, FailureWithSTRCMP_CONTAINSAndExpectedIsNULL)
{
  fixture.run_test_with_method(
      strcmp_contains_with_expected_is_null_test_method
  );
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <(null)>");
}

TEST(TestShellStringMacros, FailureWithSTRNCMP_EQUALAndActualIsNULL)
{
  fixture.run_test_with_method(strncmp_equal_with_actual_is_null_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <(null)>");
}

TEST(TestShellStringMacros, FailureWithSTRNCMP_EQUALAndExpectedIsNULL)
{
  fixture.run_test_with_method(strncmp_equal_with_expected_is_null_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <(null)>");
}

TEST(TestShellStringMacros, FailureWithSTRCMP_EQUAL)
{
  fixture.run_test_with_method(failing_test_method_with_strcmp_equal);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <hell>");
}

TEST(TestShellStringMacros, STRCMP_EQUALBothNullPasses)
{
  STRCMP_EQUAL(nullptr, nullptr);
}

SKIPPED_TEST(TestShellStringMacros, STRCMP_EQUALWorksInAnIgnoredTest)
{
  STRCMP_EQUAL("Hello", "World");
}

TEST(TestShellStringMacros, FailureWithSTRCMP_EQUAL_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_strcmp_equal_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <hell>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

SKIPPED_TEST(TestShellStringMacros, STRCMP_EQUAL_TEXTWorksInAnIgnoredTest)
{
  STRCMP_EQUAL_TEXT("Hello", "World", "Failed because it failed");
}

TEST(TestShellStringMacros, FailureWithSTRNCMP_EQUAL)
{
  fixture.run_test_with_method(failing_test_method_with_strncmp_equal);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <hallo>");
}

TEST(TestShellStringMacros, STRNCMP_EQUALBothNullPasses)
{
  STRNCMP_EQUAL(nullptr, nullptr, 0);
}

SKIPPED_TEST(TestShellStringMacros, STRNCMP_EQUALWorksInAnIgnoredTest)
{
  STRNCMP_EQUAL("Hello", "World", 3);
}

TEST(TestShellStringMacros, FailureWithSTRNCMP_EQUAL_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_strncmp_equal_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <hallo>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

SKIPPED_TEST(TestShellStringMacros, STRNCMP_EQUAL_TEXTWorksInAnIgnoredTest)
{
  STRNCMP_EQUAL_TEXT("Hello", "World", 3, "Failed because it failed");
}

TEST(TestShellStringMacros, FailureWithSTRCMP_CONTAINS)
{
  fixture.run_test_with_method(failing_test_method_with_strcmp_contains);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("actual <world>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <hello>");
}

TEST(TestShellStringMacros, STRCMP_CONTAINSBothNullPasses)
{
  STRCMP_CONTAINS(nullptr, nullptr);
}

SKIPPED_TEST(TestShellStringMacros, STRCMP_CONTAINSWorksInAnIgnoredTest)
{
  STRCMP_CONTAINS("Hello", "World");
}

TEST(TestShellStringMacros, FailureWithSTRCMP_CONTAINS_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_strcmp_contains_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("actual <world>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

SKIPPED_TEST(TestShellStringMacros, STRCMP_CONTAINS_TEXTWorksInAnIgnoredTest)
{
  STRCMP_CONTAINS_TEXT("Hello", "World", "Failed because it failed");
}

TEST(TestShellStringMacros, NFirstCharsComparison)
{
  STRNCMP_EQUAL("Hello World!", "Hello Peter!", 0);
  STRNCMP_EQUAL("Hello World!", "Hello Peter!", 1);
  STRNCMP_EQUAL("Hello World!", "Hello Peter!", 6);
  STRNCMP_EQUAL("Hello World!", "Hello", 5);
}

TEST(TestShellStringMacros, CompareNFirstCharsWithUpperAndLowercase)
{
  fixture.run_test_with_method(compare_n_first_chars_with_upper_and_lowercase);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello world!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <HELLO WORLD!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 0");
}

TEST(TestShellStringMacros, CompareNFirstCharsWithDifferenceInTheMiddle)
{
  fixture.run_test_with_method(
      compare_n_first_chars_with_difference_in_the_middle
  );
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <Hello World!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Hello Peter!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 6");
}

TEST(TestShellStringMacros, CompareNFirstCharsWithEmptyString)
{
  fixture.run_test_with_method(compare_n_first_chars_with_empty_string);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Not empty string>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 0");
}

TEST(TestShellStringMacros, CompareNFirstCharsWithLastCharDifferent)
{
  fixture.run_test_with_method(compare_n_first_chars_with_last_char_different);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <Not empty string?>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Not empty string!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 16");
}
