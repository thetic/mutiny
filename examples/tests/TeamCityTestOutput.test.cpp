#include "TeamCityOutputPlugin.hpp"

#include "mu/tiny/test.hpp"
#include "mu/tiny/time.hpp"

namespace {
class TeamCityOutputToBuffer : public TeamCityTestOutput
{
public:
  explicit TeamCityOutputToBuffer() = default;

  ~TeamCityOutputToBuffer() override = default;

  void print_buffer(const char* s) override { output_ += s; }

  void flush() override { output_ = ""; }

  const mu::tiny::String& get_output() { return output_; }

private:
  mu::tiny::String output_;
};

uint_least64_t millis_time;

uint_least64_t mock_get_time_in_millis()
{
  return millis_time;
}

} // namespace

TEST_GROUP(TeamCityTestOutput)
{
  TeamCityTestOutput* tcout;
  TeamCityOutputToBuffer* mock;
  mu::tiny::test::Shell* tst;
  mu::tiny::test::Failure *f, *f2, *f3;
  mu::tiny::test::Result* result;

  void setup() override
  {
    mock = new TeamCityOutputToBuffer();
    tcout = mock;
    tst = new mu::tiny::test::Shell("group", "test", "file", 10);
    f = new mu::tiny::test::Failure(tst, "failfile", 20, "failure message");
    f2 = new mu::tiny::test::Failure(tst, "file", 20, "message");
    f3 = new mu::tiny::test::Failure(
        tst, "file", 30, "apos' pipe| [brackets]\r\nCRLF"
    );
    result = new mu::tiny::test::Result(*mock);
    result->set_total_execution_time(10);
    millis_time = 0;
    MUTINY_PTR_SET(mu::tiny::get_time_in_millis, mock_get_time_in_millis);
  }
  void teardown() override
  {
    delete tcout;
    delete tst;
    delete f;
    delete f2;
    delete f3;
    delete result;
  }
};

TEST(TeamCityTestOutput, PrintGroupStarted)
{
  result->current_group_started(tst);
  STRCMP_EQUAL(
      "##teamcity[testSuiteStarted name='group']\n", mock->get_output().c_str()
  );
}

TEST(TeamCityTestOutput, PrintGroupStartedAndEnded)
{
  const char* expected = "##teamcity[testSuiteStarted name='group']\n"
                         "##teamcity[testSuiteFinished name='group']\n";
  result->current_group_started(tst);
  result->current_group_ended(tst);
  STRCMP_EQUAL(expected, mock->get_output().c_str());
}

TEST(TeamCityTestOutput, PrintGroupEndedButNotStarted)
{
  result->current_group_ended(tst);
  STRCMP_EQUAL("", mock->get_output().c_str());
}

TEST(TeamCityTestOutput, PrintTestStarted)
{
  tcout->print_current_test_started(*tst);
  STRCMP_EQUAL(
      "##teamcity[testStarted name='test']\n", mock->get_output().c_str()
  );
}

TEST(TeamCityTestOutput, PrintTestStartedAndEnded)
{
  result->current_test_started(tst);
  millis_time = 42;
  result->current_test_ended(tst);
  STRCMP_EQUAL(
      "##teamcity[testStarted name='test']\n##teamcity[testFinished "
      "name='test' duration='42']\n",
      mock->get_output().c_str()
  );
}

TEST(TeamCityTestOutput, PrintTestEndedButNotStarted)
{
  result->current_test_ended(tst);
  STRCMP_EQUAL("", mock->get_output().c_str());
}

TEST(TeamCityTestOutput, PrintTestIgnored)
{
  const char* expected = "##teamcity[testStarted name='test']\n"
                         "##teamcity[testIgnored name='test']\n"
                         "##teamcity[testFinished name='test' duration='41']\n";

  auto* itst = new mu::tiny::test::IgnoredShell("group", "test", "file", 10);
  result->current_test_started(itst);
  millis_time = 41;
  result->current_test_ended(itst);
  STRCMP_EQUAL(expected, mock->get_output().c_str());
  delete itst;
}

TEST(TeamCityTestOutput, PrintWithFailureInSameFile)
{
  tcout->print_failure(*f2);
  const char* expected = "##teamcity[testFailed name='test' message='file:20' "
                         "details='message']\n";
  STRCMP_EQUAL(expected, mock->get_output().c_str());
}

TEST(TeamCityTestOutput, PrintWithEscapedCharacters)
{
  tcout->print_failure(*f3);
  const char* expected = "##teamcity[testFailed name='test' message='file:30' "
                         "details='apos|' pipe|| |[brackets|]"
                         "|r|nCRLF']\n";
  STRCMP_EQUAL(expected, mock->get_output().c_str());
}

TEST(TeamCityTestOutput, PrintFailureWithFailInDifferentFile)
{
  tcout->print_failure(*f);
  const char* expected = "##teamcity[testFailed name='test' message='TEST "
                         "failed (file:10): failfile:20' "
                         "details='failure message']\n";
  STRCMP_EQUAL(expected, mock->get_output().c_str());
}

TEST(TeamCityTestOutput, TestGroupEscaped_Start)
{
  tst->set_group_name("'[]\n\r");
  result->current_group_started(tst);
  const char* expected = "##teamcity[testSuiteStarted name='|'|[|]|n|r']\n";
  STRCMP_EQUAL(expected, mock->get_output().c_str());
}

TEST(TeamCityTestOutput, TestGroupEscaped_End)
{
  tst->set_group_name("'[]\n\r");
  result->current_group_started(tst);
  result->current_group_ended(tst);
  const char* expected = "##teamcity[testSuiteStarted name='|'|[|]|n|r']\n"
                         "##teamcity[testSuiteFinished name='|'|[|]|n|r']\n";
  STRCMP_EQUAL(expected, mock->get_output().c_str());
}

TEST(TeamCityTestOutput, TestNameEscaped_Start)
{
  tst->set_test_name("'[]\n\r");
  result->current_test_started(tst);
  const char* expected = "##teamcity[testStarted name='|'|[|]|n|r']\n";
  STRCMP_EQUAL(expected, mock->get_output().c_str());
}

TEST(TeamCityTestOutput, TestNameEscaped_End)
{
  tst->set_test_name("'[]\n\r");
  result->current_test_started(tst);
  result->current_test_ended(tst);
  const char* expected =
      "##teamcity[testStarted name='|'|[|]|n|r']\n"
      "##teamcity[testFinished name='|'|[|]|n|r' duration='0']\n";
  STRCMP_EQUAL(expected, mock->get_output().c_str());
}

TEST(TeamCityTestOutput, TestNameEscaped_Ignore)
{
  mu::tiny::test::IgnoredShell itst("group", "'[]\n\r", "file", 10);
  result->current_test_started(&itst);
  const char* expected = "##teamcity[testStarted name='|'|[|]|n|r']\n"
                         "##teamcity[testIgnored name='|'|[|]|n|r']\n";
  STRCMP_EQUAL(expected, mock->get_output().c_str());
}

TEST(TeamCityTestOutput, TestNameEscaped_Fail)
{
  tst->set_test_name("'[]\n\r");
  mu::tiny::test::Failure fail(tst, "failfile", 20, "failure message");
  tcout->print_failure(fail);
  const char* expected = "##teamcity[testFailed name='|'|[|]|n|r' "
                         "message='TEST failed (file:10): failfile:20' "
                         "details='failure message']\n";
  STRCMP_EQUAL(expected, mock->get_output().c_str());
}
