#include "CppMu/MockSupportPlugin.hpp"

#include "MockFailureReporterForTest.hpp"

#include "CppMu/CppMu.hpp"
#include "CppMu/MockSupport.hpp"
#include "CppMu/StringBufferTestOutput.hpp"
#include "CppMu/TestOutput.hpp"
#include "CppMu/TestTestingFixture.hpp"

using cppmu::mock;

TEST_GROUP(MockSupportPlugin)
{
  cppmu::StringBufferTestOutput output;

  cppmu::TestShell* test;
  cppmu::TestResult* result;

  cppmu::MockSupportPlugin plugin;

  void setup() override
  {
    test = new cppmu::TestShell("group", "name", "file", 1);
    result = new cppmu::TestResult(output);
  }

  void teardown() override
  {
    delete test;
    delete result;
    mock().clear();
    mock().remove_all_comparators_and_copiers();
  }
};

TEST(MockSupportPlugin, checkExpectationsAndClearAtEnd)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function("foobar");
  cppmu::MockExpectedCallsDidntHappenFailure expected_failure(
      test, expectations);

  mock().expect_one_call("foobar");

  plugin.post_test_action(*test, *result);

  STRCMP_CONTAINS(
      expected_failure.get_message().c_str(), output.get_output().c_str());
  LONGS_EQUAL(0, mock().expected_calls_left());
  CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportPlugin, checkExpectationsWorksAlsoWithHierachicalObjects)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function("differentScope::foobar")
      ->on_object(reinterpret_cast<void*>(1));
  cppmu::MockExpectedObjectDidntHappenFailure expected_failure(
      test, "differentScope::foobar", expectations);

  mock("differentScope")
      .expect_one_call("foobar")
      .on_object(reinterpret_cast<void*>(1));
  mock("differentScope").actual_call("foobar");

  plugin.post_test_action(*test, *result);

  STRCMP_CONTAINS(
      expected_failure.get_message().c_str(), output.get_output().c_str());
  CHECK_NO_MOCK_FAILURE();
}

class DummyComparator : public cppmu::MockNamedValueComparator
{
public:
  bool is_equal(const void* object1, const void* object2) override
  {
    return object1 == object2;
  }
  cppmu::String value_to_string(const void*) override { return "string"; }
};

TEST(MockSupportPlugin,
    installComparatorRecordsTheComparatorButNotInstallsItYet)
{
  MockFailureReporterInstaller failure_reporter_installer;

  DummyComparator comparator;
  plugin.install_comparator("myType", comparator);
  mock().expect_one_call("foo").with_parameter_of_type(
      "myType", "name", nullptr);
  mock().actual_call("foo").with_parameter_of_type("myType", "name", nullptr);

  cppmu::MockNoWayToCompareCustomTypeFailure failure(test, "myType");
  CHECK_EXPECTED_MOCK_FAILURE(failure);

  plugin.clear();
}

class DummyCopier : public cppmu::MockNamedValueCopier
{
public:
  void copy(void* dst, const void* src) override
  {
    *static_cast<int*>(dst) = *static_cast<const int*>(src);
  }
};

TEST(MockSupportPlugin, installCopierRecordsTheCopierButNotInstallsItYet)
{
  MockFailureReporterInstaller failure_reporter_installer;

  DummyCopier copier;
  plugin.install_copier("myType", copier);
  mock().expect_one_call("foo").with_output_parameter_of_type_returning(
      "myType", "name", nullptr);
  mock().actual_call("foo").with_output_parameter_of_type(
      "myType", "name", nullptr);

  cppmu::MockNoWayToCopyCustomTypeFailure failure(test, "myType");
  CHECK_EXPECTED_MOCK_FAILURE(failure);

  plugin.clear();
}

TEST(MockSupportPlugin,
    preTestActionWillEnableMultipleComparatorsToTheGlobalMockSupportSpace)
{
  DummyComparator comparator;
  DummyComparator comparator2;
  plugin.install_comparator("myType", comparator);
  plugin.install_comparator("myOtherType", comparator2);

  plugin.pre_test_action(*test, *result);
  mock().expect_one_call("foo").with_parameter_of_type(
      "myType", "name", &comparator);
  mock().expect_one_call("foo").with_parameter_of_type(
      "myOtherType", "name", &comparator);
  mock().actual_call("foo").with_parameter_of_type(
      "myType", "name", &comparator);
  mock().actual_call("foo").with_parameter_of_type(
      "myOtherType", "name", &comparator);

  mock().check_expectations();
  LONGS_EQUAL(0, result->get_failure_count());

  plugin.clear();
}

namespace {
void
fail_twice_function()
{
  mock().expect_one_call("foobar");
  FAIL("This failed");
}
}

TEST(MockSupportPlugin, shouldNotFailAgainWhenTestAlreadyFailed)
{
  cppmu::TestTestingFixture fixture;
  fixture.install_plugin(&plugin);
  fixture.set_test_function(fail_twice_function);
  fixture.run_all_tests();
  fixture.assert_print_contains("1 failures, 1 tests, 1 ran, 2 checks,");
}
