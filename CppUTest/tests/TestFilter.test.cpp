#include "CppUTest/TestFilter.hpp"

#include "CppUTest/CppUTest.hpp"

TEST_GROUP(TestFilter)
{};

TEST(TestFilter, emptyFilterMatchesEverything)
{
  cpputest::TestFilter filter;
  CHECK(filter.match("random_name"));
  CHECK(filter.match(""));
  CHECK(filter.match("*&%#^&%$(*&^@#(&*@#^(&*$^@#"));
}

TEST(TestFilter, defaultAbsoluteMismatches)
{
  cpputest::TestFilter filter("filtername");
  CHECK(!filter.match("notevenclose"));
  CHECK(!filter.match("filterrname"));
  CHECK(!filter.match(""));
}

TEST(TestFilter, strictMatching)
{
  cpputest::TestFilter filter("filter");
  filter.strict_matching();
  CHECK(filter.match("filter"));
  CHECK(!filter.match("filterr"));
  CHECK(!filter.match(" filter"));
}

TEST(TestFilter, invertMatching)
{
  cpputest::TestFilter filter("filter");
  filter.invert_matching();
  CHECK(!filter.match("filter"));
  CHECK(!filter.match("filterr"));
  CHECK(filter.match("notevenclose"));
  CHECK(filter.match(""));
}

TEST(TestFilter, invertStrictMatching)
{
  cpputest::TestFilter filter("filter");
  filter.invert_matching();
  filter.strict_matching();
  CHECK(!filter.match("filter"));
  CHECK(filter.match("filterr"));
  CHECK(filter.match(" filter"));
}

TEST(TestFilter, equality)
{
  cpputest::TestFilter filter1("filter");
  cpputest::TestFilter filter2("filter");
  cpputest::TestFilter filter3("filter3");
  CHECK(filter1 == filter2);
  CHECK(!(filter1 == filter3));
}

TEST(TestFilter, equalityWithStrictness)
{
  cpputest::TestFilter filter1("filter");
  cpputest::TestFilter filter2("filter");
  filter2.strict_matching();
  CHECK(!(filter1 == filter2));
}

TEST(TestFilter, equalityWithInvertion)
{
  cpputest::TestFilter filter1("filter");
  cpputest::TestFilter filter2("filter");
  filter2.invert_matching();
  CHECK(!(filter1 == filter2));
}

TEST(TestFilter, notEqual)
{
  cpputest::TestFilter filter1("filter");
  cpputest::TestFilter filter2("filter");
  cpputest::TestFilter filter3("filter3");
  CHECK(filter1 != filter3);
  CHECK(!(filter1 != filter2));
}

TEST(TestFilter, stringFrom)
{
  cpputest::TestFilter filter("filter");
  STRCMP_EQUAL("TestFilter: \"filter\"", string_from(filter).c_str());
}

TEST(TestFilter, stringFromWithStrictMatching)
{
  cpputest::TestFilter filter("filter");
  filter.strict_matching();
  STRCMP_EQUAL("TestFilter: \"filter\" with strict matching",
      string_from(filter).c_str());
}

TEST(TestFilter, stringFromWithInvertMatching)
{
  cpputest::TestFilter filter("filter");
  filter.invert_matching();
  STRCMP_EQUAL("TestFilter: \"filter\" with invert matching",
      string_from(filter).c_str());
}

TEST(TestFilter, stringFromWithStrictInvertMatching)
{
  cpputest::TestFilter filter("filter");
  filter.strict_matching();
  filter.invert_matching();
  STRCMP_EQUAL("TestFilter: \"filter\" with strict, invert matching",
      string_from(filter).c_str());
}

TEST(TestFilter, listOfFilters)
{
  cpputest::TestFilter* list_of_filters = nullptr;
  cpputest::TestFilter first("foo");
  cpputest::TestFilter secnd("bar");
  list_of_filters = first.add(list_of_filters);
  list_of_filters = secnd.add(list_of_filters);
  cpputest::TestFilter* current = list_of_filters;
  STRCMP_EQUAL("TestFilter: \"bar\"", string_from(*current).c_str());
  current = current->get_next();
  STRCMP_EQUAL("TestFilter: \"foo\"", string_from(*current).c_str());
  POINTERS_EQUAL(nullptr, current->get_next());
}

TEST(TestFilter, constructors)
{
  cpputest::TestFilter filter1;
  cpputest::TestFilter filter2(cpputest::String("a"));
  cpputest::TestFilter filter3("a");
  CHECK(filter1.get_next() == nullptr);
  CHECK(filter2.get_next() == nullptr);
  CHECK(filter3.get_next() == nullptr);
  CHECK(filter2.match("ab"));
  CHECK(filter3.match("ab"));
}
