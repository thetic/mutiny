#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestOutput.hpp"
#include "CppUTest/TestTestingFixture.hpp"

#define CHECK_TEST_FAILS_PROPER_WITH_TEXT(text)                                \
  fixture.checkTestFailsWithProperTestLocation(text, __FILE__, __LINE__)

TEST_GROUP(UnitTestStringMacros)
{
  cpputest::TestTestingFixture fixture;
};

static void
STRCMP_EQUALWithActualIsNULLTestMethod_()
{
  STRCMP_EQUAL("ok", nullptr);
}

TEST(UnitTestStringMacros, FailureWithSTRCMP_EQUALAndActualIsNULL)
{
  fixture.runTestWithMethod(STRCMP_EQUALWithActualIsNULLTestMethod_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <(null)>");
}

static void
STRCMP_EQUALWithExpectedIsNULLTestMethod_()
{
  STRCMP_EQUAL(nullptr, "ok");
}

TEST(UnitTestStringMacros, FailureWithSTRCMP_EQUALAndExpectedIsNULL)
{
  fixture.runTestWithMethod(STRCMP_EQUALWithExpectedIsNULLTestMethod_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <(null)>");
}

static void
STRCMP_CONTAINSWithActualIsNULLTestMethod_()
{
  STRCMP_CONTAINS("ok", nullptr);
}

TEST(UnitTestStringMacros, FailureWithSTRCMP_CONTAINSAndActualIsNULL)
{
  fixture.runTestWithMethod(STRCMP_CONTAINSWithActualIsNULLTestMethod_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <ok>");
}

static void
STRCMP_CONTAINSWithExpectedIsNULLTestMethod_()
{
  STRCMP_CONTAINS(nullptr, "ok");
}

TEST(UnitTestStringMacros, FailureWithSTRCMP_CONTAINSAndExpectedIsNULL)
{
  fixture.runTestWithMethod(STRCMP_CONTAINSWithExpectedIsNULLTestMethod_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <>");
}

static void
STRNCMP_EQUALWithActualIsNULLTestMethod_()
{
  STRNCMP_EQUAL("ok", nullptr, 2);
}

TEST(UnitTestStringMacros, FailureWithSTRNCMP_EQUALAndActualIsNULL)
{
  fixture.runTestWithMethod(STRNCMP_EQUALWithActualIsNULLTestMethod_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <(null)>");
}

static void
STRNCMP_EQUALWithExpectedIsNULLTestMethod_()
{
  STRNCMP_EQUAL(nullptr, "ok", 2);
}

TEST(UnitTestStringMacros, FailureWithSTRNCMP_EQUALAndExpectedIsNULL)
{
  fixture.runTestWithMethod(STRNCMP_EQUALWithExpectedIsNULLTestMethod_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <(null)>");
}

static void
STRCMP_NOCASE_EQUALWithActualIsNULLTestMethod_()
{
  STRCMP_NOCASE_EQUAL("ok", nullptr);
}

TEST(UnitTestStringMacros, FailureWithSTRCMP_NOCASE_EQUALAndActualIsNULL)
{
  fixture.runTestWithMethod(STRCMP_NOCASE_EQUALWithActualIsNULLTestMethod_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <(null)>");
}

static void
STRCMP_NOCASE_EQUALWithExpectedIsNULLTestMethod_()
{
  STRCMP_NOCASE_EQUAL(nullptr, "ok");
}

TEST(UnitTestStringMacros, FailureWithSTRCMP_NOCASE_EQUALAndExpectedIsNULL)
{
  fixture.runTestWithMethod(STRCMP_NOCASE_EQUALWithExpectedIsNULLTestMethod_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <(null)>");
}

static void
STRCMP_NOCASE_EQUALWithUnequalInputTestMethod_()
{
  STRCMP_NOCASE_EQUAL("no", "ok");
}

TEST(UnitTestStringMacros, FailureWithSTRCMP_NOCASE_EQUALAndUnequalInput)
{
  fixture.runTestWithMethod(STRCMP_NOCASE_EQUALWithUnequalInputTestMethod_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <ok>");
}

static void
STRCMP_NOCASE_CONTAINSWithActualIsNULLTestMethod_()
{
  STRCMP_NOCASE_CONTAINS("ok", nullptr);
}

TEST(UnitTestStringMacros, FailureWithSTRCMP_NOCASE_CONTAINSAndActualIsNULL)
{
  fixture.runTestWithMethod(STRCMP_NOCASE_CONTAINSWithActualIsNULLTestMethod_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <ok>");
}

static void
STRCMP_NOCASE_CONTAINSWithExpectedIsNULLTestMethod_()
{
  STRCMP_NOCASE_CONTAINS(nullptr, "ok");
}

TEST(UnitTestStringMacros, FailureWithSTRCMP_NOCASE_CONTAINSAndExpectedIsNULL)
{
  fixture.runTestWithMethod(
      STRCMP_NOCASE_CONTAINSWithExpectedIsNULLTestMethod_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <>");
}

static void
failingTestMethodWithSTRCMP_EQUAL_()
{
  STRCMP_EQUAL("hello", "hell");
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

TEST(UnitTestStringMacros, FailureWithSTRCMP_EQUAL)
{
  fixture.runTestWithMethod(failingTestMethodWithSTRCMP_EQUAL_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <hell>");
}

TEST(UnitTestStringMacros, STRCMP_EQUALBehavesAsProperMacro)
{
  if (false)
    STRCMP_EQUAL("1", "2");
  else
    STRCMP_EQUAL("1", "1");
}

IGNORE_TEST(UnitTestStringMacros, STRCMP_EQUALWorksInAnIgnoredTest)
{
  STRCMP_EQUAL("Hello", "World");
}

static void
failingTestMethodWithSTRCMP_EQUAL_TEXT_()
{
  STRCMP_EQUAL_TEXT("hello", "hell", "Failed because it failed");
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

TEST(UnitTestStringMacros, FailureWithSTRCMP_EQUAL_TEXT)
{
  fixture.runTestWithMethod(failingTestMethodWithSTRCMP_EQUAL_TEXT_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <hell>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestStringMacros, STRCMP_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    STRCMP_EQUAL_TEXT("1", "2", "Failed because it failed");
  else
    STRCMP_EQUAL_TEXT("1", "1", "Failed because it failed");
}

IGNORE_TEST(UnitTestStringMacros, STRCMP_EQUAL_TEXTWorksInAnIgnoredTest)
{
  STRCMP_EQUAL_TEXT("Hello", "World", "Failed because it failed");
}

static void
failingTestMethodWithSTRNCMP_EQUAL_()
{
  STRNCMP_EQUAL("hello", "hallo", 5);
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

TEST(UnitTestStringMacros, FailureWithSTRNCMP_EQUAL)
{
  fixture.runTestWithMethod(failingTestMethodWithSTRNCMP_EQUAL_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <hallo>");
}

TEST(UnitTestStringMacros, STRNCMP_EQUALBehavesAsProperMacro)
{
  if (false)
    STRNCMP_EQUAL("1", "2", 1);
  else
    STRNCMP_EQUAL("1", "1", 1);
}

IGNORE_TEST(UnitTestStringMacros, STRNCMP_EQUALWorksInAnIgnoredTest)
{
  STRNCMP_EQUAL("Hello", "World", 3);
}

static void
failingTestMethodWithSTRNCMP_EQUAL_TEXT_()
{
  STRNCMP_EQUAL_TEXT("hello", "hallo", 5, "Failed because it failed");
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

TEST(UnitTestStringMacros, FailureWithSTRNCMP_EQUAL_TEXT)
{
  fixture.runTestWithMethod(failingTestMethodWithSTRNCMP_EQUAL_TEXT_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <hallo>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestStringMacros, STRNCMP_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    STRNCMP_EQUAL_TEXT("1", "2", 1, "Failed because it failed");
  else
    STRNCMP_EQUAL_TEXT("1", "1", 1, "Failed because it failed");
}

IGNORE_TEST(UnitTestStringMacros, STRNCMP_EQUAL_TEXTWorksInAnIgnoredTest)
{
  STRNCMP_EQUAL_TEXT("Hello", "World", 3, "Failed because it failed");
}

static void
failingTestMethodWithSTRCMP_NOCASE_EQUAL_()
{
  STRCMP_NOCASE_EQUAL("hello", "Hell");
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

TEST(UnitTestStringMacros, FailureWithSTRCMP_NOCASE_EQUAL)
{
  fixture.runTestWithMethod(failingTestMethodWithSTRCMP_NOCASE_EQUAL_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Hell>");
}

TEST(UnitTestStringMacros, STRCMP_NOCASE_EQUALBehavesAsProperMacro)
{
  if (false)
    STRCMP_NOCASE_EQUAL("1", "2");
  else
    STRCMP_NOCASE_EQUAL("1", "1");
}

IGNORE_TEST(UnitTestStringMacros, STRCMP_NOCASE_EQUALWorksInAnIgnoredTest)
{
  STRCMP_NOCASE_EQUAL("Hello", "World");
}

static void
failingTestMethodWithSTRCMP_NOCASE_EQUAL_TEXT_()
{
  STRCMP_NOCASE_EQUAL_TEXT("hello", "hell", "Failed because it failed");
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

TEST(UnitTestStringMacros, FailureWithSTRCMP_NOCASE_EQUAL_TEXT)
{
  fixture.runTestWithMethod(failingTestMethodWithSTRCMP_NOCASE_EQUAL_TEXT_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <hell>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestStringMacros, STRCMP_NOCASE_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    STRCMP_NOCASE_EQUAL_TEXT("1", "2", "Failed because it failed");
  else
    STRCMP_NOCASE_EQUAL_TEXT("1", "1", "Failed because it failed");
}

IGNORE_TEST(UnitTestStringMacros, STRCMP_NOCASE_EQUAL_TEXTWorksInAnIgnoredTest)
{
  STRCMP_NOCASE_EQUAL_TEXT("Hello", "World", "Failed because it failed");
}

static void
failingTestMethodWithSTRCMP_CONTAINS_()
{
  STRCMP_CONTAINS("hello", "world");
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

TEST(UnitTestStringMacros, FailureWithSTRCMP_CONTAINS)
{
  fixture.runTestWithMethod(failingTestMethodWithSTRCMP_CONTAINS_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("actual <world>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <hello>");
}

TEST(UnitTestStringMacros, STRCMP_CONTAINSBehavesAsProperMacro)
{
  if (false)
    STRCMP_CONTAINS("1", "2");
  else
    STRCMP_CONTAINS("1", "1");
}

IGNORE_TEST(UnitTestStringMacros, STRCMP_CONTAINSWorksInAnIgnoredTest)
{
  STRCMP_CONTAINS("Hello", "World");
}

static void
failingTestMethodWithSTRCMP_CONTAINS_TEXT_()
{
  STRCMP_CONTAINS_TEXT("hello", "world", "Failed because it failed");
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

TEST(UnitTestStringMacros, FailureWithSTRCMP_CONTAINS_TEXT)
{
  fixture.runTestWithMethod(failingTestMethodWithSTRCMP_CONTAINS_TEXT_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("actual <world>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestStringMacros, STRCMP_CONTAINS_TEXTBehavesAsProperMacro)
{
  if (false)
    STRCMP_CONTAINS_TEXT("1", "2", "Failed because it failed");
  else
    STRCMP_CONTAINS_TEXT("1", "1", "Failed because it failed");
}

IGNORE_TEST(UnitTestStringMacros, STRCMP_CONTAINS_TEXTWorksInAnIgnoredTest)
{
  STRCMP_CONTAINS_TEXT("Hello", "World", "Failed because it failed");
}

static void
failingTestMethodWithSTRCMP_NOCASE_CONTAINS_()
{
  STRCMP_NOCASE_CONTAINS("hello", "WORLD");
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

TEST(UnitTestStringMacros, FailureWithSTRCMP_NOCASE_CONTAINS)
{
  fixture.runTestWithMethod(failingTestMethodWithSTRCMP_NOCASE_CONTAINS_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("actual <WORLD>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <hello>");
}

TEST(UnitTestStringMacros, STRCMP_NOCASE_CONTAINSBehavesAsProperMacro)
{
  if (false)
    STRCMP_NOCASE_CONTAINS("never", "executed");
  else
    STRCMP_NOCASE_CONTAINS("hello", "HELLO WORLD");
}

IGNORE_TEST(UnitTestStringMacros, STRCMP_NO_CASE_CONTAINSWorksInAnIgnoredTest)
{
  STRCMP_NOCASE_CONTAINS("Hello", "World");
}

static void
failingTestMethodWithSTRCMP_NOCASE_CONTAINS_TEXT_()
{
  STRCMP_NOCASE_CONTAINS_TEXT("hello", "WORLD", "Failed because it failed");
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

TEST(UnitTestStringMacros, FailureWithSTRCMP_NOCASE_CONTAINS_TEXT)
{
  fixture.runTestWithMethod(failingTestMethodWithSTRCMP_NOCASE_CONTAINS_TEXT_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("actual <WORLD>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UnitTestStringMacros, STRCMP_NOCASE_CONTAINS_TEXTBehavesAsProperMacro)
{
  if (false)
    STRCMP_NOCASE_CONTAINS_TEXT(
        "never", "executed", "Failed because it failed");
  else
    STRCMP_NOCASE_CONTAINS_TEXT(
        "hello", "HELLO WORLD", "Failed because it failed");
}

IGNORE_TEST(UnitTestStringMacros,
    STRCMP_NO_CASE_CONTAINS_TEXTWorksInAnIgnoredTest)
{
  STRCMP_NOCASE_CONTAINS_TEXT("Hello", "World", "Failed because it failed");
}

TEST(UnitTestStringMacros, NFirstCharsComparison)
{
  STRNCMP_EQUAL("Hello World!", "Hello Peter!", 0);
  STRNCMP_EQUAL("Hello World!", "Hello Peter!", 1);
  STRNCMP_EQUAL("Hello World!", "Hello Peter!", 6);
  STRNCMP_EQUAL("Hello World!", "Hello", 5);
}

static void
compareNFirstCharsWithUpperAndLowercase_()
{
  STRNCMP_EQUAL("hello world!", "HELLO WORLD!", 12);
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

TEST(UnitTestStringMacros, CompareNFirstCharsWithUpperAndLowercase)
{
  fixture.runTestWithMethod(compareNFirstCharsWithUpperAndLowercase_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello world!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <HELLO WORLD!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 0");
}

static void
compareNFirstCharsWithDifferenceInTheMiddle_()
{
  STRNCMP_EQUAL("Hello World!", "Hello Peter!", 12);
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

TEST(UnitTestStringMacros, CompareNFirstCharsWithDifferenceInTheMiddle)
{
  fixture.runTestWithMethod(compareNFirstCharsWithDifferenceInTheMiddle_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <Hello World!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Hello Peter!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 6");
}

static void
compareNFirstCharsWithEmptyString_()
{
  STRNCMP_EQUAL("", "Not empty string", 5);
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

TEST(UnitTestStringMacros, CompareNFirstCharsWithEmptyString)
{
  fixture.runTestWithMethod(compareNFirstCharsWithEmptyString_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Not empty string>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 0");
}

static void
compareNFirstCharsWithLastCharDifferent_()
{
  STRNCMP_EQUAL("Not empty string?", "Not empty string!", 17);
  cpputest::TestTestingFixture::lineExecutedAfterCheck();
}

TEST(UnitTestStringMacros, CompareNFirstCharsWithLastCharDifferent)
{
  fixture.runTestWithMethod(compareNFirstCharsWithLastCharDifferent_);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <Not empty string?>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Not empty string!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 16");
}
