#include "MockFailureReporter.hpp"

#include "mu/tiny/test/Shell.hpp"

#include "mu/tiny/test.hpp"

using mu::tiny::mock::mock;

void FailureReporterForTest::fail_test(mu::tiny::mock::Failure failure)
{
  mock_failure_string = failure.get_message();
}

void FailureReporterForTest::report_failure(
    const mu::tiny::mock::Failure& failure
)
{
  mock_failure_string = failure.get_message();
}

void FailureReporterForTest::exit_test()
{
  // suppress exit in test-spy mode
}

FailureReporterForTest* FailureReporterForTest::instance_ = nullptr;

FailureReporterForTest* FailureReporterForTest::get_reporter()
{
  if (instance_ == nullptr) {
    instance_ = new FailureReporterForTest;
  }

  return instance_;
}

void FailureReporterForTest::clear_reporter()
{
  delete instance_;
  instance_ = nullptr;
}

FailureReporterInstaller::FailureReporterInstaller()
{
  mock().set_mock_failure_standard_reporter(
      FailureReporterForTest::get_reporter()
  );
}

FailureReporterInstaller::~FailureReporterInstaller()
{
  mock().set_mock_failure_standard_reporter(nullptr);
  FailureReporterForTest::clear_reporter();
}

mu::tiny::test::Shell* mock_failure_test()
{
  return FailureReporterForTest::get_reporter()->get_test_to_fail();
}

mu::tiny::String mock_failure_string()
{
  return FailureReporterForTest::get_reporter()->mock_failure_string;
}

void clear_mock_failure()
{
  FailureReporterForTest::get_reporter()->mock_failure_string = "";
}

void check_expected_mock_failure_string_location(
    const mu::tiny::String& expected_string,
    const char* file,
    size_t line
)
{
  mu::tiny::String actual_failure_string = mock_failure_string();
  clear_mock_failure();
  if (expected_string != actual_failure_string) {
    mu::tiny::String error = "MockFailures are different.\n";
    error += "Expected Failure:\n\t";
    error += expected_string;
    error += "\nActual Failure:\n\t";
    error += actual_failure_string;
    FAIL_TEST_LOCATION(error.c_str(), file, line);
  }
}

void check_expected_mock_failure_location(
    const mu::tiny::mock::Failure& expected_failure,
    const char* file,
    size_t line
)
{
  check_expected_mock_failure_string_location(
      expected_failure.get_message(), file, line
  );
}

void check_no_mock_failure_location(const char* file, size_t line)
{
  if (mock_failure_string() != "") {
    mu::tiny::String error = "Unexpected mock failure:\n";
    error += mock_failure_string();
    clear_mock_failure();
    FAIL_TEST_LOCATION(error.c_str(), file, line);
  }
  clear_mock_failure();
}
