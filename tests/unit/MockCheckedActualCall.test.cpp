#include "MockFailureReporterForTest.hpp"

#include "mutiny/mock/ActualCallTrace.hpp"
#include "mutiny/mock/CheckedActualCall.hpp"
#include "mutiny/mock/CheckedExpectedCall.hpp"
#include "mutiny/mock/ExpectedCallsList.hpp"
#include "mutiny/mock/Failure.hpp"
#include "mutiny/mock/IgnoredActualCall.hpp"

#include "mutiny/test.hpp"

TEST_GROUP(CheckedActualCall)
{
  ExpectedCallsListForTest::ExpectedCallsList* empty_list;
  ExpectedCallsListForTest::ExpectedCallsList* list;
  mu::tiny::mock::FailureReporter* reporter;

  void setup() override
  {
    empty_list = new ExpectedCallsListForTest::ExpectedCallsList;
    list = new ExpectedCallsListForTest::ExpectedCallsList;
    reporter = FailureReporterForTest::get_reporter();
  }

  void teardown() override
  {
    CHECK_NO_MOCK_FAILURE();

    FailureReporterForTest::clear_reporter();

    delete empty_list;
    delete list;
  }
};

TEST(CheckedActualCall, unExpectedCall)
{
  mu::tiny::mock::CheckedActualCall actual_call(1, reporter, *empty_list);
  actual_call.with_name("unexpected");

  mu::tiny::mock::UnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "unexpected", *list
  );
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(CheckedActualCall, unExpectedCallWithAParameter)
{
  mu::tiny::mock::CheckedActualCall actual_call(1, reporter, *empty_list);
  actual_call.with_name("unexpected").with_parameter("bar", 0);

  mu::tiny::mock::UnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "unexpected", *list
  );
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(CheckedActualCall, unExpectedCallWithAnOutputParameter)
{
  mu::tiny::mock::CheckedActualCall actual_call(1, reporter, *empty_list);
  actual_call.with_name("unexpected").with_output_parameter("bar", nullptr);

  mu::tiny::mock::UnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "unexpected", *list
  );
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(CheckedActualCall, unExpectedCallOnObject)
{
  int object = 0;

  mu::tiny::mock::CheckedActualCall actual_call(1, reporter, *empty_list);
  actual_call.with_name("unexpected").on_object(&object);

  mu::tiny::mock::UnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "unexpected", *list
  );
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);

  CHECK(
      actual_call.has_failed()
  ); // Checks that onObject() doesn't "reset" call state
}

TEST(CheckedActualCall, actualCallWithNoReturnValueAndMeaninglessCallOrderForCoverage)
{
  mu::tiny::mock::CheckedActualCall actual_call(1, reporter, *empty_list);
  actual_call.with_name("noreturn").with_call_order(0).return_value();

  mu::tiny::mock::UnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "noreturn", *list
  );
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(CheckedActualCall, unExpectedParameterName)
{
  mu::tiny::mock::CheckedExpectedCall call1;
  call1.with_name("func");
  list->add_expected_call(&call1);

  mu::tiny::mock::CheckedActualCall actual_call(1, reporter, *list);
  actual_call.with_name("func").with_parameter("integer", 1);

  mu::tiny::mock::NamedValue parameter("integer");
  parameter.set_value(1);

  mu::tiny::mock::UnexpectedInputParameterFailure expected_failure(
      mock_failure_test(), "func", parameter, *list
  );
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(CheckedActualCall, multipleSameFunctionsExpectingAndHappenGradually)
{
  auto* call1 = new mu::tiny::mock::CheckedExpectedCall();
  auto* call2 = new mu::tiny::mock::CheckedExpectedCall();
  call1->with_name("func");
  call2->with_name("func");
  list->add_expected_call(call1);
  list->add_expected_call(call2);

  CHECK_EQUAL(2U, list->amount_of_unfulfilled_expectations());

  mu::tiny::mock::CheckedActualCall actual_call1(1, reporter, *list);
  actual_call1.with_name("func");
  actual_call1.check_expectations();

  CHECK_EQUAL(1U, list->amount_of_unfulfilled_expectations());

  mu::tiny::mock::CheckedActualCall actual_call2(2, reporter, *list);
  actual_call2.with_name("func");
  actual_call2.check_expectations();

  CHECK_EQUAL(0U, list->amount_of_unfulfilled_expectations());

  list->delete_all_expectations_and_clear_list();
}

TEST(CheckedActualCall, MockIgnoredActualCallWorksAsItShould)
{
  mu::tiny::mock::IgnoredActualCall actual;
  actual.with_name("func");
  actual.with_call_order(1);

  CHECK(false == actual.return_value_as<bool>());
  CHECK(true == actual.return_value_as_or_default(true));
  CHECK(false == actual.return_value_as_or_default(false));
  CHECK(0 == actual.return_value_as<unsigned long int>());
  CHECK(0 == actual.return_value_as<int>());
  CHECK(1ul == actual.return_value_as_or_default(1ul));
  CHECK(1 == actual.return_value_as_or_default(1));
  CHECK(0 == actual.return_value_as<long int>());
  CHECK(1l == actual.return_value_as_or_default(1l));
  CHECK(0 == actual.return_value_as<unsigned int>());
  CHECK(1u == actual.return_value_as_or_default(1u));
  CHECK(0 == actual.return_value_as<long long>());
  CHECK(1ll == actual.return_value_as_or_default(1ll));
  CHECK(0 == actual.return_value_as<unsigned long long>());
  CHECK(1ull == actual.return_value_as_or_default(1ull));
  CHECK_APPROX(0.0, actual.return_value_as<double>(), 0.0);
  CHECK_APPROX(1.5, actual.return_value_as_or_default(1.5), 0.0);
  STRCMP_EQUAL("bla", actual.return_value_as_or_default("bla"));
  STRCMP_EQUAL("", actual.return_value_as<const char*>());
  CHECK(nullptr == actual.return_value_as<void*>());
  CHECK(
      reinterpret_cast<void*>(0x2) ==
      actual.return_value_as_or_default(reinterpret_cast<void*>(0x2))
  );
  CHECK(nullptr == actual.return_value_as<const void*>());
  CHECK(
      reinterpret_cast<const void*>(0x2) ==
      actual.return_value_as_or_default(reinterpret_cast<const void*>(0x2))
  );
  CHECK(
      nullptr == actual.return_value_as<
                     mu::tiny::mock::ActualCall::FunctionPointerReturnValue>()
  );
  CHECK(
      reinterpret_cast<void (*)()>(1) ==
      actual.return_value_as_or_default(reinterpret_cast<void (*)()>(0x1))
  );
  CHECK(!actual.has_return_value());
  CHECK(actual.return_value().equals(mu::tiny::mock::NamedValue("")));
}

TEST(CheckedActualCall, remainderOfMockActualCallTraceWorksAsItShould)
{
  int value = 0;
  const int const_value = 1;
  const unsigned char mem_buffer[] = { 0xFE, 0x15 };
  auto function_value = reinterpret_cast<void (*)()>(0xDEAD);
  mu::tiny::mock::ActualCallTrace actual;
  actual.with_name("func");
  actual.with_call_order(1);
  actual.on_object(&value);

  actual.with_bool_parameter("bool", true);
  actual.with_unsigned_int_parameter("unsigned_int", 1U);
  actual.with_unsigned_long_int_parameter("unsigned_long", 1UL);
  actual.with_long_int_parameter("long_int", 1L);
  actual.with_long_long_int_parameter("long_long_int", 1LL);
  actual.with_unsigned_long_long_int_parameter("unsigned_long_long_int", 1ULL);
  actual.with_pointer_parameter("pointer", &value);
  actual.with_const_pointer_parameter("const_pointer", &const_value);
  actual.with_function_pointer_parameter("function_pointer", function_value);
  actual.with_memory_buffer_parameter(
      "mem_buffer", mem_buffer, sizeof(mem_buffer)
  );
  actual.with_parameter_of_type("int", "named_type", &const_value);

  mu::tiny::String expected_string("\nFunction name:func");
  expected_string += " withCallOrder:1";
  expected_string += " onObject:0x";
  expected_string += mu::tiny::hex_string_from(&value);
  expected_string += " bool:true";
  expected_string += " unsigned_int:1 (0x1)";
  expected_string += " unsigned_long:1 (0x1)";
  expected_string += " long_int:1 (0x1)";
  expected_string += " long_long_int:1 (0x1)";
  expected_string += " unsigned_long_long_int:1 (0x1)";
  expected_string += " pointer:0x";
  expected_string += mu::tiny::hex_string_from(&value);
  expected_string += " const_pointer:0x";
  expected_string += mu::tiny::hex_string_from(&const_value);
  expected_string += " function_pointer:0x";
  expected_string += mu::tiny::hex_string_from(function_value);
  expected_string += " mem_buffer:Size = 2 | HexContents = FE 15";
  expected_string += " int named_type:0x";
  expected_string += mu::tiny::hex_string_from(&const_value);
  STRCMP_EQUAL(expected_string.c_str(), actual.get_trace_output());

  CHECK(!actual.has_return_value());
  CHECK(actual.return_value().equals(mu::tiny::mock::NamedValue("")));
  CHECK(false == actual.return_value_as<bool>());
  CHECK(false == actual.return_value_as_or_default(true));
  CHECK(0 == actual.return_value_as<long int>());
  CHECK(0 == actual.return_value_as<unsigned long int>());
  CHECK(0 == actual.return_value_as<int>());
  CHECK(0 == actual.return_value_as_or_default(1ul));
  CHECK(0 == actual.return_value_as_or_default(1));
  CHECK(0 == actual.return_value_as<long int>());
  CHECK(0 == actual.return_value_as_or_default(1l));
  CHECK(0 == actual.return_value_as<long long>());
  CHECK(0 == actual.return_value_as_or_default(1ll));
  CHECK(0 == actual.return_value_as<unsigned long long>());
  CHECK(0 == actual.return_value_as_or_default(1ull));
  CHECK(0 == actual.return_value_as<unsigned int>());
  CHECK(0 == actual.return_value_as_or_default(1u));
  CHECK_APPROX(0.0, actual.return_value_as<double>(), 0.0);
  CHECK_APPROX(0.0, actual.return_value_as_or_default(1.0), 0.0);
  STRCMP_EQUAL("", actual.return_value_as_or_default("bla"));
  STRCMP_EQUAL("", actual.return_value_as<const char*>());
  CHECK(nullptr == actual.return_value_as<void*>());
  CHECK(
      nullptr == actual.return_value_as_or_default(static_cast<void*>(nullptr))
  );
  CHECK(nullptr == actual.return_value_as<const void*>());
  CHECK(
      nullptr ==
      actual.return_value_as_or_default(static_cast<const void*>(nullptr))
  );
  using FP = mu::tiny::mock::ActualCall::FunctionPointerReturnValue;
  CHECK(nullptr == actual.return_value_as<FP>());
  CHECK(
      nullptr ==
      actual.return_value_as_or_default(static_cast<void (*)()>(nullptr))
  );
}

TEST(CheckedActualCall, MockActualCallTraceClear)
{
  mu::tiny::mock::ActualCallTrace actual;
  actual.with_name("func");
  actual.clear();
  STRCMP_EQUAL("", actual.get_trace_output());
}

TEST(CheckedActualCall, unexpectedMemoryBufferParameterStringOverload)
{
  mu::tiny::mock::CheckedExpectedCall call1;
  call1.with_name("func");
  list->add_expected_call(&call1);

  const unsigned char buf[] = { 0x01, 0x02 };
  mu::tiny::String name("mem");
  mu::tiny::mock::CheckedActualCall actual_call(1, reporter, *list);
  actual_call.with_name("func").with_parameter(name, buf, sizeof(buf));

  mu::tiny::mock::NamedValue parameter(name);
  parameter.set_memory_buffer(buf, sizeof(buf));
  mu::tiny::mock::UnexpectedInputParameterFailure expected_failure(
      mock_failure_test(), "func", parameter, *list
  );
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(CheckedActualCall, noComparatorForParameterOfTypeStringOverload)
{
  mu::tiny::mock::CheckedExpectedCall call1;
  call1.with_name("func");
  list->add_expected_call(&call1);

  int value = 0;
  mu::tiny::String type_name("MyCustomType");
  mu::tiny::String param_name("param");
  mu::tiny::mock::CheckedActualCall actual_call(1, reporter, *list);
  actual_call.with_name("func").with_parameter_of_type(
      type_name, param_name, &value
  );

  mu::tiny::mock::NoWayToCompareCustomTypeFailure expected_failure(
      mock_failure_test(), type_name
  );
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}
