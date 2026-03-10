#include "CppUTest/TestFilter.hpp"

#include "CppUTest/TestHarness.hpp"

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
  filter.strictMatching();
  CHECK(filter.match("filter"));
  CHECK(!filter.match("filterr"));
  CHECK(!filter.match(" filter"));
}

TEST(TestFilter, invertMatching)
{
  cpputest::TestFilter filter("filter");
  filter.invertMatching();
  CHECK(!filter.match("filter"));
  CHECK(!filter.match("filterr"));
  CHECK(filter.match("notevenclose"));
  CHECK(filter.match(""));
}

TEST(TestFilter, invertStrictMatching)
{
  cpputest::TestFilter filter("filter");
  filter.invertMatching();
  filter.strictMatching();
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
  filter2.strictMatching();
  CHECK(!(filter1 == filter2));
}

TEST(TestFilter, equalityWithInvertion)
{
  cpputest::TestFilter filter1("filter");
  cpputest::TestFilter filter2("filter");
  filter2.invertMatching();
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
  STRCMP_EQUAL("TestFilter: \"filter\"", StringFrom(filter).c_str());
}

TEST(TestFilter, stringFromWithStrictMatching)
{
  cpputest::TestFilter filter("filter");
  filter.strictMatching();
  STRCMP_EQUAL("TestFilter: \"filter\" with strict matching",
      StringFrom(filter).c_str());
}

TEST(TestFilter, stringFromWithInvertMatching)
{
  cpputest::TestFilter filter("filter");
  filter.invertMatching();
  STRCMP_EQUAL("TestFilter: \"filter\" with invert matching",
      StringFrom(filter).c_str());
}

TEST(TestFilter, stringFromWithStrictInvertMatching)
{
  cpputest::TestFilter filter("filter");
  filter.strictMatching();
  filter.invertMatching();
  STRCMP_EQUAL("TestFilter: \"filter\" with strict, invert matching",
      StringFrom(filter).c_str());
}

TEST(TestFilter, listOfFilters)
{
  cpputest::TestFilter* listOfFilters = nullptr;
  cpputest::TestFilter first("foo");
  cpputest::TestFilter secnd("bar");
  listOfFilters = first.add(listOfFilters);
  listOfFilters = secnd.add(listOfFilters);
  cpputest::TestFilter* current = listOfFilters;
  STRCMP_EQUAL("TestFilter: \"bar\"", StringFrom(*current).c_str());
  current = current->getNext();
  STRCMP_EQUAL("TestFilter: \"foo\"", StringFrom(*current).c_str());
  POINTERS_EQUAL(nullptr, current->getNext());
}

TEST(TestFilter, constructors)
{
  cpputest::TestFilter filter1;
  cpputest::TestFilter filter2(cpputest::String("a"));
  cpputest::TestFilter filter3("a");
  CHECK(filter1.getNext() == nullptr);
  CHECK(filter2.getNext() == nullptr);
  CHECK(filter3.getNext() == nullptr);
  CHECK(filter2.match("ab"));
  CHECK(filter3.match("ab"));
}
