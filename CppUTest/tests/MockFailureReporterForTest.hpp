#ifndef INCLUDED_CPPUTEST_MOCKFAILUREREPORTERFORTEST_HPP
#define INCLUDED_CPPUTEST_MOCKFAILUREREPORTERFORTEST_HPP

#include "CppUTest/MockCheckedExpectedCall.hpp"
#include "CppUTest/MockExpectedCallsList.hpp"
#include "CppUTest/MockFailure.hpp"
#include "CppUTest/MockSupport.hpp"

#define CHECK_EXPECTED_MOCK_FAILURE(expectedFailure)                           \
  check_expected_mock_failure_location(expectedFailure, __FILE__, __LINE__)
#define CHECK_NO_MOCK_FAILURE()                                                \
  check_no_mock_failure_location(__FILE__, __LINE__)

class MockFailureReporterForTest : public cpputest::MockFailureReporter
{
public:
  cpputest::String mock_failure_string;

  virtual void fail_test(cpputest::MockFailure failure) override;
  virtual void report_failure(const cpputest::MockFailure& failure) override;
  virtual void exit_test() override;
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

cpputest::TestShell*
mock_failure_test();
cpputest::String
mock_failure_string();
void
clear_mock_failure();
void
check_expected_mock_failure_location(
    const cpputest::MockFailure& expected_failure,
    const char* file,
    size_t line);
void
check_no_mock_failure_location(const char* file, size_t line);

class MockExpectedCallsListForTest : public cpputest::MockExpectedCallsList
{
public:
  ~MockExpectedCallsListForTest() override;
  cpputest::MockCheckedExpectedCall* add_function(const cpputest::String& name);
  cpputest::MockCheckedExpectedCall* add_function(unsigned int num_calls,
      const cpputest::String& name);
  cpputest::MockCheckedExpectedCall* add_function_ordered(
      const cpputest::String& name,
      unsigned int order);
};

#endif
