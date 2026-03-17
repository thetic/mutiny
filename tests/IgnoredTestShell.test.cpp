#include "CppMu/IgnoredTestShell.hpp"

#include "CppMu/CppMu.hpp"
#include "CppMu/ExecFunctionTestShell.hpp"
#include "CppMu/TestTestingFixture.hpp"

TEST_GROUP(IgnoredTestShell)
{
  cppmu::TestTestingFixture fixture;
  cppmu::IgnoredTestShell ignored_test;
  cppmu::ExecFunctionTestShell normal_utest_shell;

  void setup() override
  {
    fixture.add_test(&ignored_test);
    fixture.add_test(&normal_utest_shell);
  }
};

TEST(IgnoredTestShell, doesIgnoreCount)
{
  fixture.run_all_tests();
  LONGS_EQUAL(1, fixture.get_ignore_count());
}

TEST(IgnoredTestShell, printsIGNORE_TESTwhenVerbose)
{
  fixture.set_output_verbose();
  fixture.run_all_tests();
  fixture.assert_print_contains("IGNORE_TEST");
}

TEST(IgnoredTestShell, runIgnoredOptionSpecifiedThenIncreaseRunCount)
{
  ignored_test.set_run_ignored();
  fixture.run_all_tests();
  LONGS_EQUAL(3, fixture.get_run_count());
  LONGS_EQUAL(0, fixture.get_ignore_count());
}

TEST(IgnoredTestShell, runIgnoredOptionNotSpecifiedThenIncreaseIgnoredCount)
{
  fixture.run_all_tests();
  LONGS_EQUAL(2, fixture.get_run_count());
  LONGS_EQUAL(1, fixture.get_ignore_count());
}

TEST(IgnoredTestShell, runIgnoredOptionSpecifiedWillNotInfluenceNormalTestCount)
{
  normal_utest_shell.set_run_ignored();
  fixture.run_all_tests();
  LONGS_EQUAL(2, fixture.get_run_count());
  LONGS_EQUAL(1, fixture.get_ignore_count());
}

TEST(IgnoredTestShell, runIgnoredOptionSpecifiedThenReturnTESTInFormattedName)
{
  ignored_test.set_group_name("TestGroup");
  ignored_test.set_test_name("TestName");
  ignored_test.set_run_ignored();
  fixture.run_all_tests();
  STRCMP_EQUAL(
      "TEST(TestGroup, TestName)", ignored_test.get_formatted_name().c_str()
  );
}

TEST(IgnoredTestShell, runIgnoredOptionNotSpecifiedThenReturnIGNORETESTInFormattedName)
{
  ignored_test.set_group_name("TestGroup");
  ignored_test.set_test_name("TestName");
  fixture.run_all_tests();
  STRCMP_EQUAL(
      "IGNORE_TEST(TestGroup, TestName)",
      ignored_test.get_formatted_name().c_str()
  );
}

TEST(IgnoredTestShell, runIgnoredOptionNotSpecifiedThenWillRunReturnFalse)
{
  CHECK_FALSE(ignored_test.will_run());
}

TEST(IgnoredTestShell, runIgnoredOptionSpecifiedThenWillRunReturnTrue)
{
  ignored_test.set_run_ignored();
  CHECK_TRUE(ignored_test.will_run());
}
