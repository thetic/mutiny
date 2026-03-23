#include "mutiny/test.hpp"
#include "mutiny/test/Output.hpp"

#include <limits.h>

namespace {
const int fail_line_number = 2;
const char* fail_file_name = "fail.cpp";
}

TEST_GROUP(TestFailure)
{
  mu::tiny::test::TestShell* test;

  void setup() override
  {
    test = new mu::tiny::test::TestShell(
        "groupname", "testname", fail_file_name, fail_line_number - 1
    );
  }
  void teardown() override { delete test; }
};
#define FAILURE_EQUAL(a, b)                                                    \
  STRCMP_EQUAL_LOCATION(a, (b).get_message().c_str(), "", __FILE__, __LINE__)

TEST(TestFailure, CreateFailure)
{
  mu::tiny::test::TestFailure f1(
      test, fail_file_name, fail_line_number, "the failure message"
  );
  mu::tiny::test::TestFailure f2(test, "the failure message");
  mu::tiny::test::TestFailure f3(test, fail_file_name, fail_line_number);
}

TEST(TestFailure, GetTestFileAndLineFromFailure)
{
  mu::tiny::test::TestFailure f1(
      test, fail_file_name, fail_line_number, "the failure message"
  );
  STRCMP_EQUAL(fail_file_name, f1.get_test_file_name().c_str());
  LONGS_EQUAL(1, f1.get_test_line_number());
}

TEST(TestFailure, EqualsFailureWithText)
{
  mu::tiny::test::EqualsFailure f(
      test, fail_file_name, fail_line_number, "expected", "actual", "text"
  );
  FAILURE_EQUAL(
      "Message: text\n"
      "\texpected <expected>\n\tbut was  <actual>",
      f
  );
}

TEST(TestFailure, EqualsFailure)
{
  mu::tiny::test::EqualsFailure f(
      test, fail_file_name, fail_line_number, "expected", "actual", ""
  );
  FAILURE_EQUAL("expected <expected>\n\tbut was  <actual>", f);
}

TEST(TestFailure, EqualsFailureWithNullAsActual)
{
  mu::tiny::test::EqualsFailure f(
      test, fail_file_name, fail_line_number, "expected", nullptr, ""
  );
  FAILURE_EQUAL("expected <expected>\n\tbut was  <(null)>", f);
}

TEST(TestFailure, EqualsFailureWithNullAsExpected)
{
  mu::tiny::test::EqualsFailure f(
      test, fail_file_name, fail_line_number, nullptr, "actual", ""
  );
  FAILURE_EQUAL("expected <(null)>\n\tbut was  <actual>", f);
}

TEST(TestFailure, CheckEqualFailureWithText)
{
  mu::tiny::test::CheckEqualFailure f(
      test, fail_file_name, fail_line_number, "expected", "actual", "text"
  );
  FAILURE_EQUAL(
      "Message: text\n"
      "\texpected <expected>\n"
      "\tbut was  <actual>\n"
      "\tdifference starts at position 0 at: <          actual    >\n"
      "\t                                               ^",
      f
  );
}

TEST(TestFailure, CheckEqualFailure)
{
  mu::tiny::test::CheckEqualFailure f(
      test, fail_file_name, fail_line_number, "expected", "actual", ""
  );
  FAILURE_EQUAL(
      "expected <expected>\n"
      "\tbut was  <actual>\n"
      "\tdifference starts at position 0 at: <          actual    >\n"
      "\t                                               ^",
      f
  );
}

TEST(TestFailure, CheckFailure)
{
  mu::tiny::test::CheckFailure f(
      test, fail_file_name, fail_line_number, "CHECK", "chk"
  );
  FAILURE_EQUAL("CHECK(chk) failed", f);
}

TEST(TestFailure, CheckFailureWithText)
{
  mu::tiny::test::CheckFailure f(
      test, fail_file_name, fail_line_number, "CHECK", "chk", "text"
  );
  FAILURE_EQUAL(
      "Message: text\n"
      "\tCHECK(chk) failed",
      f
  );
}

TEST(TestFailure, FailFailure)
{
  mu::tiny::test::FailFailure f(test, fail_file_name, fail_line_number, "chk");
  FAILURE_EQUAL("chk", f);
}

TEST(TestFailure, LongsEqualFailureWithText)
{
  mu::tiny::test::LongsEqualFailure f(
      test, fail_file_name, fail_line_number, 1, 2, "text"
  );
  FAILURE_EQUAL(
      "Message: text\n"
      "\texpected <1 (0x1)>\n\tbut was  <2 (0x2)>",
      f
  );
}

TEST(TestFailure, LongsEqualFailure)
{
  mu::tiny::test::LongsEqualFailure f(
      test, fail_file_name, fail_line_number, 1, 2, ""
  );
  FAILURE_EQUAL("expected <1 (0x1)>\n\tbut was  <2 (0x2)>", f);
}

TEST(TestFailure, LongLongsEqualFailure)
{
  mu::tiny::test::LongLongsEqualFailure f(
      test, fail_file_name, fail_line_number, 1, 2, ""
  );
  FAILURE_EQUAL("expected <1 (0x1)>\n\tbut was  <2 (0x2)>", f);
}

TEST(TestFailure, UnsignedLongLongsEqualFailure)
{
  mu::tiny::test::UnsignedLongLongsEqualFailure f(
      test, fail_file_name, fail_line_number, 1, 2, ""
  );
  FAILURE_EQUAL("expected <1 (0x1)>\n\tbut was  <2 (0x2)>", f);
}

TEST(TestFailure, SignedBytesEqualFailure)
{
  mu::tiny::test::SignedBytesEqualFailure f(
      test,
      fail_file_name,
      fail_line_number,
      static_cast<signed char>(-1),
      static_cast<signed char>(2),
      ""
  );
  FAILURE_EQUAL("expected <-1 (0xff)>\n\tbut was  < 2 (0x2)>", f);
}

TEST(TestFailure, StringsEqualFailureWithText)
{
  mu::tiny::test::StringEqualFailure f(
      test, fail_file_name, fail_line_number, "abc", "abd", "text"
  );
  FAILURE_EQUAL(
      "Message: text\n"
      "\texpected <abc>\n"
      "\tbut was  <abd>\n"
      "\tdifference starts at position 2 at: <        abd         >\n"
      "\t                                               ^",
      f
  );
}

TEST(TestFailure, StringsEqualFailure)
{
  mu::tiny::test::StringEqualFailure f(
      test, fail_file_name, fail_line_number, "abc", "abd", ""
  );
  FAILURE_EQUAL(
      "expected <abc>\n"
      "\tbut was  <abd>\n"
      "\tdifference starts at position 2 at: <        abd         >\n"
      "\t                                               ^",
      f
  );
}

TEST(TestFailure, StringsEqualFailureAtTheEnd)
{
  mu::tiny::test::StringEqualFailure f(
      test, fail_file_name, fail_line_number, "abc", "ab", ""
  );
  FAILURE_EQUAL(
      "expected <abc>\n"
      "\tbut was  <ab>\n"
      "\tdifference starts at position 2 at: <        ab          >\n"
      "\t                                               ^",
      f
  );
}

TEST(TestFailure, StringsEqualFailureNewVariantAtTheEnd)
{
  mu::tiny::test::StringEqualFailure f(
      test,
      fail_file_name,
      fail_line_number,
      "EndOfALongerString",
      "EndOfALongerStrinG",
      ""
  );
  FAILURE_EQUAL(
      "expected <EndOfALongerString>\n"
      "\tbut was  <EndOfALongerStrinG>\n"
      "\tdifference starts at position 17 at: <ongerStrinG         >\n"
      "\t                                                ^",
      f
  );
}

TEST(TestFailure, StringsEqualFailureWithNewLinesAndTabs)
{
  mu::tiny::test::StringEqualFailure f(
      test,
      fail_file_name,
      fail_line_number,
      "StringWith\t\nDifferentString",
      "StringWith\t\ndifferentString",
      ""
  );

  FAILURE_EQUAL(
      "expected <StringWith\\t\\nDifferentString>\n"
      "\tbut was  <StringWith\\t\\ndifferentString>\n"
      "\tdifference starts at position 12 at: <ngWith\\t\\ndifferentS>\n"
      "\t                                                ^",
      f
  );
}

TEST(TestFailure, StringsEqualFailureInTheMiddle)
{
  mu::tiny::test::StringEqualFailure f(
      test, fail_file_name, fail_line_number, "aa", "ab", ""
  );
  FAILURE_EQUAL(
      "expected <aa>\n"
      "\tbut was  <ab>\n"
      "\tdifference starts at position 1 at: <         ab         >\n"
      "\t                                               ^",
      f
  );
}

TEST(TestFailure, StringsEqualFailureAtTheBeginning)
{
  mu::tiny::test::StringEqualFailure f(
      test, fail_file_name, fail_line_number, "aaa", "bbb", ""
  );
  FAILURE_EQUAL(
      "expected <aaa>\n"
      "\tbut was  <bbb>\n"
      "\tdifference starts at position 0 at: <          bbb       >\n"
      "\t                                               ^",
      f
  );
}

TEST(TestFailure, StringsEqualFailureWithNullAsActual)
{
  mu::tiny::test::StringEqualFailure f(
      test, fail_file_name, fail_line_number, "abc", nullptr, ""
  );
  FAILURE_EQUAL(
      "expected <abc>\n"
      "\tbut was  <(null)>",
      f
  );
}

TEST(TestFailure, StringsEqualFailureWithNullAsExpected)
{
  mu::tiny::test::StringEqualFailure f(
      test, fail_file_name, fail_line_number, nullptr, "abd", ""
  );
  FAILURE_EQUAL(
      "expected <(null)>\n"
      "\tbut was  <abd>",
      f
  );
}

TEST(TestFailure, StringsEqualNoCaseFailureWithText)
{
  mu::tiny::test::StringEqualNoCaseFailure f(
      test, fail_file_name, fail_line_number, "ABC", "abd", "text"
  );
  FAILURE_EQUAL(
      "Message: text\n"
      "\texpected <ABC>\n"
      "\tbut was  <abd>\n"
      "\tdifference starts at position 2 at: <        abd         >\n"
      "\t                                               ^",
      f
  );
}

TEST(TestFailure, StringsEqualNoCaseFailure)
{
  mu::tiny::test::StringEqualNoCaseFailure f(
      test, fail_file_name, fail_line_number, "ABC", "abd", ""
  );
  FAILURE_EQUAL(
      "expected <ABC>\n"
      "\tbut was  <abd>\n"
      "\tdifference starts at position 2 at: <        abd         >\n"
      "\t                                               ^",
      f
  );
}

TEST(TestFailure, StringsEqualNoCaseFailureWithActualAsNull)
{
  mu::tiny::test::StringEqualNoCaseFailure f(
      test, fail_file_name, fail_line_number, "ABC", nullptr, ""
  );
  FAILURE_EQUAL(
      "expected <ABC>\n"
      "\tbut was  <(null)>",
      f
  );
}

TEST(TestFailure, StringsEqualNoCaseFailureWithExpectedAsNull)
{
  mu::tiny::test::StringEqualNoCaseFailure f(
      test, fail_file_name, fail_line_number, nullptr, "abd", ""
  );
  FAILURE_EQUAL(
      "expected <(null)>\n"
      "\tbut was  <abd>",
      f
  );
}

TEST(TestFailure, StringsEqualNoCaseFailure2)
{
  mu::tiny::test::StringEqualNoCaseFailure f(
      test, fail_file_name, fail_line_number, "ac", "AB", ""
  );
  FAILURE_EQUAL(
      "expected <ac>\n"
      "\tbut was  <AB>\n"
      "\tdifference starts at position 1 at: <         AB         >\n"
      "\t                                               ^",
      f
  );
}

TEST(TestFailure, DoublesEqualNormalWithText)
{
  mu::tiny::test::DoublesEqualFailure f(
      test, fail_file_name, fail_line_number, 1.0, 2.0, 3.0, "text"
  );
  FAILURE_EQUAL(
      "Message: text\n"
      "\texpected <1>\n"
      "\tbut was  <2> threshold used was <3>",
      f
  );
}

TEST(TestFailure, DoublesEqualNormal)
{
  mu::tiny::test::DoublesEqualFailure f(
      test, fail_file_name, fail_line_number, 1.0, 2.0, 3.0, ""
  );
  FAILURE_EQUAL(
      "expected <1>\n"
      "\tbut was  <2> threshold used was <3>",
      f
  );
}

TEST(TestFailure, BinaryEqualWithText)
{
  const unsigned char expected_data[] = { 0x00 };
  const unsigned char actual_data[] = { 0x01 };
  mu::tiny::test::BinaryEqualFailure f(
      test,
      fail_file_name,
      fail_line_number,
      expected_data,
      actual_data,
      sizeof(expected_data),
      "text"
  );
  FAILURE_EQUAL(
      "Message: text\n"
      "\texpected <00>\n"
      "\tbut was  <01>\n"
      "\tdifference starts at position 0 at: <         01         >\n"
      "\t                                               ^",
      f
  );
}

TEST(TestFailure, BinaryEqualOneByte)
{
  const unsigned char expected_data[] = { 0x00 };
  const unsigned char actual_data[] = { 0x01 };
  mu::tiny::test::BinaryEqualFailure f(
      test,
      fail_file_name,
      fail_line_number,
      expected_data,
      actual_data,
      sizeof(expected_data),
      ""
  );
  FAILURE_EQUAL(
      "expected <00>\n"
      "\tbut was  <01>\n"
      "\tdifference starts at position 0 at: <         01         >\n"
      "\t                                               ^",
      f
  );
}

TEST(TestFailure, BinaryEqualTwoBytes)
{
  const unsigned char expected_data[] = { 0x00, 0x01 };
  const unsigned char actual_data[] = { 0x00, 0x02 };
  mu::tiny::test::BinaryEqualFailure f(
      test,
      fail_file_name,
      fail_line_number,
      expected_data,
      actual_data,
      sizeof(expected_data),
      ""
  );
  FAILURE_EQUAL(
      "expected <00 01>\n"
      "\tbut was  <00 02>\n"
      "\tdifference starts at position 1 at: <      00 02         >\n"
      "\t                                               ^",
      f
  );
}

TEST(TestFailure, BinaryEqualThreeBytes)
{
  const unsigned char expected_data[] = { 0x00, 0x01, 0x00 };
  const unsigned char actual_data[] = { 0x00, 0x02, 0x00 };
  mu::tiny::test::BinaryEqualFailure f(
      test,
      fail_file_name,
      fail_line_number,
      expected_data,
      actual_data,
      sizeof(expected_data),
      ""
  );
  FAILURE_EQUAL(
      "expected <00 01 00>\n"
      "\tbut was  <00 02 00>\n"
      "\tdifference starts at position 1 at: <      00 02 00      >\n"
      "\t                                               ^",
      f
  );
}

TEST(TestFailure, BinaryEqualFullWidth)
{
  const unsigned char expected_data[] = { 0x00, 0x00, 0x00, 0x01,
                                          0x00, 0x00, 0x00 };
  const unsigned char actual_data[] = {
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00
  };
  mu::tiny::test::BinaryEqualFailure f(
      test,
      fail_file_name,
      fail_line_number,
      expected_data,
      actual_data,
      sizeof(expected_data),
      ""
  );
  FAILURE_EQUAL(
      "expected <00 00 00 01 00 00 00>\n"
      "\tbut was  <00 00 00 02 00 00 00>\n"
      "\tdifference starts at position 3 at: <00 00 00 02 00 00 00>\n"
      "\t                                               ^",
      f
  );
}

TEST(TestFailure, BinaryEqualLast)
{
  const unsigned char expected_data[] = { 0x00, 0x00, 0x00, 0x00,
                                          0x00, 0x00, 0x00 };
  const unsigned char actual_data[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
  };
  mu::tiny::test::BinaryEqualFailure f(
      test,
      fail_file_name,
      fail_line_number,
      expected_data,
      actual_data,
      sizeof(expected_data),
      ""
  );
  FAILURE_EQUAL(
      "expected <00 00 00 00 00 00 00>\n"
      "\tbut was  <00 00 00 00 00 00 01>\n"
      "\tdifference starts at position 6 at: <00 00 00 01         >\n"
      "\t                                               ^",
      f
  );
}

TEST(TestFailure, BinaryEqualActualNull)
{
  const unsigned char expected_data[] = { 0x00, 0x00, 0x00, 0x00,
                                          0x00, 0x00, 0x00 };
  mu::tiny::test::BinaryEqualFailure f(
      test,
      fail_file_name,
      fail_line_number,
      expected_data,
      nullptr,
      sizeof(expected_data),
      ""
  );
  FAILURE_EQUAL("expected <00 00 00 00 00 00 00>\n\tbut was  <(null)>", f);
}

TEST(TestFailure, BinaryEqualExpectedNull)
{
  const unsigned char actual_data[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
  };
  mu::tiny::test::BinaryEqualFailure f(
      test,
      fail_file_name,
      fail_line_number,
      nullptr,
      actual_data,
      sizeof(actual_data),
      ""
  );
  FAILURE_EQUAL("expected <(null)>\n\tbut was  <00 00 00 00 00 00 01>", f);
}

TEST(TestFailure, FeatureUnsupported)
{
  mu::tiny::test::FeatureUnsupportedFailure f(
      test, fail_file_name, fail_line_number, "SOME_FEATURE", ""
  );
  FAILURE_EQUAL(
      "The feature \"SOME_FEATURE\" is not supported in this "
      "environment or with "
      "the feature set selected when building the library.",
      f
  );
}

TEST(TestFailure, EqualsFailureWithShortEscapeControlChar)
{
  mu::tiny::test::CheckEqualFailure f(
      test, fail_file_name, fail_line_number, "a\tb", "a\nb", ""
  );
  STRCMP_CONTAINS("\\t", f.get_message().c_str());
  STRCMP_CONTAINS("\\n", f.get_message().c_str());
}

TEST(TestFailure, EqualsFailureWithHexEscapeControlChar)
{
  mu::tiny::test::CheckEqualFailure f(
      test, fail_file_name, fail_line_number, "a\x01z", "b\x01z", ""
  );
  STRCMP_CONTAINS("\\x01", f.get_message().c_str());
}

#if MUTINY_HAVE_EXCEPTIONS
TEST(TestFailure, UnexpectedExceptionFailure_UnknownException)
{
  mu::tiny::test::UnexpectedExceptionFailure f(test);
  FAILURE_EQUAL("Unexpected exception of unknown type was thrown.", f);
}
#endif

#if MUTINY_HAVE_EXCEPTIONS && MUTINY_USE_STD_CPP_LIB
TEST(TestFailure, UnexpectedExceptionFailure_StandardException)
{
  std::runtime_error e("Some error");
  mu::tiny::test::UnexpectedExceptionFailure f(test, e);
#if MUTINY_HAVE_RTTI
  STRCMP_CONTAINS("Unexpected exception of type '", f.get_message().c_str());
  STRCMP_CONTAINS("runtime_error", f.get_message().c_str());
  STRCMP_CONTAINS("' was thrown: Some error", f.get_message().c_str());
#else
  FAILURE_EQUAL("Unexpected exception of unknown type was thrown.", f);
#endif
}
#endif
