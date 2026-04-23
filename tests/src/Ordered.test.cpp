#include "mu/tiny/test/Ordered.hpp"

#include "mu/tiny/test/Output.hpp"
#include "mu/tiny/test/Registry.hpp"
#include "mu/tiny/test/TestingFixture.hpp"

#include "mu/tiny/test.hpp"

namespace {
class MockShell : public mu::tiny::test::Shell
{
public:
  MockShell()
    : mu::tiny::test::Shell("group", "name", "file", 1)
  {
  }
};
} // namespace

TEST_GROUP(Ordered)
{
  mu::tiny::test::TestingFixture* fixture;

  mu::tiny::test::OrderedShell ordered_test;
  mu::tiny::test::OrderedShell ordered_test2;
  mu::tiny::test::OrderedShell ordered_test3;
  MockShell normal_test;
  MockShell normal_test2;
  MockShell normal_test3;

  mu::tiny::test::OrderedShell* ordered_test_cache;
  void setup() override
  {
    ordered_test_cache = mu::tiny::test::OrderedShell::get_ordered_test_head();
    mu::tiny::test::OrderedShell::set_ordered_test_head(nullptr);

    fixture = new mu::tiny::test::TestingFixture();
    fixture->get_registry()->un_do_last_add_test();
  }

  void teardown() override
  {
    delete fixture;
    mu::tiny::test::OrderedShell::set_ordered_test_head(ordered_test_cache);
  }

  static void install_ordered_test(
      mu::tiny::test::OrderedShell& test,
      int level
  )
  {
    mu::tiny::test::OrderedInstaller(
        test, "testgroup", "testname", __FILE__, __LINE__, level
    );
  }

  static void install_normal_test(mu::tiny::test::Shell& test)
  {
    mu::tiny::test::Installer(
        test, "testgroup", "testname", __FILE__, __LINE__
    );
  }

  mu::tiny::test::Shell* first_test() const
  {
    return fixture->get_registry()->get_first_test();
  }

  mu::tiny::test::Shell* second_test() const
  {
    return first_test()->get_next();
  }
};

TEST(Ordered, TestInstallerSetsFields)
{
  mu::tiny::test::OrderedInstaller installer(
      ordered_test, "testgroup", "testname", "this.cpp", 10, 5
  );
  STRCMP_EQUAL("testgroup", ordered_test.get_group());
  STRCMP_EQUAL("testname", ordered_test.get_name());
  STRCMP_EQUAL("this.cpp", ordered_test.get_file());
  CHECK_EQUAL(10, ordered_test.get_line_number());
  CHECK_EQUAL(5, ordered_test.get_level());
}

TEST(Ordered, InstallOneText)
{
  install_ordered_test(ordered_test, 5);
  CHECK(first_test() == &ordered_test);
}

TEST(Ordered, OrderedTestsAreLast)
{
  install_normal_test(normal_test);
  install_ordered_test(ordered_test, 5);
  CHECK(first_test() == &normal_test);
  CHECK(second_test() == &ordered_test);
}

TEST(Ordered, TwoTestsAddedInReverseOrder)
{
  install_ordered_test(ordered_test, 5);
  install_ordered_test(ordered_test2, 3);
  CHECK(first_test() == &ordered_test2);
  CHECK(second_test() == &ordered_test);
}

TEST(Ordered, TwoTestsAddedInOrder)
{
  install_ordered_test(ordered_test2, 3);
  install_ordered_test(ordered_test, 5);
  CHECK(first_test() == &ordered_test2);
  CHECK(second_test() == &ordered_test);
}

TEST(Ordered, MultipleOrderedTests)
{
  install_normal_test(normal_test);
  install_ordered_test(ordered_test2, 3);
  install_normal_test(normal_test2);
  install_ordered_test(ordered_test, 5);
  install_normal_test(normal_test3);
  install_ordered_test(ordered_test3, 7);

  mu::tiny::test::Shell* first_ordered_test =
      first_test()->get_next()->get_next()->get_next();
  CHECK(first_ordered_test == &ordered_test2);
  CHECK(first_ordered_test->get_next() == &ordered_test);
  CHECK(first_ordered_test->get_next()->get_next() == &ordered_test3);
}

TEST(Ordered, MultipleOrderedTests2)
{
  install_ordered_test(ordered_test, 3);
  install_ordered_test(ordered_test2, 1);
  install_ordered_test(ordered_test3, 2);

  CHECK(first_test() == &ordered_test2);
  CHECK(second_test() == &ordered_test3);
  CHECK(second_test()->get_next() == &ordered_test);
}
