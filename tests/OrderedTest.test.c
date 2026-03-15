#include "OrderedTest.h"

#include "CppMu/CppMu.h"

TEST_C(OrderedTestC, Test11)
{
  CHECK_C(ordered_test_fixture_c_wrapper() == 13);
}

TEST_C(OrderedTestC, Test7)
{
  CHECK_C(ordered_test_fixture_c_wrapper() == 9);
}

TEST_C(OrderedTestC, Test9)
{
  CHECK_C(ordered_test_fixture_c_wrapper() == 11);
}
