#include "MockSupport.h"

#include "CppUTest/Extensions/MockSupport.h"
#include "CppUTest/Extensions/OrderedTest.hpp"

#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestTestingFixture.hpp"

namespace {
void
dummy_function_for_mock_c_test()
{
}

void
dummy_function_for_mock_c_test_two()
{
}

int
typeNameIsEqual(const void* object1, const void* object2)
{
  return object1 == object2;
}

const char*
typeNameValueToString(const void* /*object*/)
{
  return "valueToString";
}

void
typeCopy(void* dst, const void* src)
{
  *static_cast<int*>(dst) = *static_cast<const int*>(src);
}

bool destructorWasCalled = false;

void
failedCallToMockC()
{
  struct SetBoolOnDestruct
  {
    bool& b_;
    ~SetBoolOnDestruct() { b_ = true; }
  } setOneDestructor{ destructorWasCalled };
  mock()->actualCall("Not a call");
}

bool cpputestHasCrashed;

void
crashMethod()
{
  cpputestHasCrashed = true;
}

void
failingCallToMockCWithParameterOfType_()
{
  mock()->expectOneCall("bar")->withParameterOfType(
      "typeName", "name", reinterpret_cast<const void*>(1));
  mock()->actualCall("bar")->withParameterOfType(
      "typeName", "name", reinterpret_cast<const void*>(2));
}

void
callToMockCWithOutputParameterOfType_()
{
  int value1 = 7;
  const int value2 = 9;
  mock()->expectOneCall("bar")->withOutputParameterOfTypeReturning(
      "intType", "bla", &value2);
  mock()->actualCall("bar")->withOutputParameterOfType(
      "intType", "bla", &value1);
  LONGS_EQUAL(value1, value2);
}

void
failingCallToMockCWithMemoryBuffer_()
{
  unsigned char memBuffer1[] = { 0x12, 0x15, 0xFF };
  unsigned char memBuffer2[] = { 0x12, 0x05, 0xFF };
  mock()->expectOneCall("bar")->withMemoryBufferParameter(
      "name", memBuffer1, sizeof(memBuffer1));
  mock()->actualCall("bar")->withMemoryBufferParameter(
      "name", memBuffer2, sizeof(memBuffer2));
}

} // namespace

TEST_GROUP(MockSupport_c)
{
  void teardown() override { mock()->clear(); }
};

TEST(MockSupport_c, OrderObserved)
{
  mock()->strictOrder();

  mock()->expectOneCall("foo1");
  mock()->expectOneCall("foo2");
  mock()->actualCall("foo1");
  mock()->actualCall("foo2");

  mock()->checkExpectations();
}

TEST(MockSupport_c, hasReturnValue)
{
  mock()->expectOneCall("foo");
  CHECK(mock()->actualCall("foo")->hasReturnValue() == 0);
  CHECK(mock()->hasReturnValue() == 0);

  mock()->expectOneCall("foo2")->andReturnIntValue(1);
  CHECK(mock()->actualCall("foo2")->hasReturnValue() != 0);
  CHECK(mock()->hasReturnValue() != 0);
}

TEST(MockSupport_c, expectAndActualOneCall)
{
  mock()->expectOneCall("boo");
  mock()->actualCall("boo");
  mock()->checkExpectations();
}

TEST(MockSupport_c, expectAndActualThreeCalls)
{
  mock()->expectNCalls(3, "boo");
  mock()->actualCall("boo");
  mock()->actualCall("boo");
  mock()->actualCall("boo");
  mock()->checkExpectations();
}

TEST(MockSupport_c, expectNoCall)
{
  mock()->expectNoCall("foo");
  mock()->expectOneCall("bar");
  mock()->actualCall("bar");
  mock()->checkExpectations();
}

TEST(MockSupport_c, expectAndActualParameters)
{
  mock()
      ->expectOneCall("boo")
      ->withIntParameters("integer", 1)
      ->withDoubleParameters("double", 1.0)
      ->withStringParameters("string", "string")
      ->withPointerParameters("pointer", reinterpret_cast<void*>(1))
      ->withFunctionPointerParameters(
          "functionPointer", dummy_function_for_mock_c_test);
  mock()
      ->actualCall("boo")
      ->withIntParameters("integer", 1)
      ->withDoubleParameters("double", 1.0)
      ->withStringParameters("string", "string")
      ->withPointerParameters("pointer", reinterpret_cast<void*>(1))
      ->withFunctionPointerParameters(
          "functionPointer", dummy_function_for_mock_c_test);
}

TEST(MockSupport_c, expectAndActualParametersOnObject)
{
  mock()->installComparator("typeName", typeNameIsEqual, typeNameValueToString);
  mock()->expectOneCall("boo")->withParameterOfType(
      "typeName", "name", reinterpret_cast<const void*>(1));
  mock()->actualCall("boo")->withParameterOfType(
      "typeName", "name", reinterpret_cast<const void*>(1));
  mock()->checkExpectations();
  mock()->removeAllComparatorsAndCopiers();
}

TEST(MockSupport_c, boolParameter)
{
  mock()->expectOneCall("foo")->withBoolParameters("p", 1);
  mock()->actualCall("foo")->withBoolParameters("p", 1);
}

TEST(MockSupport_c, unsignedIntParameter)
{
  mock()->expectOneCall("foo")->withUnsignedIntParameters("p", 1);
  mock()->actualCall("foo")->withUnsignedIntParameters("p", 1);
}

TEST(MockSupport_c, longIntParameter)
{
  mock()->expectOneCall("foo")->withLongIntParameters("p", 1);
  mock()->actualCall("foo")->withLongIntParameters("p", 1);
}

TEST(MockSupport_c, unsignedLongIntParameter)
{
  mock()->expectOneCall("foo")->withUnsignedLongIntParameters("p", 1);
  mock()->actualCall("foo")->withUnsignedLongIntParameters("p", 1);
}

TEST(MockSupport_c, doubleParameterWithTolerance)
{
  mock()->expectOneCall("foo")->withDoubleParametersAndTolerance("p", 2.0, 0.2);
  mock()->actualCall("foo")->withDoubleParameters("p", 1.9);
}

TEST(MockSupport_c, longLongIntParameter)
{
  mock()->expectOneCall("foo")->withLongLongIntParameters("p", 1);
  mock()->actualCall("foo")->withLongLongIntParameters("p", 1);
}

TEST(MockSupport_c, unsignedLongLongIntParameter)
{
  mock()->expectOneCall("foo")->withUnsignedLongLongIntParameters("p", 1);
  mock()->actualCall("foo")->withUnsignedLongLongIntParameters("p", 1);
}

TEST(MockSupport_c, memoryBufferParameter)
{
  const unsigned char mem_buffer[] = { 1, 2, 3 };
  mock()->expectOneCall("foo")->withMemoryBufferParameter(
      "out", mem_buffer, sizeof(mem_buffer));
  mock()->actualCall("foo")->withMemoryBufferParameter(
      "out", mem_buffer, sizeof(mem_buffer));
  mock()->checkExpectations();
}

TEST(MockSupport_c, outputParameters)
{
  int param = 1;
  const int retval = 2;
  mock()->expectOneCall("foo")->withOutputParameterReturning(
      "out", &retval, sizeof(retval));
  mock()->actualCall("foo")->withOutputParameter("out", &param);
  mock()->checkExpectations();
  LONGS_EQUAL(2, param);
  LONGS_EQUAL(2, retval);
}

TEST(MockSupport_c, unmodifiedOutputParameter)
{
  int param = 1;
  mock()->expectOneCall("foo")->withUnmodifiedOutputParameter("out");
  mock()->actualCall("foo")->withOutputParameter("out", &param);
  mock()->checkExpectations();
  LONGS_EQUAL(1, param);
}

TEST(MockSupport_c, outputParameters_differentType)
{
  long param = 1;
  const long retval = 2;
  mock()->expectOneCall("foo")->withOutputParameterReturning(
      "out", &retval, sizeof(retval));
  mock()->actualCall("foo")->withOutputParameter("out", &param);
  mock()->checkExpectations();
  LONGS_EQUAL(2, param);
  LONGS_EQUAL(2, retval);
}

TEST(MockSupport_c, outputParametersOfType)
{
  int param = 1;
  const int retval = 2;
  mock()->installCopier("typeName", typeCopy);
  mock()->expectOneCall("foo")->withOutputParameterOfTypeReturning(
      "typeName", "out", &retval);
  mock()->actualCall("foo")->withOutputParameterOfType(
      "typeName", "out", &param);
  LONGS_EQUAL(2, param);
  LONGS_EQUAL(2, retval);
  mock()->checkExpectations();
  mock()->removeAllComparatorsAndCopiers();
}

TEST(MockSupport_c, ignoreOtherParameters)
{
  mock()
      ->expectOneCall("foo")
      ->withIntParameters("int", 1)
      ->ignoreOtherParameters();
  mock()->actualCall("foo")->withIntParameters("int", 1)->withDoubleParameters(
      "double", 0.01);
  mock()->checkExpectations();
}

TEST(MockSupport_c, returnBoolValue)
{
  int expected_value = 1;
  mock()->expectOneCall("boo")->andReturnBoolValue(expected_value);
  CHECK_EQUAL(expected_value, mock()->actualCall("boo")->boolReturnValue());
  CHECK_EQUAL(expected_value, mock()->boolReturnValue());
  LONGS_EQUAL(MOCKVALUETYPE_BOOL, mock()->returnValue().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnBoolValueOrDefaultShouldIgnoreTheDefault)
{
  int defaultValue = 1;
  int expectedValue = 0;
  mock()->expectOneCall("foo")->andReturnBoolValue(expectedValue);
  LONGS_EQUAL(expectedValue,
      mock()->actualCall("foo")->returnBoolValueOrDefault(defaultValue));
  LONGS_EQUAL(expectedValue, mock()->returnBoolValueOrDefault(defaultValue));
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnBoolValueOrDefaultShouldlUseTheDefaultValue)
{
  int defaultValue = 1;
  mock()->expectOneCall("foo");
  LONGS_EQUAL(defaultValue,
      mock()->actualCall("foo")->returnBoolValueOrDefault(defaultValue));
  LONGS_EQUAL(defaultValue, mock()->returnBoolValueOrDefault(defaultValue));
}

TEST(MockSupport_c, returnIntValue)
{
  int expected_value = -10;
  mock()->expectOneCall("boo")->andReturnIntValue(expected_value);
  LONGS_EQUAL(expected_value, mock()->actualCall("boo")->intReturnValue());
  LONGS_EQUAL(expected_value, mock()->intReturnValue());
  LONGS_EQUAL(MOCKVALUETYPE_INTEGER, mock()->returnValue().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnIntValueOrDefaultShouldIgnoreTheDefault)
{
  int defaultValue = -10;
  int expectedValue = defaultValue - 1;
  mock()->expectOneCall("foo")->andReturnIntValue(expectedValue);
  LONGS_EQUAL(expectedValue,
      mock()->actualCall("foo")->returnIntValueOrDefault(defaultValue));
  LONGS_EQUAL(expectedValue, mock()->returnIntValueOrDefault(defaultValue));
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnIntValueOrDefaultShouldlUseTheDefaultValue)
{
  int defaultValue = -10;
  mock()->expectOneCall("foo");
  LONGS_EQUAL(defaultValue,
      mock()->actualCall("foo")->returnIntValueOrDefault(defaultValue));
  LONGS_EQUAL(defaultValue, mock()->returnIntValueOrDefault(defaultValue));
}

TEST(MockSupport_c, returnUnsignedIntValue)
{
  unsigned int expected_value = 7;
  mock()->expectOneCall("boo")->andReturnUnsignedIntValue(expected_value);
  LONGS_EQUAL(
      expected_value, mock()->actualCall("boo")->unsignedIntReturnValue());
  LONGS_EQUAL(expected_value, mock()->unsignedIntReturnValue());
  LONGS_EQUAL(MOCKVALUETYPE_UNSIGNED_INTEGER, mock()->returnValue().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnUnsignedIntValueOrDefaultShouldIgnoreTheDefault)
{
  unsigned int defaultValue = 10;
  unsigned int expectedValue = defaultValue + 1;
  mock()->expectOneCall("foo")->andReturnUnsignedIntValue(expectedValue);
  LONGS_EQUAL(expectedValue,
      mock()->actualCall("foo")->returnUnsignedIntValueOrDefault(defaultValue));
  LONGS_EQUAL(
      expectedValue, mock()->returnUnsignedIntValueOrDefault(defaultValue));
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnUnsignedIntValueOrDefaultShouldlUseTheDefaultValue)
{
  unsigned int defaultValue = 10;
  mock()->expectOneCall("foo");
  LONGS_EQUAL(defaultValue,
      mock()->actualCall("foo")->returnUnsignedIntValueOrDefault(defaultValue));
  LONGS_EQUAL(
      defaultValue, mock()->returnUnsignedIntValueOrDefault(defaultValue));
}

TEST(MockSupport_c, returnLongIntValue)
{
  long int expected_value = -10L;
  mock()->expectOneCall("boo")->andReturnLongIntValue(expected_value);
  LONGS_EQUAL(expected_value, mock()->actualCall("boo")->longIntReturnValue());
  LONGS_EQUAL(expected_value, mock()->longIntReturnValue());
  LONGS_EQUAL(MOCKVALUETYPE_LONG_INTEGER, mock()->returnValue().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnLongIntValueOrDefaultShouldIgnoreTheDefault)
{
  long int defaultValue = -10L;
  long int expectedValue = defaultValue - 1L;
  mock()->expectOneCall("foo")->andReturnLongIntValue(expectedValue);
  LONGS_EQUAL(expectedValue,
      mock()->actualCall("foo")->returnLongIntValueOrDefault(defaultValue));
  LONGS_EQUAL(expectedValue, mock()->returnLongIntValueOrDefault(defaultValue));
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnLongIntValueOrDefaultShouldlUseTheDefaultValue)
{
  long int defaultValue = -10L;
  mock()->expectOneCall("foo");
  LONGS_EQUAL(defaultValue,
      mock()->actualCall("foo")->returnLongIntValueOrDefault(defaultValue));
  LONGS_EQUAL(defaultValue, mock()->returnLongIntValueOrDefault(defaultValue));
}

TEST(MockSupport_c, returnUnsignedLongIntValue)
{
  unsigned long int expected_value = 10;
  mock()->expectOneCall("boo")->andReturnUnsignedLongIntValue(expected_value);
  LONGS_EQUAL(
      expected_value, mock()->actualCall("boo")->unsignedLongIntReturnValue());
  LONGS_EQUAL(expected_value, mock()->unsignedLongIntReturnValue());
  LONGS_EQUAL(MOCKVALUETYPE_UNSIGNED_LONG_INTEGER, mock()->returnValue().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnUnsignedLongIntValueOrDefaultShouldIgnoreTheDefault)
{
  unsigned long int defaultValue = 10L;
  unsigned long int expectedValue = defaultValue + 1L;
  mock()->expectOneCall("foo")->andReturnUnsignedLongIntValue(expectedValue);
  LONGS_EQUAL(expectedValue,
      mock()->actualCall("foo")->returnUnsignedLongIntValueOrDefault(
          defaultValue));
  LONGS_EQUAL(
      expectedValue, mock()->returnUnsignedLongIntValueOrDefault(defaultValue));
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnUnsignedLongIntValueOrDefaultShouldlUseTheDefaultValue)
{
  unsigned long int defaultValue = 10L;
  mock()->expectOneCall("foo");
  LONGS_EQUAL(defaultValue,
      mock()->actualCall("foo")->returnUnsignedLongIntValueOrDefault(
          defaultValue));
  LONGS_EQUAL(
      defaultValue, mock()->returnUnsignedLongIntValueOrDefault(defaultValue));
}

TEST(MockSupport_c, returnLongLongIntValue)
{
  long long int expected_value = -10L;
  mock()->expectOneCall("boo")->andReturnLongLongIntValue(expected_value);
  LONGLONGS_EQUAL(
      expected_value, mock()->actualCall("boo")->longLongIntReturnValue());
  LONGLONGS_EQUAL(expected_value, mock()->longLongIntReturnValue());
  LONGLONGS_EQUAL(MOCKVALUETYPE_LONG_LONG_INTEGER, mock()->returnValue().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnLongLongIntValueOrDefaultShouldIgnoreTheDefault)
{
  long long int defaultValue = -10L;
  long long int expectedValue = defaultValue - 1L;
  mock()->expectOneCall("foo")->andReturnLongLongIntValue(expectedValue);
  LONGLONGS_EQUAL(expectedValue,
      mock()->actualCall("foo")->returnLongLongIntValueOrDefault(defaultValue));
  LONGLONGS_EQUAL(
      expectedValue, mock()->returnLongLongIntValueOrDefault(defaultValue));
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnLongLongIntValueOrDefaultShouldlUseTheDefaultValue)
{
  long long int defaultValue = -10L;
  mock()->expectOneCall("foo");
  LONGLONGS_EQUAL(defaultValue,
      mock()->actualCall("foo")->returnLongLongIntValueOrDefault(defaultValue));
  LONGLONGS_EQUAL(
      defaultValue, mock()->returnLongLongIntValueOrDefault(defaultValue));
}

TEST(MockSupport_c, returnUnsignedLongLongIntValue)
{
  unsigned long long int expected_value = 10;
  mock()->expectOneCall("boo")->andReturnUnsignedLongLongIntValue(
      expected_value);
  UNSIGNED_LONGLONGS_EQUAL(expected_value,
      mock()->actualCall("boo")->unsignedLongLongIntReturnValue());
  UNSIGNED_LONGLONGS_EQUAL(
      expected_value, mock()->unsignedLongLongIntReturnValue());
  UNSIGNED_LONGLONGS_EQUAL(
      MOCKVALUETYPE_UNSIGNED_LONG_LONG_INTEGER, mock()->returnValue().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnUnsignedLongLongIntValueOrDefaultShouldIgnoreTheDefault)
{
  unsigned long long int defaultValue = 10L;
  unsigned long long int expectedValue = defaultValue + 1L;
  mock()->expectOneCall("foo")->andReturnUnsignedLongLongIntValue(
      expectedValue);
  UNSIGNED_LONGLONGS_EQUAL(expectedValue,
      mock()->actualCall("foo")->returnUnsignedLongLongIntValueOrDefault(
          defaultValue));
  UNSIGNED_LONGLONGS_EQUAL(expectedValue,
      mock()->returnUnsignedLongLongIntValueOrDefault(defaultValue));
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnUnsignedLongLongIntValueOrDefaultShouldlUseTheDefaultValue)
{
  unsigned long long int defaultValue = 10L;
  mock()->expectOneCall("foo");
  UNSIGNED_LONGLONGS_EQUAL(defaultValue,
      mock()->actualCall("foo")->returnUnsignedLongLongIntValueOrDefault(
          defaultValue));
  UNSIGNED_LONGLONGS_EQUAL(defaultValue,
      mock()->returnUnsignedLongLongIntValueOrDefault(defaultValue));
}

TEST(MockSupport_c, returnStringValue)
{
  mock()->expectOneCall("boo")->andReturnStringValue("hello world");
  STRCMP_EQUAL("hello world", mock()->actualCall("boo")->stringReturnValue());
  STRCMP_EQUAL("hello world", mock()->stringReturnValue());
  LONGS_EQUAL(MOCKVALUETYPE_STRING, mock()->returnValue().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnStringValueOrDefaultShouldIgnoreTheDefault)
{
  const char defaultValue[] = "bar";
  const char expectedValue[] = "bla";
  mock()->expectOneCall("foo")->andReturnStringValue(expectedValue);
  STRCMP_EQUAL(expectedValue,
      mock()->actualCall("foo")->returnStringValueOrDefault(defaultValue));
  STRCMP_EQUAL(expectedValue, mock()->returnStringValueOrDefault(defaultValue));
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnStringValueOrDefaultShouldlUseTheDefaultValue)
{
  const char defaultValue[] = "bar";
  mock()->expectOneCall("foo");
  STRCMP_EQUAL(defaultValue,
      mock()->actualCall("foo")->returnStringValueOrDefault(defaultValue));
  STRCMP_EQUAL(defaultValue, mock()->returnStringValueOrDefault(defaultValue));
}

TEST(MockSupport_c, returnDoubleValue)
{
  mock()->expectOneCall("boo")->andReturnDoubleValue(1.0);
  DOUBLES_EQUAL(1.0, mock()->actualCall("boo")->doubleReturnValue(), 0.005);
  DOUBLES_EQUAL(1.0, mock()->doubleReturnValue(), 0.005);
  LONGS_EQUAL(MOCKVALUETYPE_DOUBLE, mock()->returnValue().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnDoubleValueOrDefaultShouldIgnoreTheDefault)
{
  double defaultValue = 2.2;
  double expectedValue = defaultValue + 0.1;
  mock()->expectOneCall("foo")->andReturnDoubleValue(expectedValue);
  DOUBLES_EQUAL(expectedValue,
      mock()->actualCall("foo")->returnDoubleValueOrDefault(defaultValue),
      0.005);
  DOUBLES_EQUAL(
      expectedValue, mock()->returnDoubleValueOrDefault(defaultValue), 0.005);
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnDoubleValueOrDefaultShouldlUseTheDefaultValue)
{
  double defaultValue = 2.2;
  mock()->expectOneCall("foo");
  DOUBLES_EQUAL(defaultValue,
      mock()->actualCall("foo")->returnDoubleValueOrDefault(defaultValue),
      0.005);
  DOUBLES_EQUAL(
      defaultValue, mock()->returnDoubleValueOrDefault(defaultValue), 0.005);
}

TEST(MockSupport_c, returnPointerValue)
{
  mock()->expectOneCall("boo")->andReturnPointerValue(
      reinterpret_cast<void*>(10));
  POINTERS_EQUAL(reinterpret_cast<void*>(10),
      mock()->actualCall("boo")->pointerReturnValue());
  POINTERS_EQUAL(reinterpret_cast<void*>(10), mock()->pointerReturnValue());
  LONGS_EQUAL(MOCKVALUETYPE_POINTER, mock()->returnValue().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnPointerValueOrDefaultShouldIgnoreTheDefault)
{
  void* defaultValue = reinterpret_cast<void*>(10);
  void* expectedValue = reinterpret_cast<void*>(27);
  mock()->expectOneCall("foo")->andReturnPointerValue(expectedValue);
  POINTERS_EQUAL(expectedValue,
      mock()->actualCall("foo")->returnPointerValueOrDefault(defaultValue));
  POINTERS_EQUAL(
      expectedValue, mock()->returnPointerValueOrDefault(defaultValue));
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnPointerValueOrDefaultShouldlUseTheDefaultValue)
{
  void* defaultValue = reinterpret_cast<void*>(10);
  mock()->expectOneCall("foo");
  POINTERS_EQUAL(defaultValue,
      mock()->actualCall("foo")->returnPointerValueOrDefault(defaultValue));
  POINTERS_EQUAL(
      defaultValue, mock()->returnPointerValueOrDefault(defaultValue));
}

TEST(MockSupport_c, returnConstPointerValue)
{
  mock()->expectOneCall("boo")->andReturnConstPointerValue(
      reinterpret_cast<const void*>(10));
  POINTERS_EQUAL(reinterpret_cast<const void*>(10),
      mock()->actualCall("boo")->constPointerReturnValue());
  POINTERS_EQUAL(
      reinterpret_cast<const void*>(10), mock()->constPointerReturnValue());
  LONGS_EQUAL(MOCKVALUETYPE_CONST_POINTER, mock()->returnValue().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnConstPointerValueOrDefaultShouldIgnoreTheDefault)
{
  const void* defaultValue = reinterpret_cast<void*>(10);
  const void* expectedValue = reinterpret_cast<void*>(27);
  mock()->expectOneCall("foo")->andReturnConstPointerValue(expectedValue);
  POINTERS_EQUAL(expectedValue,
      mock()->actualCall("foo")->returnConstPointerValueOrDefault(
          defaultValue));
  POINTERS_EQUAL(
      expectedValue, mock()->returnConstPointerValueOrDefault(defaultValue));
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnConstPointerValueOrDefaultShouldlUseTheDefaultValue)
{
  const void* defaultValue = reinterpret_cast<void*>(10);
  mock()->expectOneCall("foo");
  POINTERS_EQUAL(defaultValue,
      mock()->actualCall("foo")->returnConstPointerValueOrDefault(
          defaultValue));
  POINTERS_EQUAL(
      defaultValue, mock()->returnConstPointerValueOrDefault(defaultValue));
}

TEST(MockSupport_c, returnFunctionPointerValue)
{
  mock()->expectOneCall("boo")->andReturnFunctionPointerValue(
      dummy_function_for_mock_c_test);
  FUNCTIONPOINTERS_EQUAL(dummy_function_for_mock_c_test,
      mock()->actualCall("boo")->functionPointerReturnValue());
  FUNCTIONPOINTERS_EQUAL(
      dummy_function_for_mock_c_test, mock()->functionPointerReturnValue());
  LONGS_EQUAL(MOCKVALUETYPE_FUNCTIONPOINTER, mock()->returnValue().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnFunctionPointerValueOrDefaultShouldIgnoreTheDefault)
{
  mock()->expectOneCall("foo")->andReturnFunctionPointerValue(
      dummy_function_for_mock_c_test);
  FUNCTIONPOINTERS_EQUAL(dummy_function_for_mock_c_test,
      mock()->actualCall("foo")->returnFunctionPointerValueOrDefault(
          dummy_function_for_mock_c_test_two));
  FUNCTIONPOINTERS_EQUAL(dummy_function_for_mock_c_test,
      mock()->returnFunctionPointerValueOrDefault(
          dummy_function_for_mock_c_test_two));
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnFunctionPointerValueOrDefaultShouldlUseTheDefaultValue)
{
  mock()->expectOneCall("foo");
  FUNCTIONPOINTERS_EQUAL(dummy_function_for_mock_c_test_two,
      mock()->actualCall("foo")->returnFunctionPointerValueOrDefault(
          dummy_function_for_mock_c_test_two));
  FUNCTIONPOINTERS_EQUAL(dummy_function_for_mock_c_test_two,
      mock()->returnFunctionPointerValueOrDefault(
          dummy_function_for_mock_c_test_two));
}

TEST(MockSupport_c, MockSupportWithScope)
{
  mock_scope("scope")->expectOneCall("boo");
  LONGS_EQUAL(0, mock_scope("other")->expectedCallsLeft());
  LONGS_EQUAL(1, mock_scope("scope")->expectedCallsLeft());
  mock_scope("scope")->actualCall("boo");
}

TEST(MockSupport_c, MockSupportSetBoolData)
{
  mock()->setBoolData("boolean", 1);
  CHECK_EQUAL(1, mock()->getData("boolean").value.boolValue);
}

TEST(MockSupport_c, MockSupportSetIntData)
{
  mock()->setIntData("integer", 10);
  LONGS_EQUAL(10, mock()->getData("integer").value.intValue);
}

TEST(MockSupport_c, MockSupportSetLongIntData)
{
  long int i = 100;
  mock()->setLongIntData("long integer", i);
  LONGS_EQUAL(i, mock()->getData("long integer").value.longIntValue);
}

TEST(MockSupport_c, MockSupportSetUnsignedLongIntData)
{
  unsigned long int i = 100;
  mock()->setUnsignedLongIntData("unsigned long integer", i);
  UNSIGNED_LONGS_EQUAL(
      i, mock()->getData("unsigned long integer").value.unsignedLongIntValue);
}

TEST(MockSupport_c, MockSupportSetDoubleData)
{
  mock()->setDoubleData("double", 1.0);
  DOUBLES_EQUAL(1.00, mock()->getData("double").value.doubleValue, 0.05);
}

TEST(MockSupport_c, MockSupportSetStringData)
{
  mock()->setStringData("string", "hello world");
  STRCMP_EQUAL("hello world", mock()->getData("string").value.stringValue);
}

TEST(MockSupport_c, MockSupportSetPointerData)
{
  mock()->setPointerData("pointer", reinterpret_cast<void*>(1));
  POINTERS_EQUAL(reinterpret_cast<void*>(1),
      mock()->getData("pointer").value.pointerValue);
}

TEST(MockSupport_c, MockSupportSetConstPointerData)
{
  mock()->setConstPointerData("constPointer", reinterpret_cast<const void*>(1));
  POINTERS_EQUAL(reinterpret_cast<const void*>(1),
      mock()->getData("constPointer").value.constPointerValue);
}

TEST(MockSupport_c, MockSupportMemoryBufferData)
{
  mock()->setDataObject(
      "name", "const unsigned char*", reinterpret_cast<void*>(0xDEAD));
  POINTERS_EQUAL(reinterpret_cast<void*>(0xDEAD),
      mock()->getData("name").value.memoryBufferValue);
  LONGS_EQUAL(MOCKVALUETYPE_MEMORYBUFFER, mock()->getData("name").type);
}

TEST(MockSupport_c, MockSupportSetFunctionPointerData)
{
  mock()->setFunctionPointerData(
      "functionPointer", dummy_function_for_mock_c_test);
  FUNCTIONPOINTERS_EQUAL(dummy_function_for_mock_c_test,
      mock()->getData("functionPointer").value.functionPointerValue);
}

TEST(MockSupport_c, MockSupportSetDataObject)
{
  mock()->setDataObject("name", "type", reinterpret_cast<void*>(1));
  POINTERS_EQUAL(
      reinterpret_cast<void*>(1), mock()->getData("name").value.objectValue);
  LONGS_EQUAL(MOCKVALUETYPE_OBJECT, mock()->getData("name").type);
}

TEST(MockSupport_c, MockSupportSetDataConstObject)
{
  mock()->setDataConstObject("name", "type", reinterpret_cast<const void*>(5));
  POINTERS_EQUAL(reinterpret_cast<void*>(5),
      mock()->getData("name").value.constObjectValue);
  LONGS_EQUAL(MOCKVALUETYPE_CONST_OBJECT, mock()->getData("name").type);
}

TEST(MockSupport_c, WorksInCFile)
{
  all_mock_support_c_calls();
}

// Silly wrapper because of a test that only fails in Visual C++ due to
// different destructor behaviors
#ifdef _MSC_VER
#define MSC_SWITCHED_TEST(testGroup, testName) IGNORE_TEST(testGroup, testName)
#else
#define MSC_SWITCHED_TEST(testGroup, testName) TEST(testGroup, testName)
#endif

MSC_SWITCHED_TEST(MockSupport_c, NoExceptionsAreThrownWhenAMock_cCallFailed)
{
  cpputest::TestTestingFixture fixture;

  fixture.setTestFunction(failedCallToMockC);
  fixture.runAllTests();

  LONGS_EQUAL(1, fixture.getFailureCount());
  // Odd behavior in Visual C++, destructor still gets called here
  CHECK(!destructorWasCalled);
}

TEST_ORDERED(MockSupport_c, shouldCrashOnFailure, 21)
{
  cpputestHasCrashed = false;
  cpputest::TestTestingFixture fixture;
  cpputest::TestShell::setCrashMethod(crashMethod);
  mock()->crashOnFailure(true);
  fixture.setTestFunction(failedCallToMockC);

  fixture.runAllTests();

  CHECK(cpputestHasCrashed);

  cpputest::TestShell::resetCrashMethod();
  mock()->crashOnFailure(false);
}

TEST_ORDERED(MockSupport_c, nextTestShouldNotCrashOnFailure, 22)
{
  cpputestHasCrashed = false;
  cpputest::TestTestingFixture fixture;
  cpputest::TestShell::setCrashMethod(crashMethod);
  fixture.setTestFunction(failedCallToMockC);

  fixture.runAllTests();

  CHECK_FALSE(cpputestHasCrashed);

  cpputest::TestShell::resetCrashMethod();
}

TEST(MockSupport_c, FailWillNotCrashIfNotEnabled)
{
  cpputestHasCrashed = false;
  cpputest::TestTestingFixture fixture;
  cpputest::TestShell::setCrashMethod(crashMethod);

  fixture.setTestFunction(failedCallToMockC);

  fixture.runAllTests();

  CHECK_FALSE(cpputestHasCrashed);
  LONGS_EQUAL(1, fixture.getFailureCount());

  cpputest::TestShell::resetCrashMethod();
}

TEST(MockSupport_c, FailWillCrashIfEnabled)
{
  cpputestHasCrashed = false;
  cpputest::TestTestingFixture fixture;
  cpputest::TestShell::setCrashOnFail();
  cpputest::TestShell::setCrashMethod(crashMethod);

  fixture.setTestFunction(failedCallToMockC);

  fixture.runAllTests();

  CHECK(cpputestHasCrashed);
  LONGS_EQUAL(1, fixture.getFailureCount());

  cpputest::TestShell::restoreDefaultTestTerminator();
  cpputest::TestShell::resetCrashMethod();
}

TEST(MockSupport_c, failureWithParameterOfTypeCoversValueToString)
{
  cpputest::TestTestingFixture fixture;
  mock()->installComparator("typeName", typeNameIsEqual, typeNameValueToString);
  fixture.setTestFunction(failingCallToMockCWithParameterOfType_);
  fixture.runAllTests();
  fixture.assertPrintContains("typeName name: <valueToString>");
  mock()->removeAllComparatorsAndCopiers();
}

TEST(MockSupport_c, successWithOutputParameterOfType)
{
  cpputest::TestTestingFixture fixture;
  mock()->installCopier("intType", typeCopy);
  fixture.setTestFunction(callToMockCWithOutputParameterOfType_);
  fixture.runAllTests();
  LONGS_EQUAL(2, fixture.getCheckCount());
  LONGS_EQUAL(0, fixture.getFailureCount());
  mock()->removeAllComparatorsAndCopiers();
}

TEST(MockSupport_c, expectOneMemBufferParameterAndValueFailsDueToContents)
{
  cpputest::TestTestingFixture fixture;
  fixture.setTestFunction(failingCallToMockCWithMemoryBuffer_);
  fixture.runAllTests();
  fixture.assertPrintContains(
      "Unexpected parameter value to parameter \"name\" "
      "to function \"bar\": <Size = 3 | HexContents = 12 05 FF>");
}

TEST(MockSupport_c, ignoreOtherCalls)
{
  mock()->expectOneCall("foo");
  mock()->ignoreOtherCalls();
  mock()->actualCall("foo");
  mock()->actualCall("bar");
  mock()->checkExpectations();
}
