#include "MockFailureReporter.hpp"

#include "mu/tiny/mock/SupportPlugin.hpp"

#include "mu/tiny/test/Output.hpp"
#include "mu/tiny/test/Shell.hpp"
#include "mu/tiny/test/StringBufferOutput.hpp"
#include "mu/tiny/test/TestingFixture.hpp"

#include "mu/tiny/mock.hpp"
#include "mu/tiny/test.hpp"

using mu::tiny::mock::mock;

TEST_GROUP(SupportPlugin)
{
  mu::tiny::test::StringBufferOutput output;

  mu::tiny::test::Shell* test;
  mu::tiny::test::Result* result;

  mu::tiny::mock::SupportPlugin plugin;

  void setup() override
  {
    test = new mu::tiny::test::Shell("group", "name", "file", 1);
    result = new mu::tiny::test::Result(output);
  }

  void teardown() override
  {
    delete test;
    delete result;
    mock().clear();
    mock().remove_all_comparators_and_copiers();
  }
};

TEST(SupportPlugin, canBeDeletedThroughBasePointer)
{
  mu::tiny::test::Plugin* p = new mu::tiny::mock::SupportPlugin;
  delete p;
}

TEST(SupportPlugin, checkExpectationsAndClearAtEnd)
{
  FailureReporterInstaller failure_reporter_installer;

  mock().expect_one_call("foobar");

  plugin.post_test_action(*test, *result);

  STRCMP_CONTAINS("foobar", output.get_output().c_str());
  CHECK(!mock().expected_calls_left());
  CHECK_NO_MOCK_FAILURE();
}

TEST(SupportPlugin, checkExpectationsWorksAlsoWithHierachicalObjects)
{
  FailureReporterInstaller failure_reporter_installer;

  mock("differentScope")
      .expect_one_call("foobar")
      .on_object(reinterpret_cast<void*>(1));
  mock("differentScope").actual_call("foobar");

  plugin.post_test_action(*test, *result);

  STRCMP_CONTAINS("differentScope::foobar", output.get_output().c_str());
  CHECK_NO_MOCK_FAILURE();
}

class DummyComparator : public mu::tiny::mock::NamedValueComparator
{
public:
  bool is_equal(const void* object1, const void* object2) override
  {
    return object1 == object2;
  }
  mu::tiny::String value_to_string(const void* /*object*/) override
  {
    return "string";
  }
};

TEST(SupportPlugin, installComparatorRecordsTheComparatorButNotInstallsItYet)
{
  FailureReporterInstaller failure_reporter_installer;

  DummyComparator comparator;
  plugin.install_comparator("myType", comparator);
  mock().expect_one_call("foo").with_parameter_of_type(
      "myType", "name", nullptr
  );
  mock().actual_call("foo").with_parameter_of_type("myType", "name", nullptr);

  mu::tiny::mock::NoWayToCompareCustomTypeFailure failure(test, "myType");
  CHECK_EXPECTED_MOCK_FAILURE(failure);

  plugin.clear();
}

class DummyCopier : public mu::tiny::mock::NamedValueCopier
{
public:
  void copy(void* dst, const void* src) override
  {
    *static_cast<int*>(dst) = *static_cast<const int*>(src);
  }
};

TEST(SupportPlugin, installCopierRecordsTheCopierButNotInstallsItYet)
{
  FailureReporterInstaller failure_reporter_installer;

  DummyCopier copier;
  plugin.install_copier("myType", copier);
  mock().expect_one_call("foo").with_output_parameter_of_type_returning(
      "myType", "name", nullptr
  );
  mock().actual_call("foo").with_output_parameter_of_type(
      "myType", "name", nullptr
  );

  mu::tiny::mock::NoWayToCopyCustomTypeFailure failure(test, "myType");
  CHECK_EXPECTED_MOCK_FAILURE(failure);

  plugin.clear();
}

TEST(SupportPlugin, preTestActionWillEnableMultipleComparatorsToTheGlobalMockSupportSpace)
{
  DummyComparator comparator;
  DummyComparator comparator2;
  plugin.install_comparator("myType", comparator);
  plugin.install_comparator("myOtherType", comparator2);

  plugin.pre_test_action(*test, *result);
  mock().expect_one_call("foo").with_parameter_of_type(
      "myType", "name", &comparator
  );
  mock().expect_one_call("foo").with_parameter_of_type(
      "myOtherType", "name", &comparator
  );
  mock().actual_call("foo").with_parameter_of_type(
      "myType", "name", &comparator
  );
  mock().actual_call("foo").with_parameter_of_type(
      "myOtherType", "name", &comparator
  );

  mock().check_expectations();
  CHECK_EQUAL(size_t{ 0 }, result->get_failure_count());

  plugin.clear();
}

namespace {
void fail_twice_function()
{
  mock().expect_one_call("foobar");
  FAIL_TEST("This failed");
}
} // namespace

TEST(SupportPlugin, shouldNotFailAgainWhenTestAlreadyFailed)
{
  mu::tiny::test::TestingFixture fixture;
  fixture.install_plugin(&plugin);
  fixture.set_test_function(fail_twice_function);
  fixture.run_all_tests();
  fixture.assert_print_contains("1 failures, 1 tests, 1 ran, 2 checks,");
}
