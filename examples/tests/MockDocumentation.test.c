#include "CppUTest/Extensions/MockSupport.h"

#include "CppUTest/TestHarness.h"

static int
equalMethod(const void* object1, const void* object2)
{
  return object1 == object2;
}

static const char*
toStringMethod(const void* object)
{
  (void)object;
  return "string";
}

TEST_C(MockDocumentation_C, CInterface)
{
  void* object = (void*)0x1;

  mock()
      ->expectOneCall("foo")
      ->withIntParameters("integer", 10)
      ->andReturnDoubleValue(1.11);
  double d = mock()
                 ->actualCall("foo")
                 ->withIntParameters("integer", 10)
                 ->returnValue()
                 .value.doubleValue;
  CHECK_EQUAL_C_REAL(1.11, d, 0.00001);

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
