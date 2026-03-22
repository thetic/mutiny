#include "CppMu/ExpectFailTestShell.hpp"

#include "CppMu/CppMu.hpp"
#include "CppMu/TestTestingFixture.hpp"

namespace {

class FailingTest : public cppmu::Test
{
  void test_body() override { FAIL("expected failure"); }
};

class FailingExpectFailTestShell : public cppmu::ExpectFailTestShell
{
  cppmu::Test* create_test() override { return new FailingTest; }
};

} // namespace

TEST_GROUP(ExpectFailTestShell)
{
  cppmu::TestTestingFixture fixture;
};

TEST(ExpectFailTestShell, innerTestFails_outerCountsRunNotFailure)
{
  FailingExpectFailTestShell shell;
  fixture.add_test(&shell);
  fixture.run_all_tests();
  // 2 runs: fixture's built-in genTest_ + shell
  LONGS_EQUAL(2, fixture.get_run_count());
  LONGS_EQUAL(0, fixture.get_failure_count());
}

TEST(ExpectFailTestShell, innerTestPasses_outerCountsRunAndFailure)
{
  cppmu::ExpectFailTestShell shell;
  fixture.add_test(&shell);
  fixture.run_all_tests();
  // 2 runs: fixture's built-in genTest_ + shell
  LONGS_EQUAL(2, fixture.get_run_count());
  LONGS_EQUAL(1, fixture.get_failure_count());
}

TEST(ExpectFailTestShell, willRun_alwaysReturnsTrue)
{
  cppmu::ExpectFailTestShell shell;
  CHECK_TRUE(shell.will_run());
}

TEST(ExpectFailTestShell, getFormattedName_showsEXPECT_FAIL_TEST)
{
  cppmu::ExpectFailTestShell shell;
  shell.set_group_name("TestGroup");
  shell.set_test_name("TestName");
  STRCMP_EQUAL(
      "EXPECT_FAIL_TEST(TestGroup, TestName)",
      shell.get_formatted_name().c_str()
  );
}

TEST(ExpectFailTestShell, verbose_printsEXPECT_FAIL_TEST)
{
  FailingExpectFailTestShell shell;
  fixture.add_test(&shell);
  fixture.set_output_verbose();
  fixture.run_all_tests();
  fixture.assert_print_contains("EXPECT_FAIL_TEST");
}

TEST(ExpectFailTestShell, fourArgConstructor_setsGroupTestFileAndLine)
{
  cppmu::ExpectFailTestShell shell("MyGroup", "MyTest", "myfile.cpp", 42);
  STRCMP_EQUAL("MyGroup", shell.get_group());
  STRCMP_EQUAL("MyTest", shell.get_name());
  STRCMP_EQUAL("myfile.cpp", shell.get_file());
  LONGS_EQUAL(42, static_cast<long>(shell.get_line_number()));
}
