#include "mu/tiny/StringView.hpp"

#include "mu/tiny/String.hpp"
#include "mu/tiny/test.hpp"

TEST_GROUP(StringView){};

TEST(StringView, ConstructFromLiteral)
{
  mu::tiny::StringView sv("hello");

  STRCMP_EQUAL("hello", sv.data());
  CHECK_EQUAL(5U, sv.size());
}

TEST(StringView, ConstructFromPointerAndLength)
{
  mu::tiny::StringView sv("hello world", 5);

  CHECK_EQUAL(5U, sv.size());
  CHECK_EQUAL('h', sv[0]);
  CHECK_EQUAL('o', sv[4]);
}

TEST(StringView, ConstructFromString)
{
  mu::tiny::String str("test");
  mu::tiny::StringView sv(str);

  STRCMP_EQUAL("test", sv.data());
  CHECK_EQUAL(str.size(), sv.size());
}

TEST(StringView, EmptyLiteral)
{
  mu::tiny::StringView sv("");

  CHECK(sv.empty());
  CHECK_EQUAL(0U, sv.size());
}

TEST(StringView, NonEmpty)
{
  mu::tiny::StringView sv("x");

  CHECK(!sv.empty());
}

TEST(StringView, EqualViews)
{
  mu::tiny::StringView a("abc");
  mu::tiny::StringView b("abc");

  CHECK(a == b);
  CHECK(!(a != b));
}

TEST(StringView, UnequalViews)
{
  mu::tiny::StringView a("abc");
  mu::tiny::StringView b("xyz");

  CHECK(a != b);
  CHECK(!(a == b));
}

TEST(StringView, UnequalLengths)
{
  mu::tiny::StringView a("abc");
  mu::tiny::StringView b("ab");

  CHECK(a != b);
}

TEST(StringView, CompareWithLiteral)
{
  mu::tiny::StringView sv("hello");

  CHECK(sv == "hello");
  CHECK("hello" == sv);
  CHECK(sv != "world");
  CHECK("world" != sv);
}

TEST(StringView, CompareWithString)
{
  mu::tiny::String str("test");
  mu::tiny::StringView sv("test");

  CHECK(sv == str);
  CHECK(str == sv);
  CHECK(!(sv != str));
}

TEST(StringView, StringFromStringView)
{
  mu::tiny::StringView sv("convert me");
  mu::tiny::String str = mu::tiny::string_from(sv);

  STRCMP_EQUAL("convert me", str.c_str());
}

#if !MUTINY_USE_STD_STRING
TEST(StringView, StringPlusEqualsStringView)
{
  mu::tiny::String str("hello");
  mu::tiny::StringView sv(" world");

  str += sv;

  STRCMP_EQUAL("hello world", str.c_str());
}

TEST(StringView, StringConstructFromStringView)
{
  mu::tiny::StringView sv("copy me");
  mu::tiny::String str(sv);

  STRCMP_EQUAL("copy me", str.c_str());
  CHECK_EQUAL(7U, str.size());
}
#endif

TEST(StringView, StringConstructFromPointerAndLength)
{
  mu::tiny::String str("hello", 5);

  STRCMP_EQUAL("hello", str.c_str());
  CHECK_EQUAL(5U, str.size());
}
