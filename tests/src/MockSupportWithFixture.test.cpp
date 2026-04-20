#include "MockFailureReporter.hpp"

#include "mu/tiny/test/TestingFixture.hpp"

#include "mu/tiny/mock.hpp"
#include "mu/tiny/test.hpp"

using mu::tiny::mock::mock;

namespace {
bool mutiny_has_crashed;

void crash_method()
{
  mutiny_has_crashed = true;
}

void unexpected_call_test_function(void)
{
  mock().actual_call("unexpected");
}

void check_expected_mock_failure_location_failed_test_method()
{
  FailureReporterInstaller reporter;
  mock().actual_call("unexpected");
  mu::tiny::String expected = mock_failure_string();
  clear_mock_failure();
  mock().actual_call("boo");
  check_expected_mock_failure_string_location(expected, "file", 1);
}

void check_no_mock_failure_location_failed_test_method()
{
  mock().actual_call("boo");
  check_no_mock_failure_location("file", 1);
}

} // namespace

TEST_GROUP(MockSupportWithFixture)
{
  mu::tiny::test::TestingFixture fixture;

  void teardown() override
  {
    mock().clear();
    FailureReporterForTest::clear_reporter();
  }
};

TEST(MockSupportWithFixture, CHECK_EXPECTED_MOCK_FAILURE_LOCATION_failed)
{
  mock().set_mock_failure_standard_reporter(
      FailureReporterForTest::get_reporter()
  );
  fixture.set_test_function(
      check_expected_mock_failure_location_failed_test_method
  );
  fixture.run_all_tests();
  fixture.assert_print_contains("MockFailures are different.");
  fixture.assert_print_contains("Expected Failure:");
  fixture.assert_print_contains(
      "Mock Failure: Unexpected call to function: unexpected"
  );
  fixture.assert_print_contains("Actual Failure:");
  fixture.assert_print_contains(
      "Mock Failure: Unexpected call to function: boo"
  );
}

TEST(MockSupportWithFixture, CHECK_NO_MOCK_FAILURE_LOCATION_failed)
{
  mock().set_mock_failure_standard_reporter(
      FailureReporterForTest::get_reporter()
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
  mutiny_has_crashed = false;
  mock().crash_on_failure(true);
  mu::tiny::test::Shell::set_crash_method(crash_method);
  fixture.set_test_function(unexpected_call_test_function);

  fixture.run_all_tests();

  CHECK(mutiny_has_crashed);

  mock().crash_on_failure(false);
  mu::tiny::test::Shell::reset_crash_method();
}

TEST(MockSupportWithFixture, ShouldNotCrashOnFailureAfterCrashMethodWasReset)
{
  mutiny_has_crashed = false;
  mu::tiny::test::Shell::set_crash_method(crash_method);
  fixture.set_test_function(unexpected_call_test_function);
  mu::tiny::test::Shell::reset_crash_method();

  fixture.run_all_tests();

  fixture.assert_print_contains("Unexpected call to function: unexpected");
  CHECK(!mutiny_has_crashed);
}

TEST(MockSupportWithFixture, shouldCrashOnFailureWithMutinySetting)
{
  mutiny_has_crashed = false;
  mu::tiny::test::Shell::set_crash_on_fail();
  mu::tiny::test::Shell::set_crash_method(crash_method);
  fixture.set_test_function(unexpected_call_test_function);

  fixture.run_all_tests();

  CHECK(mutiny_has_crashed);

  mu::tiny::test::Shell::restore_default_test_terminator();
  mu::tiny::test::Shell::reset_crash_method();
}

TEST(MockSupportWithFixture, failedMockShouldFailAgainWhenRepeated)
{
  fixture.set_test_function(unexpected_call_test_function);
  int repeat_count = 2;
  while (repeat_count-- != 0) {
    fixture.run_all_tests();
    fixture.assert_print_contains("Unexpected call to function: unexpected");
    fixture.assert_print_contains(
        "Errors (1 failures, 1 tests, 1 ran, 0 checks, "
        "0 skipped, 0 filtered out"
    );
    fixture.flush_output_and_reset_result();
  }
}
