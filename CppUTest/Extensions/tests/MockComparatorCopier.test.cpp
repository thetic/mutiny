#include "MockFailureReporterForTest.hpp"

#include "CppUTest/TestHarness.hpp"

using cpputest::extensions::mock;

TEST_GROUP(MockComparatorCopier)
{
  void teardown() override
  {
    mock().checkExpectations();
    mock().clear();
    mock().removeAllComparatorsAndCopiers();
  }
};

namespace {
class MyTypeForTesting
{
public:
  MyTypeForTesting(long val) { value = new long(val); }
  virtual ~MyTypeForTesting() { delete value; }
  long* value;
};

class MyTypeForTestingComparator
  : public cpputest::extensions::MockNamedValueComparator
{
public:
  virtual bool isEqual(const void* object1, const void* object2) override
  {
    auto* obj1 = static_cast<const MyTypeForTesting*>(object1);
    auto* obj2 = static_cast<const MyTypeForTesting*>(object2);
    return *(obj1->value) == *(obj2->value);
  }

  virtual cpputest::String valueToString(const void* object) override
  {
    auto* obj = static_cast<const MyTypeForTesting*>(object);
    return cpputest::StringFrom(*(obj->value));
  }
};

class MyTypeForTestingCopier : public cpputest::extensions::MockNamedValueCopier
{
public:
  virtual void copy(void* dst_, const void* src_) override
  {
    auto* dst = static_cast<MyTypeForTesting*>(dst_);
    auto* src = static_cast<const MyTypeForTesting*>(src_);
    *(dst->value) = *(src->value);
  }
};
}

TEST(MockComparatorCopier,
    customObjectParameterFailsWhenNotHavingAComparisonRepository)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MyTypeForTesting object(1);
  mock()
      .expectOneCall("function")
      .withParameterOfType("MyTypeForTesting", "parameterName", &object);
  mock()
      .actualCall("function")
      .withParameterOfType("MyTypeForTesting", "parameterName", &object);

  cpputest::extensions::MockNoWayToCompareCustomTypeFailure expectedFailure(
      mockFailureTest(), "MyTypeForTesting");
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockComparatorCopier,
    customObjectParameterFailsWhenNotHavingACopierRepository)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MyTypeForTesting object(1);
  mock()
      .expectOneCall("function")
      .withOutputParameterOfTypeReturning(
          "MyTypeForTesting", "parameterName", &object);
  mock()
      .actualCall("function")
      .withOutputParameterOfType("MyTypeForTesting", "parameterName", &object);

  cpputest::extensions::MockNoWayToCopyCustomTypeFailure expectedFailure(
      mockFailureTest(), "MyTypeForTesting");
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockComparatorCopier, customObjectParameterSucceeds)
{
  MyTypeForTesting object(1);
  MyTypeForTestingComparator comparator;
  mock().installComparator("MyTypeForTesting", comparator);

  mock()
      .expectOneCall("function")
      .withParameterOfType("MyTypeForTesting", "parameterName", &object);
  mock()
      .actualCall("function")
      .withParameterOfType("MyTypeForTesting", "parameterName", &object);

  mock().checkExpectations();

  mock().removeAllComparatorsAndCopiers();
}

namespace {
bool
myTypeIsEqual(const void* object1, const void* object2)
{
  return static_cast<const MyTypeForTesting*>(object1)->value ==
         static_cast<const MyTypeForTesting*>(object2)->value;
}

cpputest::String
myTypeValueToString(const void* object)
{
  return cpputest::StringFrom(
      static_cast<const MyTypeForTesting*>(object)->value);
}
}

TEST(MockComparatorCopier, customObjectWithFunctionComparator)
{
  MyTypeForTesting object(1);
  cpputest::extensions::MockFunctionComparator comparator(
      myTypeIsEqual, myTypeValueToString);
  mock().installComparator("MyTypeForTesting", comparator);

  mock()
      .expectOneCall("function")
      .withParameterOfType("MyTypeForTesting", "parameterName", &object);
  mock()
      .actualCall("function")
      .withParameterOfType("MyTypeForTesting", "parameterName", &object);

  mock().checkExpectations();

  mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopier,
    customObjectWithFunctionComparatorThatFailsCoversValueToString)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MyTypeForTesting object(5);
  cpputest::extensions::MockFunctionComparator comparator(
      myTypeIsEqual, myTypeValueToString);
  mock().installComparator("MyTypeForTesting", comparator);

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("function")
      ->withParameterOfType("MyTypeForTesting", "parameterName", &object);
  cpputest::extensions::MockExpectedCallsDidntHappenFailure failure(
      cpputest::TestShell::getCurrent(), expectations);

  mock()
      .expectOneCall("function")
      .withParameterOfType("MyTypeForTesting", "parameterName", &object);
  mock().checkExpectations();

  CHECK_EXPECTED_MOCK_FAILURE_LOCATION(failure, __FILE__, __LINE__);
}

TEST(MockComparatorCopier, customTypeOutputParameterSucceeds)
{
  MyTypeForTesting expectedObject(55);
  MyTypeForTesting actualObject(99);
  MyTypeForTestingCopier copier;
  mock().installCopier("MyTypeForTesting", copier);

  mock()
      .expectOneCall("function")
      .withOutputParameterOfTypeReturning(
          "MyTypeForTesting", "parameterName", &expectedObject);
  mock()
      .actualCall("function")
      .withOutputParameterOfType(
          "MyTypeForTesting", "parameterName", &actualObject);

  mock().checkExpectations();
  CHECK_EQUAL(55, *(expectedObject.value));
  CHECK_EQUAL(55, *(actualObject.value));

  mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopier, noActualCallForCustomTypeOutputParameter)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MyTypeForTesting expectedObject(1);
  MyTypeForTestingCopier copier;
  mock().installCopier("MyTypeForTesting", copier);

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foo")->withOutputParameterOfTypeReturning(
      "MyTypeForTesting", "output", &expectedObject);
  cpputest::extensions::MockExpectedCallsDidntHappenFailure expectedFailure(
      mockFailureTest(), expectations);

  mock().expectOneCall("foo").withOutputParameterOfTypeReturning(
      "MyTypeForTesting", "output", &expectedObject);
  mock().checkExpectations();

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);

  mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopier, unexpectedCustomTypeOutputParameter)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MyTypeForTesting actualObject(8834);
  MyTypeForTestingCopier copier;
  mock().installCopier("MyTypeForTesting", copier);

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foo");
  cpputest::extensions::MockNamedValue parameter("parameterName");
  parameter.setConstObjectPointer("MyTypeForTesting", &actualObject);
  cpputest::extensions::MockUnexpectedOutputParameterFailure expectedFailure(
      mockFailureTest(), "foo", parameter, expectations);

  mock().expectOneCall("foo");
  mock().actualCall("foo").withOutputParameterOfType(
      "MyTypeForTesting", "parameterName", &actualObject);
  mock().checkExpectations();

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);

  mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopier, customTypeOutputParameterMissing)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MyTypeForTesting expectedObject(123464);
  MyTypeForTestingCopier copier;
  mock().installCopier("MyTypeForTesting", copier);

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foo")->withOutputParameterOfTypeReturning(
      "MyTypeForTesting", "output", &expectedObject);
  cpputest::extensions::MockExpectedParameterDidntHappenFailure expectedFailure(
      mockFailureTest(), "foo", expectations, expectations);

  mock().expectOneCall("foo").withOutputParameterOfTypeReturning(
      "MyTypeForTesting", "output", &expectedObject);
  mock().actualCall("foo");

  mock().checkExpectations();
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);

  mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopier, customTypeOutputParameterOfWrongType)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MyTypeForTesting expectedObject(123464);
  MyTypeForTesting actualObject(75646);
  MyTypeForTestingCopier copier;
  mock().installCopier("MyTypeForTesting", copier);

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foo")->withOutputParameterOfTypeReturning(
      "MyTypeForTesting", "output", &expectedObject);
  cpputest::extensions::MockNamedValue parameter("output");
  parameter.setConstObjectPointer("OtherTypeForTesting", &actualObject);
  cpputest::extensions::MockUnexpectedOutputParameterFailure expectedFailure(
      mockFailureTest(), "foo", parameter, expectations);

  mock().expectOneCall("foo").withOutputParameterOfTypeReturning(
      "MyTypeForTesting", "output", &expectedObject);
  mock().actualCall("foo").withOutputParameterOfType(
      "OtherTypeForTesting", "output", &actualObject);

  mock().checkExpectations();
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);

  mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopier, noCopierForCustomTypeOutputParameter)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MyTypeForTesting expectedObject(123464);
  MyTypeForTesting actualObject(8834);

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foo")->withOutputParameterOfTypeReturning(
      "MyTypeForTesting", "output", &expectedObject);
  cpputest::extensions::MockNoWayToCopyCustomTypeFailure expectedFailure(
      mockFailureTest(), "MyTypeForTesting");

  mock().expectOneCall("foo").withOutputParameterOfTypeReturning(
      "MyTypeForTesting", "output", &expectedObject);
  mock().actualCall("foo").withOutputParameterOfType(
      "MyTypeForTesting", "output", &actualObject);

  mock().checkExpectations();
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockComparatorCopier, twoCustomTypeOutputParameters)
{
  MyTypeForTesting expectedObject1(545);
  MyTypeForTesting actualObject1(979);
  MyTypeForTesting expectedObject2(123);
  MyTypeForTesting actualObject2(4567);
  MyTypeForTestingCopier copier;
  mock().installCopier("MyTypeForTesting", copier);

  mock()
      .expectOneCall("function")
      .withOutputParameterOfTypeReturning(
          "MyTypeForTesting", "parameterName", &expectedObject1)
      .withParameter("id", 1);
  mock()
      .expectOneCall("function")
      .withOutputParameterOfTypeReturning(
          "MyTypeForTesting", "parameterName", &expectedObject2)
      .withParameter("id", 2);
  mock()
      .actualCall("function")
      .withOutputParameterOfType(
          "MyTypeForTesting", "parameterName", &actualObject1)
      .withParameter("id", 1);
  mock()
      .actualCall("function")
      .withOutputParameterOfType(
          "MyTypeForTesting", "parameterName", &actualObject2)
      .withParameter("id", 2);

  mock().checkExpectations();
  CHECK_EQUAL(545, *(expectedObject1.value));
  CHECK_EQUAL(545, *(actualObject1.value));
  CHECK_EQUAL(123, *(expectedObject2.value));
  CHECK_EQUAL(123, *(actualObject2.value));

  mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopier, twoInterleavedCustomTypeOutputParameters)
{
  MyTypeForTesting expectedObject1(9545);
  MyTypeForTesting actualObject1(79);
  MyTypeForTesting expectedObject2(132);
  MyTypeForTesting actualObject2(743);
  MyTypeForTestingCopier copier;

  mock().installCopier("MyTypeForTesting", copier);
  mock()
      .expectOneCall("function")
      .withOutputParameterOfTypeReturning(
          "MyTypeForTesting", "parameterName", &expectedObject1)
      .withParameter("id", 1);
  mock()
      .expectOneCall("function")
      .withOutputParameterOfTypeReturning(
          "MyTypeForTesting", "parameterName", &expectedObject2)
      .withParameter("id", 2);
  mock()
      .actualCall("function")
      .withOutputParameterOfType(
          "MyTypeForTesting", "parameterName", &actualObject2)
      .withParameter("id", 2);
  mock()
      .actualCall("function")
      .withOutputParameterOfType(
          "MyTypeForTesting", "parameterName", &actualObject1)
      .withParameter("id", 1);

  mock().checkExpectations();
  CHECK_EQUAL(9545, *(expectedObject1.value));
  CHECK_EQUAL(9545, *(actualObject1.value));
  CHECK_EQUAL(132, *(expectedObject2.value));
  CHECK_EQUAL(132, *(actualObject2.value));

  mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopier,
    twoDifferentCustomTypeOutputParametersInSameFunctionCallSucceeds)
{
  MyTypeForTesting expectedObject1(11);
  MyTypeForTesting actualObject1(22);
  MyTypeForTesting expectedObject2(33);
  MyTypeForTesting actualObject2(44);
  MyTypeForTestingCopier copier;
  mock().installCopier("MyTypeForTesting", copier);

  mock()
      .expectOneCall("foo")
      .withOutputParameterOfTypeReturning(
          "MyTypeForTesting", "bar", &expectedObject1)
      .withOutputParameterOfTypeReturning(
          "MyTypeForTesting", "foobar", &expectedObject2);
  mock()
      .actualCall("foo")
      .withOutputParameterOfType("MyTypeForTesting", "bar", &actualObject1)
      .withOutputParameterOfType("MyTypeForTesting", "foobar", &actualObject2);

  mock().checkExpectations();
  CHECK_EQUAL(11, *(expectedObject1.value));
  CHECK_EQUAL(11, *(actualObject1.value));
  CHECK_EQUAL(33, *(expectedObject2.value));
  CHECK_EQUAL(33, *(actualObject2.value));

  mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopier,
    customTypeOutputAndInputParametersOfSameNameInDifferentFunctionCallsOfSameFunctionSucceeds)
{
  MyTypeForTesting expectedObject1(911);
  MyTypeForTesting actualObject1(6576878);
  MyTypeForTesting expectedObject2(123);
  MyTypeForTesting actualObject2(123);
  MyTypeForTestingCopier copier;
  MyTypeForTestingComparator comparator;
  mock().installCopier("MyTypeForTesting", copier);
  mock().installComparator("MyTypeForTesting", comparator);

  mock().expectOneCall("foo").withOutputParameterOfTypeReturning(
      "MyTypeForTesting", "bar", &expectedObject1);
  mock().expectOneCall("foo").withParameterOfType(
      "MyTypeForTesting", "bar", &expectedObject2);
  mock().actualCall("foo").withOutputParameterOfType(
      "MyTypeForTesting", "bar", &actualObject1);
  mock().actualCall("foo").withParameterOfType(
      "MyTypeForTesting", "bar", &actualObject2);

  mock().checkExpectations();
  CHECK_EQUAL(911, *(expectedObject1.value));
  CHECK_EQUAL(911, *(actualObject1.value));
  CHECK_EQUAL(123, *(expectedObject2.value));
  CHECK_EQUAL(123, *(actualObject2.value));

  mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopier,
    twoCustomTypeOutputParametersOfSameNameInDifferentFunctionsSucceeds)
{
  MyTypeForTesting expectedObject1(657);
  MyTypeForTesting actualObject1(984465);
  MyTypeForTesting expectedObject2(987);
  MyTypeForTesting actualObject2(987);
  MyTypeForTestingCopier copier;
  MyTypeForTestingComparator comparator;
  mock().installCopier("MyTypeForTesting", copier);
  mock().installComparator("MyTypeForTesting", comparator);

  mock().expectOneCall("foo1").withOutputParameterOfTypeReturning(
      "MyTypeForTesting", "bar", &expectedObject1);
  mock().expectOneCall("foo2").withParameterOfType(
      "MyTypeForTesting", "bar", &expectedObject2);
  mock().actualCall("foo1").withOutputParameterOfType(
      "MyTypeForTesting", "bar", &actualObject1);
  mock().actualCall("foo2").withParameterOfType(
      "MyTypeForTesting", "bar", &actualObject2);

  mock().checkExpectations();
  CHECK_EQUAL(657, *(expectedObject1.value));
  CHECK_EQUAL(657, *(actualObject1.value));
  CHECK_EQUAL(987, *(expectedObject2.value));
  CHECK_EQUAL(987, *(actualObject2.value));

  mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopier,
    customTypeOutputAndInputParameterOfSameTypeInSameFunctionCall)
{
  MyTypeForTesting expectedObject1(45);
  MyTypeForTesting actualObject1(45);
  MyTypeForTesting expectedObject2(987765443);
  MyTypeForTesting actualObject2(0);
  MyTypeForTestingCopier copier;
  MyTypeForTestingComparator comparator;
  mock().installCopier("MyTypeForTesting", copier);
  mock().installComparator("MyTypeForTesting", comparator);

  mock()
      .expectOneCall("foo")
      .withParameterOfType("MyTypeForTesting", "bar", &expectedObject1)
      .withOutputParameterOfTypeReturning(
          "MyTypeForTesting", "bar", &expectedObject2);
  mock()
      .actualCall("foo")
      .withParameterOfType("MyTypeForTesting", "bar", &actualObject1)
      .withOutputParameterOfType("MyTypeForTesting", "bar", &actualObject2);

  mock().checkExpectations();
  CHECK_EQUAL(45, *(expectedObject1.value));
  CHECK_EQUAL(45, *(actualObject1.value));
  CHECK_EQUAL(987765443, *(expectedObject2.value));
  CHECK_EQUAL(987765443, *(actualObject2.value));

  mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopier, customTypeOutputParameterTraced)
{
  MyTypeForTesting actualObject(676789);
  MyTypeForTestingCopier copier;
  mock().installCopier("MyTypeForTesting", copier);
  mock().tracing(true);

  mock()
      .actualCall("someFunc")
      .withOutputParameterOfType(
          "MyTypeForTesting", "someParameter", &actualObject);

  mock().checkExpectations();
  STRCMP_CONTAINS("Function name:someFunc MyTypeForTesting someParameter:",
      mock().getTraceOutput());

  mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopier, customTypeOutputParameterWithIgnoredParameters)
{
  MyTypeForTesting expectedObject(444537909);
  MyTypeForTesting actualObject(98765);
  MyTypeForTestingCopier copier;
  mock().installCopier("MyTypeForTesting", copier);

  mock()
      .expectOneCall("foo")
      .withOutputParameterOfTypeReturning(
          "MyTypeForTesting", "bar", &expectedObject)
      .ignoreOtherParameters();
  mock()
      .actualCall("foo")
      .withOutputParameterOfType("MyTypeForTesting", "bar", &actualObject)
      .withParameter("other", 1);

  mock().checkExpectations();
  CHECK_EQUAL(444537909, *(expectedObject.value));
  CHECK_EQUAL(444537909, *(actualObject.value));

  mock().removeAllComparatorsAndCopiers();
}

namespace {
void
myTypeCopy(void* dst_, const void* src_)
{
  auto* dst = static_cast<MyTypeForTesting*>(dst_);
  auto* src = static_cast<const MyTypeForTesting*>(src_);
  *(dst->value) = *(src->value);
}
}

TEST(MockComparatorCopier, customObjectWithFunctionCopier)
{
  MyTypeForTesting expectedObject(9874452);
  MyTypeForTesting actualObject(2034);
  cpputest::extensions::MockFunctionCopier copier(myTypeCopy);
  mock().installCopier("MyTypeForTesting", copier);

  mock()
      .expectOneCall("function")
      .withOutputParameterOfTypeReturning(
          "MyTypeForTesting", "parameterName", &expectedObject);
  mock()
      .actualCall("function")
      .withOutputParameterOfType(
          "MyTypeForTesting", "parameterName", &actualObject);

  mock().checkExpectations();
  CHECK_EQUAL(9874452, *(expectedObject.value));
  CHECK_EQUAL(9874452, *(actualObject.value));

  mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopier, removingComparatorsWorksHierachically)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MyTypeForTesting object(1);
  MyTypeForTestingComparator comparator;

  mock("scope").installComparator("MyTypeForTesting", comparator);
  mock().removeAllComparatorsAndCopiers();
  mock("scope")
      .expectOneCall("function")
      .withParameterOfType("MyTypeForTesting", "parameterName", &object);
  mock("scope")
      .actualCall("function")
      .withParameterOfType("MyTypeForTesting", "parameterName", &object);

  cpputest::extensions::MockNoWayToCompareCustomTypeFailure expectedFailure(
      mockFailureTest(), "MyTypeForTesting");
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockComparatorCopier, removingCopiersWorksHierachically)
{
  MockFailureReporterInstaller failureReporterInstaller;
  MyTypeForTesting object(1);

  MyTypeForTestingCopier copier;

  mock("scope").installCopier("MyTypeForTesting", copier);
  mock().removeAllComparatorsAndCopiers();
  mock("scope").expectOneCall("foo").withOutputParameterOfTypeReturning(
      "MyTypeForTesting", "bar", &object);
  mock("scope").actualCall("foo").withOutputParameterOfType(
      "MyTypeForTesting", "bar", &object);

  cpputest::extensions::MockNoWayToCopyCustomTypeFailure expectedFailure(
      mockFailureTest(), "MyTypeForTesting");
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockComparatorCopier,
    installComparatorWorksHierarchicalOnBothExistingAndDynamicallyCreatedMockSupports)
{
  MyTypeForTesting object(1);
  MyTypeForTestingComparator comparator;

  mock("existing");
  mock().installComparator("MyTypeForTesting", comparator);
  mock("existing")
      .expectOneCall("function")
      .withParameterOfType("MyTypeForTesting", "parameterName", &object);
  mock("existing")
      .actualCall("function")
      .withParameterOfType("MyTypeForTesting", "parameterName", &object);
  mock("dynamic")
      .expectOneCall("function")
      .withParameterOfType("MyTypeForTesting", "parameterName", &object);
  mock("dynamic")
      .actualCall("function")
      .withParameterOfType("MyTypeForTesting", "parameterName", &object);

  mock().checkExpectations();
  mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopier, installComparatorsWorksHierarchical)
{
  MyTypeForTesting object(1);
  MyTypeForTestingComparator comparator;
  cpputest::extensions::MockNamedValueComparatorsAndCopiersRepository repos;
  repos.installComparator("MyTypeForTesting", comparator);

  mock("existing");
  mock().installComparatorsAndCopiers(repos);
  mock("existing")
      .expectOneCall("function")
      .withParameterOfType("MyTypeForTesting", "parameterName", &object);
  mock("existing")
      .actualCall("function")
      .withParameterOfType("MyTypeForTesting", "parameterName", &object);

  mock().checkExpectations();
  mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopier, installCopiersWorksHierarchically)
{
  MyTypeForTesting object(1);
  MyTypeForTestingCopier copier;

  mock("existing");
  mock().installCopier("MyTypeForTesting", copier);
  mock("existing")
      .expectOneCall("function")
      .withOutputParameterOfTypeReturning(
          "MyTypeForTesting", "parameterName", &object);
  mock("existing")
      .actualCall("function")
      .withOutputParameterOfType("MyTypeForTesting", "parameterName", &object);

  mock().checkExpectations();
  mock().removeAllComparatorsAndCopiers();
}

namespace {
class StubComparator
  : public MyTypeForTestingComparator::MockNamedValueComparator
{
public:
  virtual bool isEqual(const void*, const void*) override { return true; }
  virtual cpputest::String valueToString(const void*) override { return ""; }
};

struct SomeClass
{
  int someDummy_;
};

void
functionWithConstParam(const SomeClass param)
{
  mock()
      .actualCall("functionWithConstParam")
      .withParameterOfType("SomeClass", "param", &param);
}
}

TEST(MockComparatorCopier, shouldSupportConstParameters)
{
  StubComparator comparator;
  mock().installComparator("SomeClass", comparator);

  SomeClass param;
  mock()
      .expectOneCall("functionWithConstParam")
      .withParameterOfType("SomeClass", "param", &param);
  functionWithConstParam(param);

  mock().checkExpectations();
}
