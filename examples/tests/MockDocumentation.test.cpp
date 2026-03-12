#include "CppUTest/Extensions/MockSupport.h"
#include "CppUTest/Extensions/MockSupport.hpp"

#include "CppUTest/TestHarness.hpp"

using cpputest::extensions::mock;

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
  mock().actualCall("productionCode");
}

TEST(MockDocumentation, SimpleScenario)
{
  mock().expectOneCall("productionCode");
  productionCode();
  mock().checkExpectations();
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
    mock().actualCall("importantFunction").onObject(this);
  }
};

TEST(MockDocumentation, SimpleScenarioObject)
{
  ClassFromProductionCode* object =
      new ClassFromProductionCodeMock; /* create mock instead of real thing */

  mock().expectOneCall("importantFunction").onObject(object);
  object->importantFunction();
  mock().checkExpectations();

  delete object;
}

static void
parameters_function(int p1, const char* p2)
{
  void* object = reinterpret_cast<void*>(1);
  mock()
      .actualCall("function")
      .onObject(object)
      .withParameter("p1", p1)
      .withParameter("p2", p2);
}

TEST(MockDocumentation, parameters)
{
  void* object = reinterpret_cast<void*>(1);
  mock()
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
  mock().installComparator("myType", comparator);
  mock()
      .expectOneCall("function")
      .withParameterOfType("myType", "parameterName", object);
  mock().clear();
  mock().removeAllComparatorsAndCopiers();
}

TEST(MockDocumentation, returnValue)
{
  mock().expectOneCall("function").andReturnValue(10);
  mock().actualCall("function").returnValue().getIntValue();
  int value = mock().returnValue().getIntValue();
  LONGS_EQUAL(10, value);
}

TEST(MockDocumentation, setData)
{
  ClassFromProductionCode object;
  mock().setData("importantValue", 10);
  mock().setDataObject("importantObject", "ClassFromProductionCode", &object);

  ClassFromProductionCode* pobject;
  int value = mock().getData("importantValue").getIntValue();
  pobject = static_cast<ClassFromProductionCode*>(
      mock().getData("importantObject").getObjectPointer());

  LONGS_EQUAL(10, value);
  POINTERS_EQUAL(pobject, &object);
}

static void
doSomethingThatWouldOtherwiseBlowUpTheMockingFramework()
{
}

TEST(MockDocumentation, otherMockSupport)
{
  mock().crashOnFailure();
  //	mock().actualCall("unex");

  mock().expectOneCall("foo");
  mock().ignoreOtherCalls();

  mock().disable();
  doSomethingThatWouldOtherwiseBlowUpTheMockingFramework();
  mock().enable();

  mock().clear();
}

TEST(MockDocumentation, scope)
{
  mock("xmlparser").expectOneCall("open");
  mock("filesystem").ignoreOtherCalls();

  mock("xmlparser").actualCall("open");
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

  mock_c()
      ->expectOneCall("foo")
      ->withIntParameters("integer", 10)
      ->andReturnDoubleValue(1.11);
  double d = mock_c()
                 ->actualCall("foo")
                 ->withIntParameters("integer", 10)
                 ->returnValue()
                 .value.doubleValue;
  DOUBLES_EQUAL(1.11, d, 0.00001);

  mock_c()->installComparator("type", equalMethod, toStringMethod);
  mock_scope_c("scope")->expectOneCall("bar")->withParameterOfType(
      "type", "name", object);
  mock_scope_c("scope")->actualCall("bar")->withParameterOfType(
      "type", "name", object);
  mock_c()->removeAllComparatorsAndCopiers();

  mock_c()->setIntData("important", 10);
  mock_c()->checkExpectations();
  mock_c()->clear();
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
