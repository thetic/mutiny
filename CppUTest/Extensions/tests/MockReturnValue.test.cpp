#include "MockFailureReporterForTest.hpp"

#include "CppUTest/TestHarness.hpp"

using cpputest::extensions::mock;

TEST_GROUP(MockReturnValue)
{
  void teardown() override
  {
    mock().checkExpectations();
    mock().clear();
  }
};

TEST(MockReturnValue, hasReturnValue)
{
  CHECK(!mock().hasReturnValue());
  mock().expectOneCall("foo");
  CHECK(!mock().actualCall("foo").hasReturnValue());
  CHECK(!mock().hasReturnValue());

  mock().expectOneCall("foo2").andReturnValue(1);
  CHECK(mock().actualCall("foo2").hasReturnValue());
  CHECK(mock().hasReturnValue());
}

TEST(MockReturnValue, UnsignedIntegerReturnValue)
{
  unsigned int expected_value = 7;

  mock().expectOneCall("foo").andReturnValue(expected_value);

  cpputest::extensions::MockActualCall& actual_call = mock().actualCall("foo");

  LONGS_EQUAL(expected_value, actual_call.returnValue().getUnsignedIntValue());
  LONGS_EQUAL(expected_value, actual_call.returnUnsignedIntValue());

  LONGS_EQUAL(expected_value, mock().returnValue().getUnsignedIntValue());
  LONGS_EQUAL(expected_value, mock().unsignedIntReturnValue());
}

TEST(MockReturnValue, PositiveIntReturnValueCanBeRetrievedAsUnsignedInt)
{
  int expected_value = 7;
  mock().expectOneCall("foo").andReturnValue(expected_value);
  LONGS_EQUAL(expected_value,
      mock().actualCall("foo").returnValue().getUnsignedIntValue());
}

TEST(MockReturnValue, IntReturnValueCanBeRetrievedAsLongInt)
{
  int expected_value = 7;
  mock().expectOneCall("foo").andReturnValue(expected_value);
  LONGS_EQUAL(
      expected_value, mock().actualCall("foo").returnValue().getLongIntValue());
}

TEST(MockReturnValue, UnsignedIntReturnValueCanBeRetrievedAsLongInt)
{
  unsigned int expected_value = 7;
  mock().expectOneCall("foo").andReturnValue(expected_value);
  LONGS_EQUAL(
      expected_value, mock().actualCall("foo").returnValue().getLongIntValue());
}

TEST(MockReturnValue, PositiveIntReturnValueCanBeRetrievedAsUnsignedLongInt)
{
  int expected_value = 7;
  mock().expectOneCall("foo").andReturnValue(expected_value);
  UNSIGNED_LONGS_EQUAL(expected_value,
      mock().actualCall("foo").returnValue().getUnsignedLongIntValue());
}

TEST(MockReturnValue, PositiveLongIntReturnValueCanBeRetrievedAsUnsignedLongInt)
{
  long int expected_value = 7;
  mock().expectOneCall("foo").andReturnValue(expected_value);
  UNSIGNED_LONGS_EQUAL(expected_value,
      mock().actualCall("foo").returnValue().getUnsignedLongIntValue());
}

TEST(MockReturnValue, UnsignedIntReturnValueCanBeRetrievedAsUnsignedLongInt)
{
  unsigned int expected_value = 7;
  mock().expectOneCall("foo").andReturnValue(expected_value);
  UNSIGNED_LONGS_EQUAL(expected_value,
      mock().actualCall("foo").returnValue().getUnsignedLongIntValue());
}

TEST(MockReturnValue, PositiveIntReturnValueCanBeRetrievedAsUnsignedLongLongInt)
{
  int expected_value = 7;
  mock().expectOneCall("foo").andReturnValue(expected_value);
  UNSIGNED_LONGLONGS_EQUAL(expected_value,
      mock().actualCall("foo").returnValue().getUnsignedLongLongIntValue());
}

TEST(MockReturnValue,
    PositiveLongIntReturnValueCanBeRetrievedAsUnsignedLongLongInt)
{
  long int expected_value = 7;
  mock().expectOneCall("foo").andReturnValue(expected_value);
  UNSIGNED_LONGLONGS_EQUAL(expected_value,
      mock().actualCall("foo").returnValue().getUnsignedLongLongIntValue());
}

TEST(MockReturnValue,
    PositiveLongLongIntReturnValueCanBeRetrievedAsUnsignedLongLongInt)
{
  long long int expected_value = 7;
  mock().expectOneCall("foo").andReturnValue(expected_value);
  UNSIGNED_LONGLONGS_EQUAL(expected_value,
      mock().actualCall("foo").returnValue().getUnsignedLongLongIntValue());
}

TEST(MockReturnValue, UnsignedIntReturnValueCanBeRetrievedAsUnsignedLongLongInt)
{
  unsigned int expected_value = 7;
  mock().expectOneCall("foo").andReturnValue(expected_value);
  UNSIGNED_LONGLONGS_EQUAL(expected_value,
      mock().actualCall("foo").returnValue().getUnsignedLongLongIntValue());
}

TEST(MockReturnValue,
    UnsignedLongIntReturnValueCanBeRetrievedAsUnsignedLongLongInt)
{
  unsigned long int expected_value = 7;
  mock().expectOneCall("foo").andReturnValue(expected_value);
  UNSIGNED_LONGLONGS_EQUAL(expected_value,
      mock().actualCall("foo").returnValue().getUnsignedLongLongIntValue());
}

TEST(MockReturnValue, UnsignedLongLongIntReturnValueCanBeRetrieved)
{
  unsigned long long int expected_value = 2ULL;
  mock().expectOneCall("foo").andReturnValue(expected_value);
  UNSIGNED_LONGLONGS_EQUAL(expected_value,
      mock().actualCall("foo").returnValue().getUnsignedLongLongIntValue());
}

TEST(MockReturnValue, PositiveIntReturnValueCanBeRetrievedAsLongLongInt)
{
  int expected_value = 7;
  mock().expectOneCall("foo").andReturnValue(expected_value);
  LONGLONGS_EQUAL(expected_value,
      mock().actualCall("foo").returnValue().getLongLongIntValue());
}

TEST(MockReturnValue, PositiveLongIntReturnValueCanBeRetrievedAsLongLongInt)
{
  long int expected_value = 7;
  mock().expectOneCall("foo").andReturnValue(expected_value);
  LONGLONGS_EQUAL(expected_value,
      mock().actualCall("foo").returnValue().getLongLongIntValue());
}

TEST(MockReturnValue, UnsignedIntReturnValueCanBeRetrievedAsLongLongInt)
{
  unsigned int expected_value = 7;
  mock().expectOneCall("foo").andReturnValue(expected_value);
  LONGLONGS_EQUAL(expected_value,
      mock().actualCall("foo").returnValue().getLongLongIntValue());
}

TEST(MockReturnValue, UnsignedLongIntReturnValueCanBeRetrievedAsLongLongInt)
{
  unsigned long int expected_value = 7;
  mock().expectOneCall("foo").andReturnValue(expected_value);
  LONGLONGS_EQUAL(expected_value,
      mock().actualCall("foo").returnValue().getLongLongIntValue());
}

TEST(MockReturnValue, LongLongIntReturnValueCanBeRetrieved)
{
  long long int expected_value = 2LL;
  mock().expectOneCall("foo").andReturnValue(expected_value);
  LONGLONGS_EQUAL(expected_value,
      mock().actualCall("foo").returnValue().getLongLongIntValue());
}

TEST(MockReturnValue, UnsignedIntegerReturnValueSetsDifferentValues)
{
  unsigned int expected_value = 1;
  unsigned int another_expected_value = 2;

  mock().expectOneCall("foo").andReturnValue(expected_value);
  mock().expectOneCall("foo").andReturnValue(another_expected_value);

  LONGS_EQUAL(expected_value,
      mock().actualCall("foo").returnValue().getUnsignedIntValue());
  LONGS_EQUAL(expected_value, mock().returnValue().getUnsignedIntValue());
  LONGS_EQUAL(another_expected_value,
      mock().actualCall("foo").returnValue().getUnsignedIntValue());
  LONGS_EQUAL(
      another_expected_value, mock().returnValue().getUnsignedIntValue());
}

TEST(MockReturnValue,
    UnsignedIntegerReturnValueSetsDifferentValuesWhileParametersAreIgnored)
{
  unsigned int ret_value = 1;
  unsigned int another_ret_value = 2;

  mock()
      .expectOneCall("foo")
      .withParameter("p1", 1)
      .ignoreOtherParameters()
      .andReturnValue(ret_value);
  mock()
      .expectOneCall("foo")
      .withParameter("p1", 1)
      .ignoreOtherParameters()
      .andReturnValue(another_ret_value);

  LONGS_EQUAL(ret_value,
      mock()
          .actualCall("foo")
          .withParameter("p1", 1)
          .returnValue()
          .getUnsignedIntValue());
  LONGS_EQUAL(ret_value, mock().returnValue().getUnsignedIntValue());
  LONGS_EQUAL(another_ret_value,
      mock()
          .actualCall("foo")
          .withParameter("p1", 1)
          .returnValue()
          .getUnsignedIntValue());
  LONGS_EQUAL(another_ret_value, mock().returnValue().getUnsignedIntValue());
}

TEST(MockReturnValue,
    WhenADoubleReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  double default_return_value = 10.7;
  double expected_return_value = default_return_value + 1.3;
  mock().expectOneCall("foo").andReturnValue(expected_return_value);

  DOUBLES_EQUAL(expected_return_value,
      mock().actualCall("foo").returnDoubleValueOrDefault(default_return_value),
      0.05);
  DOUBLES_EQUAL(expected_return_value,
      mock().returnDoubleValueOrDefault(default_return_value),
      0.05);
}

TEST(MockReturnValue,
    WhenNoDoubleReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  double default_return_value = 7.7;
  mock().expectOneCall("foo");
  DOUBLES_EQUAL(default_return_value,
      mock().actualCall("foo").returnDoubleValueOrDefault(default_return_value),
      0.05);
  DOUBLES_EQUAL(default_return_value,
      mock().returnDoubleValueOrDefault(default_return_value),
      0.05);
}

TEST(MockReturnValue,
    WhenAUnsignedIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  unsigned int default_return_value = 10;
  unsigned int expected_return_value = default_return_value + 1;
  mock().expectOneCall("foo").andReturnValue(expected_return_value);
  LONGS_EQUAL(expected_return_value,
      mock().actualCall("foo").returnUnsignedIntValueOrDefault(
          default_return_value));
  LONGS_EQUAL(expected_return_value,
      mock().returnUnsignedIntValueOrDefault(default_return_value));
}

TEST(MockReturnValue,
    WhenNoUnsignedIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  unsigned int default_return_value = 10;
  mock().expectOneCall("foo");
  LONGS_EQUAL(default_return_value,
      mock().actualCall("foo").returnUnsignedIntValueOrDefault(
          default_return_value));
  LONGS_EQUAL(default_return_value,
      mock().returnUnsignedIntValueOrDefault(default_return_value));
}

TEST(MockReturnValue,
    WhenAUnsignedLongIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  unsigned long int default_return_value = 121487;
  unsigned long int expected_return_value = default_return_value + 1;
  mock().expectOneCall("foo").andReturnValue(expected_return_value);
  LONGS_EQUAL(expected_return_value,
      mock().actualCall("foo").returnUnsignedLongIntValueOrDefault(
          default_return_value));
  LONGS_EQUAL(expected_return_value,
      mock().returnUnsignedLongIntValueOrDefault(default_return_value));
}

TEST(MockReturnValue,
    WhenNoUnsignedLongIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  unsigned long int default_return_value = 7710144;
  mock().expectOneCall("foo");
  LONGS_EQUAL(default_return_value,
      mock().actualCall("foo").returnUnsignedLongIntValueOrDefault(
          default_return_value));
  LONGS_EQUAL(default_return_value,
      mock().returnUnsignedLongIntValueOrDefault(default_return_value));
}

TEST(MockReturnValue,
    WhenALongIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  long int default_return_value = 748797;
  long int expected_return_value = default_return_value + 1;
  mock().expectOneCall("foo").andReturnValue(expected_return_value);
  LONGS_EQUAL(expected_return_value,
      mock().actualCall("foo").returnLongIntValueOrDefault(
          default_return_value));
  LONGS_EQUAL(expected_return_value,
      mock().returnLongIntValueOrDefault(default_return_value));
}

TEST(MockReturnValue,
    WhenNoLongIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  long int default_return_value = 123659;
  mock().expectOneCall("foo");
  LONGS_EQUAL(default_return_value,
      mock().actualCall("foo").returnLongIntValueOrDefault(
          default_return_value));
  LONGS_EQUAL(default_return_value,
      mock().returnLongIntValueOrDefault(default_return_value));
}

TEST(MockReturnValue,
    WhenAUnsignedLongLongIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  unsigned long long int default_return_value = 2ULL;
  unsigned long long int expected_return_value = default_return_value - 1;
  mock().expectOneCall("foo").andReturnValue(expected_return_value);
  LONGS_EQUAL(expected_return_value,
      mock().actualCall("foo").returnUnsignedLongLongIntValueOrDefault(
          default_return_value));
  LONGS_EQUAL(expected_return_value,
      mock().returnUnsignedLongLongIntValueOrDefault(default_return_value));
}

TEST(MockReturnValue,
    WhenNoUnsignedLongLongIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  unsigned long long int default_return_value = 2ULL;
  mock().expectOneCall("foo");
  LONGS_EQUAL(default_return_value,
      mock().actualCall("foo").returnUnsignedLongLongIntValueOrDefault(
          default_return_value));
  LONGS_EQUAL(default_return_value,
      mock().returnUnsignedLongLongIntValueOrDefault(default_return_value));
}

TEST(MockReturnValue,
    WhenALongLongIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  long long int default_return_value = 2LL;
  long long int expected_return_value = default_return_value - 1;
  mock().expectOneCall("foo").andReturnValue(expected_return_value);
  LONGS_EQUAL(expected_return_value,
      mock().actualCall("foo").returnLongLongIntValueOrDefault(
          default_return_value));
  LONGS_EQUAL(expected_return_value,
      mock().returnLongLongIntValueOrDefault(default_return_value));
}

TEST(MockReturnValue,
    WhenNoLongLongIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  long long int default_return_value = 2LL;
  mock().expectOneCall("foo");
  LONGS_EQUAL(default_return_value,
      mock().actualCall("foo").returnLongLongIntValueOrDefault(
          default_return_value));
  LONGS_EQUAL(default_return_value,
      mock().returnLongLongIntValueOrDefault(default_return_value));
}

TEST(MockReturnValue,
    WhenABooleanReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  bool default_return_value = true;
  bool expected_return_value = false;
  mock().expectOneCall("foo").andReturnValue(expected_return_value);
  CHECK_EQUAL(expected_return_value,
      mock().actualCall("foo").returnBoolValueOrDefault(default_return_value));
  CHECK_EQUAL(expected_return_value,
      mock().returnBoolValueOrDefault(default_return_value));
}

TEST(MockReturnValue,
    WhenNoBooleanReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  bool default_return_value = true;
  mock().expectOneCall("foo");
  CHECK_EQUAL(default_return_value,
      mock().actualCall("foo").returnBoolValueOrDefault(default_return_value));
  CHECK_EQUAL(default_return_value,
      mock().returnBoolValueOrDefault(default_return_value));
}

TEST(MockReturnValue,
    WhenAIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  int default_return_value = 777;
  int expected_return_value = default_return_value + 1;
  mock().expectOneCall("foo").andReturnValue(expected_return_value);
  LONGS_EQUAL(expected_return_value,
      mock().actualCall("foo").returnIntValueOrDefault(default_return_value));
  LONGS_EQUAL(expected_return_value,
      mock().returnIntValueOrDefault(default_return_value));
}

TEST(MockReturnValue,
    WhenNoIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  int default_return_value = 777;
  mock().expectOneCall("foo");
  LONGS_EQUAL(default_return_value,
      mock().actualCall("foo").returnIntValueOrDefault(default_return_value));
  LONGS_EQUAL(default_return_value,
      mock().returnIntValueOrDefault(default_return_value));
}

TEST(MockReturnValue, BooleanReturnValue)
{
  bool expected_value = true;
  mock().expectOneCall("foo").andReturnValue(true);
  cpputest::extensions::MockActualCall& actual_call = mock().actualCall("foo");

  CHECK_EQUAL(expected_value, actual_call.returnValue().getBoolValue());
  CHECK_EQUAL(expected_value, actual_call.returnBoolValue());

  CHECK_EQUAL(expected_value, mock().returnValue().getBoolValue());
  CHECK_EQUAL(expected_value, mock().boolReturnValue());
}

TEST(MockReturnValue, BooleanReturnValueSetsDifferentValues)
{
  bool expected_value = true;
  bool another_expected_value = false;

  mock().expectOneCall("foo").andReturnValue(expected_value);
  mock().expectOneCall("foo").andReturnValue(another_expected_value);

  CHECK_EQUAL(
      expected_value, mock().actualCall("foo").returnValue().getBoolValue());
  CHECK_EQUAL(expected_value, mock().returnValue().getBoolValue());
  CHECK_EQUAL(another_expected_value,
      mock().actualCall("foo").returnValue().getBoolValue());
  CHECK_EQUAL(another_expected_value, mock().returnValue().getBoolValue());
}

TEST(MockReturnValue,
    BooleanReturnValueSetsDifferentValuesWhileParametersAreIgnored)
{
  bool ret_value = true;
  bool another_ret_value = false;

  mock()
      .expectOneCall("foo")
      .withParameter("p1", 1)
      .ignoreOtherParameters()
      .andReturnValue(ret_value);
  mock()
      .expectOneCall("foo")
      .withParameter("p1", 1)
      .ignoreOtherParameters()
      .andReturnValue(another_ret_value);

  CHECK_EQUAL(ret_value,
      mock()
          .actualCall("foo")
          .withParameter("p1", 1)
          .returnValue()
          .getBoolValue());
  CHECK_EQUAL(ret_value, mock().returnValue().getBoolValue());
  CHECK_EQUAL(another_ret_value,
      mock()
          .actualCall("foo")
          .withParameter("p1", 1)
          .returnValue()
          .getBoolValue());
  CHECK_EQUAL(another_ret_value, mock().returnValue().getBoolValue());
}

TEST(MockReturnValue, IntegerReturnValue)
{
  int expected_value = 1;
  mock().expectOneCall("foo").andReturnValue(1);
  cpputest::extensions::MockActualCall& actual_call = mock().actualCall("foo");

  LONGS_EQUAL(expected_value, actual_call.returnValue().getIntValue());
  LONGS_EQUAL(expected_value, actual_call.returnIntValue());

  LONGS_EQUAL(expected_value, mock().returnValue().getIntValue());
  LONGS_EQUAL(expected_value, mock().intReturnValue());
}

TEST(MockReturnValue, IntegerReturnValueSetsDifferentValues)
{
  int expected_value = 1;
  int another_expected_value = -1;

  mock().expectOneCall("foo").andReturnValue(expected_value);
  mock().expectOneCall("foo").andReturnValue(another_expected_value);

  LONGS_EQUAL(
      expected_value, mock().actualCall("foo").returnValue().getIntValue());
  LONGS_EQUAL(expected_value, mock().returnValue().getIntValue());
  LONGS_EQUAL(another_expected_value,
      mock().actualCall("foo").returnValue().getIntValue());
  LONGS_EQUAL(another_expected_value, mock().returnValue().getIntValue());
}

TEST(MockReturnValue,
    IntegerReturnValueSetsDifferentValuesWhileParametersAreIgnored)
{
  int ret_value = 1;
  int another_ret_value = -1;

  mock()
      .expectOneCall("foo")
      .withParameter("p1", 1)
      .ignoreOtherParameters()
      .andReturnValue(ret_value);
  mock()
      .expectOneCall("foo")
      .withParameter("p1", 1)
      .ignoreOtherParameters()
      .andReturnValue(another_ret_value);

  LONGS_EQUAL(ret_value,
      mock()
          .actualCall("foo")
          .withParameter("p1", 1)
          .returnValue()
          .getIntValue());
  LONGS_EQUAL(ret_value, mock().returnValue().getIntValue());
  LONGS_EQUAL(another_ret_value,
      mock()
          .actualCall("foo")
          .withParameter("p1", 1)
          .returnValue()
          .getIntValue());
  LONGS_EQUAL(another_ret_value, mock().returnValue().getIntValue());
}

TEST(MockReturnValue, LongIntegerReturnValue)
{
  long int expected_value = 7;
  mock().expectOneCall("foo").andReturnValue(expected_value);

  cpputest::extensions::MockActualCall& actual_call = mock().actualCall("foo");
  LONGS_EQUAL(expected_value, actual_call.returnValue().getLongIntValue());
  LONGS_EQUAL(expected_value, actual_call.returnLongIntValue());
  LONGS_EQUAL(expected_value, mock().returnValue().getLongIntValue());
  LONGS_EQUAL(expected_value, mock().longIntReturnValue());
}

TEST(MockReturnValue, LongIntegerReturnValueSetsDifferentValues)
{
  long int expected_value = 1;
  long int another_expected_value = 2;

  mock().expectOneCall("foo").andReturnValue(expected_value);
  mock().expectOneCall("foo").andReturnValue(another_expected_value);

  LONGS_EQUAL(
      expected_value, mock().actualCall("foo").returnValue().getLongIntValue());
  LONGS_EQUAL(expected_value, mock().returnValue().getLongIntValue());
  LONGS_EQUAL(another_expected_value,
      mock().actualCall("foo").returnValue().getLongIntValue());
  LONGS_EQUAL(another_expected_value, mock().returnValue().getLongIntValue());
}

TEST(MockReturnValue,
    LongIntegerReturnValueSetsDifferentValuesWhileParametersAreIgnored)
{
  long int ret_value = 1;
  long int another_ret_value = 2;

  mock()
      .expectOneCall("foo")
      .withParameter("p1", 1)
      .ignoreOtherParameters()
      .andReturnValue(ret_value);
  mock()
      .expectOneCall("foo")
      .withParameter("p1", 1)
      .ignoreOtherParameters()
      .andReturnValue(another_ret_value);

  LONGS_EQUAL(ret_value,
      mock()
          .actualCall("foo")
          .withParameter("p1", 1)
          .returnValue()
          .getLongIntValue());
  LONGS_EQUAL(ret_value, mock().returnValue().getLongIntValue());
  LONGS_EQUAL(another_ret_value,
      mock()
          .actualCall("foo")
          .withParameter("p1", 1)
          .returnValue()
          .getLongIntValue());
  LONGS_EQUAL(another_ret_value, mock().returnValue().getLongIntValue());
}

TEST(MockReturnValue, UnsignedLongIntegerReturnValue)
{
  unsigned long int expected_value = 7;
  mock().expectOneCall("foo").andReturnValue(expected_value);

  cpputest::extensions::MockActualCall& actual_call = mock().actualCall("foo");
  LONGS_EQUAL(
      expected_value, actual_call.returnValue().getUnsignedLongIntValue());
  LONGS_EQUAL(expected_value, actual_call.returnUnsignedLongIntValue());
  LONGS_EQUAL(expected_value, mock().returnValue().getUnsignedLongIntValue());
  LONGS_EQUAL(expected_value, mock().unsignedLongIntReturnValue());
}

TEST(MockReturnValue, UnsignedLongIntegerReturnValueSetsDifferentValues)
{
  unsigned long int expected_value = 1;
  unsigned long int another_expected_value = 2;

  mock().expectOneCall("foo").andReturnValue(expected_value);
  mock().expectOneCall("foo").andReturnValue(another_expected_value);

  LONGS_EQUAL(expected_value,
      mock().actualCall("foo").returnValue().getUnsignedLongIntValue());
  LONGS_EQUAL(expected_value, mock().returnValue().getUnsignedLongIntValue());
  LONGS_EQUAL(another_expected_value,
      mock().actualCall("foo").returnValue().getUnsignedLongIntValue());
  LONGS_EQUAL(
      another_expected_value, mock().returnValue().getUnsignedLongIntValue());
}

TEST(MockReturnValue,
    UnsignedLongIntegerReturnValueSetsDifferentValuesWhileParametersAreIgnored)
{
  unsigned long int ret_value = 1;
  unsigned long int another_ret_value = 2;

  mock()
      .expectOneCall("foo")
      .withParameter("p1", 1)
      .ignoreOtherParameters()
      .andReturnValue(ret_value);
  mock()
      .expectOneCall("foo")
      .withParameter("p1", 1)
      .ignoreOtherParameters()
      .andReturnValue(another_ret_value);

  LONGS_EQUAL(ret_value,
      mock()
          .actualCall("foo")
          .withParameter("p1", 1)
          .returnValue()
          .getUnsignedLongIntValue());
  LONGS_EQUAL(ret_value, mock().returnValue().getUnsignedLongIntValue());
  LONGS_EQUAL(another_ret_value,
      mock()
          .actualCall("foo")
          .withParameter("p1", 1)
          .returnValue()
          .getUnsignedLongIntValue());
  LONGS_EQUAL(
      another_ret_value, mock().returnValue().getUnsignedLongIntValue());
}

TEST(MockReturnValue, LongLongIntegerReturnValue)
{
  long long int expected_value = 7;
  mock().expectOneCall("foo").andReturnValue(expected_value);

  cpputest::extensions::MockActualCall& actual_call = mock().actualCall("foo");
  LONGLONGS_EQUAL(
      expected_value, actual_call.returnValue().getLongLongIntValue());
  LONGLONGS_EQUAL(expected_value, actual_call.returnLongLongIntValue());
  LONGLONGS_EQUAL(expected_value, mock().returnValue().getLongLongIntValue());
  LONGLONGS_EQUAL(expected_value, mock().longLongIntReturnValue());
}

TEST(MockReturnValue, LongLongIntegerReturnValueSetsDifferentValues)
{
  long long int expected_value = 1;
  long long int another_expected_value = 2;

  mock().expectOneCall("foo").andReturnValue(expected_value);
  mock().expectOneCall("foo").andReturnValue(another_expected_value);

  LONGLONGS_EQUAL(expected_value,
      mock().actualCall("foo").returnValue().getLongLongIntValue());
  LONGLONGS_EQUAL(expected_value, mock().returnValue().getLongLongIntValue());
  LONGLONGS_EQUAL(another_expected_value,
      mock().actualCall("foo").returnValue().getLongLongIntValue());
  LONGLONGS_EQUAL(
      another_expected_value, mock().returnValue().getLongLongIntValue());
}

TEST(MockReturnValue,
    LongLongIntegerReturnValueSetsDifferentValuesWhileParametersAreIgnored)
{
  long long int ret_value = 1;
  long long int another_ret_value = 2;

  mock()
      .expectOneCall("foo")
      .withParameter("p1", 1)
      .ignoreOtherParameters()
      .andReturnValue(ret_value);
  mock()
      .expectOneCall("foo")
      .withParameter("p1", 1)
      .ignoreOtherParameters()
      .andReturnValue(another_ret_value);

  LONGLONGS_EQUAL(ret_value,
      mock()
          .actualCall("foo")
          .withParameter("p1", 1)
          .returnValue()
          .getLongLongIntValue());
  LONGLONGS_EQUAL(ret_value, mock().returnValue().getLongLongIntValue());
  LONGLONGS_EQUAL(another_ret_value,
      mock()
          .actualCall("foo")
          .withParameter("p1", 1)
          .returnValue()
          .getLongLongIntValue());
  LONGLONGS_EQUAL(
      another_ret_value, mock().returnValue().getLongLongIntValue());
}

TEST(MockReturnValue, UnsignedLongLongIntegerReturnValue)
{
  unsigned long long int expected_value = 7;
  mock().expectOneCall("foo").andReturnValue(expected_value);

  cpputest::extensions::MockActualCall& actual_call = mock().actualCall("foo");
  UNSIGNED_LONGLONGS_EQUAL(
      expected_value, actual_call.returnValue().getUnsignedLongLongIntValue());
  UNSIGNED_LONGLONGS_EQUAL(
      expected_value, actual_call.returnUnsignedLongLongIntValue());
  UNSIGNED_LONGLONGS_EQUAL(
      expected_value, mock().returnValue().getUnsignedLongLongIntValue());
  UNSIGNED_LONGLONGS_EQUAL(
      expected_value, mock().unsignedLongLongIntReturnValue());
}

TEST(MockReturnValue, UnsignedLongLongIntegerReturnValueSetsDifferentValues)
{
  unsigned long long int expected_value = 1;
  unsigned long long int another_expected_value = 2;

  mock().expectOneCall("foo").andReturnValue(expected_value);
  mock().expectOneCall("foo").andReturnValue(another_expected_value);

  UNSIGNED_LONGLONGS_EQUAL(expected_value,
      mock().actualCall("foo").returnValue().getUnsignedLongLongIntValue());
  UNSIGNED_LONGLONGS_EQUAL(
      expected_value, mock().returnValue().getUnsignedLongLongIntValue());
  UNSIGNED_LONGLONGS_EQUAL(another_expected_value,
      mock().actualCall("foo").returnValue().getUnsignedLongLongIntValue());
  UNSIGNED_LONGLONGS_EQUAL(another_expected_value,
      mock().returnValue().getUnsignedLongLongIntValue());
}

TEST(MockReturnValue,
    UnsignedLongLongIntegerReturnValueSetsDifferentValuesWhileParametersAreIgnored)
{
  unsigned long long int ret_value = 1;
  unsigned long long int another_ret_value = 2;

  mock()
      .expectOneCall("foo")
      .withParameter("p1", 1)
      .ignoreOtherParameters()
      .andReturnValue(ret_value);
  mock()
      .expectOneCall("foo")
      .withParameter("p1", 1)
      .ignoreOtherParameters()
      .andReturnValue(another_ret_value);

  UNSIGNED_LONGLONGS_EQUAL(ret_value,
      mock()
          .actualCall("foo")
          .withParameter("p1", 1)
          .returnValue()
          .getUnsignedLongLongIntValue());
  UNSIGNED_LONGLONGS_EQUAL(
      ret_value, mock().returnValue().getUnsignedLongLongIntValue());
  UNSIGNED_LONGLONGS_EQUAL(another_ret_value,
      mock()
          .actualCall("foo")
          .withParameter("p1", 1)
          .returnValue()
          .getUnsignedLongLongIntValue());
  UNSIGNED_LONGLONGS_EQUAL(
      another_ret_value, mock().returnValue().getUnsignedLongLongIntValue());
}

TEST(MockReturnValue, MatchingReturnValueOnWhileSignature)
{
  mock().expectOneCall("foo").withParameter("p1", 1).andReturnValue(1);
  mock().expectOneCall("foo").withParameter("p1", 2).andReturnValue(2);
  mock().expectOneCall("foo").withParameter("p1", 3).andReturnValue(3);
  mock().expectOneCall("foo").ignoreOtherParameters().andReturnValue(4);

  LONGS_EQUAL(3,
      mock()
          .actualCall("foo")
          .withParameter("p1", 3)
          .returnValue()
          .getIntValue());
  LONGS_EQUAL(4,
      mock()
          .actualCall("foo")
          .withParameter("p1", 4)
          .returnValue()
          .getIntValue());
  LONGS_EQUAL(1,
      mock()
          .actualCall("foo")
          .withParameter("p1", 1)
          .returnValue()
          .getIntValue());
  LONGS_EQUAL(2,
      mock()
          .actualCall("foo")
          .withParameter("p1", 2)
          .returnValue()
          .getIntValue());
}

TEST(MockReturnValue,
    WhenAStringReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  const char* default_return_value = "default";
  const char* expected_return_value = "expected";
  mock().expectOneCall("foo").andReturnValue(expected_return_value);
  STRCMP_EQUAL(expected_return_value,
      mock().actualCall("foo").returnStringValueOrDefault(
          default_return_value));
  STRCMP_EQUAL(expected_return_value,
      mock().returnStringValueOrDefault(default_return_value));
}

TEST(MockReturnValue,
    WhenNoStringReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  const char* default_return_value = "default";
  mock().expectOneCall("foo");
  STRCMP_EQUAL(default_return_value,
      mock().actualCall("foo").returnStringValueOrDefault(
          default_return_value));
  STRCMP_EQUAL(default_return_value,
      mock().returnStringValueOrDefault(default_return_value));
}

TEST(MockReturnValue, StringReturnValue)
{
  mock().expectOneCall("foo").andReturnValue("hello world");
  cpputest::extensions::MockActualCall& actual_call = mock().actualCall("foo");

  STRCMP_EQUAL("hello world", actual_call.returnValue().getStringValue());
  STRCMP_EQUAL("hello world", actual_call.returnStringValue());
  STRCMP_EQUAL("hello world", mock().stringReturnValue());
}

TEST(MockReturnValue, DoubleReturnValue)
{
  double expected_return_value = 7.8;
  mock().expectOneCall("foo").andReturnValue(expected_return_value);

  cpputest::extensions::MockActualCall& actual_call = mock().actualCall("foo");
  DOUBLES_EQUAL(
      expected_return_value, actual_call.returnValue().getDoubleValue(), 0.05);
  DOUBLES_EQUAL(expected_return_value, actual_call.returnDoubleValue(), 0.05);
  DOUBLES_EQUAL(expected_return_value, mock().doubleReturnValue(), 0.05);
}

TEST(MockReturnValue,
    WhenAConstPointerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  const void* default_return_value = reinterpret_cast<void*>(0x7778);
  const void* expected_return_value = reinterpret_cast<void*>(0x144010);
  mock().expectOneCall("foo").andReturnValue(expected_return_value);
  POINTERS_EQUAL(expected_return_value,
      mock().actualCall("foo").returnConstPointerValueOrDefault(
          default_return_value));
  POINTERS_EQUAL(expected_return_value,
      mock().returnConstPointerValueOrDefault(default_return_value));
}

TEST(MockReturnValue,
    WhenNoConstPointerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  const void* default_return_value = reinterpret_cast<void*>(0x11);
  mock().expectOneCall("foo");
  POINTERS_EQUAL(default_return_value,
      mock().actualCall("foo").returnConstPointerValueOrDefault(
          default_return_value));
  POINTERS_EQUAL(default_return_value,
      mock().returnConstPointerValueOrDefault(default_return_value));
}

TEST(MockReturnValue,
    WhenAPointerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  void* default_return_value = reinterpret_cast<void*>(0x777);
  void* expected_return_value = reinterpret_cast<void*>(0x144000);
  mock().expectOneCall("foo").andReturnValue(expected_return_value);
  POINTERS_EQUAL(expected_return_value,
      mock().actualCall("foo").returnPointerValueOrDefault(
          default_return_value));
  POINTERS_EQUAL(expected_return_value,
      mock().returnPointerValueOrDefault(default_return_value));
}

TEST(MockReturnValue,
    WhenNoPointerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  void* default_return_value = reinterpret_cast<void*>(0x10);
  mock().expectOneCall("foo");
  POINTERS_EQUAL(default_return_value,
      mock().actualCall("foo").returnPointerValueOrDefault(
          default_return_value));
  POINTERS_EQUAL(default_return_value,
      mock().returnPointerValueOrDefault(default_return_value));
}

TEST(MockReturnValue,
    WhenAFunctionPointerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  auto default_return_value = reinterpret_cast<void (*)()>(0x777);
  auto expected_return_value = reinterpret_cast<void (*)()>(0x144000);
  mock().expectOneCall("foo").andReturnValue(expected_return_value);
  FUNCTIONPOINTERS_EQUAL(expected_return_value,
      mock().actualCall("foo").returnFunctionPointerValueOrDefault(
          default_return_value));
  FUNCTIONPOINTERS_EQUAL(expected_return_value,
      mock().returnFunctionPointerValueOrDefault(default_return_value));
}

TEST(MockReturnValue,
    WhenNoFunctionPointerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  auto default_return_value = reinterpret_cast<void (*)()>(0x10);
  mock().expectOneCall("foo");
  FUNCTIONPOINTERS_EQUAL(default_return_value,
      mock().actualCall("foo").returnFunctionPointerValueOrDefault(
          default_return_value));
  FUNCTIONPOINTERS_EQUAL(default_return_value,
      mock().returnFunctionPointerValueOrDefault(default_return_value));
}

TEST(MockReturnValue, PointerReturnValue)
{
  void* ptr = reinterpret_cast<void*>(0x00107);
  mock().expectOneCall("foo").andReturnValue(ptr);
  cpputest::extensions::MockActualCall& actual_call = mock().actualCall("foo");

  POINTERS_EQUAL(ptr, actual_call.returnValue().getPointerValue());
  POINTERS_EQUAL(ptr, actual_call.returnPointerValue());
  POINTERS_EQUAL(ptr, mock().pointerReturnValue());
}

TEST(MockReturnValue, ConstPointerReturnValue)
{
  const void* ptr = reinterpret_cast<const void*>(0x001074);
  mock().expectOneCall("foo").andReturnValue(ptr);
  cpputest::extensions::MockActualCall& actual_call = mock().actualCall("foo");
  POINTERS_EQUAL(ptr, actual_call.returnValue().getConstPointerValue());
  POINTERS_EQUAL(ptr, actual_call.returnConstPointerValue());
  POINTERS_EQUAL(ptr, mock().constPointerReturnValue());
}

TEST(MockReturnValue, FunctionPointerReturnValue)
{
  auto ptr = reinterpret_cast<void (*)()>(0x00107);
  mock().expectOneCall("foo").andReturnValue(ptr);
  cpputest::extensions::MockActualCall& actual_call = mock().actualCall("foo");

  FUNCTIONPOINTERS_EQUAL(
      ptr, actual_call.returnValue().getFunctionPointerValue());
  FUNCTIONPOINTERS_EQUAL(ptr, actual_call.returnFunctionPointerValue());
  FUNCTIONPOINTERS_EQUAL(ptr, mock().functionPointerReturnValue());
}

TEST(MockReturnValue,
    whenCallingDisabledOrIgnoredActualCallsThenTheyDontReturnPreviousCallsValues)
{
  mock().expectOneCall("boo").ignoreOtherParameters().andReturnValue(10);
  mock().ignoreOtherCalls();
  mock().actualCall("boo");
  mock().actualCall("An Ignored Call");

  CHECK(!mock().hasReturnValue());
}
