#include "MockFailureReporterForTest.hpp"

#include "CppMu/CppMu.hpp"
#include "CppMu/MockSupport.hpp"
#include "CppMu/TestTestingFixture.hpp"

using cppmu::mock;

namespace {
bool cppmu_has_crashed;

void crash_method()
{
  cppmu_has_crashed = true;
}

void unexpected_call_test_function(void)
{
  mock().actual_call("unexpected");
}

void check_expected_mock_failure_location_failed_test_method()
{
  MockExpectedCallsListForTest::MockExpectedCallsList list;
  cppmu::MockUnexpectedCallHappenedFailure expected_failure(
      cppmu::TestShell::get_current(), "unexpected", list
  );
  mock().actual_call("boo");
  check_expected_mock_failure_location(expected_failure, "file", 1);
}

void check_no_mock_failure_location_failed_test_method()
{
  mock().actual_call("boo");
  check_no_mock_failure_location("file", 1);
}

}

TEST_GROUP(MockSupportWithFixture)
{
  cppmu::TestTestingFixture fixture;

  void teardown() override
  {
    mock().clear();
    MockFailureReporterForTest::clear_reporter();
  }
};

TEST(MockSupportWithFixture, CHECK_EXPECTED_MOCK_FAILURE_LOCATION_failed)
{
  mock().set_mock_failure_standard_reporter(
      MockFailureReporterForTest::get_reporter()
  );
  fixture.set_test_function(
      check_expected_mock_failure_location_failed_test_method
  );
  fixture.run_all_tests();
  fixture.assert_print_contains("MockFailures are different.");
  fixture.assert_print_contains("Expected MockFailure:");
  fixture.assert_print_contains(
      "Mock Failure: Unexpected call to function: unexpected"
  );
  fixture.assert_print_contains("Actual MockFailure:");
  fixture.assert_print_contains(
      "Mock Failure: Unexpected call to function: boo"
  );
}

TEST(MockSupportWithFixture, CHECK_NO_MOCK_FAILURE_LOCATION_failed)
{
  mock().set_mock_failure_standard_reporter(
      MockFailureReporterForTest::get_reporter()
  );
  fixture.set_test_function(check_no_mock_failure_location_failed_test_method);
  fixture.run_all_tests();
  fixture.assert_print_contains("Unexpected mock failure:");
  fixture.assert_print_contains(
      "Mock Failure: Unexpected call to function: boo"
  );
}

TEST(MockSupportWithFixture, shouldCrashOnFailure)
{
  cppmu_has_crashed = false;
  mock().crash_on_failure(true);
  cppmu::TestShell::set_crash_method(crash_method);
  fixture.set_test_function(unexpected_call_test_function);

  fixture.run_all_tests();

  CHECK(cppmu_has_crashed);

  mock().crash_on_failure(false);
  cppmu::TestShell::reset_crash_method();
}

TEST(MockSupportWithFixture, ShouldNotCrashOnFailureAfterCrashMethodWasReset)
{
  cppmu_has_crashed = false;
  cppmu::TestShell::set_crash_method(crash_method);
  fixture.set_test_function(unexpected_call_test_function);
  cppmu::TestShell::reset_crash_method();

  fixture.run_all_tests();

  fixture.assert_print_contains("Unexpected call to function: unexpected");
  CHECK_FALSE(cppmu_has_crashed);
}

TEST(MockSupportWithFixture, shouldCrashOnFailureWithCppMuSetting)
{
  cppmu_has_crashed = false;
  cppmu::TestShell::set_crash_on_fail();
  cppmu::TestShell::set_crash_method(crash_method);
  fixture.set_test_function(unexpected_call_test_function);

  fixture.run_all_tests();

  CHECK(cppmu_has_crashed);

  cppmu::TestShell::restore_default_test_terminator();
  cppmu::TestShell::reset_crash_method();
}

TEST(MockSupportWithFixture, failedMockShouldFailAgainWhenRepeated)
{
  fixture.set_test_function(unexpected_call_test_function);
  int repeat_count = 2;
  while (repeat_count--) {
    fixture.run_all_tests();
    fixture.assert_print_contains("Unexpected call to function: unexpected");
    fixture.assert_print_contains(
        "Errors (1 failures, 1 tests, 1 ran, 0 checks, "
        "0 ignored, 0 filtered out"
    );
    fixture.flush_output_and_reset_result();
  }
}
