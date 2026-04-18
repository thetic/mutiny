#include "mu/tiny/test/Filter.hpp"

#include "mu/tiny/test.hpp"

TEST_GROUP(Filter)
{};

TEST(Filter, emptyFilterMatchesEverything)
{
  mu::tiny::test::Filter filter;
  CHECK(filter.match("random_name"));
  CHECK(filter.match(""));
  CHECK(filter.match("*&%#^&%$(*&^@#(&*@#^(&*$^@#"));
}

TEST(Filter, defaultAbsoluteMismatches)
{
  mu::tiny::test::Filter filter("filtername");
  CHECK(!filter.match("notevenclose"));
  CHECK(!filter.match("filterrname"));
  CHECK(!filter.match(""));
}

TEST(Filter, strictMatching)
{
  mu::tiny::test::Filter filter("filter");
  filter.strict_matching();
  CHECK(filter.match("filter"));
  CHECK(!filter.match("filterr"));
  CHECK(!filter.match(" filter"));
}

TEST(Filter, invertMatching)
{
  mu::tiny::test::Filter filter("filter");
  filter.invert_matching();
  CHECK(!filter.match("filter"));
  CHECK(!filter.match("filterr"));
  CHECK(filter.match("notevenclose"));
  CHECK(filter.match(""));
}

TEST(Filter, invertStrictMatching)
{
  mu::tiny::test::Filter filter("filter");
  filter.invert_matching();
  filter.strict_matching();
  CHECK(!filter.match("filter"));
  CHECK(filter.match("filterr"));
  CHECK(filter.match(" filter"));
}

TEST(Filter, equality)
{
  mu::tiny::test::Filter filter1("filter");
  mu::tiny::test::Filter filter2("filter");
  mu::tiny::test::Filter filter3("filter3");
  CHECK(filter1 == filter2);
  CHECK(!(filter1 == filter3));
}

TEST(Filter, equalityWithStrictness)
{
  mu::tiny::test::Filter filter1("filter");
  mu::tiny::test::Filter filter2("filter");
  filter2.strict_matching();
  CHECK(!(filter1 == filter2));
}

TEST(Filter, equalityWithInvertion)
{
  mu::tiny::test::Filter filter1("filter");
  mu::tiny::test::Filter filter2("filter");
  filter2.invert_matching();
  CHECK(!(filter1 == filter2));
}

TEST(Filter, notEqual)
{
  mu::tiny::test::Filter filter1("filter");
  mu::tiny::test::Filter filter2("filter");
  mu::tiny::test::Filter filter3("filter3");
  CHECK(filter1 != filter3);
  CHECK(!(filter1 != filter2));
}

TEST(Filter, stringFrom)
{
  mu::tiny::test::Filter filter("filter");
  STRCMP_EQUAL("Filter: \"filter\"", mu::tiny::string_from(filter).c_str());
}

TEST(Filter, stringFromWithStrictMatching)
{
  mu::tiny::test::Filter filter("filter");
  filter.strict_matching();
  STRCMP_EQUAL(
      "Filter: \"filter\" with strict matching",
      mu::tiny::string_from(filter).c_str()
  );
}

TEST(Filter, stringFromWithInvertMatching)
{
  mu::tiny::test::Filter filter("filter");
  filter.invert_matching();
  STRCMP_EQUAL(
      "Filter: \"filter\" with invert matching",
      mu::tiny::string_from(filter).c_str()
  );
}

TEST(Filter, stringFromWithStrictInvertMatching)
{
  mu::tiny::test::Filter filter("filter");
  filter.strict_matching();
  filter.invert_matching();
  STRCMP_EQUAL(
      "Filter: \"filter\" with strict, invert matching",
      mu::tiny::string_from(filter).c_str()
  );
}

TEST(Filter, listOfFilters)
{
  mu::tiny::test::Filter* list_of_filters = nullptr;
  mu::tiny::test::Filter first("foo");
  mu::tiny::test::Filter secnd("bar");
  list_of_filters = first.add(list_of_filters);
  list_of_filters = secnd.add(list_of_filters);
  mu::tiny::test::Filter* current = list_of_filters;
  STRCMP_EQUAL("Filter: \"bar\"", mu::tiny::string_from(*current).c_str());
  current = current->get_next();
  STRCMP_EQUAL("Filter: \"foo\"", mu::tiny::string_from(*current).c_str());
  CHECK_EQUAL(nullptr, current->get_next());
}

TEST(Filter, constructors)
{
  mu::tiny::test::Filter filter1;
  mu::tiny::test::Filter filter2(mu::tiny::String("a"));
  mu::tiny::test::Filter filter3("a");
  CHECK(filter1.get_next() == nullptr);
  CHECK(filter2.get_next() == nullptr);
  CHECK(filter3.get_next() == nullptr);
  CHECK(filter2.match("ab"));
  CHECK(filter3.match("ab"));
}
