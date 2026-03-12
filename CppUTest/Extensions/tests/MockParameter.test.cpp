#include "MockFailureReporterForTest.h"

#include "CppUTest/TestHarness.hpp"

using namespace cpputest;
using namespace cpputest::extensions;

TEST_GROUP(MockParameter)
{
  void teardown() override
  {
    mock().checkExpectations();
    mock().clear();
  }
};

TEST(MockParameter, expectOneBooleanParameterAndValue)
{
  mock().expectOneCall("foo").withParameter("parameter", true);
  mock().actualCall("foo").withParameter("parameter", true);

  mock().checkExpectations();
}

TEST(MockParameter, expectOneUnsignedIntegerParameterAndValue)
{
  unsigned int value = 14400;
  mock().expectOneCall("foo").withParameter("parameter", value);
  mock().actualCall("foo").withParameter("parameter", value);

  mock().checkExpectations();
}

TEST(MockParameter, expectOneIntegerParameterAndValue)
{
  mock().expectOneCall("foo").withParameter("parameter", 10);
  mock().actualCall("foo").withParameter("parameter", 10);

  mock().checkExpectations();
}

TEST(MockParameter, expectOneUnsignedLongLongIntegerParameterAndValue)
{
  unsigned long long value = 0xFFFFAAAAFFFFAAAAULL;
  mock().expectOneCall("foo").withParameter("parameter", value);
  mock().actualCall("foo").withParameter("parameter", value);

  mock().checkExpectations();
}

TEST(MockParameter, expectOneLongLongIntegerParameterAndValue)
{
  long long value = 0x7FFFAAAAFFFFAAAAULL;

  mock().expectOneCall("foo").withParameter("parameter", value);
  mock().actualCall("foo").withParameter("parameter", value);

  mock().checkExpectations();
}

TEST(MockParameter, mismatchedIntegerTypesIntAndLongAreAllowed)
{
  mock().expectOneCall("foo").withParameter("parameter", static_cast<int>(1));
  mock().actualCall("foo").withParameter("parameter", static_cast<long>(1));

  mock().expectOneCall("foo").withParameter("parameter", static_cast<long>(1));
  mock().actualCall("foo").withParameter("parameter", static_cast<int>(1));

  mock().checkExpectations();
}

TEST(MockParameter, mismatchedIntegerTypesIntAndUnsignedAreAllowed)
{
  mock().expectOneCall("foo").withParameter("parameter", static_cast<int>(1));
  mock().actualCall("foo").withParameter("parameter", static_cast<unsigned>(1));

  mock().expectOneCall("foo").withParameter(
      "parameter", static_cast<unsigned>(1));
  mock().actualCall("foo").withParameter("parameter", static_cast<int>(1));

  mock().checkExpectations();
}

TEST(MockParameter, mismatchedIntegerTypesIntAndUnsignedLongAreAllowed)
{
  mock().expectOneCall("foo").withParameter("parameter", static_cast<int>(1));
  mock().actualCall("foo").withParameter(
      "parameter", static_cast<unsigned long>(1));

  mock().expectOneCall("foo").withParameter(
      "parameter", static_cast<unsigned long>(1));
  mock().actualCall("foo").withParameter("parameter", static_cast<int>(1));

  mock().checkExpectations();
}

TEST(MockParameter, mismatchedIntegerTypesUnsignedAndLongAreAllowed)
{
  mock().expectOneCall("foo").withParameter(
      "parameter", static_cast<unsigned>(1));
  mock().actualCall("foo").withParameter("parameter", static_cast<long>(1));

  mock().expectOneCall("foo").withParameter("parameter", static_cast<long>(1));
  mock().actualCall("foo").withParameter("parameter", static_cast<unsigned>(1));

  mock().checkExpectations();
}

TEST(MockParameter, mismatchedIntegerTypesUnsignedAndUnsignedLongAreAllowed)
{
  mock().expectOneCall("foo").withParameter(
      "parameter", static_cast<unsigned>(1));
  mock().actualCall("foo").withParameter(
      "parameter", static_cast<unsigned long>(1));

  mock().expectOneCall("foo").withParameter(
      "parameter", static_cast<unsigned long>(1));
  mock().actualCall("foo").withParameter("parameter", static_cast<unsigned>(1));

  mock().checkExpectations();
}

TEST(MockParameter, mismatchedIntegerTypesLongAndUnsignedLongAreAllowed)
{
  mock().expectOneCall("foo").withParameter("parameter", static_cast<long>(1));
  mock().actualCall("foo").withParameter(
      "parameter", static_cast<unsigned long>(1));

  mock().expectOneCall("foo").withParameter(
      "parameter", static_cast<unsigned long>(1));
  mock().actualCall("foo").withParameter("parameter", static_cast<long>(1));

  mock().checkExpectations();
}

TEST(MockParameter, mismatchedIntegerTypesIntAndLongLongAreAllowed)
{
  mock().expectOneCall("foo").withParameter("parameter", static_cast<int>(1));
  mock().actualCall("foo").withParameter(
      "parameter", static_cast<long long>(1));

  mock().expectOneCall("foo").withParameter(
      "parameter", static_cast<long long>(1));
  mock().actualCall("foo").withParameter("parameter", static_cast<int>(1));

  mock().checkExpectations();
}

TEST(MockParameter, mismatchedIntegerTypesIntAndUnsignedLongLongAreAllowed)
{
  mock().expectOneCall("foo").withParameter("parameter", static_cast<int>(1));
  mock().actualCall("foo").withParameter(
      "parameter", static_cast<unsigned long long>(1));

  mock().expectOneCall("foo").withParameter(
      "parameter", static_cast<unsigned long long>(1));
  mock().actualCall("foo").withParameter("parameter", static_cast<int>(1));

  mock().checkExpectations();
}

TEST(MockParameter, mismatchedIntegerTypesUnsignedAndLongLongAreAllowed)
{
  mock().expectOneCall("foo").withParameter(
      "parameter", static_cast<unsigned>(1));
  mock().actualCall("foo").withParameter(
      "parameter", static_cast<long long>(1));

  mock().expectOneCall("foo").withParameter(
      "parameter", static_cast<long long>(1));
  mock().actualCall("foo").withParameter("parameter", static_cast<unsigned>(1));

  mock().checkExpectations();
}

TEST(MockParameter, mismatchedIntegerTypesUnsignedAndUnsignedLongLongAreAllowed)
{
  mock().expectOneCall("foo").withParameter(
      "parameter", static_cast<unsigned>(1));
  mock().actualCall("foo").withParameter(
      "parameter", static_cast<unsigned long long>(1));

  mock().expectOneCall("foo").withParameter(
      "parameter", static_cast<unsigned long long>(1));
  mock().actualCall("foo").withParameter("parameter", static_cast<unsigned>(1));

  mock().checkExpectations();
}

TEST(MockParameter,
    mismatchedIntegerTypesUnsignedLongAndUnsignedLongLongAreAllowed)
{
  mock().expectOneCall("foo").withParameter(
      "parameter", static_cast<unsigned long>(1));
  mock().actualCall("foo").withParameter(
      "parameter", static_cast<unsigned long long>(1));

  mock().expectOneCall("foo").withParameter(
      "parameter", static_cast<unsigned long long>(1));
  mock().actualCall("foo").withParameter(
      "parameter", static_cast<unsigned long>(1));

  mock().checkExpectations();
}

TEST(MockParameter, mismatchedIntegerTypesLongAndLongLongAreAllowed)
{
  mock().expectOneCall("foo").withParameter("parameter", static_cast<long>(1));
  mock().actualCall("foo").withParameter(
      "parameter", static_cast<long long>(1));

  mock().expectOneCall("foo").withParameter(
      "parameter", static_cast<long long>(1));
  mock().actualCall("foo").withParameter("parameter", static_cast<long>(1));

  mock().checkExpectations();
}

TEST(MockParameter, mismatchedIntegerTypesLongAndUnsignedLongLongAreAllowed)
{
  mock().expectOneCall("foo").withParameter("parameter", static_cast<long>(1));
  mock().actualCall("foo").withParameter(
      "parameter", static_cast<unsigned long long>(1));

  mock().expectOneCall("foo").withParameter(
      "parameter", static_cast<unsigned long long>(1));
  mock().actualCall("foo").withParameter("parameter", static_cast<long>(1));

  mock().checkExpectations();
}

TEST(MockParameter, mismatchedIntegerTypesUnsignedLongAndLongLongAreAllowed)
{
  mock().expectOneCall("foo").withParameter(
      "parameter", static_cast<unsigned long>(1));
  mock().actualCall("foo").withParameter(
      "parameter", static_cast<long long>(1));

  mock().expectOneCall("foo").withParameter(
      "parameter", static_cast<long long>(1));
  mock().actualCall("foo").withParameter(
      "parameter", static_cast<unsigned long>(1));

  mock().checkExpectations();
}

TEST(MockParameter, mismatchedIntegerTypesLongLongAndUnsignedLongLongAreAllowed)
{
  mock().expectOneCall("foo").withParameter(
      "parameter", static_cast<long long>(1));
  mock().actualCall("foo").withParameter(
      "parameter", static_cast<unsigned long long>(1));

  mock().expectOneCall("foo").withParameter(
      "parameter", static_cast<unsigned long long>(1));
  mock().actualCall("foo").withParameter(
      "parameter", static_cast<long long>(1));

  mock().checkExpectations();
}

TEST(MockParameter,
    longAndUnsignedLongWithSameBitRepresentationShouldNotBeTreatedAsEqual)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foo")->withParameter(
      "parameter", static_cast<long>(-1));
  MockNamedValue parameter("parameter");
  parameter.setValue(static_cast<unsigned long>(-1));
  MockUnexpectedInputParameterFailure expectedFailure(
      mockFailureTest(), "foo", parameter, expectations);

  mock().expectOneCall("foo").withParameter("parameter", static_cast<long>(-1));
  mock().actualCall("foo").withParameter(
      "parameter", static_cast<unsigned long>(-1));

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameter,
    unsignedLongAndLongWithSameBitRepresentationShouldnotBeTreatedAsEqual)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foo")->withParameter(
      "parameter", static_cast<unsigned long>(-1));
  MockNamedValue parameter("parameter");
  parameter.setValue(static_cast<long>(-1));
  MockUnexpectedInputParameterFailure expectedFailure(
      mockFailureTest(), "foo", parameter, expectations);

  mock().expectOneCall("foo").withParameter(
      "parameter", static_cast<unsigned long>(-1));
  mock().actualCall("foo").withParameter("parameter", static_cast<long>(-1));

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameter, expectOneDoubleParameterAndValue)
{
  mock().expectOneCall("foo").withParameter("parameter", 1.0);
  mock().actualCall("foo").withParameter("parameter", 1.0);

  mock().checkExpectations();
}

TEST(MockParameter, expectOneDoubleParameterAndValueAndTolerance)
{
  mock().expectOneCall("foo").withParameter("parameter", 100.0, 5.0);
  mock().actualCall("foo").withParameter("parameter", 96.0);

  mock().checkExpectations();
}

TEST(MockParameter, doubleParameterNotEqualIfOutsideTolerance)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foo")->withParameter("parameter", 100.0);
  MockNamedValue parameter("parameter");
  parameter.setValue(106.0);
  MockUnexpectedInputParameterFailure expectedFailure(
      mockFailureTest(), "foo", parameter, expectations);

  mock().expectOneCall("foo").withParameter("parameter", 100.0, 5.0);
  mock().actualCall("foo").withParameter("parameter", 106.0);

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameter, expectOneStringParameterAndValue)
{
  mock().expectOneCall("foo").withParameter("parameter", "string");
  mock().actualCall("foo").withParameter("parameter", "string");

  mock().checkExpectations();
}

TEST(MockParameter, expectOnePointerParameterAndValue)
{
  mock().expectOneCall("foo").withParameter(
      "parameter", reinterpret_cast<void*>(0x01));
  mock().actualCall("foo").withParameter(
      "parameter", reinterpret_cast<void*>(0x01));

  mock().checkExpectations();
}

TEST(MockParameter, expectOneConstPointerParameterAndValue)
{
  mock().expectOneCall("foo").withParameter(
      "parameter", reinterpret_cast<const void*>(0x01));
  mock().actualCall("foo").withParameter(
      "parameter", reinterpret_cast<const void*>(0x01));

  mock().checkExpectations();
}

TEST(MockParameter, expectOneFunctionPointerParameterAndValue)
{
  mock().expectOneCall("foo").withParameter(
      "parameter", reinterpret_cast<void (*)()>(0x01));
  mock().actualCall("foo").withParameter(
      "parameter", reinterpret_cast<void (*)()>(0x01));

  mock().checkExpectations();
}

TEST(MockParameter, expectOneMemBufferParameterAndValue)
{
  unsigned char memBuffer1[] = { 0x12, 0x15, 0xFF };
  unsigned char memBuffer2[] = { 0x12, 0x15, 0xFF };
  mock().expectOneCall("foo").withParameter(
      "parameter", memBuffer1, sizeof(memBuffer1));
  mock().actualCall("foo").withParameter(
      "parameter", memBuffer2, sizeof(memBuffer2));

  mock().checkExpectations();
}

TEST(MockParameter, expectOneMemBufferParameterAndValueFailsDueToContents)
{
  MockFailureReporterInstaller failureReporterInstaller;

  unsigned char memBuffer1[] = { 0x12, 0x15, 0xFF };
  unsigned char memBuffer2[] = { 0x12, 0x05, 0xFF };
  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foo")->withParameter(
      "parameter", memBuffer1, sizeof(memBuffer1));
  MockNamedValue parameter("parameter");
  parameter.setMemoryBuffer(memBuffer2, sizeof(memBuffer2));
  MockUnexpectedInputParameterFailure expectedFailure(
      mockFailureTest(), "foo", parameter, expectations);

  mock().expectOneCall("foo").withParameter(
      "parameter", memBuffer1, sizeof(memBuffer1));
  mock().actualCall("foo").withParameter(
      "parameter", memBuffer2, sizeof(memBuffer2));

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameter, expectOneMemBufferParameterAndValueFailsDueToSize)
{
  MockFailureReporterInstaller failureReporterInstaller;

  unsigned char memBuffer1[] = { 0x12, 0x15, 0xFF };
  unsigned char memBuffer2[] = { 0x12, 0x15, 0xFF, 0x90 };

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foo")->withParameter(
      "parameter", memBuffer1, sizeof(memBuffer1));
  MockNamedValue parameter("parameter");
  parameter.setMemoryBuffer(memBuffer2, sizeof(memBuffer2));
  MockUnexpectedInputParameterFailure expectedFailure(
      mockFailureTest(), "foo", parameter, expectations);

  mock().expectOneCall("foo").withParameter(
      "parameter", memBuffer1, sizeof(memBuffer1));
  mock().actualCall("foo").withParameter(
      "parameter", memBuffer2, sizeof(memBuffer2));

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameter, expectOneStringParameterAndValueFails)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foo")->withParameter("parameter", "string");
  MockNamedValue parameter("parameter");
  parameter.setValue("different");
  MockUnexpectedInputParameterFailure expectedFailure(
      mockFailureTest(), "foo", parameter, expectations);

  mock().expectOneCall("foo").withParameter("parameter", "string");
  mock().actualCall("foo").withParameter("parameter", "different");

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameter, expectOneUnsignedIntegerParameterAndFailsDueToParameterName)
{
  MockFailureReporterInstaller failureReporterInstaller;

  unsigned int value = 7;
  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foo")->withParameter("parameter", value);
  MockNamedValue parameter("different");
  parameter.setValue(value);
  MockUnexpectedInputParameterFailure expectedFailure(
      mockFailureTest(), "foo", parameter, expectations);

  mock().expectOneCall("foo").withParameter("parameter", value);
  mock().actualCall("foo").withParameter("different", value);

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameter, expectOneIntegerParameterAndFailsDueToParameterName)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foo")->withParameter("parameter", 10);
  MockNamedValue parameter("different");
  parameter.setValue(10);
  MockUnexpectedInputParameterFailure expectedFailure(
      mockFailureTest(), "foo", parameter, expectations);

  mock().expectOneCall("foo").withParameter("parameter", 10);
  mock().actualCall("foo").withParameter("different", 10);

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameter, expectOneUnsignedIntegerParameterAndFailsDueToValue)
{
  MockFailureReporterInstaller failureReporterInstaller;

  unsigned int actual_value = 8;
  unsigned int expected_value = actual_value + 1;
  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foo")->withParameter("parameter", expected_value);
  MockNamedValue parameter("parameter");
  parameter.setValue(actual_value);
  MockUnexpectedInputParameterFailure expectedFailure(
      mockFailureTest(), "foo", parameter, expectations);

  mock().expectOneCall("foo").withParameter("parameter", expected_value);
  mock().actualCall("foo").withParameter("parameter", actual_value);

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameter, expectOneIntegerParameterAndFailsDueToValue)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foo")->withParameter("parameter", 10);
  MockNamedValue parameter("parameter");
  parameter.setValue(8);
  MockUnexpectedInputParameterFailure expectedFailure(
      mockFailureTest(), "foo", parameter, expectations);

  mock().expectOneCall("foo").withParameter("parameter", 10);
  mock().actualCall("foo").withParameter("parameter", 8);

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameter, expectOneIntegerParameterAndFailsDueToTypes)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foo")->withParameter("parameter", 10);
  MockNamedValue parameter("parameter");
  parameter.setValue("heh");
  MockUnexpectedInputParameterFailure expectedFailure(
      mockFailureTest(), "foo", parameter, expectations);

  mock().expectOneCall("foo").withParameter("parameter", 10);
  mock().actualCall("foo").withParameter("parameter", "heh");

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameter,
    expectMultipleCallsWithDifferentParametersThatHappenOutOfOrder)
{
  mock().expectOneCall("foo").withParameter("p1", 1);
  mock().expectOneCall("foo").withParameter("p1", 2);
  mock().actualCall("foo").withParameter("p1", 2);
  mock().actualCall("foo").withParameter("p1", 1);

  mock().checkExpectations();
}

TEST(MockParameter,
    expectMultipleCallsWithMultipleDifferentParametersThatHappenOutOfOrder)
{
  mock().expectOneCall("foo").withParameter("p1", 1).withParameter("p2", 2);
  mock().expectOneCall("foo").withParameter("p1", 1).withParameter("p2", 20);

  mock().actualCall("foo").withParameter("p1", 1).withParameter("p2", 20);
  mock().actualCall("foo").withParameter("p1", 1).withParameter("p2", 2);

  mock().checkExpectations();
}

TEST(MockParameter, twiceCalledWithSameParameters)
{
  mock().expectOneCall("foo").withParameter("p1", 1).withParameter("p2", 2);
  mock().expectOneCall("foo").withParameter("p1", 1).withParameter("p2", 2);
  mock().actualCall("foo").withParameter("p1", 1).withParameter("p2", 2);
  mock().actualCall("foo").withParameter("p1", 1).withParameter("p2", 2);

  mock().checkExpectations();
}

TEST(MockParameter, calledWithoutParameters)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foo")->withParameter("p1", 1);
  MockExpectedParameterDidntHappenFailure expectedFailure(
      mockFailureTest(), "foo", expectations, expectations);

  mock().expectOneCall("foo").withParameter("p1", 1);
  mock().actualCall("foo");

  mock().checkExpectations();
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameter, ignoreOtherParameters)
{
  mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters();
  mock().actualCall("foo").withParameter("p1", 1).withParameter("p2", 2);

  mock().checkExpectations();
}

TEST(MockParameter, ignoreOtherParametersButStillPassAll)
{
  mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters();
  mock().actualCall("foo").withParameter("p1", 1);

  mock().checkExpectations();
}

TEST(MockParameter, ignoreOtherParametersButExpectedParameterDidntHappen)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foo")
      ->withParameter("p1", 1)
      .ignoreOtherParameters();
  MockExpectedParameterDidntHappenFailure expectedFailure(
      mockFailureTest(), "foo", expectations, expectations);

  mock().expectOneCall("foo").withParameter("p1", 1).ignoreOtherParameters();
  mock()
      .actualCall("foo")
      .withParameter("p2", 2)
      .withParameter("p3", 3)
      .withParameter("p4", 4);

  mock().checkExpectations();
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameter, ignoreOtherParametersMultipleCalls)
{
  mock().expectOneCall("foo").ignoreOtherParameters();
  mock().expectOneCall("foo").ignoreOtherParameters();
  mock()
      .actualCall("foo")
      .withParameter("p2", 2)
      .withParameter("p3", 3)
      .withParameter("p4", 4);
  LONGS_EQUAL(1, mock().expectedCallsLeft());
  mock()
      .actualCall("foo")
      .withParameter("p2", 2)
      .withParameter("p3", 3)
      .withParameter("p4", 4);

  mock().checkExpectations();
}

TEST(MockParameter, ignoreOtherParametersMultipleCallsButOneDidntHappen)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  MockCheckedExpectedCall* call = expectations.addFunction("boo");
  call->ignoreOtherParameters();
  call->callWasMade(1);
  call->finalizeActualCallMatch();
  call->ignoreOtherParameters();
  expectations.addFunction("boo")->ignoreOtherParameters();
  MockExpectedCallsDidntHappenFailure expectedFailure(
      mockFailureTest(), expectations);

  mock().expectOneCall("boo").ignoreOtherParameters();
  mock().expectOneCall("boo").ignoreOtherParameters();
  mock().actualCall("boo");

  mock().checkExpectations();
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameter, newCallStartsWhileNotAllParametersWerePassed)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foo")->withParameter("p1", 1);
  MockExpectedParameterDidntHappenFailure expectedFailure(
      mockFailureTest(), "foo", expectations, expectations);

  mock().expectOneCall("foo").withParameter("p1", 1);
  mock().actualCall("foo");
  mock().actualCall("foo").withParameter("p1", 1);

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameter, outputParameterSucceeds)
{
  int param = 1;
  int retval = 2;

  mock()
      .expectOneCall("function")
      .withOutputParameterReturning("parameterName", &retval, sizeof(retval));
  mock().actualCall("function").withOutputParameter("parameterName", &param);

  CHECK_EQUAL(param, 2);
  CHECK_EQUAL(retval, 2);
  mock().checkExpectations();
}

TEST(MockParameter, unmodifiedOutputParameterSucceeds)
{
  int param = 1;

  mock()
      .expectOneCall("function")
      .withUnmodifiedOutputParameter("parameterName");
  mock().actualCall("function").withOutputParameter("parameterName", &param);

  CHECK_EQUAL(param, 1);
  mock().checkExpectations();
}

TEST(MockParameter, noActualCallForOutputParameter)
{
  MockFailureReporterInstaller failureReporterInstaller;

  int output = 0;
  MockExpectedCallsListForTest expectations;
  mock().expectOneCall("foo").withOutputParameterReturning(
      "output", &output, sizeof(output));

  expectations.addFunction("foo")->withOutputParameterReturning(
      "output", &output, sizeof(output));
  MockExpectedCallsDidntHappenFailure expectedFailure(
      mockFailureTest(), expectations);

  mock().checkExpectations();
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameter, noActualCallForUnmodifiedOutputParameter)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  mock().expectOneCall("foo").withUnmodifiedOutputParameter("output");

  expectations.addFunction("foo")->withUnmodifiedOutputParameter("output");
  MockExpectedCallsDidntHappenFailure expectedFailure(
      mockFailureTest(), expectations);

  mock().checkExpectations();
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameter, unexpectedOutputParameter)
{
  MockFailureReporterInstaller failureReporterInstaller;

  int param;
  MockExpectedCallsListForTest expectations;
  mock().expectOneCall("foo");
  mock().actualCall("foo").withOutputParameter("parameterName", &param);

  expectations.addFunction("foo");
  MockNamedValue parameter("parameterName");
  parameter.setValue(&param);
  MockUnexpectedOutputParameterFailure expectedFailure(
      mockFailureTest(), "foo", parameter, expectations);

  mock().checkExpectations();
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameter, outputParameterMissing)
{
  MockFailureReporterInstaller failureReporterInstaller;

  int output = 0;
  MockExpectedCallsListForTest expectations;
  mock().expectOneCall("foo").withOutputParameterReturning(
      "output", &output, sizeof(output));
  mock().actualCall("foo");

  expectations.addFunction("foo")->withOutputParameterReturning(
      "output", &output, sizeof(output));
  MockExpectedParameterDidntHappenFailure expectedFailure(
      mockFailureTest(), "foo", expectations, expectations);

  mock().checkExpectations();
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockParameter, twoOutputParameters)
{
  int param1 = 55;
  int retval1 = 1;
  int param2 = 77;
  int retval2 = 2;

  mock()
      .expectOneCall("function")
      .withOutputParameterReturning("parameterName", &retval1, sizeof(retval1))
      .withParameter("id", 1);
  mock()
      .expectOneCall("function")
      .withOutputParameterReturning("parameterName", &retval2, sizeof(retval2))
      .withParameter("id", 2);
  mock()
      .actualCall("function")
      .withOutputParameter("parameterName", &param1)
      .withParameter("id", 1);
  mock()
      .actualCall("function")
      .withOutputParameter("parameterName", &param2)
      .withParameter("id", 2);

  CHECK_EQUAL(retval1, param1);
  CHECK_EQUAL(retval2, param2);
  mock().checkExpectations();
}

TEST(MockParameter, twoInterleavedOutputParameters)
{
  int param1 = 55;
  int retval1 = 1;
  int param2 = 77;
  int retval2 = 2;

  mock()
      .expectOneCall("function")
      .withOutputParameterReturning("parameterName", &retval1, sizeof(retval1))
      .withParameter("id", 1);
  mock()
      .expectOneCall("function")
      .withOutputParameterReturning("parameterName", &retval2, sizeof(retval2))
      .withParameter("id", 2);
  mock()
      .actualCall("function")
      .withOutputParameter("parameterName", &param2)
      .withParameter("id", 2);
  mock()
      .actualCall("function")
      .withOutputParameter("parameterName", &param1)
      .withParameter("id", 1);

  CHECK_EQUAL(retval1, param1);
  CHECK_EQUAL(retval2, param2);
  mock().checkExpectations();
}

TEST(MockParameter, twoDifferentOutputParametersInSameFunctionCallSucceeds)
{
  int param1 = 1;
  int param2 = 1;
  int retval1 = 2;
  int retval2 = 3;

  mock()
      .expectOneCall("foo")
      .withOutputParameterReturning("bar", &retval1, sizeof(retval1))
      .withOutputParameterReturning("foobar", &retval2, sizeof(retval2));
  mock()
      .actualCall("foo")
      .withOutputParameter("bar", &param1)
      .withOutputParameter("foobar", &param2);

  CHECK_EQUAL(2, retval1);
  CHECK_EQUAL(2, param1);
  CHECK_EQUAL(3, retval2);
  CHECK_EQUAL(3, param2);
  mock().checkExpectations();
}

TEST(MockParameter,
    outputAndIntParametersOfSameNameInDifferentFunctionCallsOfSameFunctionSucceeds)
{
  int param = 1;
  int retval = 2;

  mock().expectOneCall("foo").withOutputParameterReturning(
      "bar", &retval, sizeof(retval));
  mock().expectOneCall("foo").withIntParameter("bar", 25);
  mock().actualCall("foo").withOutputParameter("bar", &param);
  mock().actualCall("foo").withIntParameter("bar", 25);

  CHECK_EQUAL(2, retval);
  CHECK_EQUAL(2, param);
  mock().checkExpectations();
}

TEST(MockParameter,
    twoOutputParameterOfSameNameInDifferentFunctionCallsOfSameFunctionSucceeds)
{
  int param1 = 1;
  int param2 = 1;
  int retval1 = 2;
  int retval2 = 3;

  mock().expectOneCall("foo").withOutputParameterReturning(
      "bar", &retval1, sizeof(retval1));
  mock().expectOneCall("foo").withOutputParameterReturning(
      "bar", &retval2, sizeof(retval2));
  mock().actualCall("foo").withOutputParameter("bar", &param1);
  mock().actualCall("foo").withOutputParameter("bar", &param2);

  CHECK_EQUAL(2, retval1);
  CHECK_EQUAL(2, param1);
  CHECK_EQUAL(3, retval2);
  CHECK_EQUAL(3, param2);
  mock().checkExpectations();
}

TEST(MockParameter, twoOutputParametersOfSameNameInDifferentFunctionsSucceeds)
{
  int param = 1;
  int retval = 2;

  mock().expectOneCall("foo1").withOutputParameterReturning(
      "bar", &retval, sizeof(retval));
  mock().expectOneCall("foo2").withIntParameter("bar", 25);
  mock().actualCall("foo1").withOutputParameter("bar", &param);
  mock().actualCall("foo2").withIntParameter("bar", 25);

  CHECK_EQUAL(2, retval);
  CHECK_EQUAL(2, param);
  mock().checkExpectations();
}

TEST(MockParameter, outputAndInputParameter)
{
  int return_value = 5;
  int returned_value = 7;

  mock()
      .expectOneCall("foo")
      .withParameter("bar", 10)
      .withOutputParameterReturning("bar", &return_value, sizeof(return_value));
  mock().actualCall("foo").withParameter("bar", 10).withOutputParameter(
      "bar", &returned_value);

  LONGS_EQUAL(5, returned_value);
  mock().checkExpectations();
}

TEST(MockParameter, outputParameterTraced)
{
  mock().tracing(true);

  int param = 1;
  mock().actualCall("someFunc").withOutputParameter("someParameter", &param);
  mock().checkExpectations();
  STRCMP_CONTAINS(
      "Function name:someFunc someParameter:", mock().getTraceOutput());

  mock().checkExpectations();
}

TEST(MockParameter, outputParameterThatIsIgnoredShouldNotFail)
{
  int param;
  mock().expectOneCall("function").ignoreOtherParameters();
  mock().actualCall("function").withOutputParameter("parameterName", &param);

  mock().checkExpectations();
}

TEST(MockParameter, outputParameterWithIgnoredParameters)
{
  int param = 1;
  int retval = 2;

  mock()
      .expectOneCall("foo")
      .withOutputParameterReturning("bar", &param, sizeof(param))
      .ignoreOtherParameters();
  mock()
      .actualCall("foo")
      .withOutputParameter("bar", &retval)
      .withParameter("other", 1);

  LONGS_EQUAL(param, retval);

  mock().checkExpectations();
}

/*
 * This test checks that the proper output parameters are copied when multiple
 * calls to the same function are expected.
 */
TEST(MockParameter, properOutputParametersAreCopied)
{
  int expectedValue1 = 1;
  int expectedValue2 = 2;
  mock()
      .expectOneCall("foo")
      .withOutputParameterReturning(
          "param", &expectedValue1, sizeof(expectedValue1))
      .ignoreOtherParameters();
  mock().expectOneCall("foo").withOutputParameterReturning(
      "param", &expectedValue2, sizeof(expectedValue2));

  int returnedValue1 = 0;
  int returnedValue2 = 0;
  mock().actualCall("foo").withOutputParameter("param", &returnedValue1);
  mock()
      .actualCall("foo")
      .withOutputParameter("param", &returnedValue2)
      .withParameter("optional", 50);

  CHECK_EQUAL_TEXT(
      expectedValue2, returnedValue1, "Wrong output value in 1st call");
  CHECK_EQUAL_TEXT(
      expectedValue1, returnedValue2, "Wrong output value in 2nd call");

  mock().checkExpectations();
}

TEST(MockParameter, ignoreOtherCallsIgnoresWithAllKindsOfParameters)
{
  mock().ignoreOtherCalls();
  mock()
      .actualCall("boo")
      .withParameter("umm", true)
      .withParameter("bar", 1u)
      .withParameter("foo", 1l)
      .withParameter("hey", 1ul)
      .withParameter("ick", 1ll)
      .withParameter("grr", 1ull)
      .withParameter("duh", 1.0)
      .withParameter("yoo", static_cast<const void*>(nullptr))
      .withParameter("func", static_cast<void (*)()>(nullptr))
      .withParameter("mem", static_cast<const unsigned char*>(nullptr), 0)
      .withParameterOfType("hoo", "int", static_cast<const void*>(nullptr))
      .withOutputParameter("gah", static_cast<void*>(nullptr))
      .withOutputParameterOfType("goo", "int", static_cast<void*>(nullptr));

  mock().checkExpectations();
}

TEST(MockParameter, expectMultipleCallsWithParameters)
{
  int expected_int = -7;
  unsigned int expected_uint = 7;

  mock()
      .expectNCalls(2, "boo")
      .withParameter("double", 1.0)
      .withParameter("int", expected_int)
      .withParameter("string", "string")
      .withParameter("uint", expected_uint);
  mock()
      .actualCall("boo")
      .withParameter("double", 1.0)
      .withParameter("int", expected_int)
      .withParameter("string", "string")
      .withParameter("uint", expected_uint);
  mock()
      .actualCall("boo")
      .withParameter("double", 1.0)
      .withParameter("int", expected_int)
      .withParameter("string", "string")
      .withParameter("uint", expected_uint);

  mock().checkExpectations();
}

TEST(MockParameter, expectMultipleMultipleCallsWithParameters)
{
  mock()
      .expectNCalls(2, "boo")
      .withParameter("double", 1.0)
      .ignoreOtherParameters();
  mock()
      .expectNCalls(2, "boo")
      .withParameter("double", 1.0)
      .ignoreOtherParameters();
  mock()
      .actualCall("boo")
      .withParameter("double", 1.0)
      .withParameter("int", 1)
      .withParameter("string", "string");
  mock()
      .actualCall("boo")
      .withParameter("double", 1.0)
      .withParameter("int", 1)
      .withParameter("string", "string");
  mock()
      .actualCall("boo")
      .withParameter("double", 1.0)
      .withParameter("int", 1)
      .withParameter("string", "string");
  mock()
      .actualCall("boo")
      .withParameter("double", 1.0)
      .withParameter("int", 1)
      .withParameter("string", "string");

  mock().checkExpectations();
}
