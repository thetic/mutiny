#include "CppUTestExt/MockSupportPlugin.hpp"

#include "MockFailureReporterForTest.h"

#include "CppUTestExt/MockSupport.hpp"

#include "CppUTest/StringBufferTestOutput.hpp"
#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestOutput.hpp"
#include "CppUTest/TestTestingFixture.hpp"

using namespace cpputest;
using namespace cpputest::extensions;

TEST_GROUP(MockPlugin)
{
  StringBufferTestOutput output;

  UtestShell* test;
  TestResult* result;

  MockSupportPlugin plugin;

  void setup() override
  {
    test = new UtestShell("group", "name", "file", 1);
    result = new TestResult(output);
  }

  void teardown() override
  {
    delete test;
    delete result;
    mock().clear();
    mock().removeAllComparatorsAndCopiers();
  }
};

TEST(MockPlugin, checkExpectationsAndClearAtEnd)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foobar");
  MockExpectedCallsDidntHappenFailure expectedFailure(test, expectations);

  mock().expectOneCall("foobar");

  plugin.postTestAction(*test, *result);

  STRCMP_CONTAINS(
      expectedFailure.getMessage().c_str(), output.getOutput().c_str());
  LONGS_EQUAL(0, mock().expectedCallsLeft());
  CHECK_NO_MOCK_FAILURE();
}

TEST(MockPlugin, checkExpectationsWorksAlsoWithHierachicalObjects)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("differentScope::foobar")
      ->onObject(reinterpret_cast<void*>(1));
  MockExpectedObjectDidntHappenFailure expectedFailure(
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

class DummyComparator : public MockNamedValueComparator
{
public:
  bool isEqual(const void* object1, const void* object2) override
  {
    return object1 == object2;
  }
  String valueToString(const void*) override { return "string"; }
};

TEST(MockPlugin, installComparatorRecordsTheComparatorButNotInstallsItYet)
{
  MockFailureReporterInstaller failureReporterInstaller;

  DummyComparator comparator;
  plugin.installComparator("myType", comparator);
  mock().expectOneCall("foo").withParameterOfType("myType", "name", nullptr);
  mock().actualCall("foo").withParameterOfType("myType", "name", nullptr);

  MockNoWayToCompareCustomTypeFailure failure(test, "myType");
  CHECK_EXPECTED_MOCK_FAILURE(failure);

  plugin.clear();
}

class DummyCopier : public MockNamedValueCopier
{
public:
  void copy(void* dst, const void* src) override
  {
    *static_cast<int*>(dst) = *static_cast<const int*>(src);
  }
};

TEST(MockPlugin, installCopierRecordsTheCopierButNotInstallsItYet)
{
  MockFailureReporterInstaller failureReporterInstaller;

  DummyCopier copier;
  plugin.installCopier("myType", copier);
  mock().expectOneCall("foo").withOutputParameterOfTypeReturning(
      "myType", "name", nullptr);
  mock().actualCall("foo").withOutputParameterOfType("myType", "name", nullptr);

  MockNoWayToCopyCustomTypeFailure failure(test, "myType");
  CHECK_EXPECTED_MOCK_FAILURE(failure);

  plugin.clear();
}

TEST(MockPlugin,
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

TEST(MockPlugin, shouldNotFailAgainWhenTestAlreadyFailed)
{
  TestTestingFixture fixture;
  fixture.installPlugin(&plugin);
  fixture.setTestFunction(failTwiceFunction_);
  fixture.runAllTests();
  fixture.assertPrintContains("1 failures, 1 tests, 1 ran, 2 checks,");
}
