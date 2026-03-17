#include "OrderedTest.h"

#include "CppMu/CppMu.h"

TEST(OrderedTestC, Test11)
{
  CHECK(ordered_test_fixture_c_wrapper() == 13);
}

TEST(OrderedTestC, Test7)
{
  CHECK(ordered_test_fixture_c_wrapper() == 9);
}

TEST(OrderedTestC, Test9)
{
  CHECK(ordered_test_fixture_c_wrapper() == 11);
}
