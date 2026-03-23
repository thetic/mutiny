#include "mutiny/test.hpp"
#include "mutiny/test/Filter.hpp"

TEST_GROUP(TestFilter)
{};

TEST(TestFilter, emptyFilterMatchesEverything)
{
  mu::tiny::test::TestFilter filter;
  CHECK(filter.match("random_name"));
  CHECK(filter.match(""));
  CHECK(filter.match("*&%#^&%$(*&^@#(&*@#^(&*$^@#"));
}

TEST(TestFilter, defaultAbsoluteMismatches)
{
  mu::tiny::test::TestFilter filter("filtername");
  CHECK(!filter.match("notevenclose"));
  CHECK(!filter.match("filterrname"));
  CHECK(!filter.match(""));
}

TEST(TestFilter, strictMatching)
{
  mu::tiny::test::TestFilter filter("filter");
  filter.strict_matching();
  CHECK(filter.match("filter"));
  CHECK(!filter.match("filterr"));
  CHECK(!filter.match(" filter"));
}

TEST(TestFilter, invertMatching)
{
  mu::tiny::test::TestFilter filter("filter");
  filter.invert_matching();
  CHECK(!filter.match("filter"));
  CHECK(!filter.match("filterr"));
  CHECK(filter.match("notevenclose"));
  CHECK(filter.match(""));
}

TEST(TestFilter, invertStrictMatching)
{
  mu::tiny::test::TestFilter filter("filter");
  filter.invert_matching();
  filter.strict_matching();
  CHECK(!filter.match("filter"));
  CHECK(filter.match("filterr"));
  CHECK(filter.match(" filter"));
}

TEST(TestFilter, equality)
{
  mu::tiny::test::TestFilter filter1("filter");
  mu::tiny::test::TestFilter filter2("filter");
  mu::tiny::test::TestFilter filter3("filter3");
  CHECK(filter1 == filter2);
  CHECK(!(filter1 == filter3));
}

TEST(TestFilter, equalityWithStrictness)
{
  mu::tiny::test::TestFilter filter1("filter");
  mu::tiny::test::TestFilter filter2("filter");
  filter2.strict_matching();
  CHECK(!(filter1 == filter2));
}

TEST(TestFilter, equalityWithInvertion)
{
  mu::tiny::test::TestFilter filter1("filter");
  mu::tiny::test::TestFilter filter2("filter");
  filter2.invert_matching();
  CHECK(!(filter1 == filter2));
}

TEST(TestFilter, notEqual)
{
  mu::tiny::test::TestFilter filter1("filter");
  mu::tiny::test::TestFilter filter2("filter");
  mu::tiny::test::TestFilter filter3("filter3");
  CHECK(filter1 != filter3);
  CHECK(!(filter1 != filter2));
}

TEST(TestFilter, stringFrom)
{
  mu::tiny::test::TestFilter filter("filter");
  STRCMP_EQUAL(
      "TestFilter: \"filter\"", mu::tiny::test::string_from(filter).c_str()
  );
}

TEST(TestFilter, stringFromWithStrictMatching)
{
  mu::tiny::test::TestFilter filter("filter");
  filter.strict_matching();
  STRCMP_EQUAL(
      "TestFilter: \"filter\" with strict matching",
      mu::tiny::test::string_from(filter).c_str()
  );
}

TEST(TestFilter, stringFromWithInvertMatching)
{
  mu::tiny::test::TestFilter filter("filter");
  filter.invert_matching();
  STRCMP_EQUAL(
      "TestFilter: \"filter\" with invert matching",
      mu::tiny::test::string_from(filter).c_str()
  );
}

TEST(TestFilter, stringFromWithStrictInvertMatching)
{
  mu::tiny::test::TestFilter filter("filter");
  filter.strict_matching();
  filter.invert_matching();
  STRCMP_EQUAL(
      "TestFilter: \"filter\" with strict, invert matching",
      mu::tiny::test::string_from(filter).c_str()
  );
}

TEST(TestFilter, listOfFilters)
{
  mu::tiny::test::TestFilter* list_of_filters = nullptr;
  mu::tiny::test::TestFilter first("foo");
  mu::tiny::test::TestFilter secnd("bar");
  list_of_filters = first.add(list_of_filters);
  list_of_filters = secnd.add(list_of_filters);
  mu::tiny::test::TestFilter* current = list_of_filters;
  STRCMP_EQUAL(
      "TestFilter: \"bar\"", mu::tiny::test::string_from(*current).c_str()
  );
  current = current->get_next();
  STRCMP_EQUAL(
      "TestFilter: \"foo\"", mu::tiny::test::string_from(*current).c_str()
  );
  POINTERS_EQUAL(nullptr, current->get_next());
}

TEST(TestFilter, constructors)
{
  mu::tiny::test::TestFilter filter1;
  mu::tiny::test::TestFilter filter2(mu::tiny::test::String("a"));
  mu::tiny::test::TestFilter filter3("a");
  CHECK(filter1.get_next() == nullptr);
  CHECK(filter2.get_next() == nullptr);
  CHECK(filter3.get_next() == nullptr);
  CHECK(filter2.match("ab"));
  CHECK(filter3.match("ab"));
}
