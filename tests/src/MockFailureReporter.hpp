#ifndef INCLUDED_MU_TINY_MOCKFAILUREREPORTER_HPP
#define INCLUDED_MU_TINY_MOCKFAILUREREPORTER_HPP

#include "mu/tiny/mock/Failure.hpp"

#include "mu/tiny/mock.hpp"

#define CHECK_EXPECTED_MOCK_FAILURE(expectedFailure)                           \
  check_expected_mock_failure_location(expectedFailure, __FILE__, __LINE__)
#define CHECK_EXPECTED_MOCK_FAILURE_STRING(expectedString)                     \
  check_expected_mock_failure_string_location(                                 \
      expectedString, __FILE__, __LINE__                                       \
  )
#define CHECK_NO_MOCK_FAILURE()                                                \
  check_no_mock_failure_location(__FILE__, __LINE__)

class FailureReporterForTest : public mu::tiny::mock::FailureReporter
{
public:
  mu::tiny::String mock_failure_string;

  void fail_test(mu::tiny::mock::Failure failure) override;
  void report_failure(const mu::tiny::mock::Failure& failure) override;
  void exit_test() override;
  static FailureReporterForTest* get_reporter();
  static void clear_reporter();

private:
  static FailureReporterForTest* instance_;
};

class FailureReporterInstaller
{
public:
  FailureReporterInstaller();
  ~FailureReporterInstaller();
};

mu::tiny::test::Shell* mock_failure_test();
mu::tiny::String mock_failure_string();
void clear_mock_failure();
void check_expected_mock_failure_string_location(
    const mu::tiny::String& expected_string,
    const char* file,
    size_t line
);
void check_expected_mock_failure_location(
    const mu::tiny::mock::Failure& expected_failure,
    const char* file,
    size_t line
);
void check_no_mock_failure_location(const char* file, size_t line);

#endif
