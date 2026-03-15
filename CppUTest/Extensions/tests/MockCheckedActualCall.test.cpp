#include "CppUTest/Extensions/MockCheckedActualCall.hpp"

#include "MockFailureReporterForTest.hpp"

#include "CppUTest/Extensions/MockActualCallTrace.hpp"
#include "CppUTest/Extensions/MockCheckedExpectedCall.hpp"
#include "CppUTest/Extensions/MockExpectedCallsList.hpp"
#include "CppUTest/Extensions/MockFailure.hpp"
#include "CppUTest/Extensions/MockIgnoredActualCall.hpp"

#include "CppUTest/CppUTest.hpp"

TEST_GROUP(MockCheckedActualCall)
{
  MockExpectedCallsListForTest::MockExpectedCallsList* empty_list;
  MockExpectedCallsListForTest::MockExpectedCallsList* list;
  cpputest::extensions::MockFailureReporter* reporter;

  void setup() override
  {
    empty_list = new MockExpectedCallsListForTest::MockExpectedCallsList;
    list = new MockExpectedCallsListForTest::MockExpectedCallsList;
    reporter = MockFailureReporterForTest::get_reporter();
  }

  void teardown() override
  {
    CHECK_NO_MOCK_FAILURE();

    MockFailureReporterForTest::clear_reporter();

    delete empty_list;
    delete list;
  }
};

TEST(MockCheckedActualCall, unExpectedCall)
{
  cpputest::extensions::MockCheckedActualCall actual_call(
      1, reporter, *empty_list);
  actual_call.with_name("unexpected");

  cpputest::extensions::MockUnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "unexpected", *list);
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCheckedActualCall, unExpectedCallWithAParameter)
{
  cpputest::extensions::MockCheckedActualCall actual_call(
      1, reporter, *empty_list);
  actual_call.with_name("unexpected").with_parameter("bar", 0);

  cpputest::extensions::MockUnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "unexpected", *list);
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCheckedActualCall, unExpectedCallWithAnOutputParameter)
{
  cpputest::extensions::MockCheckedActualCall actual_call(
      1, reporter, *empty_list);
  actual_call.with_name("unexpected").with_output_parameter("bar", nullptr);

  cpputest::extensions::MockUnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "unexpected", *list);
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCheckedActualCall, unExpectedCallOnObject)
{
  int object = 0;

  cpputest::extensions::MockCheckedActualCall actual_call(
      1, reporter, *empty_list);
  actual_call.with_name("unexpected").on_object(&object);

  cpputest::extensions::MockUnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "unexpected", *list);
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);

  CHECK(actual_call
          .has_failed()); // Checks that onObject() doesn't "reset" call state
}

TEST(MockCheckedActualCall,
    actualCallWithNoReturnValueAndMeaninglessCallOrderForCoverage)
{
  cpputest::extensions::MockCheckedActualCall actual_call(
      1, reporter, *empty_list);
  actual_call.with_name("noreturn").with_call_order(0).return_value();

  cpputest::extensions::MockUnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "noreturn", *list);
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCheckedActualCall, unExpectedParameterName)
{
  cpputest::extensions::MockCheckedExpectedCall call1;
  call1.with_name("func");
  list->add_expected_call(&call1);

  cpputest::extensions::MockCheckedActualCall actual_call(1, reporter, *list);
  actual_call.with_name("func").with_parameter("integer", 1);

  cpputest::extensions::MockNamedValue parameter("integer");
  parameter.set_value(1);

  cpputest::extensions::MockUnexpectedInputParameterFailure expected_failure(
      mock_failure_test(), "func", parameter, *list);
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCheckedActualCall, multipleSameFunctionsExpectingAndHappenGradually)
{
  auto* call1 = new cpputest::extensions::MockCheckedExpectedCall();
  auto* call2 = new cpputest::extensions::MockCheckedExpectedCall();
  call1->with_name("func");
  call2->with_name("func");
  list->add_expected_call(call1);
  list->add_expected_call(call2);

  LONGS_EQUAL(2, list->amount_of_unfulfilled_expectations());

  cpputest::extensions::MockCheckedActualCall actual_call1(1, reporter, *list);
  actual_call1.with_name("func");
  actual_call1.check_expectations();

  LONGS_EQUAL(1, list->amount_of_unfulfilled_expectations());

  cpputest::extensions::MockCheckedActualCall actual_call2(2, reporter, *list);
  actual_call2.with_name("func");
  actual_call2.check_expectations();

  LONGS_EQUAL(0, list->amount_of_unfulfilled_expectations());

  list->delete_all_expectations_and_clear_list();
}

TEST(MockCheckedActualCall, MockIgnoredActualCallWorksAsItShould)
{
  cpputest::extensions::MockIgnoredActualCall actual;
  actual.with_name("func");
  actual.with_call_order(1);

  CHECK(false == actual.return_bool_value());
  CHECK(true == actual.return_bool_value_or_default(true));
  CHECK(false == actual.return_bool_value_or_default(false));
  CHECK(0 == actual.return_unsigned_long_int_value());
  CHECK(0 == actual.return_int_value());
  CHECK(1ul == actual.return_unsigned_long_int_value_or_default(1ul));
  CHECK(1 == actual.return_int_value_or_default(1));
  CHECK(0 == actual.return_long_int_value());
  CHECK(1l == actual.return_long_int_value_or_default(1l));
  CHECK(0 == actual.return_unsigned_int_value());
  CHECK(1u == actual.return_unsigned_int_value_or_default(1u));
  CHECK(0 == actual.return_long_long_int_value());
  CHECK(1ll == actual.return_long_long_int_value_or_default(1ll));
  CHECK(0 == actual.return_unsigned_long_long_int_value());
  CHECK(1ull == actual.return_unsigned_long_long_int_value_or_default(1ull));
  DOUBLES_EQUAL(0.0, actual.return_double_value(), 0.0);
  DOUBLES_EQUAL(1.5, actual.return_double_value_or_default(1.5), 0.0);
  STRCMP_EQUAL("bla", actual.return_string_value_or_default("bla"));
  STRCMP_EQUAL("", actual.return_string_value());
  CHECK(nullptr == actual.return_pointer_value());
  CHECK(reinterpret_cast<void*>(0x2) ==
        actual.return_pointer_value_or_default(reinterpret_cast<void*>(0x2)));
  CHECK(nullptr == actual.return_const_pointer_value());
  CHECK(reinterpret_cast<const void*>(0x2) ==
        actual.return_const_pointer_value_or_default(
            reinterpret_cast<const void*>(0x2)));
  CHECK(nullptr == actual.return_function_pointer_value());
  CHECK(reinterpret_cast<void (*)()>(1) ==
        actual.return_function_pointer_value_or_default(
            reinterpret_cast<void (*)()>(0x1)));
  CHECK_FALSE(actual.has_return_value());
  CHECK(actual.return_value().equals(cpputest::extensions::MockNamedValue("")));
}

TEST(MockCheckedActualCall, remainderOfMockActualCallTraceWorksAsItShould)
{
  int value = 0;
  const int const_value = 1;
  const unsigned char mem_buffer[] = { 0xFE, 0x15 };
  auto function_value = reinterpret_cast<void (*)()>(0xDEAD);
  cpputest::extensions::MockActualCallTrace actual;
  actual.with_name("func");
  actual.with_call_order(1);
  actual.on_object(&value);

  actual.with_bool_parameter("bool", true);
  actual.with_unsigned_int_parameter(
      "unsigned_int", static_cast<unsigned int>(1));
  actual.with_unsigned_long_int_parameter(
      "unsigned_long", static_cast<unsigned long>(1));
  actual.with_long_int_parameter("long_int", static_cast<long int>(1));
  actual.with_long_long_int_parameter(
      "long_long_int", static_cast<long long int>(1));
  actual.with_unsigned_long_long_int_parameter(
      "unsigned_long_long_int", static_cast<unsigned long long int>(1));
  actual.with_pointer_parameter("pointer", &value);
  actual.with_const_pointer_parameter("const_pointer", &const_value);
  actual.with_function_pointer_parameter("function_pointer", function_value);
  actual.with_memory_buffer_parameter(
      "mem_buffer", mem_buffer, sizeof(mem_buffer));
  actual.with_parameter_of_type("int", "named_type", &const_value);

  cpputest::String expected_string("\nFunction name:func");
  expected_string += " withCallOrder:1";
  expected_string += " onObject:0x";
  expected_string += cpputest::hex_string_from(&value);
  expected_string += " bool:true";
  expected_string += " unsigned_int:1 (0x1)";
  expected_string += " unsigned_long:1 (0x1)";
  expected_string += " long_int:1 (0x1)";
  expected_string += " long_long_int:1 (0x1)";
  expected_string += " unsigned_long_long_int:1 (0x1)";
  expected_string += " pointer:0x";
  expected_string += cpputest::hex_string_from(&value);
  expected_string += " const_pointer:0x";
  expected_string += cpputest::hex_string_from(&const_value);
  expected_string += " function_pointer:0x";
  expected_string += cpputest::hex_string_from(function_value);
  expected_string += " mem_buffer:Size = 2 | HexContents = FE 15";
  expected_string += " int named_type:0x";
  expected_string += cpputest::hex_string_from(&const_value);
  STRCMP_EQUAL(expected_string.c_str(), actual.get_trace_output());

  CHECK_FALSE(actual.has_return_value());
  CHECK(actual.return_value().equals(cpputest::extensions::MockNamedValue("")));
  CHECK(false == actual.return_bool_value());
  CHECK(false == actual.return_bool_value_or_default(true));
  CHECK(0 == actual.return_long_int_value());
  CHECK(0 == actual.return_unsigned_long_int_value());
  CHECK(0 == actual.return_int_value());
  CHECK(0 == actual.return_unsigned_long_int_value_or_default(1ul));
  CHECK(0 == actual.return_int_value_or_default(1));
  CHECK(0 == actual.return_long_int_value());
  CHECK(0 == actual.return_long_int_value_or_default(1l));
  CHECK(0 == actual.return_long_long_int_value());
  CHECK(0 == actual.return_long_long_int_value_or_default(1ll));
  CHECK(0 == actual.return_unsigned_long_long_int_value());
  CHECK(0 == actual.return_unsigned_long_long_int_value_or_default(1ull));
  CHECK(0 == actual.return_unsigned_int_value());
  CHECK(0 == actual.return_unsigned_int_value_or_default(1u));
  DOUBLES_EQUAL(0.0, actual.return_double_value(), 0.0);
  DOUBLES_EQUAL(0.0, actual.return_double_value_or_default(1.0), 0.0);
  STRCMP_EQUAL("", actual.return_string_value_or_default("bla"));
  STRCMP_EQUAL("", actual.return_string_value());
  CHECK(nullptr == actual.return_pointer_value());
  CHECK(nullptr ==
        actual.return_pointer_value_or_default(static_cast<void*>(nullptr)));
  CHECK(nullptr == actual.return_const_pointer_value());
  CHECK(nullptr == actual.return_const_pointer_value_or_default(
                       static_cast<const void*>(nullptr)));
  CHECK(nullptr == actual.return_function_pointer_value());
  CHECK(nullptr == actual.return_function_pointer_value_or_default(
                       static_cast<void (*)()>(nullptr)));
}

TEST(MockCheckedActualCall, MockActualCallTraceClear)
{
  cpputest::extensions::MockActualCallTrace actual;
  actual.with_name("func");
  actual.clear();
  STRCMP_EQUAL("", actual.get_trace_output());
}
