#include "CppUTest/String.hpp"

#include "CppUTest/PlatformSpecificFunctions.h"
#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestMemoryAllocator.hpp"

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

TEST_GROUP(String)
{
  JustUseNewStringAllocator justNewForStringTestAllocator;
  cpputest::TestMemoryAllocator* originalAllocator;
  void setup() override
  {
    originalAllocator = cpputest::String::getStringAllocator();
    cpputest::String::setStringAllocator(&justNewForStringTestAllocator);
  }
  void teardown() override
  {
    cpputest::String::setStringAllocator(originalAllocator);
  }
};

TEST(String, defaultAllocatorIsNewArrayAllocator)
{
  cpputest::String::setStringAllocator(nullptr);
  POINTERS_EQUAL(cpputest::defaultNewArrayAllocator(),
      cpputest::String::getStringAllocator());
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

TEST(String, allocatorForStringCanBeReplaced)
{
  MyOwnStringAllocator myOwnAllocator;
  cpputest::String::setStringAllocator(&myOwnAllocator);
  cpputest::String simpleString;
  CHECK(myOwnAllocator.memoryWasAllocated);
  cpputest::String::setStringAllocator(nullptr);
}

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

TEST(String, printable)
{
  cpputest::String s1("ABC\01\06\a\n\r\b\t\v\f\x0E\x1F\x7F"
                      "abc");
  cpputest::String s2(s1.printable());
  STRCMP_EQUAL(
      "ABC\\x01\\x06\\a\\n\\r\\b\\t\\v\\f\\x0E\\x1F\\x7Fabc", s2.c_str());
  STRCMP_EQUAL("ABC\01\06\a\n\r\b\t\v\f\x0E\x1F\x7F"
               "abc",
      s1.c_str());
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

TEST(String, Contains)
{
  cpputest::String s("hello!");
  cpputest::String empty("");
  cpputest::String beginning("hello");
  cpputest::String end("lo!");
  cpputest::String mid("l");
  cpputest::String notPartOfString("xxxx");

  CHECK(s.contains(empty));
  CHECK(s.contains(beginning));
  CHECK(s.contains(end));
  CHECK(s.contains(mid));
  CHECK(!s.contains(notPartOfString));

  CHECK(empty.contains(empty));
  CHECK(!empty.contains(s));
}

TEST(String, startsWith)
{
  cpputest::String hi("Hi you!");
  cpputest::String part("Hi");
  cpputest::String diff("Hrrm Hi you! ffdsfd");
  CHECK(hi.startsWith(part));
  CHECK(!part.startsWith(hi));
  CHECK(!diff.startsWith(hi));
}

TEST(String, split)
{
  cpputest::String hi("hello\nworld\nhow\ndo\nyou\ndo\n\n");

  cpputest::StringCollection collection;
  hi.split("\n", collection);

  LONGS_EQUAL(7, collection.size());
  STRCMP_EQUAL("hello\n", collection[0].c_str());
  STRCMP_EQUAL("world\n", collection[1].c_str());
  STRCMP_EQUAL("how\n", collection[2].c_str());
  STRCMP_EQUAL("do\n", collection[3].c_str());
  STRCMP_EQUAL("you\n", collection[4].c_str());
  STRCMP_EQUAL("do\n", collection[5].c_str());
  STRCMP_EQUAL("\n", collection[6].c_str());
}

TEST(String, splitNoTokenOnTheEnd)
{
  cpputest::String string("Bah Yah oops");
  cpputest::StringCollection collection;

  string.split(" ", collection);
  LONGS_EQUAL(3, collection.size());
  STRCMP_EQUAL("Bah ", collection[0].c_str());
  STRCMP_EQUAL("Yah ", collection[1].c_str());
  STRCMP_EQUAL("oops", collection[2].c_str());
}

TEST(String, endsWith)
{
  cpputest::String str("Hello World");
  CHECK(str.endsWith("World"));
  CHECK(!str.endsWith("Worl"));
  CHECK(!str.endsWith("Hello"));
  cpputest::String str2("ah");
  CHECK(str2.endsWith("ah"));
  CHECK(!str2.endsWith("baah"));
  cpputest::String str3("");
  CHECK(!str3.endsWith("baah"));

  cpputest::String str4("ha ha ha ha");
  CHECK(str4.endsWith("ha"));
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
  STRCMP_EQUAL("Hello", str.subStringFromTill('H', ' ').c_str());
}

TEST(String, subStringFromTillOutOfBounds)
{
  cpputest::String str("Hello World");
  STRCMP_EQUAL("World", str.subStringFromTill('W', '!').c_str());
}

TEST(String, subStringFromTillStartDoesntExist)
{
  cpputest::String str("Hello World");
  STRCMP_EQUAL("", str.subStringFromTill('!', ' ').c_str());
}

TEST(String, subStringFromTillWhenTheEndAppearsBeforeTheStart)
{
  cpputest::String str("Hello World");
  STRCMP_EQUAL("World", str.subStringFromTill('W', 'H').c_str());
}

TEST(String, findNormal)
{
  cpputest::String str("Hello World");
  LONGS_EQUAL(0, str.find('H'));
  LONGS_EQUAL(1, str.find('e'));
  LONGS_EQUAL(cpputest::String::npos, str.find('!'));
}

TEST(String, at)
{
  cpputest::String str("Hello World");
  BYTES_EQUAL('H', str.at(0));
}

TEST(String, ContainsNull)
{
  cpputest::String s(nullptr);
  STRCMP_EQUAL("", s.c_str());
}

TEST(String, NULLReportsNullString)
{
  STRCMP_EQUAL("(null)",
      cpputest::StringFromOrNull(static_cast<char*>(nullptr)).c_str());
}

TEST(String, NULLReportsNullStringPrintable)
{
  STRCMP_EQUAL("(null)",
      cpputest::PrintableStringFromOrNull(static_cast<char*>(nullptr)).c_str());
}

TEST(String, Booleans)
{
  cpputest::String s1(cpputest::StringFrom(true));
  cpputest::String s2(cpputest::StringFrom(false));
  CHECK(s1 == "true");
  CHECK(s2 == "false");
}

TEST(String, Pointers)
{
  cpputest::String s(cpputest::StringFrom(reinterpret_cast<void*>(0x1234)));
  STRCMP_EQUAL("0x1234", s.c_str());
}

TEST(String, FunctionPointers)
{
  cpputest::String s(
      cpputest::StringFrom(reinterpret_cast<void (*)()>(0x1234)));
  STRCMP_EQUAL("0x1234", s.c_str());
}

TEST(String, Characters)
{
  cpputest::String s(cpputest::StringFrom('a'));
  STRCMP_EQUAL("a", s.c_str());
}

TEST(String, NegativeSignedBytes)
{
  STRCMP_EQUAL(
      "-15", cpputest::StringFrom(static_cast<signed char>(-15)).c_str());
}

TEST(String, PositiveSignedBytes)
{
  STRCMP_EQUAL("4", cpputest::StringFrom(4).c_str());
}

TEST(String, LongInts)
{
  cpputest::String s(cpputest::StringFrom(static_cast<long>(1)));
  CHECK(s == "1");
}

TEST(String, UnsignedLongInts)
{
  cpputest::String s(cpputest::StringFrom(static_cast<unsigned long>(1)));
  cpputest::String s2(cpputest::StringFrom(static_cast<unsigned long>(1)));
  CHECK(s == s2);
}

TEST(String, LongLongInts)
{
  cpputest::String s(cpputest::StringFrom(static_cast<long long>(1)));
  CHECK(s == "1");
}

TEST(String, UnsignedLongLongInts)
{
  cpputest::String s(cpputest::StringFrom(static_cast<unsigned long long>(1)));
  cpputest::String s2(cpputest::StringFrom(static_cast<unsigned long long>(1)));
  CHECK(s == s2);
}

TEST(String, Doubles)
{
  cpputest::String s(cpputest::StringFrom(1.2));
  STRCMP_EQUAL("1.2", s.c_str());
}

static int
alwaysTrue(double)
{
  return true;
}

TEST(String, NaN)
{
  UT_PTR_SET(PlatformSpecificIsNan, alwaysTrue);
  cpputest::String s(cpputest::StringFrom(0.0));
  STRCMP_EQUAL("Nan - Not a number", s.c_str());
}

TEST(String, Inf)
{
  UT_PTR_SET(PlatformSpecificIsInf, alwaysTrue);
  cpputest::String s(cpputest::StringFrom(0.0));
  STRCMP_EQUAL("Inf - Infinity", s.c_str());
}

TEST(String, SmallDoubles)
{
  cpputest::String s(cpputest::StringFrom(1.2e-10));
  STRCMP_CONTAINS("1.2e", s.c_str());
}

TEST(String, Sizes)
{
  size_t size = 10;
  STRCMP_EQUAL("10", cpputest::StringFrom(static_cast<int>(size)).c_str());
}

#if CPPUTEST_USE_STD_CPP_LIB

TEST(String, nullptr_type)
{
  cpputest::String s(cpputest::StringFrom(nullptr));
  STRCMP_EQUAL("(null)", s.c_str());
}

#endif

TEST(String, HexStrings)
{
  STRCMP_EQUAL(
      "f3", cpputest::HexStringFrom(static_cast<signed char>(-13)).c_str());

  cpputest::String h1 = cpputest::HexStringFrom(0xffffL);
  STRCMP_EQUAL("ffff", h1.c_str());

  cpputest::String h15 = cpputest::HexStringFrom(0xffffLL);
  STRCMP_EQUAL("ffff", h15.c_str());

  cpputest::String h2 =
      cpputest::HexStringFrom(reinterpret_cast<void*>(0xfffeL));
  STRCMP_EQUAL("fffe", h2.c_str());

  cpputest::String h3 =
      cpputest::HexStringFrom(reinterpret_cast<void (*)()>(0xfffdL));
  STRCMP_EQUAL("fffd", h3.c_str());
}

TEST(String, StringFromFormat)
{
  cpputest::String h1 =
      cpputest::StringFromFormat("%s %s! %d", "Hello", "World", 2009);
  STRCMP_EQUAL("Hello World! 2009", h1.c_str());
}

TEST(String, StringFromFormatpointer)
{
  // this is not a great test. but %p is odd on mingw and even more odd on
  // Solaris.
  cpputest::String h1 =
      cpputest::StringFromFormat("%p", reinterpret_cast<void*>(1));
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
  cpputest::String h1 = cpputest::StringFromFormat(
      "%s%s%s%s%s%s%s%s%s%s", s, s, s, s, s, s, s, s, s, s);
  LONGS_EQUAL(10 * static_cast<long>(cpputest::String(s).size()),
      static_cast<long>(h1.size()));
}

TEST(String, StringFromConstString)
{
  STRCMP_EQUAL("bla", StringFrom(cpputest::String("bla")).c_str());
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

TEST(String, PlatformSpecificSprintf_fits)
{
  char buf[10];

  int count = WrappedUpVSNPrintf(buf, sizeof(buf), "%s", "12345");
  STRCMP_EQUAL("12345", buf);
  LONGS_EQUAL(5, count);
}

TEST(String, PlatformSpecificSprintf_doesNotFit)
{
  char buf[10];

  int count = WrappedUpVSNPrintf(buf, sizeof(buf), "%s", "12345678901");
  STRCMP_EQUAL("123456789", buf);
  LONGS_EQUAL(11, count);
}

TEST(String, PadStringsToSameLengthString1Larger)
{
  cpputest::String str1("1");
  cpputest::String str2("222");

  padStringsToSameLength(str1, str2, '4');
  STRCMP_EQUAL("441", str1.c_str());
  STRCMP_EQUAL("222", str2.c_str());
}

TEST(String, PadStringsToSameLengthString2Larger)
{
  cpputest::String str1("    ");
  cpputest::String str2("");

  padStringsToSameLength(str1, str2, ' ');
  STRCMP_EQUAL("    ", str1.c_str());
  STRCMP_EQUAL("    ", str2.c_str());
}

TEST(String, PadStringsToSameLengthWithSameLengthStrings)
{
  cpputest::String str1("123");
  cpputest::String str2("123");

  padStringsToSameLength(str1, str2, ' ');
  STRCMP_EQUAL("123", str1.c_str());
  STRCMP_EQUAL("123", str2.c_str());
}

TEST(String, NullParameters2)
{
  cpputest::String* arr = new cpputest::String[100];
  delete[] arr;
}

TEST(String, CollectionReadOutOfBoundsReturnsEmptyString)
{
  cpputest::StringCollection col;
  col.allocate(3);
  STRCMP_EQUAL("", col[3].c_str());
}

TEST(String, CollectionWritingToEmptyString)
{
  cpputest::StringCollection col;
  col.allocate(3);
  col[3] = cpputest::String("HAH");
  STRCMP_EQUAL("", col[3].c_str());
}

#if defined(__LP64__) || defined(_LP64) ||                                     \
    (defined(__WORDSIZE) && (__WORDSIZE == 64)) || defined(__x86_64) ||        \
    defined(_WIN64)

TEST(String, 64BitAddressPrintsCorrectly)
{
  char* p = reinterpret_cast<char*>(0x0012345678901234LL);
  cpputest::String expected("0x12345678901234");
  cpputest::String actual = cpputest::StringFrom(static_cast<void*>(p));
  STRCMP_EQUAL(expected.c_str(), actual.c_str());
}

#if !defined(_WIN64)

TEST(String, BracketsFormattedHexStringFromForLongOnDifferentPlatform)
{
  long value = -1;

  STRCMP_EQUAL("(0xffffffffffffffff)",
      cpputest::BracketsFormattedHexStringFrom(value).c_str());
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

  cpputest::String result = cpputest::StringFrom(i);
  const char* expected_string = "4294967295";
  CHECK_EQUAL(expected_string, result);
}

TEST(String, BuildStringFromUnsignedInteger)
{
  unsigned int i = 0xff;

  cpputest::String result = cpputest::StringFrom(i);
  const char* expected_string = "255";
  CHECK_EQUAL(expected_string, result);
}

#if CPPUTEST_USE_STD_CPP_LIB

TEST(String, fromStdString)
{
  std::string s("hello");
  cpputest::String s1(cpputest::StringFrom(s));

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

  cpputest::String result = cpputest::StringFrom(i);
  const char* expected_string = "4294967295";
  CHECK_EQUAL(expected_string, result);
}

#endif

TEST(String, StrCmp)
{
  char empty[] = "";
  char blabla[] = "blabla";
  char bla[] = "bla";
  CHECK(cpputest::StrCmp(empty, empty) == 0);
  CHECK(cpputest::StrCmp(bla, blabla) == -static_cast<int>('b'));
  CHECK(cpputest::StrCmp(blabla, bla) == 'b');
  CHECK(cpputest::StrCmp(bla, empty) == 'b');
  CHECK(cpputest::StrCmp(empty, bla) == -static_cast<int>('b'));
  CHECK(cpputest::StrCmp(bla, bla) == 0);
}

TEST(String, StrNCmp_equal)
{
  int result = cpputest::StrNCmp("teststring", "tests", 5);
  LONGS_EQUAL(0, result);
}

TEST(String, StrNCmp_should_always_return_0_when_n_is_0)
{
  int result = cpputest::StrNCmp("a", "b", 0);
  LONGS_EQUAL(0, result);
}

TEST(String, StrNCmp_s1_smaller)
{
  int result = cpputest::StrNCmp("testing", "tests", 7);
  LONGS_EQUAL('i' - 's', result);
}

TEST(String, StrNCmp_s1_larger)
{
  int result = cpputest::StrNCmp("teststring", "tester", 7);
  LONGS_EQUAL('s' - 'e', result);
}

TEST(String, StrNCmp_n_too_large)
{
  int result = cpputest::StrNCmp("teststring", "teststring", 20);
  LONGS_EQUAL(0, result);
}

TEST(String, StrNCmp_s1_empty)
{
  int result = cpputest::StrNCmp("", "foo", 2);
  LONGS_EQUAL(0 - 'f', result);
}

TEST(String, StrNCmp_s2_empty)
{
  int result = cpputest::StrNCmp("foo", "", 2);
  LONGS_EQUAL('f', result);
}

TEST(String, StrNCmp_s1_and_s2_empty)
{
  int result = cpputest::StrNCmp("", "", 2);
  LONGS_EQUAL(0, result);
}

TEST(String, AtoI)
{
  char max_short_str[] = "32767";
  char min_short_str[] = "-32768";

  CHECK(12345 == cpputest::AtoI("012345"));
  CHECK(6789 == cpputest::AtoI("6789"));
  CHECK(12345 == cpputest::AtoI("12345/"));
  CHECK(12345 == cpputest::AtoI("12345:"));
  CHECK(-12345 == cpputest::AtoI("-12345"));
  CHECK(123 == cpputest::AtoI("\t \r\n123"));
  CHECK(123 == cpputest::AtoI("123-foo"));
  CHECK(0 == cpputest::AtoI("-foo"));
  CHECK(-32768 == cpputest::AtoI(min_short_str));
  CHECK(32767 == cpputest::AtoI(max_short_str));
}

TEST(String, AtoU)
{
  char max_short_str[] = "65535";
  CHECK(12345 == cpputest::AtoU("012345"));
  CHECK(6789 == cpputest::AtoU("6789"));
  CHECK(12345 == cpputest::AtoU("12345/"));
  CHECK(12345 == cpputest::AtoU("12345:"));
  CHECK(123 == cpputest::AtoU("\t \r\n123"));
  CHECK(123 == cpputest::AtoU("123-foo"));
  CHECK(65535 == cpputest::AtoU(max_short_str));
  CHECK(0 == cpputest::AtoU("foo"));
  CHECK(0 == cpputest::AtoU("-foo"));
  CHECK(0 == cpputest::AtoU("+1"));
  CHECK(0 == cpputest::AtoU("-1"));
  CHECK(0 == cpputest::AtoU("0"));
}

TEST(String, Binary)
{
  const unsigned char value[] = { 0x00, 0x01, 0x2A, 0xFF };
  const char expectedString[] = "00 01 2A FF";

  STRCMP_EQUAL(
      expectedString, cpputest::StringFromBinary(value, sizeof(value)).c_str());
  STRCMP_EQUAL(expectedString,
      cpputest::StringFromBinaryOrNull(value, sizeof(value)).c_str());
  STRCMP_EQUAL("", cpputest::StringFromBinary(value, 0).c_str());
  STRCMP_EQUAL("(null)", cpputest::StringFromBinaryOrNull(nullptr, 0).c_str());
}

TEST(String, BinaryWithSize)
{
  const unsigned char value[] = { 0x12, 0xFE, 0xA1 };
  const char expectedString[] = "Size = 3 | HexContents = 12 FE A1";

  STRCMP_EQUAL(expectedString,
      cpputest::StringFromBinaryWithSize(value, sizeof(value)).c_str());
  STRCMP_EQUAL(expectedString,
      cpputest::StringFromBinaryWithSizeOrNull(value, sizeof(value)).c_str());
  STRCMP_EQUAL("Size = 0 | HexContents = ",
      cpputest::StringFromBinaryWithSize(value, 0).c_str());
  STRCMP_EQUAL(
      "(null)", cpputest::StringFromBinaryWithSizeOrNull(nullptr, 0).c_str());
}

TEST(String, BinaryWithSizeLargerThan128)
{
  unsigned char value[129] = {};
  value[127] = 0x00;
  value[128] = 0xff;

  STRCMP_CONTAINS("00 ...",
      cpputest::StringFromBinaryWithSize(value, sizeof(value)).c_str());
}

TEST(String, MemCmp)
{
  unsigned char smaller[] = { 0x00, 0x01, 0x2A, 0xFF };
  unsigned char greater[] = { 0x00, 0x01, 0xFF, 0xFF };

  LONGS_EQUAL(0, cpputest::MemCmp(smaller, smaller, sizeof(smaller)));
  CHECK(cpputest::MemCmp(smaller, greater, sizeof(smaller)) < 0);
  CHECK(cpputest::MemCmp(greater, smaller, sizeof(smaller)) > 0);
  LONGS_EQUAL(0, cpputest::MemCmp(nullptr, nullptr, 0));
}

TEST(String, MemCmpFirstLastNotMatching)
{
  unsigned char base[] = { 0x00, 0x01, 0x2A, 0xFF };
  unsigned char firstNotMatching[] = { 0x01, 0x01, 0x2A, 0xFF };
  unsigned char lastNotMatching[] = { 0x00, 0x01, 0x2A, 0x00 };
  CHECK(0 != cpputest::MemCmp(base, firstNotMatching, sizeof(base)));
  CHECK(0 != cpputest::MemCmp(base, lastNotMatching, sizeof(base)));
}

TEST(String, StringFromOrdinalNumberOnes)
{
  STRCMP_EQUAL("2nd", cpputest::StringFromOrdinalNumber(2).c_str());
  STRCMP_EQUAL("3rd", cpputest::StringFromOrdinalNumber(3).c_str());
  STRCMP_EQUAL("4th", cpputest::StringFromOrdinalNumber(4).c_str());
  STRCMP_EQUAL("5th", cpputest::StringFromOrdinalNumber(5).c_str());
  STRCMP_EQUAL("6th", cpputest::StringFromOrdinalNumber(6).c_str());
  STRCMP_EQUAL("7th", cpputest::StringFromOrdinalNumber(7).c_str());
}

TEST(String, StringFromOrdinalNumberTens)
{
  STRCMP_EQUAL("10th", cpputest::StringFromOrdinalNumber(10).c_str());
  STRCMP_EQUAL("11th", cpputest::StringFromOrdinalNumber(11).c_str());
  STRCMP_EQUAL("12th", cpputest::StringFromOrdinalNumber(12).c_str());
  STRCMP_EQUAL("13th", cpputest::StringFromOrdinalNumber(13).c_str());
  STRCMP_EQUAL("14th", cpputest::StringFromOrdinalNumber(14).c_str());
  STRCMP_EQUAL("18th", cpputest::StringFromOrdinalNumber(18).c_str());
}

TEST(String, StringFromOrdinalNumberOthers)
{
  STRCMP_EQUAL("21st", cpputest::StringFromOrdinalNumber(21).c_str());
  STRCMP_EQUAL("22nd", cpputest::StringFromOrdinalNumber(22).c_str());
  STRCMP_EQUAL("23rd", cpputest::StringFromOrdinalNumber(23).c_str());
  STRCMP_EQUAL("24th", cpputest::StringFromOrdinalNumber(24).c_str());
  STRCMP_EQUAL("32nd", cpputest::StringFromOrdinalNumber(32).c_str());
  STRCMP_EQUAL("100th", cpputest::StringFromOrdinalNumber(100).c_str());
  STRCMP_EQUAL("101st", cpputest::StringFromOrdinalNumber(101).c_str());
}

TEST(String, BracketsFormattedHexStringFromForSignedChar)
{
  signed char value = 'c';

  STRCMP_EQUAL(
      "(0x63)", cpputest::BracketsFormattedHexStringFrom(value).c_str());
}

TEST(String, BracketsFormattedHexStringFromForUnsignedInt)
{
  unsigned int value = 1;

  STRCMP_EQUAL(
      "(0x1)", cpputest::BracketsFormattedHexStringFrom(value).c_str());
}

TEST(String, BracketsFormattedHexStringFromForUnsignedLong)
{
  unsigned long value = 1;

  STRCMP_EQUAL(
      "(0x1)", cpputest::BracketsFormattedHexStringFrom(value).c_str());
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
  STRCMP_EQUAL(
      "(0xffffffff)", cpputest::BracketsFormattedHexStringFrom(value).c_str());
}
#endif

TEST(String, BracketsFormattedHexStringFromForLong)
{
  long value = 1;

  STRCMP_EQUAL(
      "(0x1)", cpputest::BracketsFormattedHexStringFrom(value).c_str());
}
TEST(String, BracketsFormattedHexStringFromForLongLong)
{
  long long value = 1;

  STRCMP_EQUAL(
      "(0x1)", cpputest::BracketsFormattedHexStringFrom(value).c_str());
}
TEST(String, BracketsFormattedHexStringFromForULongLong)
{
  unsigned long long value = 1;

  STRCMP_EQUAL(
      "(0x1)", cpputest::BracketsFormattedHexStringFrom(value).c_str());
}
