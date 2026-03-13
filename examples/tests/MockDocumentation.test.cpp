#include "CppUTest/Extensions/MockSupport.h"
#include "CppUTest/Extensions/MockSupport.hpp"

#include "CppUTest/TestHarness.hpp"

TEST_GROUP(FirstTestGroup)
{};

TEST(FirstTestGroup, FirsTest)
{
  //	FAIL("Fail me!");
}

TEST(FirstTestGroup, SecondTest)
{
  //	STRCMP_EQUAL("hello", "world");
}

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

static int
equalMethod(const void* object1, const void* object2)
{
  return object1 == object2;
}

static const char*
toStringMethod(const void*)
{
  return "string";
}

TEST(MockDocumentation, CInterface)
{
  void* object = reinterpret_cast<void*>(0x1);

  mock()
      ->expectOneCall("foo")
      ->withIntParameters("integer", 10)
      ->andReturnDoubleValue(1.11);
  double d = mock()
                 ->actualCall("foo")
                 ->withIntParameters("integer", 10)
                 ->returnValue()
                 .value.doubleValue;
  DOUBLES_EQUAL(1.11, d, 0.00001);

  mock()->installComparator("type", equalMethod, toStringMethod);
  mock_scope("scope")->expectOneCall("bar")->withParameterOfType(
      "type", "name", object);
  mock_scope("scope")->actualCall("bar")->withParameterOfType(
      "type", "name", object);
  mock()->removeAllComparatorsAndCopiers();

  mock()->setIntData("important", 10);
  mock()->checkExpectations();
  mock()->clear();
}

TEST_GROUP(FooTestGroup)
{
  void setup() override
  {
    // Init stuff
  }

  void teardown() override
  {
    // Uninit stuff
  }
};

TEST(FooTestGroup, Foo)
{
  // Test FOO
}

TEST(FooTestGroup, MoreFoo)
{
  // Test more FOO
}

TEST_GROUP(BarTestGroup)
{
  void setup() override
  {
    // Init Bar
  }
};

TEST(BarTestGroup, Bar)
{
  // Test Bar
}
