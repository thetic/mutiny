#include "mu/tiny/test/ShellPointerArray.hpp"
#include "mu/tiny/test/SkippedShell.hpp"

#include "mu/tiny/test.hpp"

namespace {
int get_zero()
{
  return 0;
}

int get_one()
{
  return 1;
}
} // namespace

TEST_GROUP(ShellPointerArray)
{
  mu::tiny::test::Shell* test0;
  mu::tiny::test::Shell* test1;
  mu::tiny::test::Shell* test2;

  void setup() override
  {
    test0 = new mu::tiny::test::SkippedShell();
    test1 = new mu::tiny::test::SkippedShell();
    test2 = new mu::tiny::test::SkippedShell();

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

TEST(ShellPointerArray, empty)
{
  mu::tiny::test::ShellPointerArray tests(nullptr);
  tests.shuffle(0);
  CHECK(nullptr == tests.get_first_test());
}

TEST(ShellPointerArray, testsAreInOrder)
{
  mu::tiny::test::ShellPointerArray tests(test0);
  CHECK(tests.get(0) == test0);
  CHECK(tests.get(1) == test1);
  CHECK(tests.get(2) == test2);
}

TEST(ShellPointerArray, relinkingTestsWillKeepThemTheSameWhenNothingWasDone)
{
  mu::tiny::test::ShellPointerArray tests(test0);
  tests.relink_tests_in_order();
  CHECK(tests.get(0) == test0);
  CHECK(tests.get(1) == test1);
  CHECK(tests.get(2) == test2);
}

TEST(ShellPointerArray, firstTestisNotTheFirstTestWithSeed1234)
{
  MUTINY_PTR_SET(mu::tiny::test::rand, get_zero);

  mu::tiny::test::ShellPointerArray tests(test0);
  tests.shuffle(1234);
  CHECK(tests.get_first_test() != test0);
}

TEST(ShellPointerArray, ShuffleListTestWithRandomAlwaysReturningZero)
{
  MUTINY_PTR_SET(mu::tiny::test::rand, get_zero);

  mu::tiny::test::ShellPointerArray tests(test0);
  tests.shuffle(3);
  CHECK(tests.get(0) == test1);
  CHECK(tests.get(1) == test2);
  CHECK(tests.get(2) == test0);
}

// swaps with 4 mod 3 (1) then 4 mod 2 (0): 1, [2], [0] --> [1], [0], 2 --> 0,
// 1, 2
TEST(ShellPointerArray, ShuffleListTestWithRandomAlwaysReturningOne)
{
  MUTINY_PTR_SET(mu::tiny::test::rand, get_one);

  mu::tiny::test::ShellPointerArray tests(test0);
  tests.shuffle(3);
  CHECK(tests.get(0) == test0);
  CHECK(tests.get(1) == test2);
  CHECK(tests.get(2) == test1);
}

TEST(ShellPointerArray, reverse)
{
  MUTINY_PTR_SET(mu::tiny::test::rand, get_one);

  mu::tiny::test::ShellPointerArray tests(test0);
  tests.reverse();
  CHECK(tests.get(0) == test2);
  CHECK(tests.get(1) == test1);
  CHECK(tests.get(2) == test0);
}
