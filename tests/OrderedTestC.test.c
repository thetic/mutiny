#include "OrderedTest.h"

#include "CppMu/CppMu.h"

TEST(OrderedTestC, Test7)
{
  CHECK(cppmu_ordered_test_fixture_c_wrapper() == 9);
}

TEST(OrderedTestC, Test9)
{
  CHECK(cppmu_ordered_test_fixture_c_wrapper() == 11);
}

TEST(OrderedTestC, Test11)
{
  CHECK(cppmu_ordered_test_fixture_c_wrapper() == 13);
}
