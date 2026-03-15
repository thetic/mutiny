#ifndef INCLUDED_CPPMU_MOCKFAILUREREPORTERFORTEST_HPP
#define INCLUDED_CPPMU_MOCKFAILUREREPORTERFORTEST_HPP

#include "CppMu/MockCheckedExpectedCall.hpp"
#include "CppMu/MockExpectedCallsList.hpp"
#include "CppMu/MockFailure.hpp"
#include "CppMu/MockSupport.hpp"

#define CHECK_EXPECTED_MOCK_FAILURE(expectedFailure)                           \
  check_expected_mock_failure_location(expectedFailure, __FILE__, __LINE__)
#define CHECK_NO_MOCK_FAILURE()                                                \
  check_no_mock_failure_location(__FILE__, __LINE__)

class MockFailureReporterForTest : public cppmu::MockFailureReporter
{
public:
  cppmu::String mock_failure_string;

  virtual void fail_test(cppmu::MockFailure failure) override;
  virtual void report_failure(const cppmu::MockFailure& failure) override;
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

cppmu::TestShell*
mock_failure_test();
cppmu::String
mock_failure_string();
void
clear_mock_failure();
void
check_expected_mock_failure_location(const cppmu::MockFailure& expected_failure,
    const char* file,
    size_t line);
void
check_no_mock_failure_location(const char* file, size_t line);

class MockExpectedCallsListForTest : public cppmu::MockExpectedCallsList
{
public:
  ~MockExpectedCallsListForTest() override;
  cppmu::MockCheckedExpectedCall* add_function(const cppmu::String& name);
  cppmu::MockCheckedExpectedCall* add_function(unsigned int num_calls,
      const cppmu::String& name);
  cppmu::MockCheckedExpectedCall* add_function_ordered(
      const cppmu::String& name,
      unsigned int order);
};

#endif
