#include "CppMu/String.hpp"

#include "CppMu/CppMu.hpp"
#include "CppMu/math.hpp"

#include <limits.h>
#include <stdio.h>

namespace {

int always_true(double)
{
  return true;
}

int wrapped_up_vsn_printf(char* buf, size_t n, const char* format, ...)
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

TEST(String, CreateRepeatedChar)
{
  cppmu::String expected("aaaa");
  cppmu::String actual(4, 'a');

  CHECK_EQUAL(expected, actual);
}

TEST(String, CreateRepeatedCharZero)
{
  cppmu::String expected("");
  cppmu::String actual(0, 'x'); // NOLINT(bugprone-string-constructor)

  CHECK_EQUAL(expected, actual);
}

TEST(String, Copy)
{
  cppmu::String s1("hello");
  cppmu::String s2(s1);

  CHECK_EQUAL(s1, s2);
}

TEST(String, Assignment)
{
  cppmu::String s1("hello");
  cppmu::String s2("goodbye");

  s2 = s1;

  CHECK_EQUAL(s1, s2);
}

TEST(String, Equality)
{
  cppmu::String s1("hello");
  cppmu::String s2("hello");

  CHECK(s1 == s2);
}

TEST(String, InEquality)
{
  cppmu::String s1("hello");
  cppmu::String s2("goodbye");

  CHECK(s1 != s2);
}

TEST(String, c_str)
{
  cppmu::String s1("hello");

  STRCMP_EQUAL("hello", s1.c_str());
}

TEST(String, Size)
{
  cppmu::String s1("hello!");

  LONGS_EQUAL(6, s1.size());
}

TEST(String, Addition)
{
  cppmu::String s1("hello!");
  cppmu::String s2("goodbye!");
  cppmu::String s3("hello!goodbye!");
  cppmu::String s4;
  s4 = s1 + s2;

  CHECK_EQUAL(s3, s4);
}

TEST(String, Concatenation)
{
  cppmu::String s1("hello!");
  cppmu::String s2("goodbye!");
  cppmu::String s3("hello!goodbye!");
  cppmu::String s4;
  s4 += s1;
  s4 += s2;

  CHECK_EQUAL(s3, s4);

  cppmu::String s5("hello!goodbye!hello!goodbye!");
  s4 += s4;

  CHECK_EQUAL(s5, s4);
}

TEST(String, stringContains)
{
  cppmu::String s("hello!");
  cppmu::String empty("");
  cppmu::String beginning("hello");
  cppmu::String end("lo!");
  cppmu::String mid("l");
  cppmu::String not_part_of_string("xxxx");

  CHECK(cppmu::string_contains(s, empty));
  CHECK(cppmu::string_contains(s, beginning));
  CHECK(cppmu::string_contains(s, end));
  CHECK(cppmu::string_contains(s, mid));
  CHECK(!cppmu::string_contains(s, not_part_of_string));

  CHECK(cppmu::string_contains(empty, empty));
  CHECK(!cppmu::string_contains(empty, s));
}

TEST(String, startsWith)
{
  cppmu::String hi("Hi you!");
  cppmu::String part("Hi");
  cppmu::String diff("Hrrm Hi you! ffdsfd");
  CHECK(cppmu::string_starts_with(hi, part));
  CHECK(!cppmu::string_starts_with(part, hi));
  CHECK(!cppmu::string_starts_with(diff, hi));
}

TEST(String, stringEndsWith)
{
  cppmu::String str("Hello World");
  CHECK(cppmu::string_ends_with(str, "World"));
  CHECK(!cppmu::string_ends_with(str, "Worl"));
  CHECK(!cppmu::string_ends_with(str, "Hello"));
  cppmu::String str2("ah");
  CHECK(cppmu::string_ends_with(str2, "ah"));
  CHECK(!cppmu::string_ends_with(str2, "baah"));
  cppmu::String str3("");
  CHECK(!cppmu::string_ends_with(str3, "baah"));

  cppmu::String str4("ha ha ha ha");
  CHECK(cppmu::string_ends_with(str4, "ha"));
}

TEST(String, stringReplaceCharWithChar)
{
  cppmu::String str("abcabcabca");
  cppmu::string_replace(str, 'a', 'b');
  STRCMP_EQUAL("bbcbbcbbcb", str.c_str());
}

TEST(String, stringReplaceEmptyStringWithEmptyString)
{
  cppmu::String str;
  cppmu::string_replace(str, "", "");
  STRCMP_EQUAL("", str.c_str());
}

TEST(String, stringReplaceWholeString)
{
  cppmu::String str("boo");
  cppmu::string_replace(str, "boo", "");
  STRCMP_EQUAL("", str.c_str());
}

TEST(String, stringReplaceStringWithString)
{
  cppmu::String str("boo baa boo baa boo");
  cppmu::string_replace(str, "boo", "boohoo");
  STRCMP_EQUAL("boohoo baa boohoo baa boohoo", str.c_str());
}

TEST(String, subStringFromEmptyString)
{
  cppmu::String str("");
  STRCMP_EQUAL("", str.substr(0, 1).c_str());
}

TEST(String, subStringFromSmallString)
{
  cppmu::String str("H");
  STRCMP_EQUAL("H", str.substr(0, 1).c_str());
}

TEST(String, subStringFromPos0)
{
  cppmu::String str("Hello World");
  STRCMP_EQUAL("Hello", str.substr(0, 5).c_str());
}

TEST(String, subStringFromPos1)
{
  cppmu::String str("Hello World");
  STRCMP_EQUAL("ello ", str.substr(1, 5).c_str());
}

TEST(String, subStringFromPos5WithAmountLargerThanString)
{
  cppmu::String str("Hello World");
  STRCMP_EQUAL("World", str.substr(6, 10).c_str());
}

TEST(String, subStringFromPos6ToEndOfString)
{
  cppmu::String str("Hello World");
  STRCMP_EQUAL("World", str.substr(6).c_str());
}

TEST(String, findNormal)
{
  cppmu::String str("Hello World");
  LONGS_EQUAL(0, str.find('H'));
  LONGS_EQUAL(1, str.find('e'));
  LONGS_EQUAL(cppmu::String::npos, str.find('!'));
}

TEST(String, findCharFromPos)
{
  cppmu::String str("Hello World");
  LONGS_EQUAL(2, str.find('l', 0));
  LONGS_EQUAL(3, str.find('l', 3));
  LONGS_EQUAL(cppmu::String::npos, str.find('l', 10));
}

TEST(String, findSubstring)
{
  cppmu::String str("Hello World");
  LONGS_EQUAL(0, str.find("Hello"));
  LONGS_EQUAL(6, str.find("World"));
  LONGS_EQUAL(cppmu::String::npos, str.find("xyz"));
}

TEST(String, findSubstringFromPos)
{
  cppmu::String str("abcabc");
  LONGS_EQUAL(0, str.find("abc", 0));
  LONGS_EQUAL(3, str.find("abc", 1));
  LONGS_EQUAL(cppmu::String::npos, str.find("abc", 4));
}

TEST(String, findEmptySubstring)
{
  cppmu::String str("Hello");
  LONGS_EQUAL(0, str.find(""));
  LONGS_EQUAL(3, str.find("", 3));
}

TEST(String, clear)
{
  cppmu::String str("Hello");
  str.clear();
  STRCMP_EQUAL("", str.c_str());
  LONGS_EQUAL(0, str.size());
}

TEST(String, lessThan)
{
  cppmu::String a("abc");
  cppmu::String b("abd");
  cppmu::String c("abc");
  CHECK(a < b);
  CHECK(!(b < a));
  CHECK(!(a < c));
}

TEST(String, dataConst)
{
  const cppmu::String str("Hello");
  STRCMP_EQUAL("Hello", str.data());
}

TEST(String, index)
{
  cppmu::String str("Hello World");
  BYTES_EQUAL('H', str[0]);
}

TEST(String, NULLReportsNullString)
{
  STRCMP_EQUAL(
      "(null)", cppmu::string_from_or_null(static_cast<char*>(nullptr)).c_str()
  );
}

TEST(String, Booleans)
{
  cppmu::String s1(cppmu::string_from(true));
  cppmu::String s2(cppmu::string_from(false));
  CHECK(s1 == "true");
  CHECK(s2 == "false");
}

TEST(String, Pointers)
{
  cppmu::String s(cppmu::string_from(reinterpret_cast<void*>(0x1234)));
  STRCMP_EQUAL("0x1234", s.c_str());
}

TEST(String, FunctionPointers)
{
  cppmu::String s(cppmu::string_from(reinterpret_cast<void (*)()>(0x1234)));
  STRCMP_EQUAL("0x1234", s.c_str());
}

TEST(String, Characters)
{
  cppmu::String s(cppmu::string_from('a'));
  STRCMP_EQUAL("a", s.c_str());
}

TEST(String, NegativeSignedBytes)
{
  STRCMP_EQUAL(
      "-15", cppmu::string_from(static_cast<signed char>(-15)).c_str()
  );
}

TEST(String, PositiveSignedBytes)
{
  STRCMP_EQUAL("4", cppmu::string_from(4).c_str());
}

TEST(String, LongInts)
{
  cppmu::String s(cppmu::string_from(static_cast<long>(1)));
  CHECK(s == "1");
}

TEST(String, UnsignedLongInts)
{
  cppmu::String s(cppmu::string_from(static_cast<unsigned long>(1)));
  cppmu::String s2(cppmu::string_from(static_cast<unsigned long>(1)));
  CHECK(s == s2);
}

TEST(String, LongLongInts)
{
  cppmu::String s(cppmu::string_from(static_cast<long long>(1)));
  CHECK(s == "1");
}

TEST(String, UnsignedLongLongInts)
{
  cppmu::String s(cppmu::string_from(static_cast<unsigned long long>(1)));
  cppmu::String s2(cppmu::string_from(static_cast<unsigned long long>(1)));
  CHECK(s == s2);
}

TEST(String, Doubles)
{
  cppmu::String s(cppmu::string_from(1.2));
  STRCMP_EQUAL("1.2", s.c_str());
}

TEST(String, NaN)
{
  CPPMU_PTR_SET(cppmu::is_nan, always_true);
  cppmu::String s(cppmu::string_from(0.0));
  STRCMP_EQUAL("Nan - Not a number", s.c_str());
}

TEST(String, Inf)
{
  CPPMU_PTR_SET(cppmu::is_inf, always_true);
  cppmu::String s(cppmu::string_from(0.0));
  STRCMP_EQUAL("Inf - Infinity", s.c_str());
}

TEST(String, SmallDoubles)
{
  cppmu::String s(cppmu::string_from(1.2e-10));
  STRCMP_CONTAINS("1.2e", s.c_str());
}

TEST(String, Sizes)
{
  size_t size = 10;
  STRCMP_EQUAL("10", cppmu::string_from(static_cast<int>(size)).c_str());
}

#if CPPMU_USE_STD_CPP_LIB

TEST(String, nullptr_type)
{
  cppmu::String s(cppmu::string_from(nullptr));
  STRCMP_EQUAL("(null)", s.c_str());
}

#endif

TEST(String, HexStrings)
{
  STRCMP_EQUAL(
      "f3", cppmu::hex_string_from(static_cast<signed char>(-13)).c_str()
  );

  cppmu::String h1 = cppmu::hex_string_from(0xffffL);
  STRCMP_EQUAL("ffff", h1.c_str());

  cppmu::String h15 = cppmu::hex_string_from(0xffffLL);
  STRCMP_EQUAL("ffff", h15.c_str());

  cppmu::String h2 = cppmu::hex_string_from(reinterpret_cast<void*>(0xfffeL));
  STRCMP_EQUAL("fffe", h2.c_str());

  cppmu::String h3 =
      cppmu::hex_string_from(reinterpret_cast<void (*)()>(0xfffdL));
  STRCMP_EQUAL("fffd", h3.c_str());
}

TEST(String, StringFromFormat)
{
  cppmu::String h1 =
      cppmu::string_from_format("%s %s! %d", "Hello", "World", 2009);
  STRCMP_EQUAL("Hello World! 2009", h1.c_str());
}

TEST(String, StringFromFormatpointer)
{
  // this is not a great test. but %p is odd on mingw and even more odd on
  // Solaris.
  cppmu::String h1 =
      cppmu::string_from_format("%p", reinterpret_cast<void*>(1));
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
  cppmu::String h1 = cppmu::string_from_format(
      "%s%s%s%s%s%s%s%s%s%s", s, s, s, s, s, s, s, s, s, s
  );
  LONGS_EQUAL(
      10 * static_cast<long>(cppmu::String(s).size()),
      static_cast<long>(h1.size())
  );
}

TEST(String, StringFromConstString)
{
  STRCMP_EQUAL("bla", cppmu::string_from(cppmu::String("bla")).c_str());
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
  cppmu::String str1("1");
  cppmu::String str2("222");

  cppmu::pad_strings_to_same_length(str1, str2, '4');
  STRCMP_EQUAL("441", str1.c_str());
  STRCMP_EQUAL("222", str2.c_str());
}

TEST(String, PadStringsToSameLengthString2Larger)
{
  cppmu::String str1("    ");
  cppmu::String str2("");

  cppmu::pad_strings_to_same_length(str1, str2, ' ');
  STRCMP_EQUAL("    ", str1.c_str());
  STRCMP_EQUAL("    ", str2.c_str());
}

TEST(String, PadStringsToSameLengthWithSameLengthStrings)
{
  cppmu::String str1("123");
  cppmu::String str2("123");

  cppmu::pad_strings_to_same_length(str1, str2, ' ');
  STRCMP_EQUAL("123", str1.c_str());
  STRCMP_EQUAL("123", str2.c_str());
}

TEST(String, NullParameters2)
{
  auto* arr = new cppmu::String[100];
  delete[] arr;
}

#if defined(__LP64__) || defined(_LP64) ||                                     \
    (defined(__WORDSIZE) && (__WORDSIZE == 64)) || defined(__x86_64) ||        \
    defined(_WIN64)

TEST(String, 64BitAddressPrintsCorrectly)
{
  char* p = reinterpret_cast<char*>(0x0012345678901234LL);
  cppmu::String expected("0x12345678901234");
  cppmu::String actual = cppmu::string_from(static_cast<void*>(p));
  STRCMP_EQUAL(expected.c_str(), actual.c_str());
}

#if !defined(_WIN64)

TEST(String, BracketsFormattedHexStringFromForLongOnDifferentPlatform)
{
  long value = -1;

  STRCMP_EQUAL(
      "(0xffffffffffffffff)",
      cppmu::brackets_formatted_hex_string_from(value).c_str()
  );
}

#else

TEST(String, BracketsFormattedHexStringFromForLongOnDifferentPlatform)
{
  long value = -1;

  STRCMP_EQUAL(
      "(0xffffffff)", cppmu::brackets_formatted_hex_string_from(value).c_str()
  );
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
      "(0xffffffff)", cppmu::brackets_formatted_hex_string_from(value).c_str()
  );
}
#endif

TEST(String, BuildStringFromUnsignedLongInteger)
{
  unsigned long int i = 0xffffffff;

  cppmu::String result = cppmu::string_from(i);
  const char* expected_string = "4294967295";
  CHECK_EQUAL(expected_string, result);
}

TEST(String, BuildStringFromUnsignedInteger)
{
  unsigned int i = 0xff;

  cppmu::String result = cppmu::string_from(i);
  const char* expected_string = "255";
  CHECK_EQUAL(expected_string, result);
}

#if CPPMU_USE_STD_CPP_LIB

TEST(String, fromStdString)
{
  std::string s("hello");
  cppmu::String s1(cppmu::string_from(s));

  STRCMP_EQUAL("hello", s1.c_str());
}

TEST( String, CHECK_EQUAL_unsigned_long)
{
  unsigned long i = 0xffffffffUL;
  CHECK_EQUAL(i, i);
}

TEST( String, unsigned_long)
{
  unsigned long i = 0xffffffffUL;

  cppmu::String result = cppmu::string_from(i);
  const char* expected_string = "4294967295";
  CHECK_EQUAL(expected_string, result);
}

#endif

TEST(String, StrCmp)
{
  char empty[] = "";
  char blabla[] = "blabla";
  char bla[] = "bla";
  CHECK(cppmu::str_cmp(empty, empty) == 0);
  CHECK(cppmu::str_cmp(bla, blabla) == -static_cast<int>('b'));
  CHECK(cppmu::str_cmp(blabla, bla) == 'b');
  CHECK(cppmu::str_cmp(bla, empty) == 'b');
  CHECK(cppmu::str_cmp(empty, bla) == -static_cast<int>('b'));
  CHECK(cppmu::str_cmp(bla, bla) == 0);
}

TEST(String, StrNCmp_equal)
{
  int result = cppmu::str_n_cmp("teststring", "tests", 5);
  LONGS_EQUAL(0, result);
}

TEST(String, StrNCmp_should_always_return_0_when_n_is_0)
{
  int result = cppmu::str_n_cmp("a", "b", 0);
  LONGS_EQUAL(0, result);
}

TEST(String, StrNCmp_s1_smaller)
{
  int result = cppmu::str_n_cmp("testing", "tests", 7);
  LONGS_EQUAL('i' - 's', result);
}

TEST(String, StrNCmp_s1_larger)
{
  int result = cppmu::str_n_cmp("teststring", "tester", 7);
  LONGS_EQUAL('s' - 'e', result);
}

TEST(String, StrNCmp_n_too_large)
{
  int result = cppmu::str_n_cmp("teststring", "teststring", 20);
  LONGS_EQUAL(0, result);
}

TEST(String, StrNCmp_s1_empty)
{
  int result = cppmu::str_n_cmp("", "foo", 2);
  LONGS_EQUAL(0 - 'f', result);
}

TEST(String, StrNCmp_s2_empty)
{
  int result = cppmu::str_n_cmp("foo", "", 2);
  LONGS_EQUAL('f', result);
}

TEST(String, StrNCmp_s1_and_s2_empty)
{
  int result = cppmu::str_n_cmp("", "", 2);
  LONGS_EQUAL(0, result);
}

TEST(String, AtoI)
{
  char max_short_str[] = "32767";
  char min_short_str[] = "-32768";

  CHECK(12345 == cppmu::ato_i("012345"));
  CHECK(6789 == cppmu::ato_i("6789"));
  CHECK(12345 == cppmu::ato_i("12345/"));
  CHECK(12345 == cppmu::ato_i("12345:"));
  CHECK(-12345 == cppmu::ato_i("-12345"));
  CHECK(123 == cppmu::ato_i("\t \r\n123"));
  CHECK(123 == cppmu::ato_i("123-foo"));
  CHECK(0 == cppmu::ato_i("-foo"));
  CHECK(-32768 == cppmu::ato_i(min_short_str));
  CHECK(32767 == cppmu::ato_i(max_short_str));
}

TEST(String, AtoU)
{
  char max_short_str[] = "65535";
  CHECK(12345 == cppmu::ato_u("012345"));
  CHECK(6789 == cppmu::ato_u("6789"));
  CHECK(12345 == cppmu::ato_u("12345/"));
  CHECK(12345 == cppmu::ato_u("12345:"));
  CHECK(123 == cppmu::ato_u("\t \r\n123"));
  CHECK(123 == cppmu::ato_u("123-foo"));
  CHECK(65535 == cppmu::ato_u(max_short_str));
  CHECK(0 == cppmu::ato_u("foo"));
  CHECK(0 == cppmu::ato_u("-foo"));
  CHECK(0 == cppmu::ato_u("+1"));
  CHECK(0 == cppmu::ato_u("-1"));
  CHECK(0 == cppmu::ato_u("0"));
}

TEST(String, Binary)
{
  const unsigned char value[] = { 0x00, 0x01, 0x2A, 0xFF };
  const char expected_string[] = "00 01 2A FF";

  STRCMP_EQUAL(
      expected_string, cppmu::string_from_binary(value, sizeof(value)).c_str()
  );
  STRCMP_EQUAL(
      expected_string,
      cppmu::string_from_binary_or_null(value, sizeof(value)).c_str()
  );
  STRCMP_EQUAL("", cppmu::string_from_binary(value, 0).c_str());
  STRCMP_EQUAL("(null)", cppmu::string_from_binary_or_null(nullptr, 0).c_str());
}

TEST(String, BinaryWithSize)
{
  const unsigned char value[] = { 0x12, 0xFE, 0xA1 };
  const char expected_string[] = "Size = 3 | HexContents = 12 FE A1";

  STRCMP_EQUAL(
      expected_string,
      cppmu::string_from_binary_with_size(value, sizeof(value)).c_str()
  );
  STRCMP_EQUAL(
      expected_string,
      cppmu::string_from_binary_with_size_or_null(value, sizeof(value)).c_str()
  );
  STRCMP_EQUAL(
      "Size = 0 | HexContents = ",
      cppmu::string_from_binary_with_size(value, 0).c_str()
  );
  STRCMP_EQUAL(
      "(null)", cppmu::string_from_binary_with_size_or_null(nullptr, 0).c_str()
  );
}

TEST(String, BinaryWithSizeLargerThan128)
{
  unsigned char value[129] = {};
  value[127] = 0x00;
  value[128] = 0xff;

  STRCMP_CONTAINS(
      "00 ...",
      cppmu::string_from_binary_with_size(value, sizeof(value)).c_str()
  );
}

TEST(String, MemCmp)
{
  unsigned char smaller[] = { 0x00, 0x01, 0x2A, 0xFF };
  unsigned char greater[] = { 0x00, 0x01, 0xFF, 0xFF };

  LONGS_EQUAL(0, cppmu::mem_cmp(smaller, smaller, sizeof(smaller)));
  CHECK(cppmu::mem_cmp(smaller, greater, sizeof(smaller)) < 0);
  CHECK(cppmu::mem_cmp(greater, smaller, sizeof(smaller)) > 0);
  LONGS_EQUAL(0, cppmu::mem_cmp(nullptr, nullptr, 0));
}

TEST(String, MemCmpFirstLastNotMatching)
{
  unsigned char base[] = { 0x00, 0x01, 0x2A, 0xFF };
  unsigned char first_not_matching[] = { 0x01, 0x01, 0x2A, 0xFF };
  unsigned char last_not_matching[] = { 0x00, 0x01, 0x2A, 0x00 };
  CHECK(0 != cppmu::mem_cmp(base, first_not_matching, sizeof(base)));
  CHECK(0 != cppmu::mem_cmp(base, last_not_matching, sizeof(base)));
}

TEST(String, StringFromOrdinalNumberOnes)
{
  STRCMP_EQUAL("2nd", cppmu::string_from_ordinal_number(2).c_str());
  STRCMP_EQUAL("3rd", cppmu::string_from_ordinal_number(3).c_str());
  STRCMP_EQUAL("4th", cppmu::string_from_ordinal_number(4).c_str());
  STRCMP_EQUAL("5th", cppmu::string_from_ordinal_number(5).c_str());
  STRCMP_EQUAL("6th", cppmu::string_from_ordinal_number(6).c_str());
  STRCMP_EQUAL("7th", cppmu::string_from_ordinal_number(7).c_str());
}

TEST(String, StringFromOrdinalNumberTens)
{
  STRCMP_EQUAL("10th", cppmu::string_from_ordinal_number(10).c_str());
  STRCMP_EQUAL("11th", cppmu::string_from_ordinal_number(11).c_str());
  STRCMP_EQUAL("12th", cppmu::string_from_ordinal_number(12).c_str());
  STRCMP_EQUAL("13th", cppmu::string_from_ordinal_number(13).c_str());
  STRCMP_EQUAL("14th", cppmu::string_from_ordinal_number(14).c_str());
  STRCMP_EQUAL("18th", cppmu::string_from_ordinal_number(18).c_str());
}

TEST(String, StringFromOrdinalNumberOthers)
{
  STRCMP_EQUAL("21st", cppmu::string_from_ordinal_number(21).c_str());
  STRCMP_EQUAL("22nd", cppmu::string_from_ordinal_number(22).c_str());
  STRCMP_EQUAL("23rd", cppmu::string_from_ordinal_number(23).c_str());
  STRCMP_EQUAL("24th", cppmu::string_from_ordinal_number(24).c_str());
  STRCMP_EQUAL("32nd", cppmu::string_from_ordinal_number(32).c_str());
  STRCMP_EQUAL("100th", cppmu::string_from_ordinal_number(100).c_str());
  STRCMP_EQUAL("101st", cppmu::string_from_ordinal_number(101).c_str());
}

TEST(String, BracketsFormattedHexStringFromForSignedChar)
{
  signed char value = 'c';

  STRCMP_EQUAL(
      "(0x63)", cppmu::brackets_formatted_hex_string_from(value).c_str()
  );
}

TEST(String, BracketsFormattedHexStringFromForUnsignedInt)
{
  unsigned int value = 1;

  STRCMP_EQUAL(
      "(0x1)", cppmu::brackets_formatted_hex_string_from(value).c_str()
  );
}

TEST(String, BracketsFormattedHexStringFromForUnsignedLong)
{
  unsigned long value = 1;

  STRCMP_EQUAL(
      "(0x1)", cppmu::brackets_formatted_hex_string_from(value).c_str()
  );
}

#if (INT_MAX == 0x7fff)
TEST(String, BracketsFormattedHexStringFromForInt)
{
  int value = -1;

  STRCMP_EQUAL(
      "(0xffff)", cppmu::brackets_formatted_hex_string_from(value).c_str()
  );
}
#else
TEST(String, BracketsFormattedHexStringFromForInt)
{
  int value = -1;
  STRCMP_EQUAL(
      "(0xffffffff)", cppmu::brackets_formatted_hex_string_from(value).c_str()
  );
}
#endif

TEST(String, BracketsFormattedHexStringFromForLong)
{
  long value = 1;

  STRCMP_EQUAL(
      "(0x1)", cppmu::brackets_formatted_hex_string_from(value).c_str()
  );
}
TEST(String, BracketsFormattedHexStringFromForLongLong)
{
  long long value = 1;

  STRCMP_EQUAL(
      "(0x1)", cppmu::brackets_formatted_hex_string_from(value).c_str()
  );
}
TEST(String, BracketsFormattedHexStringFromForULongLong)
{
  unsigned long long value = 1;

  STRCMP_EQUAL(
      "(0x1)", cppmu::brackets_formatted_hex_string_from(value).c_str()
  );
}

TEST(String, StartsWithEmptyPrefixIsAlwaysTrue)
{
  CHECK(cppmu::string_starts_with("anything", ""));
}

TEST(String, StartsWithOnEmptyStringIsAlwaysFalse)
{
  CHECK_FALSE(cppmu::string_starts_with("", "abc"));
}

TEST(String, EndsWithEmptySuffixIsAlwaysTrue)
{
  CHECK(cppmu::string_ends_with("anything", ""));
}

TEST(String, HexStringFromPositiveSignedChar)
{
  signed char value = 13;
  STRCMP_EQUAL("d", cppmu::hex_string_from(value).c_str());
}
