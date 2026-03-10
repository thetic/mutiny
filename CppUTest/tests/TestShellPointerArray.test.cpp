#include "CppUTest/TestShellPointerArray.hpp"

#include "CppUTest/IgnoredTestShell.hpp"
#include "CppUTest/PlatformSpecificFunctions.hpp"
#include "CppUTest/TestHarness.hpp"

static int
getZero()
{
  return 0;
}

static int
getOne()
{
  return 1;
}

TEST_GROUP(UtestShellPointerArrayTest)
{
  cpputest::TestShell* test0;
  cpputest::TestShell* test1;
  cpputest::TestShell* test2;

  void setup() override
  {
    test0 = new cpputest::IgnoredTestShell();
    test1 = new cpputest::IgnoredTestShell();
    test2 = new cpputest::IgnoredTestShell();

    test0->addTest(test1);
    test1->addTest(test2);
  }

  void teardown() override
  {
    delete test0;
    delete test1;
    delete test2;
  }
};

TEST(UtestShellPointerArrayTest, empty)
{
  cpputest::TestShellPointerArray tests(nullptr);
  tests.shuffle(0);
  CHECK(nullptr == tests.getFirstTest());
}

TEST(UtestShellPointerArrayTest, testsAreInOrder)
{
  cpputest::TestShellPointerArray tests(test0);
  CHECK(tests.get(0) == test0);
  CHECK(tests.get(1) == test1);
  CHECK(tests.get(2) == test2);
}

TEST(UtestShellPointerArrayTest,
    relinkingTestsWillKeepThemTheSameWhenNothingWasDone)
{
  cpputest::TestShellPointerArray tests(test0);
  tests.relinkTestsInOrder();
  CHECK(tests.get(0) == test0);
  CHECK(tests.get(1) == test1);
  CHECK(tests.get(2) == test2);
}

TEST(UtestShellPointerArrayTest, firstTestisNotTheFirstTestWithSeed1234)
{
  UT_PTR_SET(PlatformSpecificRand, getZero);

  cpputest::TestShellPointerArray tests(test0);
  tests.shuffle(1234);
  CHECK(tests.getFirstTest() != test0);
}

TEST(UtestShellPointerArrayTest, ShuffleListTestWithRandomAlwaysReturningZero)
{
  UT_PTR_SET(PlatformSpecificRand, getZero);

  cpputest::TestShellPointerArray tests(test0);
  tests.shuffle(3);
  CHECK(tests.get(0) == test1);
  CHECK(tests.get(1) == test2);
  CHECK(tests.get(2) == test0);
}

// swaps with 4 mod 3 (1) then 4 mod 2 (0): 1, [2], [0] --> [1], [0], 2 --> 0,
// 1, 2
TEST(UtestShellPointerArrayTest, ShuffleListTestWithRandomAlwaysReturningOne)
{
  UT_PTR_SET(PlatformSpecificRand, getOne);

  cpputest::TestShellPointerArray tests(test0);
  tests.shuffle(3);
  CHECK(tests.get(0) == test0);
  CHECK(tests.get(1) == test2);
  CHECK(tests.get(2) == test1);
}

TEST(UtestShellPointerArrayTest, reverse)
{
  UT_PTR_SET(PlatformSpecificRand, getOne);

  cpputest::TestShellPointerArray tests(test0);
  tests.reverse();
  CHECK(tests.get(0) == test2);
  CHECK(tests.get(1) == test1);
  CHECK(tests.get(2) == test0);
}
