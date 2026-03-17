#include "CppMu/OrderedTest.hpp"

#include "OrderedTest.h"

#include "CppMu/CppMu.hpp"
#include "CppMu/ExecFunctionTestShell.hpp"
#include "CppMu/TestOutput.hpp"
#include "CppMu/TestRegistry.hpp"
#include "CppMu/TestTestingFixture.hpp"

TEST_GROUP(OrderedTest)
{
  cppmu::TestTestingFixture* fixture;

  cppmu::OrderedTestShell ordered_test;
  cppmu::OrderedTestShell ordered_test2;
  cppmu::OrderedTestShell ordered_test3;
  cppmu::ExecFunctionTestShell normal_test;
  cppmu::ExecFunctionTestShell normal_test2;
  cppmu::ExecFunctionTestShell normal_test3;

  cppmu::OrderedTestShell* ordered_test_cache;
  void setup() override
  {
    ordered_test_cache = cppmu::OrderedTestShell::get_ordered_test_head();
    cppmu::OrderedTestShell::set_ordered_test_head(nullptr);

    fixture = new cppmu::TestTestingFixture();
    fixture->get_registry()->un_do_last_add_test();
  }

  void teardown() override
  {
    delete fixture;
    cppmu::OrderedTestShell::set_ordered_test_head(ordered_test_cache);
  }

  void install_ordered_test(cppmu::OrderedTestShell& test, int level)
  {
    cppmu::OrderedTestInstaller(
        test, "testgroup", "testname", __FILE__, __LINE__, level
    );
  }

  void install_normal_test(cppmu::TestShell& test)
  {
    cppmu::TestInstaller(test, "testgroup", "testname", __FILE__, __LINE__);
  }

  cppmu::TestShell* first_test()
  {
    return fixture->get_registry()->get_first_test();
  }

  cppmu::TestShell* second_test() { return first_test()->get_next(); }
};

TEST(OrderedTest, TestInstallerSetsFields)
{
  cppmu::OrderedTestInstaller installer(
      ordered_test, "testgroup", "testname", "this.cpp", 10, 5
  );
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

  cppmu::TestShell* first_ordered_test =
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
