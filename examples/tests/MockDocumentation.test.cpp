#include "CppUTest/Extensions/MockSupport.hpp"

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness.hpp"

TEST_GROUP_C_WRAPPER(MockDocumentation_C)
{};

TEST_C_WRAPPER(MockDocumentation_C, CInterface)

TEST_GROUP(MockDocumentation)
{};

static void
productionCode()
{
  cpputest::extensions::mock().actualCall("productionCode");
}

TEST(MockDocumentation, SimpleScenario)
{
  cpputest::extensions::mock().expectOneCall("productionCode");
  productionCode();
  cpputest::extensions::mock().checkExpectations();
}

class ClassFromProductionCode
{
public:
  virtual void importantFunction() {}
  virtual ~ClassFromProductionCode() {}
};

class ClassFromProductionCodeMock : public ClassFromProductionCode
{
public:
  virtual void importantFunction() override
  {
    cpputest::extensions::mock().actualCall("importantFunction").onObject(this);
  }
};

TEST(MockDocumentation, SimpleScenarioObject)
{
  ClassFromProductionCode* object =
      new ClassFromProductionCodeMock; /* create mock instead of real thing */

  cpputest::extensions::mock()
      .expectOneCall("importantFunction")
      .onObject(object);
  object->importantFunction();
  cpputest::extensions::mock().checkExpectations();

  delete object;
}

static void
parameters_function(int p1, const char* p2)
{
  void* object = reinterpret_cast<void*>(1);
  cpputest::extensions::mock()
      .actualCall("function")
      .onObject(object)
      .withParameter("p1", p1)
      .withParameter("p2", p2);
}

TEST(MockDocumentation, parameters)
{
  void* object = reinterpret_cast<void*>(1);
  cpputest::extensions::mock()
      .expectOneCall("function")
      .onObject(object)
      .withParameter("p1", 2)
      .withParameter("p2", "hah");
  parameters_function(2, "hah");
}

class MyTypeComparator : public cpputest::extensions::MockNamedValueComparator
{
public:
  virtual bool isEqual(const void* object1, const void* object2) override
  {
    return object1 == object2;
  }
  virtual cpputest::String valueToString(const void* object) override
  {
    return cpputest::StringFrom(object);
  }
};

TEST(MockDocumentation, ObjectParameters)
{
  void* object = reinterpret_cast<void*>(1);
  MyTypeComparator comparator;
  cpputest::extensions::mock().installComparator("myType", comparator);
  cpputest::extensions::mock()
      .expectOneCall("function")
      .withParameterOfType("myType", "parameterName", object);
  cpputest::extensions::mock().clear();
  cpputest::extensions::mock().removeAllComparatorsAndCopiers();
}

TEST(MockDocumentation, returnValue)
{
  cpputest::extensions::mock().expectOneCall("function").andReturnValue(10);
  cpputest::extensions::mock()
      .actualCall("function")
      .returnValue()
      .getIntValue();
  int value = cpputest::extensions::mock().returnValue().getIntValue();
  LONGS_EQUAL(10, value);
}

TEST(MockDocumentation, setData)
{
  ClassFromProductionCode object;
  cpputest::extensions::mock().setData("importantValue", 10);
  cpputest::extensions::mock().setDataObject(
      "importantObject", "ClassFromProductionCode", &object);

  ClassFromProductionCode* pobject;
  int value =
      cpputest::extensions::mock().getData("importantValue").getIntValue();
  pobject = static_cast<ClassFromProductionCode*>(cpputest::extensions::mock()
          .getData("importantObject")
          .getObjectPointer());

  LONGS_EQUAL(10, value);
  POINTERS_EQUAL(pobject, &object);
}

static void
doSomethingThatWouldOtherwiseBlowUpTheMockingFramework()
{
}

TEST(MockDocumentation, otherMockSupport)
{
  cpputest::extensions::mock().crashOnFailure();
  //	mock().actualCall("unex");

  cpputest::extensions::mock().expectOneCall("foo");
  cpputest::extensions::mock().ignoreOtherCalls();

  cpputest::extensions::mock().disable();
  doSomethingThatWouldOtherwiseBlowUpTheMockingFramework();
  cpputest::extensions::mock().enable();

  cpputest::extensions::mock().clear();
}

TEST(MockDocumentation, scope)
{
  cpputest::extensions::mock("xmlparser").expectOneCall("open");
  cpputest::extensions::mock("filesystem").ignoreOtherCalls();

  cpputest::extensions::mock("xmlparser").actualCall("open");
}
