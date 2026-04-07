#include "mutiny/test/ExecFunctionShell.hpp"
#include "mutiny/test/IgnoredShell.hpp"
#include "mutiny/test/TestingFixture.hpp"

#include "mutiny/test.hpp"

TEST_GROUP(IgnoredShell)
{
  mu::tiny::test::TestingFixture fixture;
  mu::tiny::test::IgnoredShell ignored_test;
  mu::tiny::test::ExecFunctionShell normal_utest_shell;

  void setup() override
  {
    fixture.add_test(&ignored_test);
    fixture.add_test(&normal_utest_shell);
  }
};

TEST(IgnoredShell, doesIgnoreCount)
{
  fixture.run_all_tests();
  CHECK_EQUAL(size_t{ 1 }, fixture.get_ignore_count());
}

TEST(IgnoredShell, printsIGNORE_TESTwhenVerbose)
{
  fixture.set_output_verbose();
  fixture.run_all_tests();
  fixture.assert_print_contains("IGNORE_TEST");
}

TEST(IgnoredShell, runIgnoredOptionSpecifiedThenIncreaseRunCount)
{
  ignored_test.set_run_ignored();
  fixture.run_all_tests();
  CHECK_EQUAL(size_t{ 3 }, fixture.get_run_count());
  CHECK_EQUAL(size_t{ 0 }, fixture.get_ignore_count());
}

TEST(IgnoredShell, runIgnoredOptionNotSpecifiedThenIncreaseIgnoredCount)
{
  fixture.run_all_tests();
  CHECK_EQUAL(size_t{ 2 }, fixture.get_run_count());
  CHECK_EQUAL(size_t{ 1 }, fixture.get_ignore_count());
}

TEST(IgnoredShell, runIgnoredOptionSpecifiedWillNotInfluenceNormalTestCount)
{
  normal_utest_shell.set_run_ignored();
  fixture.run_all_tests();
  CHECK_EQUAL(size_t{ 2 }, fixture.get_run_count());
  CHECK_EQUAL(size_t{ 1 }, fixture.get_ignore_count());
}

TEST(IgnoredShell, runIgnoredOptionSpecifiedThenReturnTESTInFormattedName)
{
  ignored_test.set_group_name("TestGroup");
  ignored_test.set_test_name("TestName");
  ignored_test.set_run_ignored();
  fixture.run_all_tests();
  STRCMP_EQUAL(
      "TEST(TestGroup, TestName)", ignored_test.get_formatted_name().c_str()
  );
}

TEST(IgnoredShell, runIgnoredOptionNotSpecifiedThenReturnIGNORETESTInFormattedName)
{
  ignored_test.set_group_name("TestGroup");
  ignored_test.set_test_name("TestName");
  fixture.run_all_tests();
  STRCMP_EQUAL(
      "IGNORE_TEST(TestGroup, TestName)",
      ignored_test.get_formatted_name().c_str()
  );
}

TEST(IgnoredShell, runIgnoredOptionNotSpecifiedThenWillRunReturnFalse)
{
  CHECK(!ignored_test.will_run());
}

TEST(IgnoredShell, runIgnoredOptionSpecifiedThenWillRunReturnTrue)
{
  ignored_test.set_run_ignored();
  CHECK(ignored_test.will_run());
}
