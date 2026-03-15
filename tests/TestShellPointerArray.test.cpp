#include "CppMu/TestShellPointerArray.hpp"

#include "CppMu/CppMu.hpp"
#include "CppMu/IgnoredTestShell.hpp"

namespace {
int
get_zero()
{
  return 0;
}

int
get_one()
{
  return 1;
}
}

TEST_GROUP(TestShellPointerArray)
{
  cppmu::TestShell* test0;
  cppmu::TestShell* test1;
  cppmu::TestShell* test2;

  void setup() override
  {
    test0 = new cppmu::IgnoredTestShell();
    test1 = new cppmu::IgnoredTestShell();
    test2 = new cppmu::IgnoredTestShell();

    test0->add_test(test1);
    test1->add_test(test2);
  }

  void teardown() override
  {
    delete test0;
    delete test1;
    delete test2;
  }
};

TEST(TestShellPointerArray, empty)
{
  cppmu::TestShellPointerArray tests(nullptr);
  tests.shuffle(0);
  CHECK(nullptr == tests.get_first_test());
}

TEST(TestShellPointerArray, testsAreInOrder)
{
  cppmu::TestShellPointerArray tests(test0);
  CHECK(tests.get(0) == test0);
  CHECK(tests.get(1) == test1);
  CHECK(tests.get(2) == test2);
}

TEST(TestShellPointerArray, relinkingTestsWillKeepThemTheSameWhenNothingWasDone)
{
  cppmu::TestShellPointerArray tests(test0);
  tests.relink_tests_in_order();
  CHECK(tests.get(0) == test0);
  CHECK(tests.get(1) == test1);
  CHECK(tests.get(2) == test2);
}

TEST(TestShellPointerArray, firstTestisNotTheFirstTestWithSeed1234)
{
  CPPMU_PTR_SET(cppmu::rand, get_zero);

  cppmu::TestShellPointerArray tests(test0);
  tests.shuffle(1234);
  CHECK(tests.get_first_test() != test0);
}

TEST(TestShellPointerArray, ShuffleListTestWithRandomAlwaysReturningZero)
{
  CPPMU_PTR_SET(cppmu::rand, get_zero);

  cppmu::TestShellPointerArray tests(test0);
  tests.shuffle(3);
  CHECK(tests.get(0) == test1);
  CHECK(tests.get(1) == test2);
  CHECK(tests.get(2) == test0);
}

// swaps with 4 mod 3 (1) then 4 mod 2 (0): 1, [2], [0] --> [1], [0], 2 --> 0,
// 1, 2
TEST(TestShellPointerArray, ShuffleListTestWithRandomAlwaysReturningOne)
{
  CPPMU_PTR_SET(cppmu::rand, get_one);

  cppmu::TestShellPointerArray tests(test0);
  tests.shuffle(3);
  CHECK(tests.get(0) == test0);
  CHECK(tests.get(1) == test2);
  CHECK(tests.get(2) == test1);
}

TEST(TestShellPointerArray, reverse)
{
  CPPMU_PTR_SET(cppmu::rand, get_one);

  cppmu::TestShellPointerArray tests(test0);
  tests.reverse();
  CHECK(tests.get(0) == test2);
  CHECK(tests.get(1) == test1);
  CHECK(tests.get(2) == test0);
}
