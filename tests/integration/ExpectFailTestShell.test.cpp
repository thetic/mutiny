#include "mutiny/test/ExpectFailShell.hpp"
#include "mutiny/test/TestingFixture.hpp"

#include "mutiny/test.hpp"

namespace {

class FailingTest : public mu::tiny::test::Test
{
  void test_body() override { FAIL_TEST("expected failure"); }
};

class FailingExpectFailTestShell : public mu::tiny::test::ExpectFailShell
{
  mu::tiny::test::Test* create_test() override { return new FailingTest; }
};

} // namespace

TEST_GROUP(ExpectFailShell)
{
  mu::tiny::test::TestingFixture fixture;
};

TEST(ExpectFailShell, innerTestFails_outerCountsRunNotFailure)
{
  FailingExpectFailTestShell shell;
  fixture.add_test(&shell);
  fixture.run_all_tests();
  // 2 runs: fixture's built-in genTest_ + shell
  CHECK_EQUAL(size_t{ 2 }, fixture.get_run_count());
  CHECK_EQUAL(size_t{ 0 }, fixture.get_failure_count());
}

TEST(ExpectFailShell, innerTestPasses_outerCountsRunAndFailure)
{
  mu::tiny::test::ExpectFailShell shell;
  fixture.add_test(&shell);
  fixture.run_all_tests();
  // 2 runs: fixture's built-in genTest_ + shell
  CHECK_EQUAL(size_t{ 2 }, fixture.get_run_count());
  CHECK_EQUAL(size_t{ 1 }, fixture.get_failure_count());
}

TEST(ExpectFailShell, willRun_alwaysReturnsTrue)
{
  mu::tiny::test::ExpectFailShell shell;
  CHECK(shell.will_run());
}

TEST(ExpectFailShell, getFormattedName_showsEXPECT_FAIL_TEST)
{
  mu::tiny::test::ExpectFailShell shell;
  shell.set_group_name("TestGroup");
  shell.set_test_name("TestName");
  STRCMP_EQUAL(
      "EXPECT_FAIL_TEST(TestGroup, TestName)",
      shell.get_formatted_name().c_str()
  );
}

TEST(ExpectFailShell, verbose_printsEXPECT_FAIL_TEST)
{
  FailingExpectFailTestShell shell;
  fixture.add_test(&shell);
  fixture.set_output_verbose();
  fixture.run_all_tests();
  fixture.assert_print_contains("EXPECT_FAIL_TEST");
}

TEST(ExpectFailShell, fourArgConstructor_setsGroupTestFileAndLine)
{
  mu::tiny::test::ExpectFailShell shell("MyGroup", "MyTest", "myfile.cpp", 42);
  STRCMP_EQUAL("MyGroup", shell.get_group());
  STRCMP_EQUAL("MyTest", shell.get_name());
  STRCMP_EQUAL("myfile.cpp", shell.get_file());
  CHECK_EQUAL(size_t{ 42 }, shell.get_line_number());
}
