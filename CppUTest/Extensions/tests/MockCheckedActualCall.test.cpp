#include "CppUTest/Extensions/MockCheckedActualCall.hpp"

#include "MockFailureReporterForTest.hpp"

#include "CppUTest/Extensions/MockActualCallTrace.hpp"
#include "CppUTest/Extensions/MockCheckedExpectedCall.hpp"
#include "CppUTest/Extensions/MockExpectedCallsList.hpp"
#include "CppUTest/Extensions/MockFailure.hpp"
#include "CppUTest/Extensions/MockIgnoredActualCall.hpp"

#include "CppUTest/TestHarness.hpp"

TEST_GROUP(MockCheckedActualCall)
{
  MockExpectedCallsListForTest::MockExpectedCallsList* emptyList;
  MockExpectedCallsListForTest::MockExpectedCallsList* list;
  cpputest::extensions::MockFailureReporter* reporter;

  void setup() override
  {
    emptyList = new MockExpectedCallsListForTest::MockExpectedCallsList;
    list = new MockExpectedCallsListForTest::MockExpectedCallsList;
    reporter = MockFailureReporterForTest::getReporter();
  }

  void teardown() override
  {
    CHECK_NO_MOCK_FAILURE();

    MockFailureReporterForTest::clearReporter();

    delete emptyList;
    delete list;
  }
};

TEST(MockCheckedActualCall, unExpectedCall)
{
  cpputest::extensions::MockCheckedActualCall actualCall(
      1, reporter, *emptyList);
  actualCall.withName("unexpected");

  cpputest::extensions::MockUnexpectedCallHappenedFailure expectedFailure(
      mockFailureTest(), "unexpected", *list);
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCheckedActualCall, unExpectedCallWithAParameter)
{
  cpputest::extensions::MockCheckedActualCall actualCall(
      1, reporter, *emptyList);
  actualCall.withName("unexpected").withParameter("bar", 0);

  cpputest::extensions::MockUnexpectedCallHappenedFailure expectedFailure(
      mockFailureTest(), "unexpected", *list);
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCheckedActualCall, unExpectedCallWithAnOutputParameter)
{
  cpputest::extensions::MockCheckedActualCall actualCall(
      1, reporter, *emptyList);
  actualCall.withName("unexpected").withOutputParameter("bar", nullptr);

  cpputest::extensions::MockUnexpectedCallHappenedFailure expectedFailure(
      mockFailureTest(), "unexpected", *list);
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCheckedActualCall, unExpectedCallOnObject)
{
  int object = 0;

  cpputest::extensions::MockCheckedActualCall actualCall(
      1, reporter, *emptyList);
  actualCall.withName("unexpected").onObject(&object);

  cpputest::extensions::MockUnexpectedCallHappenedFailure expectedFailure(
      mockFailureTest(), "unexpected", *list);
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);

  CHECK(actualCall
          .hasFailed()); // Checks that onObject() doesn't "reset" call state
}

TEST(MockCheckedActualCall,
    actualCallWithNoReturnValueAndMeaninglessCallOrderForCoverage)
{
  cpputest::extensions::MockCheckedActualCall actualCall(
      1, reporter, *emptyList);
  actualCall.withName("noreturn").withCallOrder(0).returnValue();

  cpputest::extensions::MockUnexpectedCallHappenedFailure expectedFailure(
      mockFailureTest(), "noreturn", *list);
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCheckedActualCall, unExpectedParameterName)
{
  cpputest::extensions::MockCheckedExpectedCall call1;
  call1.withName("func");
  list->addExpectedCall(&call1);

  cpputest::extensions::MockCheckedActualCall actualCall(1, reporter, *list);
  actualCall.withName("func").withParameter("integer", 1);

  cpputest::extensions::MockNamedValue parameter("integer");
  parameter.setValue(1);

  cpputest::extensions::MockUnexpectedInputParameterFailure expectedFailure(
      mockFailureTest(), "func", parameter, *list);
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCheckedActualCall, multipleSameFunctionsExpectingAndHappenGradually)
{
  auto* call1 = new cpputest::extensions::MockCheckedExpectedCall();
  auto* call2 = new cpputest::extensions::MockCheckedExpectedCall();
  call1->withName("func");
  call2->withName("func");
  list->addExpectedCall(call1);
  list->addExpectedCall(call2);

  LONGS_EQUAL(2, list->amountOfUnfulfilledExpectations());

  cpputest::extensions::MockCheckedActualCall actualCall1(1, reporter, *list);
  actualCall1.withName("func");
  actualCall1.checkExpectations();

  LONGS_EQUAL(1, list->amountOfUnfulfilledExpectations());

  cpputest::extensions::MockCheckedActualCall actualCall2(2, reporter, *list);
  actualCall2.withName("func");
  actualCall2.checkExpectations();

  LONGS_EQUAL(0, list->amountOfUnfulfilledExpectations());

  list->deleteAllExpectationsAndClearList();
}

TEST(MockCheckedActualCall, MockIgnoredActualCallWorksAsItShould)
{
  cpputest::extensions::MockIgnoredActualCall actual;
  actual.withName("func");
  actual.withCallOrder(1);

  CHECK(false == actual.returnBoolValue());
  CHECK(true == actual.returnBoolValueOrDefault(true));
  CHECK(false == actual.returnBoolValueOrDefault(false));
  CHECK(0 == actual.returnUnsignedLongIntValue());
  CHECK(0 == actual.returnIntValue());
  CHECK(1ul == actual.returnUnsignedLongIntValueOrDefault(1ul));
  CHECK(1 == actual.returnIntValueOrDefault(1));
  CHECK(0 == actual.returnLongIntValue());
  CHECK(1l == actual.returnLongIntValueOrDefault(1l));
  CHECK(0 == actual.returnUnsignedIntValue());
  CHECK(1u == actual.returnUnsignedIntValueOrDefault(1u));
  CHECK(0 == actual.returnLongLongIntValue());
  CHECK(1ll == actual.returnLongLongIntValueOrDefault(1ll));
  CHECK(0 == actual.returnUnsignedLongLongIntValue());
  CHECK(1ull == actual.returnUnsignedLongLongIntValueOrDefault(1ull));
  DOUBLES_EQUAL(0.0, actual.returnDoubleValue(), 0.0);
  DOUBLES_EQUAL(1.5, actual.returnDoubleValueOrDefault(1.5), 0.0);
  STRCMP_EQUAL("bla", actual.returnStringValueOrDefault("bla"));
  STRCMP_EQUAL("", actual.returnStringValue());
  CHECK(nullptr == actual.returnPointerValue());
  CHECK(reinterpret_cast<void*>(0x2) ==
        actual.returnPointerValueOrDefault(reinterpret_cast<void*>(0x2)));
  CHECK(nullptr == actual.returnConstPointerValue());
  CHECK(reinterpret_cast<const void*>(0x2) ==
        actual.returnConstPointerValueOrDefault(
            reinterpret_cast<const void*>(0x2)));
  CHECK(nullptr == actual.returnFunctionPointerValue());
  CHECK(reinterpret_cast<void (*)()>(1) ==
        actual.returnFunctionPointerValueOrDefault(
            reinterpret_cast<void (*)()>(0x1)));
  CHECK_FALSE(actual.hasReturnValue());
  CHECK(actual.returnValue().equals(cpputest::extensions::MockNamedValue("")));
}

TEST(MockCheckedActualCall, remainderOfMockActualCallTraceWorksAsItShould)
{
  int value = 0;
  const int const_value = 1;
  const unsigned char mem_buffer[] = { 0xFE, 0x15 };
  void (*function_value)() = reinterpret_cast<void (*)()>(0xDEAD);
  cpputest::extensions::MockActualCallTrace actual;
  actual.withName("func");
  actual.withCallOrder(1);
  actual.onObject(&value);

  actual.withBoolParameter("bool", true);
  actual.withUnsignedIntParameter("unsigned_int", static_cast<unsigned int>(1));
  actual.withUnsignedLongIntParameter(
      "unsigned_long", static_cast<unsigned long>(1));
  actual.withLongIntParameter("long_int", static_cast<long int>(1));
  actual.withLongLongIntParameter(
      "long_long_int", static_cast<long long int>(1));
  actual.withUnsignedLongLongIntParameter(
      "unsigned_long_long_int", static_cast<unsigned long long int>(1));
  actual.withPointerParameter("pointer", &value);
  actual.withConstPointerParameter("const_pointer", &const_value);
  actual.withFunctionPointerParameter("function_pointer", function_value);
  actual.withMemoryBufferParameter(
      "mem_buffer", mem_buffer, sizeof(mem_buffer));
  actual.withParameterOfType("int", "named_type", &const_value);

  cpputest::String expectedString("\nFunction name:func");
  expectedString += " withCallOrder:1";
  expectedString += " onObject:0x";
  expectedString += cpputest::HexStringFrom(&value);
  expectedString += " bool:true";
  expectedString += " unsigned_int:1 (0x1)";
  expectedString += " unsigned_long:1 (0x1)";
  expectedString += " long_int:1 (0x1)";
  expectedString += " long_long_int:1 (0x1)";
  expectedString += " unsigned_long_long_int:1 (0x1)";
  expectedString += " pointer:0x";
  expectedString += cpputest::HexStringFrom(&value);
  expectedString += " const_pointer:0x";
  expectedString += cpputest::HexStringFrom(&const_value);
  expectedString += " function_pointer:0x";
  expectedString += cpputest::HexStringFrom(function_value);
  expectedString += " mem_buffer:Size = 2 | HexContents = FE 15";
  expectedString += " int named_type:0x";
  expectedString += cpputest::HexStringFrom(&const_value);
  STRCMP_EQUAL(expectedString.c_str(), actual.getTraceOutput());

  CHECK_FALSE(actual.hasReturnValue());
  CHECK(actual.returnValue().equals(cpputest::extensions::MockNamedValue("")));
  CHECK(false == actual.returnBoolValue());
  CHECK(false == actual.returnBoolValueOrDefault(true));
  CHECK(0 == actual.returnLongIntValue());
  CHECK(0 == actual.returnUnsignedLongIntValue());
  CHECK(0 == actual.returnIntValue());
  CHECK(0 == actual.returnUnsignedLongIntValueOrDefault(1ul));
  CHECK(0 == actual.returnIntValueOrDefault(1));
  CHECK(0 == actual.returnLongIntValue());
  CHECK(0 == actual.returnLongIntValueOrDefault(1l));
  CHECK(0 == actual.returnLongLongIntValue());
  CHECK(0 == actual.returnLongLongIntValueOrDefault(1ll));
  CHECK(0 == actual.returnUnsignedLongLongIntValue());
  CHECK(0 == actual.returnUnsignedLongLongIntValueOrDefault(1ull));
  CHECK(0 == actual.returnUnsignedIntValue());
  CHECK(0 == actual.returnUnsignedIntValueOrDefault(1u));
  DOUBLES_EQUAL(0.0, actual.returnDoubleValue(), 0.0);
  DOUBLES_EQUAL(0.0, actual.returnDoubleValueOrDefault(1.0), 0.0);
  STRCMP_EQUAL("", actual.returnStringValueOrDefault("bla"));
  STRCMP_EQUAL("", actual.returnStringValue());
  CHECK(nullptr == actual.returnPointerValue());
  CHECK(nullptr ==
        actual.returnPointerValueOrDefault(static_cast<void*>(nullptr)));
  CHECK(nullptr == actual.returnConstPointerValue());
  CHECK(nullptr == actual.returnConstPointerValueOrDefault(
                       static_cast<const void*>(nullptr)));
  CHECK(nullptr == actual.returnFunctionPointerValue());
  CHECK(nullptr == actual.returnFunctionPointerValueOrDefault(
                       static_cast<void (*)()>(nullptr)));
}

TEST(MockCheckedActualCall, MockActualCallTraceClear)
{
  cpputest::extensions::MockActualCallTrace actual;
  actual.withName("func");
  actual.clear();
  STRCMP_EQUAL("", actual.getTraceOutput());
}
