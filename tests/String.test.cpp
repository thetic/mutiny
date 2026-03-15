#include "CppUTest/String.hpp"

#include "CppUTest/CppUTest.hpp"
#include "CppUTest/math.hpp"

#include <limits.h>
#include <stdio.h>

namespace {

int
always_true(double)
{
  return true;
}

int
wrapped_up_vsn_printf(char* buf, size_t n, const char* format, ...)
{
  va_list arguments;
  va_start(arguments, format);

  int result = vsnprintf(buf, n, format, arguments);
  va_end(arguments);
  return result;
}

} // namespace

TEST_GROUP(String)
{};

TEST(String, CreateSequence)
{
  cpputest::String expected("hellohello");
  cpputest::String actual("hello", 2);

  CHECK_EQUAL(expected, actual);
}

TEST(String, CreateSequenceOfZero)
{
  cpputest::String expected("");
  cpputest::String actual("hello", 0);

  CHECK_EQUAL(expected, actual);
}

TEST(String, Copy)
{
  cpputest::String s1("hello");
  cpputest::String s2(s1);

  CHECK_EQUAL(s1, s2);
}

TEST(String, Assignment)
{
  cpputest::String s1("hello");
  cpputest::String s2("goodbye");

  s2 = s1;

  CHECK_EQUAL(s1, s2);
}

TEST(String, Equality)
{
  cpputest::String s1("hello");
  cpputest::String s2("hello");

  CHECK(s1 == s2);
}

TEST(String, InEquality)
{
  cpputest::String s1("hello");
  cpputest::String s2("goodbye");

  CHECK(s1 != s2);
}

TEST(String, c_str)
{
  cpputest::String s1("hello");

  STRCMP_EQUAL("hello", s1.c_str());
}

TEST(String, Size)
{
  cpputest::String s1("hello!");

  LONGS_EQUAL(6, s1.size());
}

TEST(String, Addition)
{
  cpputest::String s1("hello!");
  cpputest::String s2("goodbye!");
  cpputest::String s3("hello!goodbye!");
  cpputest::String s4;
  s4 = s1 + s2;

  CHECK_EQUAL(s3, s4);
}

TEST(String, Concatenation)
{
  cpputest::String s1("hello!");
  cpputest::String s2("goodbye!");
  cpputest::String s3("hello!goodbye!");
  cpputest::String s4;
  s4 += s1;
  s4 += s2;

  CHECK_EQUAL(s3, s4);

  cpputest::String s5("hello!goodbye!hello!goodbye!");
  s4 += s4;

  CHECK_EQUAL(s5, s4);
}

TEST(String, stringContains)
{
  cpputest::String s("hello!");
  cpputest::String empty("");
  cpputest::String beginning("hello");
  cpputest::String end("lo!");
  cpputest::String mid("l");
  cpputest::String not_part_of_string("xxxx");

  CHECK(string_contains(s, empty));
  CHECK(string_contains(s, beginning));
  CHECK(string_contains(s, end));
  CHECK(string_contains(s, mid));
  CHECK(!string_contains(s, not_part_of_string));

  CHECK(string_contains(empty, empty));
  CHECK(!string_contains(empty, s));
}

TEST(String, startsWith)
{
  cpputest::String hi("Hi you!");
  cpputest::String part("Hi");
  cpputest::String diff("Hrrm Hi you! ffdsfd");
  CHECK(string_starts_with(hi, part));
  CHECK(!string_starts_with(part, hi));
  CHECK(!string_starts_with(diff, hi));
}

TEST(String, stringEndsWith)
{
  cpputest::String str("Hello World");
  CHECK(string_ends_with(str, "World"));
  CHECK(!string_ends_with(str, "Worl"));
  CHECK(!string_ends_with(str, "Hello"));
  cpputest::String str2("ah");
  CHECK(string_ends_with(str2, "ah"));
  CHECK(!string_ends_with(str2, "baah"));
  cpputest::String str3("");
  CHECK(!string_ends_with(str3, "baah"));

  cpputest::String str4("ha ha ha ha");
  CHECK(string_ends_with(str4, "ha"));
}

TEST(String, replaceCharWithChar)
{
  cpputest::String str("abcabcabca");
  str.replace('a', 'b');
  STRCMP_EQUAL("bbcbbcbbcb", str.c_str());
}

TEST(String, replaceEmptyStringWithEmptyString)
{
  cpputest::String str;
  str.replace("", "");
  STRCMP_EQUAL("", str.c_str());
}

TEST(String, replaceWholeString)
{
  cpputest::String str("boo");
  str.replace("boo", "");
  STRCMP_EQUAL("", str.c_str());
}

TEST(String, replaceStringWithString)
{
  cpputest::String str("boo baa boo baa boo");
  str.replace("boo", "boohoo");
  STRCMP_EQUAL("boohoo baa boohoo baa boohoo", str.c_str());
}

TEST(String, subStringFromEmptyString)
{
  cpputest::String str("");
  STRCMP_EQUAL("", str.substr(0, 1).c_str());
}

TEST(String, subStringFromSmallString)
{
  cpputest::String str("H");
  STRCMP_EQUAL("H", str.substr(0, 1).c_str());
}

TEST(String, subStringFromPos0)
{
  cpputest::String str("Hello World");
  STRCMP_EQUAL("Hello", str.substr(0, 5).c_str());
}

TEST(String, subStringFromPos1)
{
  cpputest::String str("Hello World");
  STRCMP_EQUAL("ello ", str.substr(1, 5).c_str());
}

TEST(String, subStringFromPos5WithAmountLargerThanString)
{
  cpputest::String str("Hello World");
  STRCMP_EQUAL("World", str.substr(6, 10).c_str());
}

TEST(String, subStringFromPos6ToEndOfString)
{
  cpputest::String str("Hello World");
  STRCMP_EQUAL("World", str.substr(6).c_str());
}

TEST(String, subStringBeginPosOutOfBounds)
{
  cpputest::String str("Hello World");
  STRCMP_EQUAL("", str.substr(13, 5).c_str());
}

TEST(String, subStringFromTillNormal)
{
  cpputest::String str("Hello World");
  STRCMP_EQUAL("Hello", str.sub_string_from_till('H', ' ').c_str());
}

TEST(String, subStringFromTillOutOfBounds)
{
  cpputest::String str("Hello World");
  STRCMP_EQUAL("World", str.sub_string_from_till('W', '!').c_str());
}

TEST(String, subStringFromTillStartDoesntExist)
{
  cpputest::String str("Hello World");
  STRCMP_EQUAL("", str.sub_string_from_till('!', ' ').c_str());
}

TEST(String, subStringFromTillWhenTheEndAppearsBeforeTheStart)
{
  cpputest::String str("Hello World");
  STRCMP_EQUAL("World", str.sub_string_from_till('W', 'H').c_str());
}

TEST(String, findNormal)
{
  cpputest::String str("Hello World");
  LONGS_EQUAL(0, str.find('H'));
  LONGS_EQUAL(1, str.find('e'));
  LONGS_EQUAL(cpputest::String::npos, str.find('!'));
}

TEST(String, index)
{
  cpputest::String str("Hello World");
  BYTES_EQUAL('H', str[0]);
}

TEST(String, ContainsNull)
{
  cpputest::String s(nullptr);
  STRCMP_EQUAL("", s.c_str());
}

TEST(String, NULLReportsNullString)
{
  STRCMP_EQUAL("(null)",
      cpputest::string_from_or_null(static_cast<char*>(nullptr)).c_str());
}

TEST(String, Booleans)
{
  cpputest::String s1(cpputest::string_from(true));
  cpputest::String s2(cpputest::string_from(false));
  CHECK(s1 == "true");
  CHECK(s2 == "false");
}

TEST(String, Pointers)
{
  cpputest::String s(cpputest::string_from(reinterpret_cast<void*>(0x1234)));
  STRCMP_EQUAL("0x1234", s.c_str());
}

TEST(String, FunctionPointers)
{
  cpputest::String s(
      cpputest::string_from(reinterpret_cast<void (*)()>(0x1234)));
  STRCMP_EQUAL("0x1234", s.c_str());
}

TEST(String, Characters)
{
  cpputest::String s(cpputest::string_from('a'));
  STRCMP_EQUAL("a", s.c_str());
}

TEST(String, NegativeSignedBytes)
{
  STRCMP_EQUAL(
      "-15", cpputest::string_from(static_cast<signed char>(-15)).c_str());
}

TEST(String, PositiveSignedBytes)
{
  STRCMP_EQUAL("4", cpputest::string_from(4).c_str());
}

TEST(String, LongInts)
{
  cpputest::String s(cpputest::string_from(static_cast<long>(1)));
  CHECK(s == "1");
}

TEST(String, UnsignedLongInts)
{
  cpputest::String s(cpputest::string_from(static_cast<unsigned long>(1)));
  cpputest::String s2(cpputest::string_from(static_cast<unsigned long>(1)));
  CHECK(s == s2);
}

TEST(String, LongLongInts)
{
  cpputest::String s(cpputest::string_from(static_cast<long long>(1)));
  CHECK(s == "1");
}

TEST(String, UnsignedLongLongInts)
{
  cpputest::String s(cpputest::string_from(static_cast<unsigned long long>(1)));
  cpputest::String s2(
      cpputest::string_from(static_cast<unsigned long long>(1)));
  CHECK(s == s2);
}

TEST(String, Doubles)
{
  cpputest::String s(cpputest::string_from(1.2));
  STRCMP_EQUAL("1.2", s.c_str());
}

TEST(String, NaN)
{
  UT_PTR_SET(cpputest::is_nan, always_true);
  cpputest::String s(cpputest::string_from(0.0));
  STRCMP_EQUAL("Nan - Not a number", s.c_str());
}

TEST(String, Inf)
{
  UT_PTR_SET(cpputest::is_inf, always_true);
  cpputest::String s(cpputest::string_from(0.0));
  STRCMP_EQUAL("Inf - Infinity", s.c_str());
}

TEST(String, SmallDoubles)
{
  cpputest::String s(cpputest::string_from(1.2e-10));
  STRCMP_CONTAINS("1.2e", s.c_str());
}

TEST(String, Sizes)
{
  size_t size = 10;
  STRCMP_EQUAL("10", cpputest::string_from(static_cast<int>(size)).c_str());
}

#if CPPUTEST_USE_STD_CPP_LIB

TEST(String, nullptr_type)
{
  cpputest::String s(cpputest::string_from(nullptr));
  STRCMP_EQUAL("(null)", s.c_str());
}

#endif

TEST(String, HexStrings)
{
  STRCMP_EQUAL(
      "f3", cpputest::hex_string_from(static_cast<signed char>(-13)).c_str());

  cpputest::String h1 = cpputest::hex_string_from(0xffffL);
  STRCMP_EQUAL("ffff", h1.c_str());

  cpputest::String h15 = cpputest::hex_string_from(0xffffLL);
  STRCMP_EQUAL("ffff", h15.c_str());

  cpputest::String h2 =
      cpputest::hex_string_from(reinterpret_cast<void*>(0xfffeL));
  STRCMP_EQUAL("fffe", h2.c_str());

  cpputest::String h3 =
      cpputest::hex_string_from(reinterpret_cast<void (*)()>(0xfffdL));
  STRCMP_EQUAL("fffd", h3.c_str());
}

TEST(String, StringFromFormat)
{
  cpputest::String h1 =
      cpputest::string_from_format("%s %s! %d", "Hello", "World", 2009);
  STRCMP_EQUAL("Hello World! 2009", h1.c_str());
}

TEST(String, StringFromFormatpointer)
{
  // this is not a great test. but %p is odd on mingw and even more odd on
  // Solaris.
  cpputest::String h1 =
      cpputest::string_from_format("%p", reinterpret_cast<void*>(1));
  if (h1.size() == 3)
    STRCMP_EQUAL("0x1", h1.c_str());
  else if (h1.size() == 8)
    STRCMP_EQUAL("00000001", h1.c_str());
  else if (h1.size() == 9)
    STRCMP_EQUAL("0000:0001", h1.c_str());
  else if (h1.size() == 16)
    STRCMP_EQUAL("0000000000000001", h1.c_str());
  else if (h1.size() == 1)
    STRCMP_EQUAL("1", h1.c_str());
  else
    FAIL("Off %p behavior");
}

TEST(String, StringFromFormatLarge)
{
  const char* s =
      "ThisIsAPrettyLargeStringAndIfWeAddThisManyTimesToABufferItWillbeFull";
  cpputest::String h1 = cpputest::string_from_format(
      "%s%s%s%s%s%s%s%s%s%s", s, s, s, s, s, s, s, s, s, s);
  LONGS_EQUAL(10 * static_cast<long>(cpputest::String(s).size()),
      static_cast<long>(h1.size()));
}

TEST(String, StringFromConstString)
{
  STRCMP_EQUAL("bla", string_from(cpputest::String("bla")).c_str());
}

TEST(String, PlatformSpecificSprintf_fits)
{
  char buf[10];

  int count = wrapped_up_vsn_printf(buf, sizeof(buf), "%s", "12345");
  STRCMP_EQUAL("12345", buf);
  LONGS_EQUAL(5, count);
}

TEST(String, PlatformSpecificSprintf_doesNotFit)
{
  char buf[10];

  int count = wrapped_up_vsn_printf(buf, sizeof(buf), "%s", "12345678901");
  STRCMP_EQUAL("123456789", buf);
  LONGS_EQUAL(11, count);
}

TEST(String, PadStringsToSameLengthString1Larger)
{
  cpputest::String str1("1");
  cpputest::String str2("222");

  pad_strings_to_same_length(str1, str2, '4');
  STRCMP_EQUAL("441", str1.c_str());
  STRCMP_EQUAL("222", str2.c_str());
}

TEST(String, PadStringsToSameLengthString2Larger)
{
  cpputest::String str1("    ");
  cpputest::String str2("");

  pad_strings_to_same_length(str1, str2, ' ');
  STRCMP_EQUAL("    ", str1.c_str());
  STRCMP_EQUAL("    ", str2.c_str());
}

TEST(String, PadStringsToSameLengthWithSameLengthStrings)
{
  cpputest::String str1("123");
  cpputest::String str2("123");

  pad_strings_to_same_length(str1, str2, ' ');
  STRCMP_EQUAL("123", str1.c_str());
  STRCMP_EQUAL("123", str2.c_str());
}

TEST(String, NullParameters2)
{
  auto* arr = new cpputest::String[100];
  delete[] arr;
}

#if defined(__LP64__) || defined(_LP64) ||                                     \
    (defined(__WORDSIZE) && (__WORDSIZE == 64)) || defined(__x86_64) ||        \
    defined(_WIN64)

TEST(String, 64BitAddressPrintsCorrectly)
{
  char* p = reinterpret_cast<char*>(0x0012345678901234LL);
  cpputest::String expected("0x12345678901234");
  cpputest::String actual = cpputest::string_from(static_cast<void*>(p));
  STRCMP_EQUAL(expected.c_str(), actual.c_str());
}

#if !defined(_WIN64)

TEST(String, BracketsFormattedHexStringFromForLongOnDifferentPlatform)
{
  long value = -1;

  STRCMP_EQUAL("(0xffffffffffffffff)",
      cpputest::brackets_formatted_hex_string_from(value).c_str());
}

#else

TEST(String, BracketsFormattedHexStringFromForLongOnDifferentPlatform)
{
  long value = -1;

  STRCMP_EQUAL(
      "(0xffffffff)", cpputest::BracketsFormattedHexStringFrom(value).c_str());
}

#endif
#else
/*
 * This test case cannot pass on 32 bit systems.
 */
IGNORE_TEST(String, 64BitAddressPrintsCorrectly) {}

TEST(String, BracketsFormattedHexStringFromForLongOnDifferentPlatform)
{
  long value = -1;

  STRCMP_EQUAL(
      "(0xffffffff)", cpputest::BracketsFormattedHexStringFrom(value).c_str());
}
#endif

TEST(String, BuildStringFromUnsignedLongInteger)
{
  unsigned long int i = 0xffffffff;

  cpputest::String result = cpputest::string_from(i);
  const char* expected_string = "4294967295";
  CHECK_EQUAL(expected_string, result);
}

TEST(String, BuildStringFromUnsignedInteger)
{
  unsigned int i = 0xff;

  cpputest::String result = cpputest::string_from(i);
  const char* expected_string = "255";
  CHECK_EQUAL(expected_string, result);
}

#if CPPUTEST_USE_STD_CPP_LIB

TEST(String, fromStdString)
{
  std::string s("hello");
  cpputest::String s1(cpputest::string_from(s));

  STRCMP_EQUAL("hello", s1.c_str());
}

TEST(
String, CHECK_EQUAL_unsigned_long)
{
  unsigned long i = 0xffffffffUL;
  CHECK_EQUAL(i, i);
}

TEST(
String, unsigned_long)
{
  unsigned long i = 0xffffffffUL;

  cpputest::String result = cpputest::string_from(i);
  const char* expected_string = "4294967295";
  CHECK_EQUAL(expected_string, result);
}

#endif

TEST(String, StrCmp)
{
  char empty[] = "";
  char blabla[] = "blabla";
  char bla[] = "bla";
  CHECK(cpputest::str_cmp(empty, empty) == 0);
  CHECK(cpputest::str_cmp(bla, blabla) == -static_cast<int>('b'));
  CHECK(cpputest::str_cmp(blabla, bla) == 'b');
  CHECK(cpputest::str_cmp(bla, empty) == 'b');
  CHECK(cpputest::str_cmp(empty, bla) == -static_cast<int>('b'));
  CHECK(cpputest::str_cmp(bla, bla) == 0);
}

TEST(String, StrNCmp_equal)
{
  int result = cpputest::str_n_cmp("teststring", "tests", 5);
  LONGS_EQUAL(0, result);
}

TEST(String, StrNCmp_should_always_return_0_when_n_is_0)
{
  int result = cpputest::str_n_cmp("a", "b", 0);
  LONGS_EQUAL(0, result);
}

TEST(String, StrNCmp_s1_smaller)
{
  int result = cpputest::str_n_cmp("testing", "tests", 7);
  LONGS_EQUAL('i' - 's', result);
}

TEST(String, StrNCmp_s1_larger)
{
  int result = cpputest::str_n_cmp("teststring", "tester", 7);
  LONGS_EQUAL('s' - 'e', result);
}

TEST(String, StrNCmp_n_too_large)
{
  int result = cpputest::str_n_cmp("teststring", "teststring", 20);
  LONGS_EQUAL(0, result);
}

TEST(String, StrNCmp_s1_empty)
{
  int result = cpputest::str_n_cmp("", "foo", 2);
  LONGS_EQUAL(0 - 'f', result);
}

TEST(String, StrNCmp_s2_empty)
{
  int result = cpputest::str_n_cmp("foo", "", 2);
  LONGS_EQUAL('f', result);
}

TEST(String, StrNCmp_s1_and_s2_empty)
{
  int result = cpputest::str_n_cmp("", "", 2);
  LONGS_EQUAL(0, result);
}

TEST(String, AtoI)
{
  char max_short_str[] = "32767";
  char min_short_str[] = "-32768";

  CHECK(12345 == cpputest::ato_i("012345"));
  CHECK(6789 == cpputest::ato_i("6789"));
  CHECK(12345 == cpputest::ato_i("12345/"));
  CHECK(12345 == cpputest::ato_i("12345:"));
  CHECK(-12345 == cpputest::ato_i("-12345"));
  CHECK(123 == cpputest::ato_i("\t \r\n123"));
  CHECK(123 == cpputest::ato_i("123-foo"));
  CHECK(0 == cpputest::ato_i("-foo"));
  CHECK(-32768 == cpputest::ato_i(min_short_str));
  CHECK(32767 == cpputest::ato_i(max_short_str));
}

TEST(String, AtoU)
{
  char max_short_str[] = "65535";
  CHECK(12345 == cpputest::ato_u("012345"));
  CHECK(6789 == cpputest::ato_u("6789"));
  CHECK(12345 == cpputest::ato_u("12345/"));
  CHECK(12345 == cpputest::ato_u("12345:"));
  CHECK(123 == cpputest::ato_u("\t \r\n123"));
  CHECK(123 == cpputest::ato_u("123-foo"));
  CHECK(65535 == cpputest::ato_u(max_short_str));
  CHECK(0 == cpputest::ato_u("foo"));
  CHECK(0 == cpputest::ato_u("-foo"));
  CHECK(0 == cpputest::ato_u("+1"));
  CHECK(0 == cpputest::ato_u("-1"));
  CHECK(0 == cpputest::ato_u("0"));
}

TEST(String, Binary)
{
  const unsigned char value[] = { 0x00, 0x01, 0x2A, 0xFF };
  const char expected_string[] = "00 01 2A FF";

  STRCMP_EQUAL(expected_string,
      cpputest::string_from_binary(value, sizeof(value)).c_str());
  STRCMP_EQUAL(expected_string,
      cpputest::string_from_binary_or_null(value, sizeof(value)).c_str());
  STRCMP_EQUAL("", cpputest::string_from_binary(value, 0).c_str());
  STRCMP_EQUAL(
      "(null)", cpputest::string_from_binary_or_null(nullptr, 0).c_str());
}

TEST(String, BinaryWithSize)
{
  const unsigned char value[] = { 0x12, 0xFE, 0xA1 };
  const char expected_string[] = "Size = 3 | HexContents = 12 FE A1";

  STRCMP_EQUAL(expected_string,
      cpputest::string_from_binary_with_size(value, sizeof(value)).c_str());
  STRCMP_EQUAL(expected_string,
      cpputest::string_from_binary_with_size_or_null(value, sizeof(value))
          .c_str());
  STRCMP_EQUAL("Size = 0 | HexContents = ",
      cpputest::string_from_binary_with_size(value, 0).c_str());
  STRCMP_EQUAL("(null)",
      cpputest::string_from_binary_with_size_or_null(nullptr, 0).c_str());
}

TEST(String, BinaryWithSizeLargerThan128)
{
  unsigned char value[129] = {};
  value[127] = 0x00;
  value[128] = 0xff;

  STRCMP_CONTAINS("00 ...",
      cpputest::string_from_binary_with_size(value, sizeof(value)).c_str());
}

TEST(String, MemCmp)
{
  unsigned char smaller[] = { 0x00, 0x01, 0x2A, 0xFF };
  unsigned char greater[] = { 0x00, 0x01, 0xFF, 0xFF };

  LONGS_EQUAL(0, cpputest::mem_cmp(smaller, smaller, sizeof(smaller)));
  CHECK(cpputest::mem_cmp(smaller, greater, sizeof(smaller)) < 0);
  CHECK(cpputest::mem_cmp(greater, smaller, sizeof(smaller)) > 0);
  LONGS_EQUAL(0, cpputest::mem_cmp(nullptr, nullptr, 0));
}

TEST(String, MemCmpFirstLastNotMatching)
{
  unsigned char base[] = { 0x00, 0x01, 0x2A, 0xFF };
  unsigned char first_not_matching[] = { 0x01, 0x01, 0x2A, 0xFF };
  unsigned char last_not_matching[] = { 0x00, 0x01, 0x2A, 0x00 };
  CHECK(0 != cpputest::mem_cmp(base, first_not_matching, sizeof(base)));
  CHECK(0 != cpputest::mem_cmp(base, last_not_matching, sizeof(base)));
}

TEST(String, StringFromOrdinalNumberOnes)
{
  STRCMP_EQUAL("2nd", cpputest::string_from_ordinal_number(2).c_str());
  STRCMP_EQUAL("3rd", cpputest::string_from_ordinal_number(3).c_str());
  STRCMP_EQUAL("4th", cpputest::string_from_ordinal_number(4).c_str());
  STRCMP_EQUAL("5th", cpputest::string_from_ordinal_number(5).c_str());
  STRCMP_EQUAL("6th", cpputest::string_from_ordinal_number(6).c_str());
  STRCMP_EQUAL("7th", cpputest::string_from_ordinal_number(7).c_str());
}

TEST(String, StringFromOrdinalNumberTens)
{
  STRCMP_EQUAL("10th", cpputest::string_from_ordinal_number(10).c_str());
  STRCMP_EQUAL("11th", cpputest::string_from_ordinal_number(11).c_str());
  STRCMP_EQUAL("12th", cpputest::string_from_ordinal_number(12).c_str());
  STRCMP_EQUAL("13th", cpputest::string_from_ordinal_number(13).c_str());
  STRCMP_EQUAL("14th", cpputest::string_from_ordinal_number(14).c_str());
  STRCMP_EQUAL("18th", cpputest::string_from_ordinal_number(18).c_str());
}

TEST(String, StringFromOrdinalNumberOthers)
{
  STRCMP_EQUAL("21st", cpputest::string_from_ordinal_number(21).c_str());
  STRCMP_EQUAL("22nd", cpputest::string_from_ordinal_number(22).c_str());
  STRCMP_EQUAL("23rd", cpputest::string_from_ordinal_number(23).c_str());
  STRCMP_EQUAL("24th", cpputest::string_from_ordinal_number(24).c_str());
  STRCMP_EQUAL("32nd", cpputest::string_from_ordinal_number(32).c_str());
  STRCMP_EQUAL("100th", cpputest::string_from_ordinal_number(100).c_str());
  STRCMP_EQUAL("101st", cpputest::string_from_ordinal_number(101).c_str());
}

TEST(String, BracketsFormattedHexStringFromForSignedChar)
{
  signed char value = 'c';

  STRCMP_EQUAL(
      "(0x63)", cpputest::brackets_formatted_hex_string_from(value).c_str());
}

TEST(String, BracketsFormattedHexStringFromForUnsignedInt)
{
  unsigned int value = 1;

  STRCMP_EQUAL(
      "(0x1)", cpputest::brackets_formatted_hex_string_from(value).c_str());
}

TEST(String, BracketsFormattedHexStringFromForUnsignedLong)
{
  unsigned long value = 1;

  STRCMP_EQUAL(
      "(0x1)", cpputest::brackets_formatted_hex_string_from(value).c_str());
}

#if (INT_MAX == 0x7fff)
TEST(String, BracketsFormattedHexStringFromForInt)
{
  int value = -1;

  STRCMP_EQUAL("(0xffff)", BracketsFormattedHexStringFrom(value).c_str());
}
#else
TEST(String, BracketsFormattedHexStringFromForInt)
{
  int value = -1;
  STRCMP_EQUAL("(0xffffffff)",
      cpputest::brackets_formatted_hex_string_from(value).c_str());
}
#endif

TEST(String, BracketsFormattedHexStringFromForLong)
{
  long value = 1;

  STRCMP_EQUAL(
      "(0x1)", cpputest::brackets_formatted_hex_string_from(value).c_str());
}
TEST(String, BracketsFormattedHexStringFromForLongLong)
{
  long long value = 1;

  STRCMP_EQUAL(
      "(0x1)", cpputest::brackets_formatted_hex_string_from(value).c_str());
}
TEST(String, BracketsFormattedHexStringFromForULongLong)
{
  unsigned long long value = 1;

  STRCMP_EQUAL(
      "(0x1)", cpputest::brackets_formatted_hex_string_from(value).c_str());
}
