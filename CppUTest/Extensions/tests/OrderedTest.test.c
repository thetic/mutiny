#include "OrderedTestTest.h"

#include "CppUTest/TestHarness.h"

TEST_C(OrderedTestC, Test11)
{
  CHECK_C(orderedTestFixtureCWrapper() == 13);
}

TEST_C(OrderedTestC, Test7)
{
  CHECK_C(orderedTestFixtureCWrapper() == 9);
}

TEST_C(OrderedTestC, Test9)
{
  CHECK_C(orderedTestFixtureCWrapper() == 11);
}
