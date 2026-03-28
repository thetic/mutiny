#include "OrderedTest.h"

#include "mutiny/test.h"

TEST(OrderedTestC, Test7)
{
  CHECK(mutiny_ordered_test_fixture_c_wrapper() == 9);
}

TEST(OrderedTestC, Test9)
{
  CHECK(mutiny_ordered_test_fixture_c_wrapper() == 11);
}

TEST(OrderedTestC, Test11)
{
  CHECK(mutiny_ordered_test_fixture_c_wrapper() == 13);
}
