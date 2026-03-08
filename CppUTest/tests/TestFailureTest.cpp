/*
 * Copyright (c) 2007, Michael Feathers, James Grenning and Bas Vodde
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestOutput.h"
#include <limits.h>

namespace
{
const int failLineNumber = 2;
const char* failFileName = "fail.cpp";
}

TEST_GROUP(TestFailure)
{
    cpputest::TestShell* test;

    void setup() override
    {
        test = new cpputest::TestShell("groupname", "testname", failFileName, failLineNumber-1);
    }
    void teardown() override
    {
        delete test;
    }
};
#define FAILURE_EQUAL(a, b) STRCMP_EQUAL_LOCATION(a, (b).getMessage().asCharString(), "", __FILE__, __LINE__)

TEST(TestFailure, CreateFailure)
{
    cpputest::TestFailure f1(test, failFileName, failLineNumber, "the failure message");
    cpputest::TestFailure f2(test, "the failure message");
    cpputest::TestFailure f3(test, failFileName, failLineNumber);
}

TEST(TestFailure, GetTestFileAndLineFromFailure)
{
    cpputest::TestFailure f1(test, failFileName, failLineNumber, "the failure message");
    STRCMP_EQUAL(failFileName, f1.getTestFileName().asCharString());
    LONGS_EQUAL(1, f1.getTestLineNumber());
}

TEST(TestFailure, EqualsFailureWithText)
{
    cpputest::EqualsFailure f(test, failFileName, failLineNumber, "expected", "actual", "text");
    FAILURE_EQUAL("Message: text\n"
                  "\texpected <expected>\n\tbut was  <actual>", f);
}

TEST(TestFailure, EqualsFailure)
{
    cpputest::EqualsFailure f(test, failFileName, failLineNumber, "expected", "actual", "");
    FAILURE_EQUAL("expected <expected>\n\tbut was  <actual>", f);
}

TEST(TestFailure, EqualsFailureWithNullAsActual)
{
    cpputest::EqualsFailure f(test, failFileName, failLineNumber, "expected", nullptr, "");
    FAILURE_EQUAL("expected <expected>\n\tbut was  <(null)>", f);
}

TEST(TestFailure, EqualsFailureWithNullAsExpected)
{
    cpputest::EqualsFailure f(test, failFileName, failLineNumber, nullptr, "actual", "");
    FAILURE_EQUAL("expected <(null)>\n\tbut was  <actual>", f);
}

TEST(TestFailure, CheckEqualFailureWithText)
{
    cpputest::CheckEqualFailure f(test, failFileName, failLineNumber, "expected", "actual", "text");
    FAILURE_EQUAL("Message: text\n"
                  "\texpected <expected>\n"
                  "\tbut was  <actual>\n"
                  "\tdifference starts at position 0 at: <          actual    >\n"
                  "\t                                               ^", f);
}

TEST(TestFailure, CheckEqualFailure)
{
    cpputest::CheckEqualFailure f(test, failFileName, failLineNumber, "expected", "actual", "");
    FAILURE_EQUAL("expected <expected>\n"
                  "\tbut was  <actual>\n"
                  "\tdifference starts at position 0 at: <          actual    >\n"
                  "\t                                               ^", f);
}

TEST(TestFailure, CheckFailure)
{
    cpputest::CheckFailure f(test, failFileName, failLineNumber, "CHECK", "chk");
    FAILURE_EQUAL("CHECK(chk) failed", f);
}

TEST(TestFailure, CheckFailureWithText)
{
    cpputest::CheckFailure f(test, failFileName, failLineNumber, "CHECK", "chk", "text");
    FAILURE_EQUAL("Message: text\n"
                  "\tCHECK(chk) failed", f);
}

TEST(TestFailure, FailFailure)
{
    cpputest::FailFailure f(test, failFileName, failLineNumber, "chk");
    FAILURE_EQUAL("chk", f);
}

TEST(TestFailure, LongsEqualFailureWithText)
{
    cpputest::LongsEqualFailure f(test, failFileName, failLineNumber, 1, 2, "text");
    FAILURE_EQUAL("Message: text\n"
                  "\texpected <1 (0x1)>\n\tbut was  <2 (0x2)>", f);
}

TEST(TestFailure, LongsEqualFailure)
{
    cpputest::LongsEqualFailure f(test, failFileName, failLineNumber, 1, 2, "");
    FAILURE_EQUAL("expected <1 (0x1)>\n\tbut was  <2 (0x2)>", f);
}

TEST(TestFailure, LongLongsEqualFailure)
{
    cpputest::LongLongsEqualFailure f(test, failFileName, failLineNumber, 1, 2, "");
    FAILURE_EQUAL("expected <1 (0x1)>\n\tbut was  <2 (0x2)>", f);
}

TEST(TestFailure, UnsignedLongLongsEqualFailure)
{
    cpputest::UnsignedLongLongsEqualFailure f(test, failFileName, failLineNumber, 1, 2, "");
    FAILURE_EQUAL("expected <1 (0x1)>\n\tbut was  <2 (0x2)>", f);
}

TEST(TestFailure, SignedBytesEqualFailure)
{
    cpputest::SignedBytesEqualFailure f(test, failFileName, failLineNumber, static_cast<signed char>(-1), static_cast<signed char>(2), "");
    FAILURE_EQUAL("expected <-1 (0xff)>\n\tbut was  < 2 (0x2)>", f);
}

TEST(TestFailure, StringsEqualFailureWithText)
{
    cpputest::StringEqualFailure f(test, failFileName, failLineNumber, "abc", "abd", "text");
    FAILURE_EQUAL("Message: text\n"
                  "\texpected <abc>\n"
                  "\tbut was  <abd>\n"
                  "\tdifference starts at position 2 at: <        abd         >\n"
                  "\t                                               ^", f);
}

TEST(TestFailure, StringsEqualFailure)
{
    cpputest::StringEqualFailure f(test, failFileName, failLineNumber, "abc", "abd", "");
    FAILURE_EQUAL("expected <abc>\n"
                "\tbut was  <abd>\n"
                "\tdifference starts at position 2 at: <        abd         >\n"
                "\t                                               ^", f);
}

TEST(TestFailure, StringsEqualFailureAtTheEnd)
{
    cpputest::StringEqualFailure f(test, failFileName, failLineNumber, "abc", "ab", "");
    FAILURE_EQUAL("expected <abc>\n"
                "\tbut was  <ab>\n"
                "\tdifference starts at position 2 at: <        ab          >\n"
                "\t                                               ^", f);
}

TEST(TestFailure, StringsEqualFailureNewVariantAtTheEnd)
{
    cpputest::StringEqualFailure f(test, failFileName, failLineNumber, "EndOfALongerString", "EndOfALongerStrinG", "");
    FAILURE_EQUAL("expected <EndOfALongerString>\n"
                "\tbut was  <EndOfALongerStrinG>\n"
                "\tdifference starts at position 17 at: <ongerStrinG         >\n"
                "\t                                                ^", f);
}

TEST(TestFailure, StringsEqualFailureWithNewLinesAndTabs)
{
    cpputest::StringEqualFailure f(test, failFileName, failLineNumber,
            "StringWith\t\nDifferentString",
            "StringWith\t\ndifferentString", "");

    FAILURE_EQUAL("expected <StringWith\\t\\nDifferentString>\n"
                "\tbut was  <StringWith\\t\\ndifferentString>\n"
                "\tdifference starts at position 12 at: <ngWith\\t\\ndifferentS>\n"
                "\t                                                ^", f);
}

TEST(TestFailure, StringsEqualFailureInTheMiddle)
{
    cpputest::StringEqualFailure f(test, failFileName, failLineNumber, "aa", "ab", "");
    FAILURE_EQUAL("expected <aa>\n"
                "\tbut was  <ab>\n"
                "\tdifference starts at position 1 at: <         ab         >\n"
                "\t                                               ^", f);
}

TEST(TestFailure, StringsEqualFailureAtTheBeginning)
{
    cpputest::StringEqualFailure f(test, failFileName, failLineNumber, "aaa", "bbb", "");
    FAILURE_EQUAL("expected <aaa>\n"
                "\tbut was  <bbb>\n"
                "\tdifference starts at position 0 at: <          bbb       >\n"
                "\t                                               ^", f);
}

TEST(TestFailure, StringsEqualFailureWithNullAsActual)
{
    cpputest::StringEqualFailure f(test, failFileName, failLineNumber, "abc", nullptr, "");
    FAILURE_EQUAL("expected <abc>\n"
                "\tbut was  <(null)>", f);
}

TEST(TestFailure, StringsEqualFailureWithNullAsExpected)
{
    cpputest::StringEqualFailure f(test, failFileName, failLineNumber, nullptr, "abd", "");
    FAILURE_EQUAL("expected <(null)>\n"
                "\tbut was  <abd>", f);
}

TEST(TestFailure, StringsEqualNoCaseFailureWithText)
{
    cpputest::StringEqualNoCaseFailure f(test, failFileName, failLineNumber, "ABC", "abd", "text");
    FAILURE_EQUAL("Message: text\n"
                  "\texpected <ABC>\n"
                  "\tbut was  <abd>\n"
                  "\tdifference starts at position 2 at: <        abd         >\n"
                  "\t                                               ^", f);
}

TEST(TestFailure, StringsEqualNoCaseFailure)
{
    cpputest::StringEqualNoCaseFailure f(test, failFileName, failLineNumber, "ABC", "abd", "");
    FAILURE_EQUAL("expected <ABC>\n"
                "\tbut was  <abd>\n"
                "\tdifference starts at position 2 at: <        abd         >\n"
                "\t                                               ^", f);
}

TEST(TestFailure, StringsEqualNoCaseFailureWithActualAsNull)
{
    cpputest::StringEqualNoCaseFailure f(test, failFileName, failLineNumber, "ABC", nullptr, "");
    FAILURE_EQUAL("expected <ABC>\n"
                "\tbut was  <(null)>", f);
}

TEST(TestFailure, StringsEqualNoCaseFailureWithExpectedAsNull)
{
    cpputest::StringEqualNoCaseFailure f(test, failFileName, failLineNumber, nullptr, "abd", "");
    FAILURE_EQUAL("expected <(null)>\n"
                "\tbut was  <abd>", f);
}

TEST(TestFailure, StringsEqualNoCaseFailure2)
{
    cpputest::StringEqualNoCaseFailure f(test, failFileName, failLineNumber, "ac", "AB", "");
    FAILURE_EQUAL("expected <ac>\n"
                "\tbut was  <AB>\n"
                "\tdifference starts at position 1 at: <         AB         >\n"
                "\t                                               ^", f);
}

TEST(TestFailure, DoublesEqualNormalWithText)
{
    cpputest::DoublesEqualFailure f(test, failFileName, failLineNumber, 1.0, 2.0, 3.0, "text");
    FAILURE_EQUAL("Message: text\n"
                  "\texpected <1>\n"
                  "\tbut was  <2> threshold used was <3>", f);
}

TEST(TestFailure, DoublesEqualNormal)
{
    cpputest::DoublesEqualFailure f(test, failFileName, failLineNumber, 1.0, 2.0, 3.0, "");
    FAILURE_EQUAL("expected <1>\n"
                "\tbut was  <2> threshold used was <3>", f);
}

TEST(TestFailure, BinaryEqualWithText)
{
    const unsigned char expectedData[] = { 0x00 };
    const unsigned char actualData[] = { 0x01 };
    cpputest::BinaryEqualFailure f(test, failFileName, failLineNumber, expectedData, actualData, sizeof(expectedData), "text");
    FAILURE_EQUAL("Message: text\n"
                  "\texpected <00>\n"
                  "\tbut was  <01>\n"
                  "\tdifference starts at position 0 at: <         01         >\n"
                  "\t                                               ^", f);
}

TEST(TestFailure, BinaryEqualOneByte)
{
    const unsigned char expectedData[] = { 0x00 };
    const unsigned char actualData[] = { 0x01 };
    cpputest::BinaryEqualFailure f(test, failFileName, failLineNumber, expectedData, actualData, sizeof(expectedData), "");
    FAILURE_EQUAL("expected <00>\n"
                "\tbut was  <01>\n"
    			"\tdifference starts at position 0 at: <         01         >\n"
    			"\t                                               ^", f);
}

TEST(TestFailure, BinaryEqualTwoBytes)
{
    const unsigned char expectedData[] = {0x00, 0x01};
    const unsigned char actualData[] = {0x00, 0x02};
    cpputest::BinaryEqualFailure f(test, failFileName, failLineNumber, expectedData, actualData, sizeof(expectedData), "");
    FAILURE_EQUAL("expected <00 01>\n"
                "\tbut was  <00 02>\n"
    			"\tdifference starts at position 1 at: <      00 02         >\n"
    			"\t                                               ^", f);
}

TEST(TestFailure, BinaryEqualThreeBytes)
{
    const unsigned char expectedData[] = {0x00, 0x01, 0x00};
    const unsigned char actualData[] = {0x00, 0x02, 0x00};
    cpputest::BinaryEqualFailure f(test, failFileName, failLineNumber, expectedData, actualData, sizeof(expectedData), "");
    FAILURE_EQUAL("expected <00 01 00>\n"
                "\tbut was  <00 02 00>\n"
    			"\tdifference starts at position 1 at: <      00 02 00      >\n"
    			"\t                                               ^", f);
}

TEST(TestFailure, BinaryEqualFullWidth)
{
    const unsigned char expectedData[] = {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};
    const unsigned char actualData[] = {0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00};
    cpputest::BinaryEqualFailure f(test, failFileName, failLineNumber, expectedData, actualData, sizeof(expectedData), "");
    FAILURE_EQUAL("expected <00 00 00 01 00 00 00>\n"
                "\tbut was  <00 00 00 02 00 00 00>\n"
    			"\tdifference starts at position 3 at: <00 00 00 02 00 00 00>\n"
    			"\t                                               ^", f);
}

TEST(TestFailure, BinaryEqualLast)
{
    const unsigned char expectedData[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	const unsigned char actualData[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    cpputest::BinaryEqualFailure f(test, failFileName, failLineNumber, expectedData, actualData, sizeof(expectedData), "");
    FAILURE_EQUAL("expected <00 00 00 00 00 00 00>\n"
                "\tbut was  <00 00 00 00 00 00 01>\n"
    			"\tdifference starts at position 6 at: <00 00 00 01         >\n"
    			"\t                                               ^", f);
}

TEST(TestFailure, BinaryEqualActualNull)
{
    const unsigned char expectedData[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    cpputest::BinaryEqualFailure f(test, failFileName, failLineNumber, expectedData, nullptr, sizeof(expectedData), "");
    FAILURE_EQUAL("expected <00 00 00 00 00 00 00>\n\tbut was  <(null)>", f);
}

TEST(TestFailure, BinaryEqualExpectedNull)
{
    const unsigned char actualData[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    cpputest::BinaryEqualFailure f(test, failFileName, failLineNumber, nullptr, actualData, sizeof(actualData), "");
    FAILURE_EQUAL("expected <(null)>\n\tbut was  <00 00 00 00 00 00 01>", f);
}

TEST(TestFailure, BitsEqualWithText)
{
    cpputest::BitsEqualFailure f(test, failFileName, failLineNumber, 0x0001, 0x0003, 0x00FF, 2*8/CHAR_BIT, "text");
    FAILURE_EQUAL("Message: text\n"
                  "\texpected <xxxxxxxx 00000001>\n\tbut was  <xxxxxxxx 00000011>", f);
}

#if (CHAR_BIT == 16)
TEST(TestFailure, BitsEqualChar)
{
    cpputest::BitsEqualFailure f(test, failFileName, failLineNumber, 0x01, 0x03, 0xFF, sizeof(char), "");
    FAILURE_EQUAL("expected <xxxxxxxx 00000001>\n\tbut was  <xxxxxxxx 00000011>", f);
}
#elif (CHAR_BIT == 8)
TEST(TestFailure, BitsEqualChar)
{
    cpputest::BitsEqualFailure f(test, failFileName, failLineNumber, 0x01, 0x03, 0xFF, sizeof(char), "");
    FAILURE_EQUAL("expected <00000001>\n\tbut was  <00000011>", f);
}
#endif

TEST(TestFailure, BitsEqual16Bit)
{
    cpputest::BitsEqualFailure f(test, failFileName, failLineNumber, 0x0001, 0x0003, 0xFFFF, 2*8/CHAR_BIT, "");
    FAILURE_EQUAL("expected <00000000 00000001>\n\tbut was  <00000000 00000011>", f);
}

TEST(TestFailure, BitsEqual32Bit)
{
    cpputest::BitsEqualFailure f(test, failFileName, failLineNumber, 0x00000001, 0x00000003, 0xFFFFFFFF, 4*8/CHAR_BIT, "");
    FAILURE_EQUAL("expected <00000000 00000000 00000000 00000001>\n\tbut was  <00000000 00000000 00000000 00000011>", f);
}

TEST(TestFailure, FeatureUnsupported)
{
    cpputest::FeatureUnsupportedFailure f(test, failFileName, failLineNumber, "SOME_FEATURE", "");
    FAILURE_EQUAL("The feature \"SOME_FEATURE\" is not supported in this environment or with the feature set selected when building the library.", f);
}

#if CPPUTEST_HAVE_EXCEPTIONS
TEST(TestFailure, UnexpectedExceptionFailure_UnknownException)
{
    cpputest::UnexpectedExceptionFailure f(test);
    FAILURE_EQUAL("Unexpected exception of unknown type was thrown.", f);
}
#endif

#if CPPUTEST_HAVE_EXCEPTIONS && CPPUTEST_USE_STD_CPP_LIB
TEST(TestFailure, UnexpectedExceptionFailure_StandardException)
{
    std::runtime_error e("Some error");
    cpputest::UnexpectedExceptionFailure f(test, e);
#if CPPUTEST_HAVE_RTTI
    STRCMP_CONTAINS("Unexpected exception of type '", f.getMessage().asCharString());
    STRCMP_CONTAINS("runtime_error", f.getMessage().asCharString());
    STRCMP_CONTAINS("' was thrown: Some error", f.getMessage().asCharString());
#else
    FAILURE_EQUAL("Unexpected exception of unknown type was thrown.", f);
#endif
}
#endif
