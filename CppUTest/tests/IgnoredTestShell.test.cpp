#include "CppUTest/IgnoredTestShell.hpp"

#include "CppUTest/ExecFunctionTestShell.hpp"
#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestTestingFixture.hpp"

TEST_GROUP(IgnoredTestShell)
{
  cpputest::TestTestingFixture fixture;
  cpputest::IgnoredTestShell ignoredTest;
  cpputest::ExecFunctionTestShell normalUtestShell;

  void setup() override
  {
    fixture.addTest(&ignoredTest);
    fixture.addTest(&normalUtestShell);
  }
};

TEST(IgnoredTestShell, doesIgnoreCount)
{
  fixture.runAllTests();
  LONGS_EQUAL(1, fixture.getIgnoreCount());
}

TEST(IgnoredTestShell, printsIGNORE_TESTwhenVerbose)
{
  fixture.setOutputVerbose();
  fixture.runAllTests();
  fixture.assertPrintContains("IGNORE_TEST");
}

TEST(IgnoredTestShell, runIgnoredOptionSpecifiedThenIncreaseRunCount)
{
  ignoredTest.setRunIgnored();
  fixture.runAllTests();
  LONGS_EQUAL(3, fixture.getRunCount());
  LONGS_EQUAL(0, fixture.getIgnoreCount());
}

TEST(IgnoredTestShell, runIgnoredOptionNotSpecifiedThenIncreaseIgnoredCount)
{
  fixture.runAllTests();
  LONGS_EQUAL(2, fixture.getRunCount());
  LONGS_EQUAL(1, fixture.getIgnoreCount());
}

TEST(IgnoredTestShell, runIgnoredOptionSpecifiedWillNotInfluenceNormalTestCount)
{
  normalUtestShell.setRunIgnored();
  fixture.runAllTests();
  LONGS_EQUAL(2, fixture.getRunCount());
  LONGS_EQUAL(1, fixture.getIgnoreCount());
}

TEST(IgnoredTestShell, runIgnoredOptionSpecifiedThenReturnTESTInFormattedName)
{
  ignoredTest.setGroupName("TestGroup");
  ignoredTest.setTestName("TestName");
  ignoredTest.setRunIgnored();
  fixture.runAllTests();
  STRCMP_EQUAL(
      "TEST(TestGroup, TestName)", ignoredTest.getFormattedName().c_str());
}

TEST(IgnoredTestShell,
    runIgnoredOptionNotSpecifiedThenReturnIGNORETESTInFormattedName)
{
  ignoredTest.setGroupName("TestGroup");
  ignoredTest.setTestName("TestName");
  fixture.runAllTests();
  STRCMP_EQUAL("IGNORE_TEST(TestGroup, TestName)",
      ignoredTest.getFormattedName().c_str());
}

TEST(IgnoredTestShell, runIgnoredOptionNotSpecifiedThenWillRunReturnFalse)
{
  CHECK_FALSE(ignoredTest.willRun());
}

TEST(IgnoredTestShell, runIgnoredOptionSpecifiedThenWillRunReturnTrue)
{
  ignoredTest.setRunIgnored();
  CHECK_TRUE(ignoredTest.willRun());
}
