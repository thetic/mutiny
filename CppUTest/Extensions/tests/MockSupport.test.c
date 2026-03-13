#include "CppUTest/Extensions/MockSupport.h"

#include "MockSupport_cTestCFile.h"

static int
typeNameIsEqual(const void* object1, const void* object2)
{
  return object1 == object2;
}

static const char*
typeNameValueToString(const void* object)
{
  return (const char*)object;
}

void
all_mock_support_c_calls(void)
{
  mock()->strictOrder();
  mock()->expectOneCall("boo");
  mock()->expectNoCall("bla");
  mock()->expectNCalls(1, "foo");
  mock()->actualCall("boo");
  mock()->actualCall("foo");
  mock()->checkExpectations();

  mock()
      ->expectOneCall("boo")
      ->withIntParameters("integer", 1)
      ->withBoolParameters("bool", 1)
      ->withUnsignedIntParameters("unsigned", 1)
      ->withLongIntParameters("long int", (long int)-1)
      ->withUnsignedLongIntParameters("unsigned long int", (unsigned long int)1)
      ->withLongLongIntParameters("long long int", (long long int)-1)
      ->withUnsignedLongLongIntParameters(
          "unsigned long long int", (unsigned long long int)1)
      ->withDoubleParameters("double", 1.0)
      ->withDoubleParametersAndTolerance("doubleWithTolerance", 1.0, 1.0)
      ->withStringParameters("string", "string")
      ->withPointerParameters("pointer", (void*)1)
      ->withConstPointerParameters("constpointer", (const void*)1)
      ->withFunctionPointerParameters("functionpointer", (void (*)(void))1)
      ->withMemoryBufferParameter("name", (void*)1, 0UL)
      ->ignoreOtherParameters();

  mock()
      ->actualCall("boo")
      ->withIntParameters("integer", 1)
      ->withBoolParameters("bool", 1)
      ->withUnsignedIntParameters("unsigned", 1)
      ->withLongIntParameters("long int", (long int)-1)
      ->withUnsignedLongIntParameters("unsigned long int", (unsigned long int)1)
      ->withLongLongIntParameters("long long int", (long long int)-1)
      ->withUnsignedLongLongIntParameters(
          "unsigned long long int", (unsigned long long int)1)
      ->withDoubleParameters("double", 1.0)
      ->withDoubleParameters("doubleWithTolerance", 0.0)
      ->withStringParameters("string", "string")
      ->withPointerParameters("pointer", (void*)1)
      ->withConstPointerParameters("constpointer", (const void*)1)
      ->withFunctionPointerParameters("functionpointer", (void (*)(void))1)
      ->withMemoryBufferParameter("name", (void*)1, 0UL)
      ->hasReturnValue();

  mock()->disable();
  mock()
      ->expectOneCall("boo")
      ->withParameterOfType("type", "name", (void*)1)
      ->withOutputParameterReturning("name", (void*)1, 0UL)
      ->withOutputParameterOfTypeReturning("type", "name", (void*)1);
  mock()
      ->actualCall("boo")
      ->withParameterOfType("type", "name", (void*)1)
      ->withOutputParameter("name", (void*)1)
      ->withOutputParameterOfType("type", "name", (void*)1);
  mock()->enable();

  mock()->clear();

  mock()->installComparator("typeName", typeNameIsEqual, typeNameValueToString);
  mock()->expectOneCall("boo")->withParameterOfType(
      "typeName", "name", (void*)1);
  mock()->actualCall("boo")->withParameterOfType("typeName", "name", (void*)1);
  mock()->clear();
  mock()->removeAllComparatorsAndCopiers();

  mock()->expectOneCall("boo")->andReturnBoolValue(1);
  mock()->actualCall("boo")->boolReturnValue();
  mock()->boolReturnValue();

  mock()->expectOneCall("boo")->andReturnIntValue(-10);
  mock()->actualCall("boo")->intReturnValue();
  mock()->intReturnValue();
  mock()->returnValue();

  mock()->expectOneCall("boo2")->andReturnUnsignedIntValue(1);
  mock()->actualCall("boo2")->unsignedIntReturnValue();
  mock()->unsignedIntReturnValue();

  mock()->expectOneCall("boo3")->andReturnLongIntValue(1);
  mock()->actualCall("boo3")->longIntReturnValue();
  mock()->longIntReturnValue();

  mock()->expectOneCall("boo3")->andReturnUnsignedLongIntValue(1);
  mock()->actualCall("boo3")->unsignedLongIntReturnValue();
  mock()->unsignedLongIntReturnValue();

  mock()->expectOneCall("mgrgrgr1")->andReturnLongLongIntValue(1);
  mock()->actualCall("mgrgrgr1")->longLongIntReturnValue();
  mock()->longLongIntReturnValue();

  mock()->expectOneCall("mgrgrgr2")->andReturnUnsignedLongLongIntValue(1);
  mock()->actualCall("mgrgrgr2")->unsignedLongLongIntReturnValue();
  mock()->unsignedLongLongIntReturnValue();

  mock()->expectOneCall("boo4")->andReturnDoubleValue(1.0);
  mock()->actualCall("boo4")->doubleReturnValue();
  mock()->doubleReturnValue();

  mock()->expectOneCall("boo5")->andReturnStringValue("hello world");
  mock()->actualCall("boo5")->stringReturnValue();
  mock()->stringReturnValue();

  mock()->expectOneCall("boo6")->andReturnPointerValue((void*)10);
  mock()->actualCall("boo6")->pointerReturnValue();
  mock()->pointerReturnValue();

  mock()->expectOneCall("boo7")->andReturnConstPointerValue((void*)10);
  mock()->actualCall("boo7")->constPointerReturnValue();
  mock()->constPointerReturnValue();

  mock()->expectOneCall("boo8")->andReturnFunctionPointerValue(
      (void (*)(void))10);
  mock()->actualCall("boo8")->functionPointerReturnValue();
  mock()->functionPointerReturnValue();

  mock()->setBoolData("bool", 1);
  mock()->expectOneCall("bla")->withBoolParameters("bool", 1);
  mock()->actualCall("bla")->withBoolParameters(
      "bool", mock()->getData("bool").value.boolValue);

  mock()->setIntData("int", 5);
  mock()->expectOneCall("bla")->withIntParameters("int", 5);
  mock()->actualCall("bla")->withIntParameters(
      "int", mock()->getData("int").value.intValue);

  mock()->setStringData("string", "lol");
  mock()->expectOneCall("bla")->withStringParameters("str", "lol");
  mock()->actualCall("bla")->withStringParameters(
      "str", mock()->getData("string").value.stringValue);

  mock()->setDoubleData("double", 0.001);
  mock()->expectOneCall("bla")->withDoubleParameters("double", 0.001);
  mock()->actualCall("bla")->withDoubleParameters(
      "double", mock()->getData("double").value.doubleValue);

  mock()->setPointerData("ptr", (void*)1);
  mock()->expectOneCall("bla")->withPointerParameters("ptr", (void*)1);
  mock()->actualCall("bla")->withPointerParameters(
      "ptr", mock()->getData("ptr").value.pointerValue);

  mock()->setConstPointerData("cptr", (const void*)1);
  mock()->expectOneCall("bla")->withConstPointerParameters(
      "cptr", (const void*)1);
  mock()->actualCall("bla")->withConstPointerParameters(
      "cptr", mock()->getData("ptr").value.constPointerValue);

  mock()->setFunctionPointerData("ptr", (void (*)(void))1);
  mock()->expectOneCall("bla")->withFunctionPointerParameters(
      "ptr", (void (*)(void))1);
  mock()->actualCall("bla")->withFunctionPointerParameters(
      "ptr", mock()->getData("ptr").value.functionPointerValue);

  mock()->clear();

  mock()->hasReturnValue();
  mock()->returnBoolValueOrDefault(1);
  mock()->returnIntValueOrDefault(-1);
  mock()->returnUnsignedIntValueOrDefault(1);
  mock()->returnLongIntValueOrDefault(-1L);
  mock()->returnUnsignedLongIntValueOrDefault(1L);
  mock()->returnLongLongIntValueOrDefault(-1LL);
  mock()->returnUnsignedLongLongIntValueOrDefault(1ULL);
  mock()->returnStringValueOrDefault("");
  mock()->returnDoubleValueOrDefault(0.01);
  mock()->returnPointerValueOrDefault(0);
  mock()->returnConstPointerValueOrDefault(0);
  mock()->returnFunctionPointerValueOrDefault(0);

  mock()->disable();
  mock()->actualCall("disabled");
  mock()->enable();
  mock()->checkExpectations();

  mock()->setIntData("bla1", -2);
  mock()->setUnsignedIntData("bla2", 2);
  mock()->setDoubleData("bla3", 0.035);
  mock()->setStringData("bla4", "abc");
  mock()->setPointerData("bla", (void*)2);
  mock()->setConstPointerData("bla", (const void*)2);
  mock()->setFunctionPointerData("bla", (void (*)(void))2);
  mock()->setDataObject("bla", "type", (void*)2);
  mock()->getData("bla");

  mock_scope("scope")->expectOneCall("boo");
  mock_scope("other")->expectedCallsLeft();
  mock_scope("scope")->expectedCallsLeft();
  mock_scope("scope")->actualCall("boo");
}
