#include "MockFailureReporterForTest.hpp"

#include "CppMu/CppMu.hpp"

using cppmu::mock;

TEST_GROUP(MockParameter)
{
  void teardown() override
  {
    mock().check_expectations();
    mock().clear();
  }
};

TEST(MockParameter, expectOneBooleanParameterAndValue)
{
  mock().expect_one_call("foo").with_parameter("parameter", true);
  mock().actual_call("foo").with_parameter("parameter", true);

  mock().check_expectations();
}

TEST(MockParameter, expectOneUnsignedIntegerParameterAndValue)
{
  unsigned int value = 14400;
  mock().expect_one_call("foo").with_parameter("parameter", value);
  mock().actual_call("foo").with_parameter("parameter", value);

  mock().check_expectations();
}

TEST(MockParameter, expectOneIntegerParameterAndValue)
{
  mock().expect_one_call("foo").with_parameter("parameter", 10);
  mock().actual_call("foo").with_parameter("parameter", 10);

  mock().check_expectations();
}

TEST(MockParameter, expectOneUnsignedLongLongIntegerParameterAndValue)
{
  unsigned long long value = 0xFFFFAAAAFFFFAAAAULL;
  mock().expect_one_call("foo").with_parameter("parameter", value);
  mock().actual_call("foo").with_parameter("parameter", value);

  mock().check_expectations();
}

TEST(MockParameter, expectOneLongLongIntegerParameterAndValue)
{
  long long value = 0x7FFFAAAAFFFFAAAAULL;

  mock().expect_one_call("foo").with_parameter("parameter", value);
  mock().actual_call("foo").with_parameter("parameter", value);

  mock().check_expectations();
}

TEST(MockParameter, mismatchedIntegerTypesIntAndLongAreAllowed)
{
  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<int>(1));
  mock().actual_call("foo").with_parameter("parameter", static_cast<long>(1));

  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<long>(1));
  mock().actual_call("foo").with_parameter("parameter", static_cast<int>(1));

  mock().check_expectations();
}

TEST(MockParameter, mismatchedIntegerTypesIntAndUnsignedAreAllowed)
{
  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<int>(1));
  mock().actual_call("foo").with_parameter(
      "parameter", static_cast<unsigned>(1));

  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<unsigned>(1));
  mock().actual_call("foo").with_parameter("parameter", static_cast<int>(1));

  mock().check_expectations();
}

TEST(MockParameter, mismatchedIntegerTypesIntAndUnsignedLongAreAllowed)
{
  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<int>(1));
  mock().actual_call("foo").with_parameter(
      "parameter", static_cast<unsigned long>(1));

  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<unsigned long>(1));
  mock().actual_call("foo").with_parameter("parameter", static_cast<int>(1));

  mock().check_expectations();
}

TEST(MockParameter, mismatchedIntegerTypesUnsignedAndLongAreAllowed)
{
  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<unsigned>(1));
  mock().actual_call("foo").with_parameter("parameter", static_cast<long>(1));

  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<long>(1));
  mock().actual_call("foo").with_parameter(
      "parameter", static_cast<unsigned>(1));

  mock().check_expectations();
}

TEST(MockParameter, mismatchedIntegerTypesUnsignedAndUnsignedLongAreAllowed)
{
  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<unsigned>(1));
  mock().actual_call("foo").with_parameter(
      "parameter", static_cast<unsigned long>(1));

  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<unsigned long>(1));
  mock().actual_call("foo").with_parameter(
      "parameter", static_cast<unsigned>(1));

  mock().check_expectations();
}

TEST(MockParameter, mismatchedIntegerTypesLongAndUnsignedLongAreAllowed)
{
  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<long>(1));
  mock().actual_call("foo").with_parameter(
      "parameter", static_cast<unsigned long>(1));

  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<unsigned long>(1));
  mock().actual_call("foo").with_parameter("parameter", static_cast<long>(1));

  mock().check_expectations();
}

TEST(MockParameter, mismatchedIntegerTypesIntAndLongLongAreAllowed)
{
  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<int>(1));
  mock().actual_call("foo").with_parameter(
      "parameter", static_cast<long long>(1));

  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<long long>(1));
  mock().actual_call("foo").with_parameter("parameter", static_cast<int>(1));

  mock().check_expectations();
}

TEST(MockParameter, mismatchedIntegerTypesIntAndUnsignedLongLongAreAllowed)
{
  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<int>(1));
  mock().actual_call("foo").with_parameter(
      "parameter", static_cast<unsigned long long>(1));

  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<unsigned long long>(1));
  mock().actual_call("foo").with_parameter("parameter", static_cast<int>(1));

  mock().check_expectations();
}

TEST(MockParameter, mismatchedIntegerTypesUnsignedAndLongLongAreAllowed)
{
  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<unsigned>(1));
  mock().actual_call("foo").with_parameter(
      "parameter", static_cast<long long>(1));

  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<long long>(1));
  mock().actual_call("foo").with_parameter(
      "parameter", static_cast<unsigned>(1));

  mock().check_expectations();
}

TEST(MockParameter, mismatchedIntegerTypesUnsignedAndUnsignedLongLongAreAllowed)
{
  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<unsigned>(1));
  mock().actual_call("foo").with_parameter(
      "parameter", static_cast<unsigned long long>(1));

  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<unsigned long long>(1));
  mock().actual_call("foo").with_parameter(
      "parameter", static_cast<unsigned>(1));

  mock().check_expectations();
}

TEST(MockParameter,
    mismatchedIntegerTypesUnsignedLongAndUnsignedLongLongAreAllowed)
{
  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<unsigned long>(1));
  mock().actual_call("foo").with_parameter(
      "parameter", static_cast<unsigned long long>(1));

  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<unsigned long long>(1));
  mock().actual_call("foo").with_parameter(
      "parameter", static_cast<unsigned long>(1));

  mock().check_expectations();
}

TEST(MockParameter, mismatchedIntegerTypesLongAndLongLongAreAllowed)
{
  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<long>(1));
  mock().actual_call("foo").with_parameter(
      "parameter", static_cast<long long>(1));

  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<long long>(1));
  mock().actual_call("foo").with_parameter("parameter", static_cast<long>(1));

  mock().check_expectations();
}

TEST(MockParameter, mismatchedIntegerTypesLongAndUnsignedLongLongAreAllowed)
{
  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<long>(1));
  mock().actual_call("foo").with_parameter(
      "parameter", static_cast<unsigned long long>(1));

  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<unsigned long long>(1));
  mock().actual_call("foo").with_parameter("parameter", static_cast<long>(1));

  mock().check_expectations();
}

TEST(MockParameter, mismatchedIntegerTypesUnsignedLongAndLongLongAreAllowed)
{
  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<unsigned long>(1));
  mock().actual_call("foo").with_parameter(
      "parameter", static_cast<long long>(1));

  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<long long>(1));
  mock().actual_call("foo").with_parameter(
      "parameter", static_cast<unsigned long>(1));

  mock().check_expectations();
}

TEST(MockParameter, mismatchedIntegerTypesLongLongAndUnsignedLongLongAreAllowed)
{
  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<long long>(1));
  mock().actual_call("foo").with_parameter(
      "parameter", static_cast<unsigned long long>(1));

  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<unsigned long long>(1));
  mock().actual_call("foo").with_parameter(
      "parameter", static_cast<long long>(1));

  mock().check_expectations();
}

TEST(MockParameter,
    longAndUnsignedLongWithSameBitRepresentationShouldNotBeTreatedAsEqual)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function("foo")->with_parameter(
      "parameter", static_cast<long>(-1));
  cppmu::MockNamedValue parameter("parameter");
  parameter.set_value(static_cast<unsigned long>(-1));
  cppmu::MockUnexpectedInputParameterFailure expected_failure(
      mock_failure_test(), "foo", parameter, expectations);

  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<long>(-1));
  mock().actual_call("foo").with_parameter(
      "parameter", static_cast<unsigned long>(-1));

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockParameter,
    unsignedLongAndLongWithSameBitRepresentationShouldnotBeTreatedAsEqual)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function("foo")->with_parameter(
      "parameter", static_cast<unsigned long>(-1));
  cppmu::MockNamedValue parameter("parameter");
  parameter.set_value(static_cast<long>(-1));
  cppmu::MockUnexpectedInputParameterFailure expected_failure(
      mock_failure_test(), "foo", parameter, expectations);

  mock().expect_one_call("foo").with_parameter(
      "parameter", static_cast<unsigned long>(-1));
  mock().actual_call("foo").with_parameter("parameter", static_cast<long>(-1));

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockParameter, expectOneDoubleParameterAndValue)
{
  mock().expect_one_call("foo").with_parameter("parameter", 1.0);
  mock().actual_call("foo").with_parameter("parameter", 1.0);

  mock().check_expectations();
}

TEST(MockParameter, expectOneDoubleParameterAndValueAndTolerance)
{
  mock().expect_one_call("foo").with_parameter("parameter", 100.0, 5.0);
  mock().actual_call("foo").with_parameter("parameter", 96.0);

  mock().check_expectations();
}

TEST(MockParameter, doubleParameterNotEqualIfOutsideTolerance)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function("foo")->with_parameter("parameter", 100.0);
  cppmu::MockNamedValue parameter("parameter");
  parameter.set_value(106.0);
  cppmu::MockUnexpectedInputParameterFailure expected_failure(
      mock_failure_test(), "foo", parameter, expectations);

  mock().expect_one_call("foo").with_parameter("parameter", 100.0, 5.0);
  mock().actual_call("foo").with_parameter("parameter", 106.0);

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockParameter, expectOneStringParameterAndValue)
{
  mock().expect_one_call("foo").with_parameter("parameter", "string");
  mock().actual_call("foo").with_parameter("parameter", "string");

  mock().check_expectations();
}

TEST(MockParameter, expectOnePointerParameterAndValue)
{
  mock().expect_one_call("foo").with_parameter(
      "parameter", reinterpret_cast<void*>(0x01));
  mock().actual_call("foo").with_parameter(
      "parameter", reinterpret_cast<void*>(0x01));

  mock().check_expectations();
}

TEST(MockParameter, expectOneConstPointerParameterAndValue)
{
  mock().expect_one_call("foo").with_parameter(
      "parameter", reinterpret_cast<const void*>(0x01));
  mock().actual_call("foo").with_parameter(
      "parameter", reinterpret_cast<const void*>(0x01));

  mock().check_expectations();
}

TEST(MockParameter, expectOneFunctionPointerParameterAndValue)
{
  mock().expect_one_call("foo").with_parameter(
      "parameter", reinterpret_cast<void (*)()>(0x01));
  mock().actual_call("foo").with_parameter(
      "parameter", reinterpret_cast<void (*)()>(0x01));

  mock().check_expectations();
}

TEST(MockParameter, expectOneMemBufferParameterAndValue)
{
  unsigned char mem_buffer1[] = { 0x12, 0x15, 0xFF };
  unsigned char mem_buffer2[] = { 0x12, 0x15, 0xFF };
  mock().expect_one_call("foo").with_parameter(
      "parameter", mem_buffer1, sizeof(mem_buffer1));
  mock().actual_call("foo").with_parameter(
      "parameter", mem_buffer2, sizeof(mem_buffer2));

  mock().check_expectations();
}

TEST(MockParameter, expectOneMemBufferParameterAndValueFailsDueToContents)
{
  MockFailureReporterInstaller failure_reporter_installer;

  unsigned char mem_buffer1[] = { 0x12, 0x15, 0xFF };
  unsigned char mem_buffer2[] = { 0x12, 0x05, 0xFF };
  MockExpectedCallsListForTest expectations;
  expectations.add_function("foo")->with_parameter(
      "parameter", mem_buffer1, sizeof(mem_buffer1));
  cppmu::MockNamedValue parameter("parameter");
  parameter.set_memory_buffer(mem_buffer2, sizeof(mem_buffer2));
  cppmu::MockUnexpectedInputParameterFailure expected_failure(
      mock_failure_test(), "foo", parameter, expectations);

  mock().expect_one_call("foo").with_parameter(
      "parameter", mem_buffer1, sizeof(mem_buffer1));
  mock().actual_call("foo").with_parameter(
      "parameter", mem_buffer2, sizeof(mem_buffer2));

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockParameter, expectOneMemBufferParameterAndValueFailsDueToSize)
{
  MockFailureReporterInstaller failure_reporter_installer;

  unsigned char mem_buffer1[] = { 0x12, 0x15, 0xFF };
  unsigned char mem_buffer2[] = { 0x12, 0x15, 0xFF, 0x90 };

  MockExpectedCallsListForTest expectations;
  expectations.add_function("foo")->with_parameter(
      "parameter", mem_buffer1, sizeof(mem_buffer1));
  cppmu::MockNamedValue parameter("parameter");
  parameter.set_memory_buffer(mem_buffer2, sizeof(mem_buffer2));
  cppmu::MockUnexpectedInputParameterFailure expected_failure(
      mock_failure_test(), "foo", parameter, expectations);

  mock().expect_one_call("foo").with_parameter(
      "parameter", mem_buffer1, sizeof(mem_buffer1));
  mock().actual_call("foo").with_parameter(
      "parameter", mem_buffer2, sizeof(mem_buffer2));

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockParameter, expectOneStringParameterAndValueFails)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function("foo")->with_parameter("parameter", "string");
  cppmu::MockNamedValue parameter("parameter");
  parameter.set_value("different");
  cppmu::MockUnexpectedInputParameterFailure expected_failure(
      mock_failure_test(), "foo", parameter, expectations);

  mock().expect_one_call("foo").with_parameter("parameter", "string");
  mock().actual_call("foo").with_parameter("parameter", "different");

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockParameter, expectOneUnsignedIntegerParameterAndFailsDueToParameterName)
{
  MockFailureReporterInstaller failure_reporter_installer;

  unsigned int value = 7;
  MockExpectedCallsListForTest expectations;
  expectations.add_function("foo")->with_parameter("parameter", value);
  cppmu::MockNamedValue parameter("different");
  parameter.set_value(value);
  cppmu::MockUnexpectedInputParameterFailure expected_failure(
      mock_failure_test(), "foo", parameter, expectations);

  mock().expect_one_call("foo").with_parameter("parameter", value);
  mock().actual_call("foo").with_parameter("different", value);

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockParameter, expectOneIntegerParameterAndFailsDueToParameterName)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function("foo")->with_parameter("parameter", 10);
  cppmu::MockNamedValue parameter("different");
  parameter.set_value(10);
  cppmu::MockUnexpectedInputParameterFailure expected_failure(
      mock_failure_test(), "foo", parameter, expectations);

  mock().expect_one_call("foo").with_parameter("parameter", 10);
  mock().actual_call("foo").with_parameter("different", 10);

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockParameter, expectOneUnsignedIntegerParameterAndFailsDueToValue)
{
  MockFailureReporterInstaller failure_reporter_installer;

  unsigned int actual_value = 8;
  unsigned int expected_value = actual_value + 1;
  MockExpectedCallsListForTest expectations;
  expectations.add_function("foo")->with_parameter("parameter", expected_value);
  cppmu::MockNamedValue parameter("parameter");
  parameter.set_value(actual_value);
  cppmu::MockUnexpectedInputParameterFailure expected_failure(
      mock_failure_test(), "foo", parameter, expectations);

  mock().expect_one_call("foo").with_parameter("parameter", expected_value);
  mock().actual_call("foo").with_parameter("parameter", actual_value);

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockParameter, expectOneIntegerParameterAndFailsDueToValue)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function("foo")->with_parameter("parameter", 10);
  cppmu::MockNamedValue parameter("parameter");
  parameter.set_value(8);
  cppmu::MockUnexpectedInputParameterFailure expected_failure(
      mock_failure_test(), "foo", parameter, expectations);

  mock().expect_one_call("foo").with_parameter("parameter", 10);
  mock().actual_call("foo").with_parameter("parameter", 8);

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockParameter, expectOneIntegerParameterAndFailsDueToTypes)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function("foo")->with_parameter("parameter", 10);
  cppmu::MockNamedValue parameter("parameter");
  parameter.set_value("heh");
  cppmu::MockUnexpectedInputParameterFailure expected_failure(
      mock_failure_test(), "foo", parameter, expectations);

  mock().expect_one_call("foo").with_parameter("parameter", 10);
  mock().actual_call("foo").with_parameter("parameter", "heh");

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockParameter,
    expectMultipleCallsWithDifferentParametersThatHappenOutOfOrder)
{
  mock().expect_one_call("foo").with_parameter("p1", 1);
  mock().expect_one_call("foo").with_parameter("p1", 2);
  mock().actual_call("foo").with_parameter("p1", 2);
  mock().actual_call("foo").with_parameter("p1", 1);

  mock().check_expectations();
}

TEST(MockParameter,
    expectMultipleCallsWithMultipleDifferentParametersThatHappenOutOfOrder)
{
  mock().expect_one_call("foo").with_parameter("p1", 1).with_parameter("p2", 2);
  mock().expect_one_call("foo").with_parameter("p1", 1).with_parameter(
      "p2", 20);

  mock().actual_call("foo").with_parameter("p1", 1).with_parameter("p2", 20);
  mock().actual_call("foo").with_parameter("p1", 1).with_parameter("p2", 2);

  mock().check_expectations();
}

TEST(MockParameter, twiceCalledWithSameParameters)
{
  mock().expect_one_call("foo").with_parameter("p1", 1).with_parameter("p2", 2);
  mock().expect_one_call("foo").with_parameter("p1", 1).with_parameter("p2", 2);
  mock().actual_call("foo").with_parameter("p1", 1).with_parameter("p2", 2);
  mock().actual_call("foo").with_parameter("p1", 1).with_parameter("p2", 2);

  mock().check_expectations();
}

TEST(MockParameter, calledWithoutParameters)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function("foo")->with_parameter("p1", 1);
  cppmu::MockExpectedParameterDidntHappenFailure expected_failure(
      mock_failure_test(), "foo", expectations, expectations);

  mock().expect_one_call("foo").with_parameter("p1", 1);
  mock().actual_call("foo");

  mock().check_expectations();
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockParameter, ignoreOtherParameters)
{
  mock()
      .expect_one_call("foo")
      .with_parameter("p1", 1)
      .ignore_other_parameters();
  mock().actual_call("foo").with_parameter("p1", 1).with_parameter("p2", 2);

  mock().check_expectations();
}

TEST(MockParameter, ignoreOtherParametersButStillPassAll)
{
  mock()
      .expect_one_call("foo")
      .with_parameter("p1", 1)
      .ignore_other_parameters();
  mock().actual_call("foo").with_parameter("p1", 1);

  mock().check_expectations();
}

TEST(MockParameter, ignoreOtherParametersButExpectedParameterDidntHappen)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function("foo")
      ->with_parameter("p1", 1)
      .ignore_other_parameters();
  cppmu::MockExpectedParameterDidntHappenFailure expected_failure(
      mock_failure_test(), "foo", expectations, expectations);

  mock()
      .expect_one_call("foo")
      .with_parameter("p1", 1)
      .ignore_other_parameters();
  mock()
      .actual_call("foo")
      .with_parameter("p2", 2)
      .with_parameter("p3", 3)
      .with_parameter("p4", 4);

  mock().check_expectations();
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockParameter, ignoreOtherParametersMultipleCalls)
{
  mock().expect_one_call("foo").ignore_other_parameters();
  mock().expect_one_call("foo").ignore_other_parameters();
  mock()
      .actual_call("foo")
      .with_parameter("p2", 2)
      .with_parameter("p3", 3)
      .with_parameter("p4", 4);
  LONGS_EQUAL(1, mock().expected_calls_left());
  mock()
      .actual_call("foo")
      .with_parameter("p2", 2)
      .with_parameter("p3", 3)
      .with_parameter("p4", 4);

  mock().check_expectations();
}

TEST(MockParameter, ignoreOtherParametersMultipleCallsButOneDidntHappen)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  cppmu::MockCheckedExpectedCall* call = expectations.add_function("boo");
  call->ignore_other_parameters();
  call->call_was_made(1);
  call->finalize_actual_call_match();
  call->ignore_other_parameters();
  expectations.add_function("boo")->ignore_other_parameters();
  cppmu::MockExpectedCallsDidntHappenFailure expected_failure(
      mock_failure_test(), expectations);

  mock().expect_one_call("boo").ignore_other_parameters();
  mock().expect_one_call("boo").ignore_other_parameters();
  mock().actual_call("boo");

  mock().check_expectations();
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockParameter, newCallStartsWhileNotAllParametersWerePassed)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function("foo")->with_parameter("p1", 1);
  cppmu::MockExpectedParameterDidntHappenFailure expected_failure(
      mock_failure_test(), "foo", expectations, expectations);

  mock().expect_one_call("foo").with_parameter("p1", 1);
  mock().actual_call("foo");
  mock().actual_call("foo").with_parameter("p1", 1);

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockParameter, outputParameterSucceeds)
{
  int param = 1;
  int retval = 2;

  mock()
      .expect_one_call("function")
      .with_output_parameter_returning(
          "parameterName", &retval, sizeof(retval));
  mock().actual_call("function").with_output_parameter("parameterName", &param);

  CHECK_EQUAL(param, 2);
  CHECK_EQUAL(retval, 2);
  mock().check_expectations();
}

TEST(MockParameter, unmodifiedOutputParameterSucceeds)
{
  int param = 1;

  mock()
      .expect_one_call("function")
      .with_unmodified_output_parameter("parameterName");
  mock().actual_call("function").with_output_parameter("parameterName", &param);

  CHECK_EQUAL(param, 1);
  mock().check_expectations();
}

TEST(MockParameter, noActualCallForOutputParameter)
{
  MockFailureReporterInstaller failure_reporter_installer;

  int output = 0;
  MockExpectedCallsListForTest expectations;
  mock().expect_one_call("foo").with_output_parameter_returning(
      "output", &output, sizeof(output));

  expectations.add_function("foo")->with_output_parameter_returning(
      "output", &output, sizeof(output));
  cppmu::MockExpectedCallsDidntHappenFailure expected_failure(
      mock_failure_test(), expectations);

  mock().check_expectations();
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockParameter, noActualCallForUnmodifiedOutputParameter)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  mock().expect_one_call("foo").with_unmodified_output_parameter("output");

  expectations.add_function("foo")->with_unmodified_output_parameter("output");
  cppmu::MockExpectedCallsDidntHappenFailure expected_failure(
      mock_failure_test(), expectations);

  mock().check_expectations();
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockParameter, unexpectedOutputParameter)
{
  MockFailureReporterInstaller failure_reporter_installer;

  int param;
  MockExpectedCallsListForTest expectations;
  mock().expect_one_call("foo");
  mock().actual_call("foo").with_output_parameter("parameterName", &param);

  expectations.add_function("foo");
  cppmu::MockNamedValue parameter("parameterName");
  parameter.set_value(&param);
  cppmu::MockUnexpectedOutputParameterFailure expected_failure(
      mock_failure_test(), "foo", parameter, expectations);

  mock().check_expectations();
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockParameter, outputParameterMissing)
{
  MockFailureReporterInstaller failure_reporter_installer;

  int output = 0;
  MockExpectedCallsListForTest expectations;
  mock().expect_one_call("foo").with_output_parameter_returning(
      "output", &output, sizeof(output));
  mock().actual_call("foo");

  expectations.add_function("foo")->with_output_parameter_returning(
      "output", &output, sizeof(output));
  cppmu::MockExpectedParameterDidntHappenFailure expected_failure(
      mock_failure_test(), "foo", expectations, expectations);

  mock().check_expectations();
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockParameter, twoOutputParameters)
{
  int param1 = 55;
  int retval1 = 1;
  int param2 = 77;
  int retval2 = 2;

  mock()
      .expect_one_call("function")
      .with_output_parameter_returning(
          "parameterName", &retval1, sizeof(retval1))
      .with_parameter("id", 1);
  mock()
      .expect_one_call("function")
      .with_output_parameter_returning(
          "parameterName", &retval2, sizeof(retval2))
      .with_parameter("id", 2);
  mock()
      .actual_call("function")
      .with_output_parameter("parameterName", &param1)
      .with_parameter("id", 1);
  mock()
      .actual_call("function")
      .with_output_parameter("parameterName", &param2)
      .with_parameter("id", 2);

  CHECK_EQUAL(retval1, param1);
  CHECK_EQUAL(retval2, param2);
  mock().check_expectations();
}

TEST(MockParameter, twoInterleavedOutputParameters)
{
  int param1 = 55;
  int retval1 = 1;
  int param2 = 77;
  int retval2 = 2;

  mock()
      .expect_one_call("function")
      .with_output_parameter_returning(
          "parameterName", &retval1, sizeof(retval1))
      .with_parameter("id", 1);
  mock()
      .expect_one_call("function")
      .with_output_parameter_returning(
          "parameterName", &retval2, sizeof(retval2))
      .with_parameter("id", 2);
  mock()
      .actual_call("function")
      .with_output_parameter("parameterName", &param2)
      .with_parameter("id", 2);
  mock()
      .actual_call("function")
      .with_output_parameter("parameterName", &param1)
      .with_parameter("id", 1);

  CHECK_EQUAL(retval1, param1);
  CHECK_EQUAL(retval2, param2);
  mock().check_expectations();
}

TEST(MockParameter, twoDifferentOutputParametersInSameFunctionCallSucceeds)
{
  int param1 = 1;
  int param2 = 1;
  int retval1 = 2;
  int retval2 = 3;

  mock()
      .expect_one_call("foo")
      .with_output_parameter_returning("bar", &retval1, sizeof(retval1))
      .with_output_parameter_returning("foobar", &retval2, sizeof(retval2));
  mock()
      .actual_call("foo")
      .with_output_parameter("bar", &param1)
      .with_output_parameter("foobar", &param2);

  CHECK_EQUAL(2, retval1);
  CHECK_EQUAL(2, param1);
  CHECK_EQUAL(3, retval2);
  CHECK_EQUAL(3, param2);
  mock().check_expectations();
}

TEST(MockParameter,
    outputAndIntParametersOfSameNameInDifferentFunctionCallsOfSameFunctionSucceeds)
{
  int param = 1;
  int retval = 2;

  mock().expect_one_call("foo").with_output_parameter_returning(
      "bar", &retval, sizeof(retval));
  mock().expect_one_call("foo").with_int_parameter("bar", 25);
  mock().actual_call("foo").with_output_parameter("bar", &param);
  mock().actual_call("foo").with_int_parameter("bar", 25);

  CHECK_EQUAL(2, retval);
  CHECK_EQUAL(2, param);
  mock().check_expectations();
}

TEST(MockParameter,
    twoOutputParameterOfSameNameInDifferentFunctionCallsOfSameFunctionSucceeds)
{
  int param1 = 1;
  int param2 = 1;
  int retval1 = 2;
  int retval2 = 3;

  mock().expect_one_call("foo").with_output_parameter_returning(
      "bar", &retval1, sizeof(retval1));
  mock().expect_one_call("foo").with_output_parameter_returning(
      "bar", &retval2, sizeof(retval2));
  mock().actual_call("foo").with_output_parameter("bar", &param1);
  mock().actual_call("foo").with_output_parameter("bar", &param2);

  CHECK_EQUAL(2, retval1);
  CHECK_EQUAL(2, param1);
  CHECK_EQUAL(3, retval2);
  CHECK_EQUAL(3, param2);
  mock().check_expectations();
}

TEST(MockParameter, twoOutputParametersOfSameNameInDifferentFunctionsSucceeds)
{
  int param = 1;
  int retval = 2;

  mock().expect_one_call("foo1").with_output_parameter_returning(
      "bar", &retval, sizeof(retval));
  mock().expect_one_call("foo2").with_int_parameter("bar", 25);
  mock().actual_call("foo1").with_output_parameter("bar", &param);
  mock().actual_call("foo2").with_int_parameter("bar", 25);

  CHECK_EQUAL(2, retval);
  CHECK_EQUAL(2, param);
  mock().check_expectations();
}

TEST(MockParameter, outputAndInputParameter)
{
  int return_value = 5;
  int returned_value = 7;

  mock()
      .expect_one_call("foo")
      .with_parameter("bar", 10)
      .with_output_parameter_returning(
          "bar", &return_value, sizeof(return_value));
  mock().actual_call("foo").with_parameter("bar", 10).with_output_parameter(
      "bar", &returned_value);

  LONGS_EQUAL(5, returned_value);
  mock().check_expectations();
}

TEST(MockParameter, outputParameterTraced)
{
  mock().tracing(true);

  int param = 1;
  mock().actual_call("someFunc").with_output_parameter("someParameter", &param);
  mock().check_expectations();
  STRCMP_CONTAINS(
      "Function name:someFunc someParameter:", mock().get_trace_output());

  mock().check_expectations();
}

TEST(MockParameter, outputParameterThatIsIgnoredShouldNotFail)
{
  int param;
  mock().expect_one_call("function").ignore_other_parameters();
  mock().actual_call("function").with_output_parameter("parameterName", &param);

  mock().check_expectations();
}

TEST(MockParameter, outputParameterWithIgnoredParameters)
{
  int param = 1;
  int retval = 2;

  mock()
      .expect_one_call("foo")
      .with_output_parameter_returning("bar", &param, sizeof(param))
      .ignore_other_parameters();
  mock()
      .actual_call("foo")
      .with_output_parameter("bar", &retval)
      .with_parameter("other", 1);

  LONGS_EQUAL(param, retval);

  mock().check_expectations();
}

/*
 * This test checks that the proper output parameters are copied when multiple
 * calls to the same function are expected.
 */
TEST(MockParameter, properOutputParametersAreCopied)
{
  int expected_value1 = 1;
  int expected_value2 = 2;
  mock()
      .expect_one_call("foo")
      .with_output_parameter_returning(
          "param", &expected_value1, sizeof(expected_value1))
      .ignore_other_parameters();
  mock().expect_one_call("foo").with_output_parameter_returning(
      "param", &expected_value2, sizeof(expected_value2));

  int returned_value1 = 0;
  int returned_value2 = 0;
  mock().actual_call("foo").with_output_parameter("param", &returned_value1);
  mock()
      .actual_call("foo")
      .with_output_parameter("param", &returned_value2)
      .with_parameter("optional", 50);

  CHECK_EQUAL_TEXT(
      expected_value2, returned_value1, "Wrong output value in 1st call");
  CHECK_EQUAL_TEXT(
      expected_value1, returned_value2, "Wrong output value in 2nd call");

  mock().check_expectations();
}

TEST(MockParameter, ignoreOtherCallsIgnoresWithAllKindsOfParameters)
{
  mock().ignore_other_calls();
  mock()
      .actual_call("boo")
      .with_parameter("umm", true)
      .with_parameter("bar", 1u)
      .with_parameter("foo", 1l)
      .with_parameter("hey", 1ul)
      .with_parameter("ick", 1ll)
      .with_parameter("grr", 1ull)
      .with_parameter("duh", 1.0)
      .with_parameter("yoo", static_cast<const void*>(nullptr))
      .with_parameter("func", static_cast<void (*)()>(nullptr))
      .with_parameter("mem", static_cast<const unsigned char*>(nullptr), 0)
      .with_parameter_of_type("hoo", "int", static_cast<const void*>(nullptr))
      .with_output_parameter("gah", static_cast<void*>(nullptr))
      .with_output_parameter_of_type("goo", "int", static_cast<void*>(nullptr));

  mock().check_expectations();
}

TEST(MockParameter, expectMultipleCallsWithParameters)
{
  int expected_int = -7;
  unsigned int expected_uint = 7;

  mock()
      .expect_n_calls(2, "boo")
      .with_parameter("double", 1.0)
      .with_parameter("int", expected_int)
      .with_parameter("string", "string")
      .with_parameter("uint", expected_uint);
  mock()
      .actual_call("boo")
      .with_parameter("double", 1.0)
      .with_parameter("int", expected_int)
      .with_parameter("string", "string")
      .with_parameter("uint", expected_uint);
  mock()
      .actual_call("boo")
      .with_parameter("double", 1.0)
      .with_parameter("int", expected_int)
      .with_parameter("string", "string")
      .with_parameter("uint", expected_uint);

  mock().check_expectations();
}

TEST(MockParameter, expectMultipleMultipleCallsWithParameters)
{
  mock()
      .expect_n_calls(2, "boo")
      .with_parameter("double", 1.0)
      .ignore_other_parameters();
  mock()
      .expect_n_calls(2, "boo")
      .with_parameter("double", 1.0)
      .ignore_other_parameters();
  mock()
      .actual_call("boo")
      .with_parameter("double", 1.0)
      .with_parameter("int", 1)
      .with_parameter("string", "string");
  mock()
      .actual_call("boo")
      .with_parameter("double", 1.0)
      .with_parameter("int", 1)
      .with_parameter("string", "string");
  mock()
      .actual_call("boo")
      .with_parameter("double", 1.0)
      .with_parameter("int", 1)
      .with_parameter("string", "string");
  mock()
      .actual_call("boo")
      .with_parameter("double", 1.0)
      .with_parameter("int", 1)
      .with_parameter("string", "string");

  mock().check_expectations();
}
