#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestOutput.hpp"
#include "CppUTest/TestTestingFixture.hpp"

#define CHECK_TEST_FAILS_PROPER_WITH_TEXT(text)                                \
  fixture.check_test_fails_with_proper_test_location(text, __FILE__, __LINE__)

TEST_GROUP(UtestStringMacros)
{
  cpputest::TestTestingFixture fixture;
};

namespace {

void
strcmp_equal_with_actual_is_null_test_method()
{
  STRCMP_EQUAL("ok", nullptr);
}

void
strcmp_equal_with_expected_is_null_test_method()
{
  STRCMP_EQUAL(nullptr, "ok");
}

void
strcmp_contains_with_actual_is_null_test_method()
{
  STRCMP_CONTAINS("ok", nullptr);
}

void
strcmp_contains_with_expected_is_null_test_method()
{
  STRCMP_CONTAINS(nullptr, "ok");
}

void
strncmp_equal_with_actual_is_null_test_method()
{
  STRNCMP_EQUAL("ok", nullptr, 2);
}

void
strncmp_equal_with_expected_is_null_test_method()
{
  STRNCMP_EQUAL(nullptr, "ok", 2);
}

void
failing_test_method_with_strcmp_equal()
{
  STRCMP_EQUAL("hello", "hell");
  cpputest::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_strcmp_equal_text()
{
  STRCMP_EQUAL_TEXT("hello", "hell", "Failed because it failed");
  cpputest::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_strncmp_equal()
{
  STRNCMP_EQUAL("hello", "hallo", 5);
  cpputest::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_strncmp_equal_text()
{
  STRNCMP_EQUAL_TEXT("hello", "hallo", 5, "Failed because it failed");
  cpputest::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_strcmp_contains()
{
  STRCMP_CONTAINS("hello", "world");
  cpputest::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_strcmp_contains_text()
{
  STRCMP_CONTAINS_TEXT("hello", "world", "Failed because it failed");
  cpputest::TestTestingFixture::line_executed_after_check();
}

void
compare_n_first_chars_with_upper_and_lowercase()
{
  STRNCMP_EQUAL("hello world!", "HELLO WORLD!", 12);
  cpputest::TestTestingFixture::line_executed_after_check();
}

void
compare_n_first_chars_with_difference_in_the_middle()
{
  STRNCMP_EQUAL("Hello World!", "Hello Peter!", 12);
  cpputest::TestTestingFixture::line_executed_after_check();
}

void
compare_n_first_chars_with_empty_string()
{
  STRNCMP_EQUAL("", "Not empty string", 5);
  cpputest::TestTestingFixture::line_executed_after_check();
}

void
compare_n_first_chars_with_last_char_different()
{
  STRNCMP_EQUAL("Not empty string?", "Not empty string!", 17);
  cpputest::TestTestingFixture::line_executed_after_check();
}

} // namespace

TEST(UtestStringMacros, FailureWithSTRCMP_EQUALAndActualIsNULL)
{
  fixture.run_test_with_method(strcmp_equal_with_actual_is_null_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <(null)>");
}

TEST(UtestStringMacros, FailureWithSTRCMP_EQUALAndExpectedIsNULL)
{
  fixture.run_test_with_method(strcmp_equal_with_expected_is_null_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <(null)>");
}

TEST(UtestStringMacros, FailureWithSTRCMP_CONTAINSAndActualIsNULL)
{
  fixture.run_test_with_method(strcmp_contains_with_actual_is_null_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <ok>");
}

TEST(UtestStringMacros, FailureWithSTRCMP_CONTAINSAndExpectedIsNULL)
{
  fixture.run_test_with_method(
      strcmp_contains_with_expected_is_null_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <>");
}

TEST(UtestStringMacros, FailureWithSTRNCMP_EQUALAndActualIsNULL)
{
  fixture.run_test_with_method(strncmp_equal_with_actual_is_null_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <(null)>");
}

TEST(UtestStringMacros, FailureWithSTRNCMP_EQUALAndExpectedIsNULL)
{
  fixture.run_test_with_method(strncmp_equal_with_expected_is_null_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <(null)>");
}

TEST(UtestStringMacros, FailureWithSTRCMP_EQUAL)
{
  fixture.run_test_with_method(failing_test_method_with_strcmp_equal);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <hell>");
}

TEST(UtestStringMacros, STRCMP_EQUALBehavesAsProperMacro)
{
  if (false)
    STRCMP_EQUAL("1", "2");
  else
    STRCMP_EQUAL("1", "1");
}

IGNORE_TEST(UtestStringMacros, STRCMP_EQUALWorksInAnIgnoredTest)
{
  STRCMP_EQUAL("Hello", "World");
}

TEST(UtestStringMacros, FailureWithSTRCMP_EQUAL_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_strcmp_equal_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <hell>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestStringMacros, STRCMP_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    STRCMP_EQUAL_TEXT("1", "2", "Failed because it failed");
  else
    STRCMP_EQUAL_TEXT("1", "1", "Failed because it failed");
}

IGNORE_TEST(UtestStringMacros, STRCMP_EQUAL_TEXTWorksInAnIgnoredTest)
{
  STRCMP_EQUAL_TEXT("Hello", "World", "Failed because it failed");
}

TEST(UtestStringMacros, FailureWithSTRNCMP_EQUAL)
{
  fixture.run_test_with_method(failing_test_method_with_strncmp_equal);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <hallo>");
}

TEST(UtestStringMacros, STRNCMP_EQUALBehavesAsProperMacro)
{
  if (false)
    STRNCMP_EQUAL("1", "2", 1);
  else
    STRNCMP_EQUAL("1", "1", 1);
}

IGNORE_TEST(UtestStringMacros, STRNCMP_EQUALWorksInAnIgnoredTest)
{
  STRNCMP_EQUAL("Hello", "World", 3);
}

TEST(UtestStringMacros, FailureWithSTRNCMP_EQUAL_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_strncmp_equal_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <hallo>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestStringMacros, STRNCMP_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    STRNCMP_EQUAL_TEXT("1", "2", 1, "Failed because it failed");
  else
    STRNCMP_EQUAL_TEXT("1", "1", 1, "Failed because it failed");
}

IGNORE_TEST(UtestStringMacros, STRNCMP_EQUAL_TEXTWorksInAnIgnoredTest)
{
  STRNCMP_EQUAL_TEXT("Hello", "World", 3, "Failed because it failed");
}

TEST(UtestStringMacros, FailureWithSTRCMP_CONTAINS)
{
  fixture.run_test_with_method(failing_test_method_with_strcmp_contains);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("actual <world>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <hello>");
}

TEST(UtestStringMacros, STRCMP_CONTAINSBehavesAsProperMacro)
{
  if (false)
    STRCMP_CONTAINS("1", "2");
  else
    STRCMP_CONTAINS("1", "1");
}

IGNORE_TEST(UtestStringMacros, STRCMP_CONTAINSWorksInAnIgnoredTest)
{
  STRCMP_CONTAINS("Hello", "World");
}

TEST(UtestStringMacros, FailureWithSTRCMP_CONTAINS_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_strcmp_contains_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("actual <world>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestStringMacros, STRCMP_CONTAINS_TEXTBehavesAsProperMacro)
{
  if (false)
    STRCMP_CONTAINS_TEXT("1", "2", "Failed because it failed");
  else
    STRCMP_CONTAINS_TEXT("1", "1", "Failed because it failed");
}

IGNORE_TEST(UtestStringMacros, STRCMP_CONTAINS_TEXTWorksInAnIgnoredTest)
{
  STRCMP_CONTAINS_TEXT("Hello", "World", "Failed because it failed");
}

TEST(UtestStringMacros, NFirstCharsComparison)
{
  STRNCMP_EQUAL("Hello World!", "Hello Peter!", 0);
  STRNCMP_EQUAL("Hello World!", "Hello Peter!", 1);
  STRNCMP_EQUAL("Hello World!", "Hello Peter!", 6);
  STRNCMP_EQUAL("Hello World!", "Hello", 5);
}

TEST(UtestStringMacros, CompareNFirstCharsWithUpperAndLowercase)
{
  fixture.run_test_with_method(compare_n_first_chars_with_upper_and_lowercase);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello world!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <HELLO WORLD!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 0");
}

TEST(UtestStringMacros, CompareNFirstCharsWithDifferenceInTheMiddle)
{
  fixture.run_test_with_method(
      compare_n_first_chars_with_difference_in_the_middle);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <Hello World!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Hello Peter!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 6");
}

TEST(UtestStringMacros, CompareNFirstCharsWithEmptyString)
{
  fixture.run_test_with_method(compare_n_first_chars_with_empty_string);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Not empty string>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 0");
}

TEST(UtestStringMacros, CompareNFirstCharsWithLastCharDifferent)
{
  fixture.run_test_with_method(compare_n_first_chars_with_last_char_different);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <Not empty string?>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Not empty string!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 16");
}
