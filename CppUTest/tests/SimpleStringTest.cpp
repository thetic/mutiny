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

#include "CppUTest/SimpleString.h"

#include "CppUTest/PlatformSpecificFunctions.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/TestMemoryAllocator.h"

#include <limits.h>

class JustUseNewStringAllocator : public cpputest::TestMemoryAllocator
{
public:
  virtual ~JustUseNewStringAllocator() override {}

  char* alloc_memory(size_t size, const char* file, size_t line) override
  {
    return cpputest::getCurrentNewArrayAllocator()->alloc_memory(
      size, file, line);
  }
  void free_memory(char* str,
                   size_t size,
                   const char* file,
                   size_t line) override
  {
    cpputest::getCurrentNewArrayAllocator()->free_memory(str, size, file, line);
  }
};

TEST_GROUP(SimpleString) {
JustUseNewStringAllocator justNewForSimpleStringTestAllocator;
cpputest::TestMemoryAllocator* originalAllocator;
void
setup() override
{
  originalAllocator = cpputest::SimpleString::getStringAllocator();
  cpputest::SimpleString::setStringAllocator(
    &justNewForSimpleStringTestAllocator);
}
void
teardown() override
{
  cpputest::SimpleString::setStringAllocator(originalAllocator);
}
};

TEST(SimpleString, defaultAllocatorIsNewArrayAllocator)
{
  cpputest::SimpleString::setStringAllocator(nullptr);
  POINTERS_EQUAL(cpputest::defaultNewArrayAllocator(),
                 cpputest::SimpleString::getStringAllocator());
}

class MyOwnStringAllocator : public cpputest::TestMemoryAllocator
{
public:
  MyOwnStringAllocator()
    : memoryWasAllocated(false)
  {
  }
  explicit MyOwnStringAllocator(bool wasAllocated)
    : memoryWasAllocated(wasAllocated)
  {
  }
  virtual ~MyOwnStringAllocator() override {}

  bool memoryWasAllocated;
  char* alloc_memory(size_t size, const char* file, size_t line) override
  {
    memoryWasAllocated = true;
    return TestMemoryAllocator::alloc_memory(size, file, line);
  }
};

TEST(SimpleString, allocatorForSimpleStringCanBeReplaced)
{
  MyOwnStringAllocator myOwnAllocator;
  cpputest::SimpleString::setStringAllocator(&myOwnAllocator);
  cpputest::SimpleString simpleString;
  CHECK(myOwnAllocator.memoryWasAllocated);
  cpputest::SimpleString::setStringAllocator(nullptr);
}

TEST(SimpleString, CreateSequence)
{
  cpputest::SimpleString expected("hellohello");
  cpputest::SimpleString actual("hello", 2);

  CHECK_EQUAL(expected, actual);
}

TEST(SimpleString, CreateSequenceOfZero)
{
  cpputest::SimpleString expected("");
  cpputest::SimpleString actual("hello", 0);

  CHECK_EQUAL(expected, actual);
}

TEST(SimpleString, Copy)
{
  cpputest::SimpleString s1("hello");
  cpputest::SimpleString s2(s1);

  CHECK_EQUAL(s1, s2);
}

TEST(SimpleString, Assignment)
{
  cpputest::SimpleString s1("hello");
  cpputest::SimpleString s2("goodbye");

  s2 = s1;

  CHECK_EQUAL(s1, s2);
}

TEST(SimpleString, Equality)
{
  cpputest::SimpleString s1("hello");
  cpputest::SimpleString s2("hello");

  CHECK(s1 == s2);
}

TEST(SimpleString, InEquality)
{
  cpputest::SimpleString s1("hello");
  cpputest::SimpleString s2("goodbye");

  CHECK(s1 != s2);
}

TEST(SimpleString, CompareNoCaseWithoutCase)
{
  cpputest::SimpleString s1("hello");
  cpputest::SimpleString s2("hello");

  CHECK(s1.equalsNoCase(s2));
}

TEST(SimpleString, CompareNoCaseWithCase)
{
  cpputest::SimpleString s1("hello");
  cpputest::SimpleString s2("HELLO");

  CHECK(s1.equalsNoCase(s2));
}

TEST(SimpleString, CompareNoCaseWithCaseNotEqual)
{
  cpputest::SimpleString s1("hello");
  cpputest::SimpleString s2("WORLD");

  CHECK(!s1.equalsNoCase(s2));
}

TEST(SimpleString, asCharString)
{
  cpputest::SimpleString s1("hello");

  STRCMP_EQUAL("hello", s1.asCharString());
}

TEST(SimpleString, Size)
{
  cpputest::SimpleString s1("hello!");

  LONGS_EQUAL(6, s1.size());
}

TEST(SimpleString, lowerCase)
{
  cpputest::SimpleString s1("AbCdEfG1234");
  cpputest::SimpleString s2(s1.lowerCase());
  STRCMP_EQUAL("abcdefg1234", s2.asCharString());
  STRCMP_EQUAL("AbCdEfG1234", s1.asCharString());
}

TEST(SimpleString, printable)
{
  cpputest::SimpleString s1("ABC\01\06\a\n\r\b\t\v\f\x0E\x1F\x7F"
                            "abc");
  cpputest::SimpleString s2(s1.printable());
  STRCMP_EQUAL("ABC\\x01\\x06\\a\\n\\r\\b\\t\\v\\f\\x0E\\x1F\\x7Fabc",
               s2.asCharString());
  STRCMP_EQUAL("ABC\01\06\a\n\r\b\t\v\f\x0E\x1F\x7F"
               "abc",
               s1.asCharString());
}

TEST(SimpleString, Addition)
{
  cpputest::SimpleString s1("hello!");
  cpputest::SimpleString s2("goodbye!");
  cpputest::SimpleString s3("hello!goodbye!");
  cpputest::SimpleString s4;
  s4 = s1 + s2;

  CHECK_EQUAL(s3, s4);
}

TEST(SimpleString, Concatenation)
{
  cpputest::SimpleString s1("hello!");
  cpputest::SimpleString s2("goodbye!");
  cpputest::SimpleString s3("hello!goodbye!");
  cpputest::SimpleString s4;
  s4 += s1;
  s4 += s2;

  CHECK_EQUAL(s3, s4);

  cpputest::SimpleString s5("hello!goodbye!hello!goodbye!");
  s4 += s4;

  CHECK_EQUAL(s5, s4);
}

TEST(SimpleString, Contains)
{
  cpputest::SimpleString s("hello!");
  cpputest::SimpleString empty("");
  cpputest::SimpleString beginning("hello");
  cpputest::SimpleString end("lo!");
  cpputest::SimpleString mid("l");
  cpputest::SimpleString notPartOfString("xxxx");

  CHECK(s.contains(empty));
  CHECK(s.contains(beginning));
  CHECK(s.contains(end));
  CHECK(s.contains(mid));
  CHECK(!s.contains(notPartOfString));

  CHECK(empty.contains(empty));
  CHECK(!empty.contains(s));
}

TEST(SimpleString, startsWith)
{
  cpputest::SimpleString hi("Hi you!");
  cpputest::SimpleString part("Hi");
  cpputest::SimpleString diff("Hrrm Hi you! ffdsfd");
  CHECK(hi.startsWith(part));
  CHECK(!part.startsWith(hi));
  CHECK(!diff.startsWith(hi));
}

TEST(SimpleString, split)
{
  cpputest::SimpleString hi("hello\nworld\nhow\ndo\nyou\ndo\n\n");

  cpputest::SimpleStringCollection collection;
  hi.split("\n", collection);

  LONGS_EQUAL(7, collection.size());
  STRCMP_EQUAL("hello\n", collection[0].asCharString());
  STRCMP_EQUAL("world\n", collection[1].asCharString());
  STRCMP_EQUAL("how\n", collection[2].asCharString());
  STRCMP_EQUAL("do\n", collection[3].asCharString());
  STRCMP_EQUAL("you\n", collection[4].asCharString());
  STRCMP_EQUAL("do\n", collection[5].asCharString());
  STRCMP_EQUAL("\n", collection[6].asCharString());
}

TEST(SimpleString, splitNoTokenOnTheEnd)
{
  cpputest::SimpleString string("Bah Yah oops");
  cpputest::SimpleStringCollection collection;

  string.split(" ", collection);
  LONGS_EQUAL(3, collection.size());
  STRCMP_EQUAL("Bah ", collection[0].asCharString());
  STRCMP_EQUAL("Yah ", collection[1].asCharString());
  STRCMP_EQUAL("oops", collection[2].asCharString());
}

TEST(SimpleString, endsWith)
{
  cpputest::SimpleString str("Hello World");
  CHECK(str.endsWith("World"));
  CHECK(!str.endsWith("Worl"));
  CHECK(!str.endsWith("Hello"));
  cpputest::SimpleString str2("ah");
  CHECK(str2.endsWith("ah"));
  CHECK(!str2.endsWith("baah"));
  cpputest::SimpleString str3("");
  CHECK(!str3.endsWith("baah"));

  cpputest::SimpleString str4("ha ha ha ha");
  CHECK(str4.endsWith("ha"));
}

TEST(SimpleString, replaceCharWithChar)
{
  cpputest::SimpleString str("abcabcabca");
  str.replace('a', 'b');
  STRCMP_EQUAL("bbcbbcbbcb", str.asCharString());
}

TEST(SimpleString, replaceEmptyStringWithEmptyString)
{
  cpputest::SimpleString str;
  str.replace("", "");
  STRCMP_EQUAL("", str.asCharString());
}

TEST(SimpleString, replaceWholeString)
{
  cpputest::SimpleString str("boo");
  str.replace("boo", "");
  STRCMP_EQUAL("", str.asCharString());
}

TEST(SimpleString, replaceStringWithString)
{
  cpputest::SimpleString str("boo baa boo baa boo");
  str.replace("boo", "boohoo");
  STRCMP_EQUAL("boohoo baa boohoo baa boohoo", str.asCharString());
}

TEST(SimpleString, subStringFromEmptyString)
{
  cpputest::SimpleString str("");
  STRCMP_EQUAL("", str.subString(0, 1).asCharString());
}

TEST(SimpleString, subStringFromSmallString)
{
  cpputest::SimpleString str("H");
  STRCMP_EQUAL("H", str.subString(0, 1).asCharString());
}

TEST(SimpleString, subStringFromPos0)
{
  cpputest::SimpleString str("Hello World");
  STRCMP_EQUAL("Hello", str.subString(0, 5).asCharString());
}

TEST(SimpleString, subStringFromPos1)
{
  cpputest::SimpleString str("Hello World");
  STRCMP_EQUAL("ello ", str.subString(1, 5).asCharString());
}

TEST(SimpleString, subStringFromPos5WithAmountLargerThanString)
{
  cpputest::SimpleString str("Hello World");
  STRCMP_EQUAL("World", str.subString(6, 10).asCharString());
}

TEST(SimpleString, subStringFromPos6ToEndOfString)
{
  cpputest::SimpleString str("Hello World");
  STRCMP_EQUAL("World", str.subString(6).asCharString());
}

TEST(SimpleString, subStringBeginPosOutOfBounds)
{
  cpputest::SimpleString str("Hello World");
  STRCMP_EQUAL("", str.subString(13, 5).asCharString());
}

TEST(SimpleString, subStringFromTillNormal)
{
  cpputest::SimpleString str("Hello World");
  STRCMP_EQUAL("Hello", str.subStringFromTill('H', ' ').asCharString());
}

TEST(SimpleString, subStringFromTillOutOfBounds)
{
  cpputest::SimpleString str("Hello World");
  STRCMP_EQUAL("World", str.subStringFromTill('W', '!').asCharString());
}

TEST(SimpleString, subStringFromTillStartDoesntExist)
{
  cpputest::SimpleString str("Hello World");
  STRCMP_EQUAL("", str.subStringFromTill('!', ' ').asCharString());
}

TEST(SimpleString, subStringFromTillWhenTheEndAppearsBeforeTheStart)
{
  cpputest::SimpleString str("Hello World");
  STRCMP_EQUAL("World", str.subStringFromTill('W', 'H').asCharString());
}

TEST(SimpleString, findNormal)
{
  cpputest::SimpleString str("Hello World");
  LONGS_EQUAL(0, str.find('H'));
  LONGS_EQUAL(1, str.find('e'));
  LONGS_EQUAL(cpputest::SimpleString::npos, str.find('!'));
}

TEST(SimpleString, at)
{
  cpputest::SimpleString str("Hello World");
  BYTES_EQUAL('H', str.at(0));
}

TEST(SimpleString, ContainsNull)
{
  cpputest::SimpleString s(nullptr);
  STRCMP_EQUAL("", s.asCharString());
}

TEST(SimpleString, NULLReportsNullString)
{
  STRCMP_EQUAL(
    "(null)",
    cpputest::StringFromOrNull(static_cast<char*>(nullptr)).asCharString());
}

TEST(SimpleString, NULLReportsNullStringPrintable)
{
  STRCMP_EQUAL("(null)",
               cpputest::PrintableStringFromOrNull(static_cast<char*>(nullptr))
                 .asCharString());
}

TEST(SimpleString, Booleans)
{
  cpputest::SimpleString s1(cpputest::StringFrom(true));
  cpputest::SimpleString s2(cpputest::StringFrom(false));
  CHECK(s1 == "true");
  CHECK(s2 == "false");
}

TEST(SimpleString, Pointers)
{
  cpputest::SimpleString s(
    cpputest::StringFrom(reinterpret_cast<void*>(0x1234)));
  STRCMP_EQUAL("0x1234", s.asCharString());
}

TEST(SimpleString, FunctionPointers)
{
  cpputest::SimpleString s(
    cpputest::StringFrom(reinterpret_cast<void (*)()>(0x1234)));
  STRCMP_EQUAL("0x1234", s.asCharString());
}

TEST(SimpleString, Characters)
{
  cpputest::SimpleString s(cpputest::StringFrom('a'));
  STRCMP_EQUAL("a", s.asCharString());
}

TEST(SimpleString, NegativeSignedBytes)
{
  STRCMP_EQUAL(
    "-15", cpputest::StringFrom(static_cast<signed char>(-15)).asCharString());
}

TEST(SimpleString, PositiveSignedBytes)
{
  STRCMP_EQUAL("4", cpputest::StringFrom(4).asCharString());
}

TEST(SimpleString, LongInts)
{
  cpputest::SimpleString s(cpputest::StringFrom(static_cast<long>(1)));
  CHECK(s == "1");
}

TEST(SimpleString, UnsignedLongInts)
{
  cpputest::SimpleString s(cpputest::StringFrom(static_cast<unsigned long>(1)));
  cpputest::SimpleString s2(
    cpputest::StringFrom(static_cast<unsigned long>(1)));
  CHECK(s == s2);
}

TEST(SimpleString, LongLongInts)
{
  cpputest::SimpleString s(cpputest::StringFrom(static_cast<long long>(1)));
  CHECK(s == "1");
}

TEST(SimpleString, UnsignedLongLongInts)
{
  cpputest::SimpleString s(
    cpputest::StringFrom(static_cast<unsigned long long>(1)));
  cpputest::SimpleString s2(
    cpputest::StringFrom(static_cast<unsigned long long>(1)));
  CHECK(s == s2);
}

TEST(SimpleString, Doubles)
{
  cpputest::SimpleString s(cpputest::StringFrom(1.2));
  STRCMP_EQUAL("1.2", s.asCharString());
}

static int
alwaysTrue(double)
{
  return true;
}

TEST(SimpleString, NaN)
{
  UT_PTR_SET(PlatformSpecificIsNan, alwaysTrue);
  cpputest::SimpleString s(cpputest::StringFrom(0.0));
  STRCMP_EQUAL("Nan - Not a number", s.asCharString());
}

TEST(SimpleString, Inf)
{
  UT_PTR_SET(PlatformSpecificIsInf, alwaysTrue);
  cpputest::SimpleString s(cpputest::StringFrom(0.0));
  STRCMP_EQUAL("Inf - Infinity", s.asCharString());
}

TEST(SimpleString, SmallDoubles)
{
  cpputest::SimpleString s(cpputest::StringFrom(1.2e-10));
  STRCMP_CONTAINS("1.2e", s.asCharString());
}

TEST(SimpleString, Sizes)
{
  size_t size = 10;
  STRCMP_EQUAL("10",
               cpputest::StringFrom(static_cast<int>(size)).asCharString());
}

#if CPPUTEST_USE_STD_CPP_LIB

TEST(SimpleString, nullptr_type)
{
  cpputest::SimpleString s(cpputest::StringFrom(nullptr));
  STRCMP_EQUAL("(null)", s.asCharString());
}

#endif

TEST(SimpleString, HexStrings)
{
  STRCMP_EQUAL(
    "f3",
    cpputest::HexStringFrom(static_cast<signed char>(-13)).asCharString());

  cpputest::SimpleString h1 = cpputest::HexStringFrom(0xffffL);
  STRCMP_EQUAL("ffff", h1.asCharString());

  cpputest::SimpleString h15 = cpputest::HexStringFrom(0xffffLL);
  STRCMP_EQUAL("ffff", h15.asCharString());

  cpputest::SimpleString h2 =
    cpputest::HexStringFrom(reinterpret_cast<void*>(0xfffeL));
  STRCMP_EQUAL("fffe", h2.asCharString());

  cpputest::SimpleString h3 =
    cpputest::HexStringFrom(reinterpret_cast<void (*)()>(0xfffdL));
  STRCMP_EQUAL("fffd", h3.asCharString());
}

TEST(SimpleString, StringFromFormat)
{
  cpputest::SimpleString h1 =
    cpputest::StringFromFormat("%s %s! %d", "Hello", "World", 2009);
  STRCMP_EQUAL("Hello World! 2009", h1.asCharString());
}

TEST(SimpleString, StringFromFormatpointer)
{
  // this is not a great test. but %p is odd on mingw and even more odd on
  // Solaris.
  cpputest::SimpleString h1 =
    cpputest::StringFromFormat("%p", reinterpret_cast<void*>(1));
  if (h1.size() == 3)
    STRCMP_EQUAL("0x1", h1.asCharString());
  else if (h1.size() == 8)
    STRCMP_EQUAL("00000001", h1.asCharString());
  else if (h1.size() == 9)
    STRCMP_EQUAL("0000:0001", h1.asCharString());
  else if (h1.size() == 16)
    STRCMP_EQUAL("0000000000000001", h1.asCharString());
  else if (h1.size() == 1)
    STRCMP_EQUAL("1", h1.asCharString());
  else
    FAIL("Off %p behavior");
}

TEST(SimpleString, StringFromFormatLarge)
{
  const char* s =
    "ThisIsAPrettyLargeStringAndIfWeAddThisManyTimesToABufferItWillbeFull";
  cpputest::SimpleString h1 = cpputest::StringFromFormat(
    "%s%s%s%s%s%s%s%s%s%s", s, s, s, s, s, s, s, s, s, s);
  LONGS_EQUAL(10 * static_cast<long>(cpputest::SimpleString(s).size()),
              static_cast<long>(h1.size()));
}

TEST(SimpleString, StringFromConstSimpleString)
{
  STRCMP_EQUAL("bla", StringFrom(cpputest::SimpleString("bla")).asCharString());
}

static int
WrappedUpVSNPrintf(char* buf, size_t n, const char* format, ...)
{
  va_list arguments;
  va_start(arguments, format);

  int result = PlatformSpecificVSNprintf(buf, n, format, arguments);
  va_end(arguments);
  return result;
}

TEST(SimpleString, PlatformSpecificSprintf_fits)
{
  char buf[10];

  int count = WrappedUpVSNPrintf(buf, sizeof(buf), "%s", "12345");
  STRCMP_EQUAL("12345", buf);
  LONGS_EQUAL(5, count);
}

TEST(SimpleString, PlatformSpecificSprintf_doesNotFit)
{
  char buf[10];

  int count = WrappedUpVSNPrintf(buf, sizeof(buf), "%s", "12345678901");
  STRCMP_EQUAL("123456789", buf);
  LONGS_EQUAL(11, count);
}

TEST(SimpleString, PadStringsToSameLengthString1Larger)
{
  cpputest::SimpleString str1("1");
  cpputest::SimpleString str2("222");

  cpputest::SimpleString::padStringsToSameLength(str1, str2, '4');
  STRCMP_EQUAL("441", str1.asCharString());
  STRCMP_EQUAL("222", str2.asCharString());
}

TEST(SimpleString, PadStringsToSameLengthString2Larger)
{
  cpputest::SimpleString str1("    ");
  cpputest::SimpleString str2("");

  cpputest::SimpleString::padStringsToSameLength(str1, str2, ' ');
  STRCMP_EQUAL("    ", str1.asCharString());
  STRCMP_EQUAL("    ", str2.asCharString());
}

TEST(SimpleString, PadStringsToSameLengthWithSameLengthStrings)
{
  cpputest::SimpleString str1("123");
  cpputest::SimpleString str2("123");

  cpputest::SimpleString::padStringsToSameLength(str1, str2, ' ');
  STRCMP_EQUAL("123", str1.asCharString());
  STRCMP_EQUAL("123", str2.asCharString());
}

TEST(SimpleString, NullParameters2)
{
  cpputest::SimpleString* arr = new cpputest::SimpleString[100];
  delete[] arr;
}

TEST(SimpleString, CollectionReadOutOfBoundsReturnsEmptyString)
{
  cpputest::SimpleStringCollection col;
  col.allocate(3);
  STRCMP_EQUAL("", col[3].asCharString());
}

TEST(SimpleString, CollectionWritingToEmptyString)
{
  cpputest::SimpleStringCollection col;
  col.allocate(3);
  col[3] = cpputest::SimpleString("HAH");
  STRCMP_EQUAL("", col[3].asCharString());
}

#if defined(__LP64__) || defined(_LP64) ||                                     \
  (defined(__WORDSIZE) && (__WORDSIZE == 64)) || defined(__x86_64) ||          \
  defined(_WIN64)

TEST(SimpleString, 64BitAddressPrintsCorrectly)
{
  char* p = reinterpret_cast<char*>(0x0012345678901234LL);
  cpputest::SimpleString expected("0x12345678901234");
  cpputest::SimpleString actual = cpputest::StringFrom(static_cast<void*>(p));
  STRCMP_EQUAL(expected.asCharString(), actual.asCharString());
}

#if !defined(_WIN64)

TEST(SimpleString, BracketsFormattedHexStringFromForLongOnDifferentPlatform)
{
  long value = -1;

  STRCMP_EQUAL("(0xffffffffffffffff)",
               cpputest::BracketsFormattedHexStringFrom(value).asCharString());
}

#else

TEST(SimpleString, BracketsFormattedHexStringFromForLongOnDifferentPlatform)
{
  long value = -1;

  STRCMP_EQUAL("(0xffffffff)",
               cpputest::BracketsFormattedHexStringFrom(value).asCharString());
}

#endif
#else
/*
 * This test case cannot pass on 32 bit systems.
 */
IGNORE_TEST(SimpleString, 64BitAddressPrintsCorrectly) {}

TEST(SimpleString, BracketsFormattedHexStringFromForLongOnDifferentPlatform)
{
  long value = -1;

  STRCMP_EQUAL("(0xffffffff)",
               cpputest::BracketsFormattedHexStringFrom(value).asCharString());
}
#endif

TEST(SimpleString, BuildStringFromUnsignedLongInteger)
{
  unsigned long int i = 0xffffffff;

  cpputest::SimpleString result = cpputest::StringFrom(i);
  const char* expected_string = "4294967295";
  CHECK_EQUAL(expected_string, result);
}

TEST(SimpleString, BuildStringFromUnsignedInteger)
{
  unsigned int i = 0xff;

  cpputest::SimpleString result = cpputest::StringFrom(i);
  const char* expected_string = "255";
  CHECK_EQUAL(expected_string, result);
}

#if CPPUTEST_USE_STD_CPP_LIB

TEST(SimpleString, fromStdString)
{
  std::string s("hello");
  cpputest::SimpleString s1(cpputest::StringFrom(s));

  STRCMP_EQUAL("hello", s1.asCharString());
}

TEST(SimpleString, CHECK_EQUAL_unsigned_long)
{
  unsigned long i = 0xffffffffUL;
  CHECK_EQUAL(i, i);
}

TEST(SimpleString, unsigned_long)
{
  unsigned long i = 0xffffffffUL;

  cpputest::SimpleString result = cpputest::StringFrom(i);
  const char* expected_string = "4294967295";
  CHECK_EQUAL(expected_string, result);
}

#endif

TEST(SimpleString, StrCmp)
{
  char empty[] = "";
  char blabla[] = "blabla";
  char bla[] = "bla";
  CHECK(cpputest::SimpleString::StrCmp(empty, empty) == 0);
  CHECK(cpputest::SimpleString::StrCmp(bla, blabla) == -static_cast<int>('b'));
  CHECK(cpputest::SimpleString::StrCmp(blabla, bla) == 'b');
  CHECK(cpputest::SimpleString::StrCmp(bla, empty) == 'b');
  CHECK(cpputest::SimpleString::StrCmp(empty, bla) == -static_cast<int>('b'));
  CHECK(cpputest::SimpleString::StrCmp(bla, bla) == 0);
}

TEST(SimpleString, StrNCmp_equal)
{
  int result = cpputest::SimpleString::StrNCmp("teststring", "tests", 5);
  LONGS_EQUAL(0, result);
}

TEST(SimpleString, StrNCmp_should_always_return_0_when_n_is_0)
{
  int result = cpputest::SimpleString::StrNCmp("a", "b", 0);
  LONGS_EQUAL(0, result);
}

TEST(SimpleString, StrNCmp_s1_smaller)
{
  int result = cpputest::SimpleString::StrNCmp("testing", "tests", 7);
  LONGS_EQUAL('i' - 's', result);
}

TEST(SimpleString, StrNCmp_s1_larger)
{
  int result = cpputest::SimpleString::StrNCmp("teststring", "tester", 7);
  LONGS_EQUAL('s' - 'e', result);
}

TEST(SimpleString, StrNCmp_n_too_large)
{
  int result = cpputest::SimpleString::StrNCmp("teststring", "teststring", 20);
  LONGS_EQUAL(0, result);
}

TEST(SimpleString, StrNCmp_s1_empty)
{
  int result = cpputest::SimpleString::StrNCmp("", "foo", 2);
  LONGS_EQUAL(0 - 'f', result);
}

TEST(SimpleString, StrNCmp_s2_empty)
{
  int result = cpputest::SimpleString::StrNCmp("foo", "", 2);
  LONGS_EQUAL('f', result);
}

TEST(SimpleString, StrNCmp_s1_and_s2_empty)
{
  int result = cpputest::SimpleString::StrNCmp("", "", 2);
  LONGS_EQUAL(0, result);
}

TEST(SimpleString, AtoI)
{
  char max_short_str[] = "32767";
  char min_short_str[] = "-32768";

  CHECK(12345 == cpputest::SimpleString::AtoI("012345"));
  CHECK(6789 == cpputest::SimpleString::AtoI("6789"));
  CHECK(12345 == cpputest::SimpleString::AtoI("12345/"));
  CHECK(12345 == cpputest::SimpleString::AtoI("12345:"));
  CHECK(-12345 == cpputest::SimpleString::AtoI("-12345"));
  CHECK(123 == cpputest::SimpleString::AtoI("\t \r\n123"));
  CHECK(123 == cpputest::SimpleString::AtoI("123-foo"));
  CHECK(0 == cpputest::SimpleString::AtoI("-foo"));
  CHECK(-32768 == cpputest::SimpleString::AtoI(min_short_str));
  CHECK(32767 == cpputest::SimpleString::AtoI(max_short_str));
}

TEST(SimpleString, AtoU)
{
  char max_short_str[] = "65535";
  CHECK(12345 == cpputest::SimpleString::AtoU("012345"));
  CHECK(6789 == cpputest::SimpleString::AtoU("6789"));
  CHECK(12345 == cpputest::SimpleString::AtoU("12345/"));
  CHECK(12345 == cpputest::SimpleString::AtoU("12345:"));
  CHECK(123 == cpputest::SimpleString::AtoU("\t \r\n123"));
  CHECK(123 == cpputest::SimpleString::AtoU("123-foo"));
  CHECK(65535 == cpputest::SimpleString::AtoU(max_short_str));
  CHECK(0 == cpputest::SimpleString::AtoU("foo"));
  CHECK(0 == cpputest::SimpleString::AtoU("-foo"));
  CHECK(0 == cpputest::SimpleString::AtoU("+1"));
  CHECK(0 == cpputest::SimpleString::AtoU("-1"));
  CHECK(0 == cpputest::SimpleString::AtoU("0"));
}

TEST(SimpleString, Binary)
{
  const unsigned char value[] = { 0x00, 0x01, 0x2A, 0xFF };
  const char expectedString[] = "00 01 2A FF";

  STRCMP_EQUAL(expectedString,
               cpputest::StringFromBinary(value, sizeof(value)).asCharString());
  STRCMP_EQUAL(
    expectedString,
    cpputest::StringFromBinaryOrNull(value, sizeof(value)).asCharString());
  STRCMP_EQUAL("", cpputest::StringFromBinary(value, 0).asCharString());
  STRCMP_EQUAL("(null)",
               cpputest::StringFromBinaryOrNull(nullptr, 0).asCharString());
}

TEST(SimpleString, BinaryWithSize)
{
  const unsigned char value[] = { 0x12, 0xFE, 0xA1 };
  const char expectedString[] = "Size = 3 | HexContents = 12 FE A1";

  STRCMP_EQUAL(
    expectedString,
    cpputest::StringFromBinaryWithSize(value, sizeof(value)).asCharString());
  STRCMP_EQUAL(expectedString,
               cpputest::StringFromBinaryWithSizeOrNull(value, sizeof(value))
                 .asCharString());
  STRCMP_EQUAL("Size = 0 | HexContents = ",
               cpputest::StringFromBinaryWithSize(value, 0).asCharString());
  STRCMP_EQUAL(
    "(null)",
    cpputest::StringFromBinaryWithSizeOrNull(nullptr, 0).asCharString());
}

TEST(SimpleString, BinaryWithSizeLargerThan128)
{
  unsigned char value[129] = {};
  value[127] = 0x00;
  value[128] = 0xff;

  STRCMP_CONTAINS(
    "00 ...",
    cpputest::StringFromBinaryWithSize(value, sizeof(value)).asCharString());
}

TEST(SimpleString, MemCmp)
{
  unsigned char smaller[] = { 0x00, 0x01, 0x2A, 0xFF };
  unsigned char greater[] = { 0x00, 0x01, 0xFF, 0xFF };

  LONGS_EQUAL(
    0, cpputest::SimpleString::MemCmp(smaller, smaller, sizeof(smaller)));
  CHECK(cpputest::SimpleString::MemCmp(smaller, greater, sizeof(smaller)) < 0);
  CHECK(cpputest::SimpleString::MemCmp(greater, smaller, sizeof(smaller)) > 0);
  LONGS_EQUAL(0, cpputest::SimpleString::MemCmp(nullptr, nullptr, 0));
}

TEST(SimpleString, MemCmpFirstLastNotMatching)
{
  unsigned char base[] = { 0x00, 0x01, 0x2A, 0xFF };
  unsigned char firstNotMatching[] = { 0x01, 0x01, 0x2A, 0xFF };
  unsigned char lastNotMatching[] = { 0x00, 0x01, 0x2A, 0x00 };
  CHECK(0 !=
        cpputest::SimpleString::MemCmp(base, firstNotMatching, sizeof(base)));
  CHECK(0 !=
        cpputest::SimpleString::MemCmp(base, lastNotMatching, sizeof(base)));
}

#if (CHAR_BIT == 16)
TEST(SimpleString, MaskedBitsChar)
{
  STRCMP_EQUAL("xxxxxxxx xxxxxxxx",
               cpputest::StringFromMaskedBits(0x00, 0x00, 1).asCharString());
  STRCMP_EQUAL("xxxxxxxx 00000000",
               cpputest::StringFromMaskedBits(0x00, 0xFF, 1).asCharString());
  STRCMP_EQUAL("xxxxxxxx 11111111",
               cpputest::StringFromMaskedBits(0xFF, 0xFF, 1).asCharString());
  STRCMP_EQUAL("xxxxxxxx 1xxxxxxx",
               cpputest::StringFromMaskedBits(0x80, 0x80, 1).asCharString());
  STRCMP_EQUAL("xxxxxxxx xxxxxxx1",
               cpputest::StringFromMaskedBits(0x01, 0x01, 1).asCharString());
  STRCMP_EQUAL("xxxxxxxx 11xx11xx",
               cpputest::StringFromMaskedBits(0xFF, 0xCC, 1).asCharString());
}
#elif (CHAR_BIT == 8)
TEST(SimpleString, MaskedBitsChar)
{
  STRCMP_EQUAL("xxxxxxxx",
               cpputest::StringFromMaskedBits(0x00, 0x00, 1).asCharString());
  STRCMP_EQUAL("00000000",
               cpputest::StringFromMaskedBits(0x00, 0xFF, 1).asCharString());
  STRCMP_EQUAL("11111111",
               cpputest::StringFromMaskedBits(0xFF, 0xFF, 1).asCharString());
  STRCMP_EQUAL("1xxxxxxx",
               cpputest::StringFromMaskedBits(0x80, 0x80, 1).asCharString());
  STRCMP_EQUAL("xxxxxxx1",
               cpputest::StringFromMaskedBits(0x01, 0x01, 1).asCharString());
  STRCMP_EQUAL("11xx11xx",
               cpputest::StringFromMaskedBits(0xFF, 0xCC, 1).asCharString());
}
#endif

TEST(SimpleString, MaskedBits16Bit)
{
  STRCMP_EQUAL("xxxxxxxx xxxxxxxx",
               cpputest::StringFromMaskedBits(0x0000, 0x0000, 2 * 8 / CHAR_BIT)
                 .asCharString());
  STRCMP_EQUAL("00000000 00000000",
               cpputest::StringFromMaskedBits(0x0000, 0xFFFF, 2 * 8 / CHAR_BIT)
                 .asCharString());
  STRCMP_EQUAL("11111111 11111111",
               cpputest::StringFromMaskedBits(0xFFFF, 0xFFFF, 2 * 8 / CHAR_BIT)
                 .asCharString());
  STRCMP_EQUAL("1xxxxxxx xxxxxxxx",
               cpputest::StringFromMaskedBits(0x8000, 0x8000, 2 * 8 / CHAR_BIT)
                 .asCharString());
  STRCMP_EQUAL("xxxxxxxx xxxxxxx1",
               cpputest::StringFromMaskedBits(0x0001, 0x0001, 2 * 8 / CHAR_BIT)
                 .asCharString());
  STRCMP_EQUAL("11xx11xx 11xx11xx",
               cpputest::StringFromMaskedBits(0xFFFF, 0xCCCC, 2 * 8 / CHAR_BIT)
                 .asCharString());
}

TEST(SimpleString, MaskedBits32Bit)
{
  STRCMP_EQUAL(
    "xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx",
    cpputest::StringFromMaskedBits(0x00000000, 0x00000000, 4 * 8 / CHAR_BIT)
      .asCharString());
  STRCMP_EQUAL(
    "00000000 00000000 00000000 00000000",
    cpputest::StringFromMaskedBits(0x00000000, 0xFFFFFFFF, 4 * 8 / CHAR_BIT)
      .asCharString());
  STRCMP_EQUAL(
    "11111111 11111111 11111111 11111111",
    cpputest::StringFromMaskedBits(0xFFFFFFFF, 0xFFFFFFFF, 4 * 8 / CHAR_BIT)
      .asCharString());
  STRCMP_EQUAL(
    "1xxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx",
    cpputest::StringFromMaskedBits(0x80000000, 0x80000000, 4 * 8 / CHAR_BIT)
      .asCharString());
  STRCMP_EQUAL(
    "xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxx1",
    cpputest::StringFromMaskedBits(0x00000001, 0x00000001, 4 * 8 / CHAR_BIT)
      .asCharString());
  STRCMP_EQUAL(
    "11xx11xx 11xx11xx 11xx11xx 11xx11xx",
    cpputest::StringFromMaskedBits(0xFFFFFFFF, 0xCCCCCCCC, 4 * 8 / CHAR_BIT)
      .asCharString());
}

TEST(SimpleString, StringFromOrdinalNumberOnes)
{
  STRCMP_EQUAL("2nd", cpputest::StringFromOrdinalNumber(2).asCharString());
  STRCMP_EQUAL("3rd", cpputest::StringFromOrdinalNumber(3).asCharString());
  STRCMP_EQUAL("4th", cpputest::StringFromOrdinalNumber(4).asCharString());
  STRCMP_EQUAL("5th", cpputest::StringFromOrdinalNumber(5).asCharString());
  STRCMP_EQUAL("6th", cpputest::StringFromOrdinalNumber(6).asCharString());
  STRCMP_EQUAL("7th", cpputest::StringFromOrdinalNumber(7).asCharString());
}

TEST(SimpleString, StringFromOrdinalNumberTens)
{
  STRCMP_EQUAL("10th", cpputest::StringFromOrdinalNumber(10).asCharString());
  STRCMP_EQUAL("11th", cpputest::StringFromOrdinalNumber(11).asCharString());
  STRCMP_EQUAL("12th", cpputest::StringFromOrdinalNumber(12).asCharString());
  STRCMP_EQUAL("13th", cpputest::StringFromOrdinalNumber(13).asCharString());
  STRCMP_EQUAL("14th", cpputest::StringFromOrdinalNumber(14).asCharString());
  STRCMP_EQUAL("18th", cpputest::StringFromOrdinalNumber(18).asCharString());
}

TEST(SimpleString, StringFromOrdinalNumberOthers)
{
  STRCMP_EQUAL("21st", cpputest::StringFromOrdinalNumber(21).asCharString());
  STRCMP_EQUAL("22nd", cpputest::StringFromOrdinalNumber(22).asCharString());
  STRCMP_EQUAL("23rd", cpputest::StringFromOrdinalNumber(23).asCharString());
  STRCMP_EQUAL("24th", cpputest::StringFromOrdinalNumber(24).asCharString());
  STRCMP_EQUAL("32nd", cpputest::StringFromOrdinalNumber(32).asCharString());
  STRCMP_EQUAL("100th", cpputest::StringFromOrdinalNumber(100).asCharString());
  STRCMP_EQUAL("101st", cpputest::StringFromOrdinalNumber(101).asCharString());
}

TEST(SimpleString, BracketsFormattedHexStringFromForSignedChar)
{
  signed char value = 'c';

  STRCMP_EQUAL("(0x63)",
               cpputest::BracketsFormattedHexStringFrom(value).asCharString());
}

TEST(SimpleString, BracketsFormattedHexStringFromForUnsignedInt)
{
  unsigned int value = 1;

  STRCMP_EQUAL("(0x1)",
               cpputest::BracketsFormattedHexStringFrom(value).asCharString());
}

TEST(SimpleString, BracketsFormattedHexStringFromForUnsignedLong)
{
  unsigned long value = 1;

  STRCMP_EQUAL("(0x1)",
               cpputest::BracketsFormattedHexStringFrom(value).asCharString());
}

#if (INT_MAX == 0x7fff)
TEST(SimpleString, BracketsFormattedHexStringFromForInt)
{
  int value = -1;

  STRCMP_EQUAL("(0xffff)",
               BracketsFormattedHexStringFrom(value).asCharString());
}
#else
TEST(SimpleString, BracketsFormattedHexStringFromForInt)
{
  int value = -1;
  STRCMP_EQUAL("(0xffffffff)",
               cpputest::BracketsFormattedHexStringFrom(value).asCharString());
}
#endif

TEST(SimpleString, BracketsFormattedHexStringFromForLong)
{
  long value = 1;

  STRCMP_EQUAL("(0x1)",
               cpputest::BracketsFormattedHexStringFrom(value).asCharString());
}
TEST(SimpleString, BracketsFormattedHexStringFromForLongLong)
{
  long long value = 1;

  STRCMP_EQUAL("(0x1)",
               cpputest::BracketsFormattedHexStringFrom(value).asCharString());
}
TEST(SimpleString, BracketsFormattedHexStringFromForULongLong)
{
  unsigned long long value = 1;

  STRCMP_EQUAL("(0x1)",
               cpputest::BracketsFormattedHexStringFrom(value).asCharString());
}
