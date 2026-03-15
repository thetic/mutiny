#include "CppUTest/Extensions/OrderedTest.hpp"

#include "OrderedTest.h"

#include "CppUTest/CppUTest.hpp"
#include "CppUTest/ExecFunctionTestShell.hpp"
#include "CppUTest/TestOutput.hpp"
#include "CppUTest/TestRegistry.hpp"
#include "CppUTest/TestTestingFixture.hpp"

TEST_GROUP(OrderedTest)
{
  cpputest::TestTestingFixture* fixture;

  cpputest::extensions::OrderedTestShell ordered_test;
  cpputest::extensions::OrderedTestShell ordered_test2;
  cpputest::extensions::OrderedTestShell ordered_test3;
  cpputest::ExecFunctionTestShell normal_test;
  cpputest::ExecFunctionTestShell normal_test2;
  cpputest::ExecFunctionTestShell normal_test3;

  cpputest::extensions::OrderedTestShell* ordered_test_cache;
  void setup() override
  {
    ordered_test_cache =
        cpputest::extensions::OrderedTestShell::get_ordered_test_head();
    cpputest::extensions::OrderedTestShell::set_ordered_test_head(nullptr);

    fixture = new cpputest::TestTestingFixture();
    fixture->get_registry()->un_do_last_add_test();
  }

  void teardown() override
  {
    delete fixture;
    cpputest::extensions::OrderedTestShell::set_ordered_test_head(
        ordered_test_cache);
  }

  void install_ordered_test(cpputest::extensions::OrderedTestShell& test,
      int level)
  {
    cpputest::extensions::OrderedTestInstaller(
        test, "testgroup", "testname", __FILE__, __LINE__, level);
  }

  void install_normal_test(cpputest::TestShell& test)
  {
    cpputest::TestInstaller(test, "testgroup", "testname", __FILE__, __LINE__);
  }

  cpputest::TestShell* first_test()
  {
    return fixture->get_registry()->get_first_test();
  }

  cpputest::TestShell* second_test() { return first_test()->get_next(); }
};

TEST(OrderedTest, TestInstallerSetsFields)
{
  cpputest::extensions::OrderedTestInstaller installer(
      ordered_test, "testgroup", "testname", "this.cpp", 10, 5);
  STRCMP_EQUAL("testgroup", ordered_test.get_group().c_str());
  STRCMP_EQUAL("testname", ordered_test.get_name().c_str());
  STRCMP_EQUAL("this.cpp", ordered_test.get_file().c_str());
  LONGS_EQUAL(10, ordered_test.get_line_number());
  LONGS_EQUAL(5, ordered_test.get_level());
}

TEST(OrderedTest, InstallOneText)
{
  install_ordered_test(ordered_test, 5);
  CHECK(first_test() == &ordered_test);
}

TEST(OrderedTest, OrderedTestsAreLast)
{
  install_normal_test(normal_test);
  install_ordered_test(ordered_test, 5);
  CHECK(first_test() == &normal_test);
  CHECK(second_test() == &ordered_test);
}

TEST(OrderedTest, TwoTestsAddedInReverseOrder)
{
  install_ordered_test(ordered_test, 5);
  install_ordered_test(ordered_test2, 3);
  CHECK(first_test() == &ordered_test2);
  CHECK(second_test() == &ordered_test);
}

TEST(OrderedTest, TwoTestsAddedInOrder)
{
  install_ordered_test(ordered_test2, 3);
  install_ordered_test(ordered_test, 5);
  CHECK(first_test() == &ordered_test2);
  CHECK(second_test() == &ordered_test);
}

TEST(OrderedTest, MultipleOrderedTests)
{
  install_normal_test(normal_test);
  install_ordered_test(ordered_test2, 3);
  install_normal_test(normal_test2);
  install_ordered_test(ordered_test, 5);
  install_normal_test(normal_test3);
  install_ordered_test(ordered_test3, 7);

  cpputest::TestShell* first_ordered_test =
      first_test()->get_next()->get_next()->get_next();
  CHECK(first_ordered_test == &ordered_test2);
  CHECK(first_ordered_test->get_next() == &ordered_test);
  CHECK(first_ordered_test->get_next()->get_next() == &ordered_test3);
}

TEST(OrderedTest, MultipleOrderedTests2)
{
  install_ordered_test(ordered_test, 3);
  install_ordered_test(ordered_test2, 1);
  install_ordered_test(ordered_test3, 2);

  CHECK(first_test() == &ordered_test2);
  CHECK(second_test() == &ordered_test3);
  CHECK(second_test()->get_next() == &ordered_test);
}

namespace {
class OrderedTestTestingFixture
{
public:
  static void check_run(int run)
  {
    if (run != run_) {
      run_ = run;
      count_ = 0;
    }
  }
  static int count(void) { return count_++; }

private:
  static int run_;
  static int count_;
};

int OrderedTestTestingFixture::run_ = 0;
int OrderedTestTestingFixture::count_ = 0;
}

TEST_GROUP(OrderedTestC)
{
  void setup() override
  {
    OrderedTestTestingFixture::check_run(
        cpputest::TestRegistry::get_current_registry()
            ->get_current_repetition());
  }
};

TEST(OrderedTestC, NormalTest)
{
  CHECK(OrderedTestTestingFixture::count() == 0);
}

TEST_ORDERED(OrderedTestC, Test2, 2)
{
  CHECK(OrderedTestTestingFixture::count() == 2);
}

TEST_ORDERED(OrderedTestC, Test1, 1)
{
  CHECK(OrderedTestTestingFixture::count() == 1);
}

TEST_ORDERED(OrderedTestC, Test4, 4)
{
  CHECK(OrderedTestTestingFixture::count() == 4);
}

TEST_ORDERED(OrderedTestC, Test3, 3)
{
  CHECK(OrderedTestTestingFixture::count() == 3);
}

// Test with same level
TEST_ORDERED(OrderedTestC, Test5_1, 5)
{
  CHECK(OrderedTestTestingFixture::count() == 5);
}

TEST_ORDERED(OrderedTestC, Test6_1, 6)
{
  CHECK(OrderedTestTestingFixture::count() == 7);
}

TEST_ORDERED(OrderedTestC, Test5_2, 5)
{
  CHECK(OrderedTestTestingFixture::count() == 6);
}

TEST_ORDERED(OrderedTestC, Test6_2, 6)
{
  CHECK(OrderedTestTestingFixture::count() == 8);
}

// Test C-Interface
TEST_ORDERED(OrderedTestC, Test10, 10)
{
  CHECK(OrderedTestTestingFixture::count() == 12);
}

TEST_ORDERED(OrderedTestC, Test8, 8)
{
  CHECK(OrderedTestTestingFixture::count() == 10);
}

int
ordered_test_fixture_c_wrapper(void)
{
  return OrderedTestTestingFixture::count();
}

TEST_ORDERED_C_WRAPPER(OrderedTestC, Test11, 11)

TEST_ORDERED_C_WRAPPER(OrderedTestC, Test7, 7)

TEST_ORDERED_C_WRAPPER(OrderedTestC, Test9, 9)
