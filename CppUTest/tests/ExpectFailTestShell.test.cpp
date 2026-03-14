#include "CppUTest/ExpectFailTestShell.hpp"

#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestTestingFixture.hpp"

namespace {

class FailingTest : public cpputest::Test
{
  void testBody() override { FAIL("expected failure"); }
};

class FailingExpectFailTestShell : public cpputest::ExpectFailTestShell
{
  cpputest::Test* createTest() override { return new FailingTest; }
};

} // namespace

TEST_GROUP(ExpectFailTestShell)
{
  cpputest::TestTestingFixture fixture;
};

TEST(ExpectFailTestShell, innerTestFails_outerCountsRunNotFailure)
{
  FailingExpectFailTestShell shell;
  fixture.addTest(&shell);
  fixture.runAllTests();
  // 2 runs: fixture's built-in genTest_ + shell
  LONGS_EQUAL(2, fixture.getRunCount());
  LONGS_EQUAL(0, fixture.getFailureCount());
}

TEST(ExpectFailTestShell, innerTestPasses_outerCountsRunAndFailure)
{
  cpputest::ExpectFailTestShell shell;
  fixture.addTest(&shell);
  fixture.runAllTests();
  // 2 runs: fixture's built-in genTest_ + shell
  LONGS_EQUAL(2, fixture.getRunCount());
  LONGS_EQUAL(1, fixture.getFailureCount());
}

TEST(ExpectFailTestShell, willRun_alwaysReturnsTrue)
{
  cpputest::ExpectFailTestShell shell;
  CHECK_TRUE(shell.willRun());
}

TEST(ExpectFailTestShell, getFormattedName_showsEXPECT_FAIL_TEST)
{
  cpputest::ExpectFailTestShell shell;
  shell.setGroupName("TestGroup");
  shell.setTestName("TestName");
  STRCMP_EQUAL("EXPECT_FAIL_TEST(TestGroup, TestName)",
      shell.getFormattedName().c_str());
}

TEST(ExpectFailTestShell, verbose_printsEXPECT_FAIL_TEST)
{
  FailingExpectFailTestShell shell;
  fixture.addTest(&shell);
  fixture.setOutputVerbose();
  fixture.runAllTests();
  fixture.assertPrintContains("EXPECT_FAIL_TEST");
}
