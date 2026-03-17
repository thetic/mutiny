#include "MockFailureReporterForTest.hpp"

#include "CppMu/CppMu.hpp"

using cppmu::mock;

void MockFailureReporterForTest::fail_test(cppmu::MockFailure failure)
{
  mock_failure_string = failure.get_message();
}

void MockFailureReporterForTest::report_failure(
    const cppmu::MockFailure& failure
)
{
  mock_failure_string = failure.get_message();
}

void MockFailureReporterForTest::exit_test()
{
  // suppress exit in test-spy mode
}

MockFailureReporterForTest* MockFailureReporterForTest::instance_ = nullptr;

MockFailureReporterForTest* MockFailureReporterForTest::get_reporter()
{
  if (instance_ == nullptr)
    instance_ = new MockFailureReporterForTest;

  return instance_;
}

void MockFailureReporterForTest::clear_reporter()
{
  delete instance_;
  instance_ = nullptr;
}

MockFailureReporterInstaller::MockFailureReporterInstaller()
{
  mock().set_mock_failure_standard_reporter(
      MockFailureReporterForTest::get_reporter()
  );
}

MockFailureReporterInstaller::~MockFailureReporterInstaller()
{
  mock().set_mock_failure_standard_reporter(nullptr);
  MockFailureReporterForTest::clear_reporter();
}

cppmu::TestShell* mock_failure_test()
{
  return MockFailureReporterForTest::get_reporter()->get_test_to_fail();
}

cppmu::String mock_failure_string()
{
  return MockFailureReporterForTest::get_reporter()->mock_failure_string;
}

void clear_mock_failure()
{
  MockFailureReporterForTest::get_reporter()->mock_failure_string = "";
}

void check_expected_mock_failure_location(
    const cppmu::MockFailure& expected_failure,
    const char* file,
    size_t line
)
{
  cppmu::String expected_failure_string = expected_failure.get_message();
  cppmu::String actual_failure_string = mock_failure_string();
  clear_mock_failure();
  if (expected_failure_string != actual_failure_string) {
    cppmu::String error = "MockFailures are different.\n";
    error += "Expected MockFailure:\n\t";
    error += expected_failure_string;
    error += "\nActual MockFailure:\n\t";
    error += actual_failure_string;
    FAIL_LOCATION(error.c_str(), file, line);
  }
}

void check_no_mock_failure_location(const char* file, size_t line)
{
  if (mock_failure_string() != "") {
    cppmu::String error = "Unexpected mock failure:\n";
    error += mock_failure_string();
    clear_mock_failure();
    FAIL_LOCATION(error.c_str(), file, line);
  }
  clear_mock_failure();
}

MockExpectedCallsListForTest::~MockExpectedCallsListForTest()
{
  delete_all_expectations_and_clear_list();
}

cppmu::MockCheckedExpectedCall* MockExpectedCallsListForTest::add_function(
    const cppmu::String& name
)
{
  auto* new_call = new cppmu::MockCheckedExpectedCall;
  new_call->with_name(name);
  add_expected_call(new_call);
  return new_call;
}

cppmu::MockCheckedExpectedCall* MockExpectedCallsListForTest::add_function(
    unsigned int num_calls,
    const cppmu::String& name
)
{
  auto* new_call = new cppmu::MockCheckedExpectedCall(num_calls);
  new_call->with_name(name);
  add_expected_call(new_call);
  return new_call;
}

cppmu::MockCheckedExpectedCall* MockExpectedCallsListForTest::
    add_function_ordered(const cppmu::String& name, unsigned int order)
{
  auto* new_call = add_function(name);
  new_call->with_call_order(order);
  return new_call;
}
