#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestOutput.hpp"
#include "CppUTest/TestTestingFixture.hpp"

#define CHECK_TEST_FAILS_PROPER_WITH_TEXT(text)                                \
  fixture.checkTestFailsWithProperTestLocation(text, __FILE__, __LINE__)

TEST_GROUP(UtestStringMacros)
{
  cpputest::TestTestingFixture fixture;
};

namespace {

void
STRCMP_EQUALWithActualIsNULLTestMethod_()
{
  STRCMP_EQUAL("ok", nullptr);
}

void
STRCMP_EQUALWithExpectedIsNULLTestMethod_()
{
  STRCMP_EQUAL(nullptr, "ok");
}

void
STRCMP_CONTAINSWithActualIsNULLTestMethod_()
{
  STRCMP_CONTAINS("ok", nullptr);
}

void
STRCMP_CONTAINSWithExpectedIsNULLTestMethod_()
{
  STRCMP_CONTAINS(nullptr, "ok");
}

void
STRNCMP_EQUALWithActualIsNULLTestMethod_()
{
  STRNCMP_EQUAL("ok", nullptr, 2);
}

void
STRNCMP_EQUALWithExpectedIsNULLTestMethod_()
{
  STRNCMP_EQUAL(nullptr, "ok", 2);
}

void
failingTestMethodWithSTRCMP_EQUAL_()
{
  STRCMP_EQUAL("hello", "hell");
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

void
failingTestMethodWithSTRCMP_EQUAL_TEXT_()
{
  STRCMP_EQUAL_TEXT("hello", "hell", "Failed because it failed");
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

void
failingTestMethodWithSTRNCMP_EQUAL_()
{
  STRNCMP_EQUAL("hello", "hallo", 5);
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

void
failingTestMethodWithSTRNCMP_EQUAL_TEXT_()
{
  STRNCMP_EQUAL_TEXT("hello", "hallo", 5, "Failed because it failed");
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

void
failingTestMethodWithSTRCMP_CONTAINS_()
{
  STRCMP_CONTAINS("hello", "world");
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

void
failingTestMethodWithSTRCMP_CONTAINS_TEXT_()
{
  STRCMP_CONTAINS_TEXT("hello", "world", "Failed because it failed");
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

void
compareNFirstCharsWithUpperAndLowercase_()
{
  STRNCMP_EQUAL("hello world!", "HELLO WORLD!", 12);
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

void
compareNFirstCharsWithDifferenceInTheMiddle_()
{
  STRNCMP_EQUAL("Hello World!", "Hello Peter!", 12);
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

void
compareNFirstCharsWithEmptyString_()
{
  STRNCMP_EQUAL("", "Not empty string", 5);
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

void
compareNFirstCharsWithLastCharDifferent_()
{
  STRNCMP_EQUAL("Not empty string?", "Not empty string!", 17);
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

} // namespace

TEST(UtestStringMacros, FailureWithSTRCMP_EQUALAndActualIsNULL)
{
  fixture.runTestWithMethod(STRCMP_EQUALWithActualIsNULLTestMethod_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <(null)>");
}

TEST(UtestStringMacros, FailureWithSTRCMP_EQUALAndExpectedIsNULL)
{
  fixture.runTestWithMethod(STRCMP_EQUALWithExpectedIsNULLTestMethod_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <(null)>");
}

TEST(UtestStringMacros, FailureWithSTRCMP_CONTAINSAndActualIsNULL)
{
  fixture.runTestWithMethod(STRCMP_CONTAINSWithActualIsNULLTestMethod_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <ok>");
}

TEST(UtestStringMacros, FailureWithSTRCMP_CONTAINSAndExpectedIsNULL)
{
  fixture.runTestWithMethod(STRCMP_CONTAINSWithExpectedIsNULLTestMethod_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <>");
}

TEST(UtestStringMacros, FailureWithSTRNCMP_EQUALAndActualIsNULL)
{
  fixture.runTestWithMethod(STRNCMP_EQUALWithActualIsNULLTestMethod_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <(null)>");
}

TEST(UtestStringMacros, FailureWithSTRNCMP_EQUALAndExpectedIsNULL)
{
  fixture.runTestWithMethod(STRNCMP_EQUALWithExpectedIsNULLTestMethod_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <(null)>");
}

TEST(UtestStringMacros, FailureWithSTRCMP_EQUAL)
{
  fixture.runTestWithMethod(failingTestMethodWithSTRCMP_EQUAL_);
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
  fixture.runTestWithMethod(failingTestMethodWithSTRCMP_EQUAL_TEXT_);
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
  fixture.runTestWithMethod(failingTestMethodWithSTRNCMP_EQUAL_);
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
  fixture.runTestWithMethod(failingTestMethodWithSTRNCMP_EQUAL_TEXT_);
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
  fixture.runTestWithMethod(failingTestMethodWithSTRCMP_CONTAINS_);
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
  fixture.runTestWithMethod(failingTestMethodWithSTRCMP_CONTAINS_TEXT_);
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
  fixture.runTestWithMethod(compareNFirstCharsWithUpperAndLowercase_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello world!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <HELLO WORLD!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 0");
}

TEST(UtestStringMacros, CompareNFirstCharsWithDifferenceInTheMiddle)
{
  fixture.runTestWithMethod(compareNFirstCharsWithDifferenceInTheMiddle_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <Hello World!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Hello Peter!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 6");
}

TEST(UtestStringMacros, CompareNFirstCharsWithEmptyString)
{
  fixture.runTestWithMethod(compareNFirstCharsWithEmptyString_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Not empty string>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 0");
}

TEST(UtestStringMacros, CompareNFirstCharsWithLastCharDifferent)
{
  fixture.runTestWithMethod(compareNFirstCharsWithLastCharDifferent_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <Not empty string?>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Not empty string!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 16");
}
