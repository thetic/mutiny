#include "CppUTest/Extensions/MockSupportPlugin.hpp"

#include "MockFailureReporterForTest.h"

#include "CppUTest/Extensions/MockSupport.hpp"

#include "CppUTest/StringBufferTestOutput.hpp"
#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestOutput.hpp"
#include "CppUTest/TestTestingFixture.hpp"

using cpputest::extensions::mock;

TEST_GROUP(MockSupportPlugin)
{
  cpputest::StringBufferTestOutput output;

  cpputest::TestShell* test;
  cpputest::TestResult* result;

  cpputest::extensions::MockSupportPlugin plugin;

  void setup() override
  {
    test = new cpputest::TestShell("group", "name", "file", 1);
    result = new cpputest::TestResult(output);
  }

  void teardown() override
  {
    delete test;
    delete result;
    mock().clear();
    mock().removeAllComparatorsAndCopiers();
  }
};

TEST(MockSupportPlugin, checkExpectationsAndClearAtEnd)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foobar");
  cpputest::extensions::MockExpectedCallsDidntHappenFailure expectedFailure(
      test, expectations);

  mock().expectOneCall("foobar");

  plugin.postTestAction(*test, *result);

  STRCMP_CONTAINS(
      expectedFailure.getMessage().c_str(), output.getOutput().c_str());
  LONGS_EQUAL(0, mock().expectedCallsLeft());
  CHECK_NO_MOCK_FAILURE();
}

TEST(MockSupportPlugin, checkExpectationsWorksAlsoWithHierachicalObjects)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("differentScope::foobar")
      ->onObject(reinterpret_cast<void*>(1));
  cpputest::extensions::MockExpectedObjectDidntHappenFailure expectedFailure(
      test, "differentScope::foobar", expectations);

  mock("differentScope")
      .expectOneCall("foobar")
      .onObject(reinterpret_cast<void*>(1));
  mock("differentScope").actualCall("foobar");

  plugin.postTestAction(*test, *result);

  STRCMP_CONTAINS(
      expectedFailure.getMessage().c_str(), output.getOutput().c_str());
  CHECK_NO_MOCK_FAILURE();
}

class DummyComparator : public cpputest::extensions::MockNamedValueComparator
{
public:
  bool isEqual(const void* object1, const void* object2) override
  {
    return object1 == object2;
  }
  cpputest::String valueToString(const void*) override { return "string"; }
};

TEST(MockSupportPlugin,
    installComparatorRecordsTheComparatorButNotInstallsItYet)
{
  MockFailureReporterInstaller failureReporterInstaller;

  DummyComparator comparator;
  plugin.installComparator("myType", comparator);
  mock().expectOneCall("foo").withParameterOfType("myType", "name", nullptr);
  mock().actualCall("foo").withParameterOfType("myType", "name", nullptr);

  cpputest::extensions::MockNoWayToCompareCustomTypeFailure failure(
      test, "myType");
  CHECK_EXPECTED_MOCK_FAILURE(failure);

  plugin.clear();
}

class DummyCopier : public cpputest::extensions::MockNamedValueCopier
{
public:
  void copy(void* dst, const void* src) override
  {
    *static_cast<int*>(dst) = *static_cast<const int*>(src);
  }
};

TEST(MockSupportPlugin, installCopierRecordsTheCopierButNotInstallsItYet)
{
  MockFailureReporterInstaller failureReporterInstaller;

  DummyCopier copier;
  plugin.installCopier("myType", copier);
  mock().expectOneCall("foo").withOutputParameterOfTypeReturning(
      "myType", "name", nullptr);
  mock().actualCall("foo").withOutputParameterOfType("myType", "name", nullptr);

  cpputest::extensions::MockNoWayToCopyCustomTypeFailure failure(
      test, "myType");
  CHECK_EXPECTED_MOCK_FAILURE(failure);

  plugin.clear();
}

TEST(MockSupportPlugin,
    preTestActionWillEnableMultipleComparatorsToTheGlobalMockSupportSpace)
{
  DummyComparator comparator;
  DummyComparator comparator2;
  plugin.installComparator("myType", comparator);
  plugin.installComparator("myOtherType", comparator2);

  plugin.preTestAction(*test, *result);
  mock().expectOneCall("foo").withParameterOfType(
      "myType", "name", &comparator);
  mock().expectOneCall("foo").withParameterOfType(
      "myOtherType", "name", &comparator);
  mock().actualCall("foo").withParameterOfType("myType", "name", &comparator);
  mock().actualCall("foo").withParameterOfType(
      "myOtherType", "name", &comparator);

  mock().checkExpectations();
  LONGS_EQUAL(0, result->getFailureCount());

  plugin.clear();
}

static void
failTwiceFunction_()
{
  mock().expectOneCall("foobar");
  FAIL("This failed");
}

TEST(MockSupportPlugin, shouldNotFailAgainWhenTestAlreadyFailed)
{
  cpputest::TestTestingFixture fixture;
  fixture.installPlugin(&plugin);
  fixture.setTestFunction(failTwiceFunction_);
  fixture.runAllTests();
  fixture.assertPrintContains("1 failures, 1 tests, 1 ran, 2 checks,");
}
