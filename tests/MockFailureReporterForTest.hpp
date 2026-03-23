#ifndef INCLUDED_MUTINY_MOCKFAILUREREPORTERFORTEST_HPP
#define INCLUDED_MUTINY_MOCKFAILUREREPORTERFORTEST_HPP

#include "mutiny/mock.hpp"
#include "mutiny/mock/CheckedExpectedCall.hpp"
#include "mutiny/mock/ExpectedCallsList.hpp"
#include "mutiny/mock/Failure.hpp"

#define CHECK_EXPECTED_MOCK_FAILURE(expectedFailure)                           \
  check_expected_mock_failure_location(expectedFailure, __FILE__, __LINE__)
#define CHECK_NO_MOCK_FAILURE()                                                \
  check_no_mock_failure_location(__FILE__, __LINE__)

class MockFailureReporterForTest : public mu::tiny::mock::MockFailureReporter
{
public:
  mu::tiny::test::String mock_failure_string;

  void fail_test(mu::tiny::mock::MockFailure failure) override;
  void report_failure(const mu::tiny::mock::MockFailure& failure) override;
  void exit_test() override;
  static MockFailureReporterForTest* get_reporter();
  static void clear_reporter();

private:
  static MockFailureReporterForTest* instance_;
};

class MockFailureReporterInstaller
{
public:
  MockFailureReporterInstaller();
  ~MockFailureReporterInstaller();
};

mu::tiny::test::TestShell* mock_failure_test();
mu::tiny::test::String mock_failure_string();
void clear_mock_failure();
void check_expected_mock_failure_location(
    const mu::tiny::mock::MockFailure& expected_failure,
    const char* file,
    size_t line
);
void check_no_mock_failure_location(const char* file, size_t line);

class MockExpectedCallsListForTest
  : public mu::tiny::mock::MockExpectedCallsList
{
public:
  ~MockExpectedCallsListForTest() override;
  mu::tiny::mock::MockCheckedExpectedCall* add_function(
      const mu::tiny::test::String& name
  );
  mu::tiny::mock::MockCheckedExpectedCall* add_function(
      unsigned int num_calls,
      const mu::tiny::test::String& name
  );
  mu::tiny::mock::MockCheckedExpectedCall* add_function_ordered(
      const mu::tiny::test::String& name,
      unsigned int order
  );
};

#endif
