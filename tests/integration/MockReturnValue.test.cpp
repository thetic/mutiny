#include "mutiny/mock.hpp"
#include "mutiny/test.hpp"

#include <stdint.h>

using mu::tiny::mock::mock;

TEST_GROUP(MockReturnValue)
{
  void teardown() override
  {
    mock().check_expectations();
    mock().clear();
  }
};

TEST(MockReturnValue, hasReturnValue)
{
  CHECK(!mock().has_return_value());
  mock().expect_one_call("foo");
  CHECK(!mock().actual_call("foo").has_return_value());
  CHECK(!mock().has_return_value());

  mock().expect_one_call("foo2").and_return_value(1);
  CHECK(mock().actual_call("foo2").has_return_value());
  CHECK(mock().has_return_value());
}

TEST(MockReturnValue, UnsignedIntegerReturnValue)
{
  unsigned int expected_value = 7;

  mock().expect_one_call("foo").and_return_value(expected_value);

  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");

  CHECK_EQUAL(
      expected_value, actual_call.return_value().get_unsigned_int_value()
  );
  CHECK_EQUAL(expected_value, actual_call.return_value_as<unsigned int>());

  CHECK_EQUAL(expected_value, mock().return_value().get_unsigned_int_value());
  CHECK_EQUAL(expected_value, mock().unsigned_int_return_value());
}

TEST(MockReturnValue, PositiveIntReturnValueCanBeRetrievedAsUnsignedInt)
{
  int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      static_cast<unsigned int>(expected_value),
      mock().actual_call("foo").return_value().get_unsigned_int_value()
  );
}

TEST(MockReturnValue, IntReturnValueCanBeRetrievedAsLongInt)
{
  int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      expected_value,
      mock().actual_call("foo").return_value().get_long_int_value()
  );
}

TEST(MockReturnValue, UnsignedIntReturnValueCanBeRetrievedAsLongInt)
{
  unsigned int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      static_cast<long int>(expected_value),
      mock().actual_call("foo").return_value().get_long_int_value()
  );
}

TEST(MockReturnValue, PositiveIntReturnValueCanBeRetrievedAsUnsignedLongInt)
{
  int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      static_cast<unsigned long int>(expected_value),
      mock().actual_call("foo").return_value().get_unsigned_long_int_value()
  );
}

TEST(MockReturnValue, PositiveLongIntReturnValueCanBeRetrievedAsUnsignedLongInt)
{
  long int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      static_cast<unsigned long int>(expected_value),
      mock().actual_call("foo").return_value().get_unsigned_long_int_value()
  );
}

TEST(MockReturnValue, UnsignedIntReturnValueCanBeRetrievedAsUnsignedLongInt)
{
  unsigned int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      expected_value,
      mock().actual_call("foo").return_value().get_unsigned_long_int_value()
  );
}

TEST(MockReturnValue, PositiveIntReturnValueCanBeRetrievedAsUnsignedLongLongInt)
{
  int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      static_cast<unsigned long long int>(expected_value),
      mock()
          .actual_call("foo")
          .return_value()
          .get_unsigned_long_long_int_value()
  );
}

TEST(MockReturnValue, PositiveLongIntReturnValueCanBeRetrievedAsUnsignedLongLongInt)
{
  long int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      static_cast<unsigned long long int>(expected_value),
      mock()
          .actual_call("foo")
          .return_value()
          .get_unsigned_long_long_int_value()
  );
}

TEST(MockReturnValue, PositiveLongLongIntReturnValueCanBeRetrievedAsUnsignedLongLongInt)
{
  long long int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      static_cast<unsigned long long int>(expected_value),
      mock()
          .actual_call("foo")
          .return_value()
          .get_unsigned_long_long_int_value()
  );
}

TEST(MockReturnValue, UnsignedIntReturnValueCanBeRetrievedAsUnsignedLongLongInt)
{
  unsigned int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      expected_value,
      mock()
          .actual_call("foo")
          .return_value()
          .get_unsigned_long_long_int_value()
  );
}

TEST(MockReturnValue, UnsignedLongIntReturnValueCanBeRetrievedAsUnsignedLongLongInt)
{
  unsigned long int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      expected_value,
      mock()
          .actual_call("foo")
          .return_value()
          .get_unsigned_long_long_int_value()
  );
}

TEST(MockReturnValue, UnsignedLongLongIntReturnValueCanBeRetrieved)
{
  unsigned long long int expected_value = 2ULL;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      expected_value,
      mock()
          .actual_call("foo")
          .return_value()
          .get_unsigned_long_long_int_value()
  );
}

TEST(MockReturnValue, PositiveIntReturnValueCanBeRetrievedAsLongLongInt)
{
  int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      expected_value,
      mock().actual_call("foo").return_value().get_long_long_int_value()
  );
}

TEST(MockReturnValue, PositiveLongIntReturnValueCanBeRetrievedAsLongLongInt)
{
  long int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      expected_value,
      mock().actual_call("foo").return_value().get_long_long_int_value()
  );
}

TEST(MockReturnValue, UnsignedIntReturnValueCanBeRetrievedAsLongLongInt)
{
  unsigned int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      static_cast<long long int>(expected_value),
      mock().actual_call("foo").return_value().get_long_long_int_value()
  );
}

TEST(MockReturnValue, UnsignedLongIntReturnValueCanBeRetrievedAsLongLongInt)
{
  unsigned long int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      static_cast<long long int>(expected_value),
      mock().actual_call("foo").return_value().get_long_long_int_value()
  );
}

TEST(MockReturnValue, LongLongIntReturnValueCanBeRetrieved)
{
  long long int expected_value = 2LL;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      expected_value,
      mock().actual_call("foo").return_value().get_long_long_int_value()
  );
}

TEST(MockReturnValue, UnsignedIntegerReturnValueSetsDifferentValues)
{
  unsigned int expected_value = 1;
  unsigned int another_expected_value = 2;

  mock().expect_one_call("foo").and_return_value(expected_value);
  mock().expect_one_call("foo").and_return_value(another_expected_value);

  CHECK_EQUAL(
      expected_value,
      mock().actual_call("foo").return_value().get_unsigned_int_value()
  );
  CHECK_EQUAL(expected_value, mock().return_value().get_unsigned_int_value());
  CHECK_EQUAL(
      another_expected_value,
      mock().actual_call("foo").return_value().get_unsigned_int_value()
  );
  CHECK_EQUAL(
      another_expected_value, mock().return_value().get_unsigned_int_value()
  );
}

TEST(MockReturnValue, UnsignedIntegerReturnValueSetsDifferentValuesWhileParametersAreIgnored)
{
  unsigned int ret_value = 1;
  unsigned int another_ret_value = 2;

  mock()
      .expect_one_call("foo")
      .with_parameter("p1", 1)
      .ignore_other_parameters()
      .and_return_value(ret_value);
  mock()
      .expect_one_call("foo")
      .with_parameter("p1", 1)
      .ignore_other_parameters()
      .and_return_value(another_ret_value);

  CHECK_EQUAL(
      ret_value,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_unsigned_int_value()
  );
  CHECK_EQUAL(ret_value, mock().return_value().get_unsigned_int_value());
  CHECK_EQUAL(
      another_ret_value,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_unsigned_int_value()
  );
  CHECK_EQUAL(
      another_ret_value, mock().return_value().get_unsigned_int_value()
  );
}

TEST(MockReturnValue, WhenADoubleReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  double default_return_value = 10.7;
  double expected_return_value = default_return_value + 1.3;
  mock().expect_one_call("foo").and_return_value(expected_return_value);

  CHECK_APPROX(
      expected_return_value,
      mock().actual_call("foo").return_double_value_or_default(
          default_return_value
      ),
      0.05
  );
  CHECK_APPROX(
      expected_return_value,
      mock().return_double_value_or_default(default_return_value),
      0.05
  );
}

TEST(MockReturnValue, WhenNoDoubleReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  double default_return_value = 7.7;
  mock().expect_one_call("foo");
  CHECK_APPROX(
      default_return_value,
      mock().actual_call("foo").return_double_value_or_default(
          default_return_value
      ),
      0.05
  );
  CHECK_APPROX(
      default_return_value,
      mock().return_double_value_or_default(default_return_value),
      0.05
  );
}

TEST(MockReturnValue, WhenAUnsignedIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  unsigned int default_return_value = 10;
  unsigned int expected_return_value = default_return_value + 1;
  mock().expect_one_call("foo").and_return_value(expected_return_value);
  CHECK_EQUAL(
      expected_return_value,
      mock().actual_call("foo").return_value_as_or_default(default_return_value)
  );
  CHECK_EQUAL(
      expected_return_value,
      mock().return_unsigned_int_value_or_default(default_return_value)
  );
}

TEST(MockReturnValue, WhenNoUnsignedIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  unsigned int default_return_value = 10;
  mock().expect_one_call("foo");
  CHECK_EQUAL(
      default_return_value,
      mock().actual_call("foo").return_value_as_or_default(default_return_value)
  );
  CHECK_EQUAL(
      default_return_value,
      mock().return_unsigned_int_value_or_default(default_return_value)
  );
}

TEST(MockReturnValue, WhenAUnsignedLongIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  unsigned long int default_return_value = 121487;
  unsigned long int expected_return_value = default_return_value + 1;
  mock().expect_one_call("foo").and_return_value(expected_return_value);
  CHECK_EQUAL(
      expected_return_value,
      mock().actual_call("foo").return_value_as_or_default(default_return_value)
  );
  CHECK_EQUAL(
      expected_return_value,
      mock().return_unsigned_long_int_value_or_default(default_return_value)
  );
}

TEST(MockReturnValue, WhenNoUnsignedLongIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  unsigned long int default_return_value = 7710144;
  mock().expect_one_call("foo");
  CHECK_EQUAL(
      default_return_value,
      mock().actual_call("foo").return_value_as_or_default(default_return_value)
  );
  CHECK_EQUAL(
      default_return_value,
      mock().return_unsigned_long_int_value_or_default(default_return_value)
  );
}

TEST(MockReturnValue, WhenALongIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  long int default_return_value = 748797;
  long int expected_return_value = default_return_value + 1;
  mock().expect_one_call("foo").and_return_value(expected_return_value);
  CHECK_EQUAL(
      expected_return_value,
      mock().actual_call("foo").return_value_as_or_default(default_return_value)
  );
  CHECK_EQUAL(
      expected_return_value,
      mock().return_long_int_value_or_default(default_return_value)
  );
}

TEST(MockReturnValue, WhenNoLongIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  long int default_return_value = 123659;
  mock().expect_one_call("foo");
  CHECK_EQUAL(
      default_return_value,
      mock().actual_call("foo").return_value_as_or_default(default_return_value)
  );
  CHECK_EQUAL(
      default_return_value,
      mock().return_long_int_value_or_default(default_return_value)
  );
}

TEST(MockReturnValue, WhenAUnsignedLongLongIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  unsigned long long int default_return_value = 2ULL;
  unsigned long long int expected_return_value = default_return_value - 1;
  mock().expect_one_call("foo").and_return_value(expected_return_value);
  CHECK_EQUAL(
      expected_return_value,
      mock().actual_call("foo").return_unsigned_long_long_int_value_or_default(
          default_return_value
      )
  );
  CHECK_EQUAL(
      expected_return_value,
      mock().return_unsigned_long_long_int_value_or_default(
          default_return_value
      )
  );
}

TEST(MockReturnValue, WhenNoUnsignedLongLongIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  unsigned long long int default_return_value = 2ULL;
  mock().expect_one_call("foo");
  CHECK_EQUAL(
      default_return_value,
      mock().actual_call("foo").return_unsigned_long_long_int_value_or_default(
          default_return_value
      )
  );
  CHECK_EQUAL(
      default_return_value,
      mock().return_unsigned_long_long_int_value_or_default(
          default_return_value
      )
  );
}

TEST(MockReturnValue, WhenALongLongIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  long long int default_return_value = 2LL;
  long long int expected_return_value = default_return_value - 1;
  mock().expect_one_call("foo").and_return_value(expected_return_value);
  CHECK_EQUAL(
      expected_return_value,
      mock().actual_call("foo").return_long_long_int_value_or_default(
          default_return_value
      )
  );
  CHECK_EQUAL(
      expected_return_value,
      mock().return_long_long_int_value_or_default(default_return_value)
  );
}

TEST(MockReturnValue, WhenNoLongLongIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  long long int default_return_value = 2LL;
  mock().expect_one_call("foo");
  CHECK_EQUAL(
      default_return_value,
      mock().actual_call("foo").return_long_long_int_value_or_default(
          default_return_value
      )
  );
  CHECK_EQUAL(
      default_return_value,
      mock().return_long_long_int_value_or_default(default_return_value)
  );
}

TEST(MockReturnValue, WhenABooleanReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  bool default_return_value = true;
  bool expected_return_value = false;
  mock().expect_one_call("foo").and_return_value(expected_return_value);
  CHECK_EQUAL(
      expected_return_value,
      mock().actual_call("foo").return_bool_value_or_default(
          default_return_value
      )
  );
  CHECK_EQUAL(
      expected_return_value,
      mock().return_bool_value_or_default(default_return_value)
  );
}

TEST(MockReturnValue, WhenNoBooleanReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  bool default_return_value = true;
  mock().expect_one_call("foo");
  CHECK_EQUAL(
      default_return_value,
      mock().actual_call("foo").return_bool_value_or_default(
          default_return_value
      )
  );
  CHECK_EQUAL(
      default_return_value,
      mock().return_bool_value_or_default(default_return_value)
  );
}

TEST(MockReturnValue, WhenAIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  int default_return_value = 777;
  int expected_return_value = default_return_value + 1;
  mock().expect_one_call("foo").and_return_value(expected_return_value);
  CHECK_EQUAL(
      expected_return_value,
      mock().actual_call("foo").return_value_as_or_default(default_return_value)
  );
  CHECK_EQUAL(
      expected_return_value,
      mock().return_int_value_or_default(default_return_value)
  );
}

TEST(MockReturnValue, WhenNoIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  int default_return_value = 777;
  mock().expect_one_call("foo");
  CHECK_EQUAL(
      default_return_value,
      mock().actual_call("foo").return_value_as_or_default(default_return_value)
  );
  CHECK_EQUAL(
      default_return_value,
      mock().return_int_value_or_default(default_return_value)
  );
}

TEST(MockReturnValue, BooleanReturnValue)
{
  bool expected_value = true;
  mock().expect_one_call("foo").and_return_value(true);
  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");

  CHECK_EQUAL(expected_value, actual_call.return_value().get_bool_value());
  CHECK_EQUAL(expected_value, actual_call.return_bool_value());

  CHECK_EQUAL(expected_value, mock().return_value().get_bool_value());
  CHECK_EQUAL(expected_value, mock().bool_return_value());
}

TEST(MockReturnValue, BooleanReturnValueSetsDifferentValues)
{
  bool expected_value = true;
  bool another_expected_value = false;

  mock().expect_one_call("foo").and_return_value(expected_value);
  mock().expect_one_call("foo").and_return_value(another_expected_value);

  CHECK_EQUAL(
      expected_value, mock().actual_call("foo").return_value().get_bool_value()
  );
  CHECK_EQUAL(expected_value, mock().return_value().get_bool_value());
  CHECK_EQUAL(
      another_expected_value,
      mock().actual_call("foo").return_value().get_bool_value()
  );
  CHECK_EQUAL(another_expected_value, mock().return_value().get_bool_value());
}

TEST(MockReturnValue, BooleanReturnValueSetsDifferentValuesWhileParametersAreIgnored)
{
  bool ret_value = true;
  bool another_ret_value = false;

  mock()
      .expect_one_call("foo")
      .with_parameter("p1", 1)
      .ignore_other_parameters()
      .and_return_value(ret_value);
  mock()
      .expect_one_call("foo")
      .with_parameter("p1", 1)
      .ignore_other_parameters()
      .and_return_value(another_ret_value);

  CHECK_EQUAL(
      ret_value,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_bool_value()
  );
  CHECK_EQUAL(ret_value, mock().return_value().get_bool_value());
  CHECK_EQUAL(
      another_ret_value,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_bool_value()
  );
  CHECK_EQUAL(another_ret_value, mock().return_value().get_bool_value());
}

TEST(MockReturnValue, IntegerReturnValue)
{
  int expected_value = 1;
  mock().expect_one_call("foo").and_return_value(1);
  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");

  CHECK_EQUAL(expected_value, actual_call.return_value().get_int_value());
  CHECK_EQUAL(expected_value, actual_call.return_value_as<int>());

  CHECK_EQUAL(expected_value, mock().return_value().get_int_value());
  CHECK_EQUAL(expected_value, mock().int_return_value());
}

TEST(MockReturnValue, IntegerReturnValueSetsDifferentValues)
{
  int expected_value = 1;
  int another_expected_value = -1;

  mock().expect_one_call("foo").and_return_value(expected_value);
  mock().expect_one_call("foo").and_return_value(another_expected_value);

  CHECK_EQUAL(
      expected_value, mock().actual_call("foo").return_value().get_int_value()
  );
  CHECK_EQUAL(expected_value, mock().return_value().get_int_value());
  CHECK_EQUAL(
      another_expected_value,
      mock().actual_call("foo").return_value().get_int_value()
  );
  CHECK_EQUAL(another_expected_value, mock().return_value().get_int_value());
}

TEST(MockReturnValue, IntegerReturnValueSetsDifferentValuesWhileParametersAreIgnored)
{
  int ret_value = 1;
  int another_ret_value = -1;

  mock()
      .expect_one_call("foo")
      .with_parameter("p1", 1)
      .ignore_other_parameters()
      .and_return_value(ret_value);
  mock()
      .expect_one_call("foo")
      .with_parameter("p1", 1)
      .ignore_other_parameters()
      .and_return_value(another_ret_value);

  CHECK_EQUAL(
      ret_value,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_int_value()
  );
  CHECK_EQUAL(ret_value, mock().return_value().get_int_value());
  CHECK_EQUAL(
      another_ret_value,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_int_value()
  );
  CHECK_EQUAL(another_ret_value, mock().return_value().get_int_value());
}

TEST(MockReturnValue, LongIntegerReturnValue)
{
  long int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);

  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");
  CHECK_EQUAL(expected_value, actual_call.return_value().get_long_int_value());
  CHECK_EQUAL(expected_value, actual_call.return_value_as<long int>());
  CHECK_EQUAL(expected_value, mock().return_value().get_long_int_value());
  CHECK_EQUAL(expected_value, mock().long_int_return_value());
}

TEST(MockReturnValue, LongIntegerReturnValueSetsDifferentValues)
{
  long int expected_value = 1;
  long int another_expected_value = 2;

  mock().expect_one_call("foo").and_return_value(expected_value);
  mock().expect_one_call("foo").and_return_value(another_expected_value);

  CHECK_EQUAL(
      expected_value,
      mock().actual_call("foo").return_value().get_long_int_value()
  );
  CHECK_EQUAL(expected_value, mock().return_value().get_long_int_value());
  CHECK_EQUAL(
      another_expected_value,
      mock().actual_call("foo").return_value().get_long_int_value()
  );
  CHECK_EQUAL(
      another_expected_value, mock().return_value().get_long_int_value()
  );
}

TEST(MockReturnValue, LongIntegerReturnValueSetsDifferentValuesWhileParametersAreIgnored)
{
  long int ret_value = 1;
  long int another_ret_value = 2;

  mock()
      .expect_one_call("foo")
      .with_parameter("p1", 1)
      .ignore_other_parameters()
      .and_return_value(ret_value);
  mock()
      .expect_one_call("foo")
      .with_parameter("p1", 1)
      .ignore_other_parameters()
      .and_return_value(another_ret_value);

  CHECK_EQUAL(
      ret_value,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_long_int_value()
  );
  CHECK_EQUAL(ret_value, mock().return_value().get_long_int_value());
  CHECK_EQUAL(
      another_ret_value,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_long_int_value()
  );
  CHECK_EQUAL(another_ret_value, mock().return_value().get_long_int_value());
}

TEST(MockReturnValue, UnsignedLongIntegerReturnValue)
{
  unsigned long int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);

  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");
  CHECK_EQUAL(
      expected_value, actual_call.return_value().get_unsigned_long_int_value()
  );
  CHECK_EQUAL(expected_value, actual_call.return_value_as<unsigned long int>());
  CHECK_EQUAL(
      expected_value, mock().return_value().get_unsigned_long_int_value()
  );
  CHECK_EQUAL(expected_value, mock().unsigned_long_int_return_value());
}

TEST(MockReturnValue, UnsignedLongIntegerReturnValueSetsDifferentValues)
{
  unsigned long int expected_value = 1;
  unsigned long int another_expected_value = 2;

  mock().expect_one_call("foo").and_return_value(expected_value);
  mock().expect_one_call("foo").and_return_value(another_expected_value);

  CHECK_EQUAL(
      expected_value,
      mock().actual_call("foo").return_value().get_unsigned_long_int_value()
  );
  CHECK_EQUAL(
      expected_value, mock().return_value().get_unsigned_long_int_value()
  );
  CHECK_EQUAL(
      another_expected_value,
      mock().actual_call("foo").return_value().get_unsigned_long_int_value()
  );
  CHECK_EQUAL(
      another_expected_value,
      mock().return_value().get_unsigned_long_int_value()
  );
}

TEST(MockReturnValue, UnsignedLongIntegerReturnValueSetsDifferentValuesWhileParametersAreIgnored)
{
  unsigned long int ret_value = 1;
  unsigned long int another_ret_value = 2;

  mock()
      .expect_one_call("foo")
      .with_parameter("p1", 1)
      .ignore_other_parameters()
      .and_return_value(ret_value);
  mock()
      .expect_one_call("foo")
      .with_parameter("p1", 1)
      .ignore_other_parameters()
      .and_return_value(another_ret_value);

  CHECK_EQUAL(
      ret_value,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_unsigned_long_int_value()
  );
  CHECK_EQUAL(ret_value, mock().return_value().get_unsigned_long_int_value());
  CHECK_EQUAL(
      another_ret_value,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_unsigned_long_int_value()
  );
  CHECK_EQUAL(
      another_ret_value, mock().return_value().get_unsigned_long_int_value()
  );
}

TEST(MockReturnValue, LongLongIntegerReturnValue)
{
  long long int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);

  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");
  CHECK_EQUAL(
      expected_value, actual_call.return_value().get_long_long_int_value()
  );
  CHECK_EQUAL(expected_value, actual_call.return_long_long_int_value());
  CHECK_EQUAL(expected_value, mock().return_value().get_long_long_int_value());
  CHECK_EQUAL(expected_value, mock().long_long_int_return_value());
}

TEST(MockReturnValue, LongLongIntegerReturnValueSetsDifferentValues)
{
  long long int expected_value = 1;
  long long int another_expected_value = 2;

  mock().expect_one_call("foo").and_return_value(expected_value);
  mock().expect_one_call("foo").and_return_value(another_expected_value);

  CHECK_EQUAL(
      expected_value,
      mock().actual_call("foo").return_value().get_long_long_int_value()
  );
  CHECK_EQUAL(expected_value, mock().return_value().get_long_long_int_value());
  CHECK_EQUAL(
      another_expected_value,
      mock().actual_call("foo").return_value().get_long_long_int_value()
  );
  CHECK_EQUAL(
      another_expected_value, mock().return_value().get_long_long_int_value()
  );
}

TEST(MockReturnValue, LongLongIntegerReturnValueSetsDifferentValuesWhileParametersAreIgnored)
{
  long long int ret_value = 1;
  long long int another_ret_value = 2;

  mock()
      .expect_one_call("foo")
      .with_parameter("p1", 1)
      .ignore_other_parameters()
      .and_return_value(ret_value);
  mock()
      .expect_one_call("foo")
      .with_parameter("p1", 1)
      .ignore_other_parameters()
      .and_return_value(another_ret_value);

  CHECK_EQUAL(
      ret_value,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_long_long_int_value()
  );
  CHECK_EQUAL(ret_value, mock().return_value().get_long_long_int_value());
  CHECK_EQUAL(
      another_ret_value,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_long_long_int_value()
  );
  CHECK_EQUAL(
      another_ret_value, mock().return_value().get_long_long_int_value()
  );
}

TEST(MockReturnValue, UnsignedLongLongIntegerReturnValue)
{
  unsigned long long int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);

  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");
  CHECK_EQUAL(
      expected_value,
      actual_call.return_value().get_unsigned_long_long_int_value()
  );
  CHECK_EQUAL(
      expected_value, actual_call.return_unsigned_long_long_int_value()
  );
  CHECK_EQUAL(
      expected_value, mock().return_value().get_unsigned_long_long_int_value()
  );
  CHECK_EQUAL(expected_value, mock().unsigned_long_long_int_return_value());
}

TEST(MockReturnValue, UnsignedLongLongIntegerReturnValueSetsDifferentValues)
{
  unsigned long long int expected_value = 1;
  unsigned long long int another_expected_value = 2;

  mock().expect_one_call("foo").and_return_value(expected_value);
  mock().expect_one_call("foo").and_return_value(another_expected_value);

  CHECK_EQUAL(
      expected_value,
      mock()
          .actual_call("foo")
          .return_value()
          .get_unsigned_long_long_int_value()
  );
  CHECK_EQUAL(
      expected_value, mock().return_value().get_unsigned_long_long_int_value()
  );
  CHECK_EQUAL(
      another_expected_value,
      mock()
          .actual_call("foo")
          .return_value()
          .get_unsigned_long_long_int_value()
  );
  CHECK_EQUAL(
      another_expected_value,
      mock().return_value().get_unsigned_long_long_int_value()
  );
}

TEST(MockReturnValue, UnsignedLongLongIntegerReturnValueSetsDifferentValuesWhileParametersAreIgnored)
{
  unsigned long long int ret_value = 1;
  unsigned long long int another_ret_value = 2;

  mock()
      .expect_one_call("foo")
      .with_parameter("p1", 1)
      .ignore_other_parameters()
      .and_return_value(ret_value);
  mock()
      .expect_one_call("foo")
      .with_parameter("p1", 1)
      .ignore_other_parameters()
      .and_return_value(another_ret_value);

  CHECK_EQUAL(
      ret_value,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_unsigned_long_long_int_value()
  );
  CHECK_EQUAL(
      ret_value, mock().return_value().get_unsigned_long_long_int_value()
  );
  CHECK_EQUAL(
      another_ret_value,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_unsigned_long_long_int_value()
  );
  CHECK_EQUAL(
      another_ret_value,
      mock().return_value().get_unsigned_long_long_int_value()
  );
}

TEST(MockReturnValue, MatchingReturnValueOnWhileSignature)
{
  mock().expect_one_call("foo").with_parameter("p1", 1).and_return_value(1);
  mock().expect_one_call("foo").with_parameter("p1", 2).and_return_value(2);
  mock().expect_one_call("foo").with_parameter("p1", 3).and_return_value(3);
  mock().expect_one_call("foo").ignore_other_parameters().and_return_value(4);

  CHECK_EQUAL(
      3,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 3)
          .return_value()
          .get_int_value()
  );
  CHECK_EQUAL(
      4,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 4)
          .return_value()
          .get_int_value()
  );
  CHECK_EQUAL(
      1,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_int_value()
  );
  CHECK_EQUAL(
      2,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 2)
          .return_value()
          .get_int_value()
  );
}

TEST(MockReturnValue, WhenAStringReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  const char* default_return_value = "default";
  const char* expected_return_value = "expected";
  mock().expect_one_call("foo").and_return_value(expected_return_value);
  STRCMP_EQUAL(
      expected_return_value,
      mock().actual_call("foo").return_string_value_or_default(
          default_return_value
      )
  );
  STRCMP_EQUAL(
      expected_return_value,
      mock().return_string_value_or_default(default_return_value)
  );
}

TEST(MockReturnValue, WhenNoStringReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  const char* default_return_value = "default";
  mock().expect_one_call("foo");
  STRCMP_EQUAL(
      default_return_value,
      mock().actual_call("foo").return_string_value_or_default(
          default_return_value
      )
  );
  STRCMP_EQUAL(
      default_return_value,
      mock().return_string_value_or_default(default_return_value)
  );
}

TEST(MockReturnValue, StringReturnValue)
{
  mock().expect_one_call("foo").and_return_value("hello world");
  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");

  STRCMP_EQUAL("hello world", actual_call.return_value().get_string_value());
  STRCMP_EQUAL("hello world", actual_call.return_string_value());
  STRCMP_EQUAL("hello world", mock().string_return_value());
}

TEST(MockReturnValue, DoubleReturnValue)
{
  double expected_return_value = 7.8;
  mock().expect_one_call("foo").and_return_value(expected_return_value);

  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");
  CHECK_APPROX(
      expected_return_value, actual_call.return_value().get_double_value(), 0.05
  );
  CHECK_APPROX(expected_return_value, actual_call.return_double_value(), 0.05);
  CHECK_APPROX(expected_return_value, mock().double_return_value(), 0.05);
}

TEST(MockReturnValue, WhenAConstPointerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  const void* default_return_value = reinterpret_cast<void*>(0x7778);
  const void* expected_return_value = reinterpret_cast<void*>(0x144010);
  mock().expect_one_call("foo").and_return_value(expected_return_value);
  CHECK_EQUAL(
      expected_return_value,
      mock().actual_call("foo").return_const_pointer_value_or_default(
          default_return_value
      )
  );
  CHECK_EQUAL(
      expected_return_value,
      mock().return_const_pointer_value_or_default(default_return_value)
  );
}

TEST(MockReturnValue, WhenNoConstPointerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  const void* default_return_value = reinterpret_cast<void*>(0x11);
  mock().expect_one_call("foo");
  CHECK_EQUAL(
      default_return_value,
      mock().actual_call("foo").return_const_pointer_value_or_default(
          default_return_value
      )
  );
  CHECK_EQUAL(
      default_return_value,
      mock().return_const_pointer_value_or_default(default_return_value)
  );
}

TEST(MockReturnValue, WhenAPointerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  void* default_return_value = reinterpret_cast<void*>(0x777);
  void* expected_return_value = reinterpret_cast<void*>(0x144000);
  mock().expect_one_call("foo").and_return_value(expected_return_value);
  CHECK_EQUAL(
      expected_return_value,
      mock().actual_call("foo").return_pointer_value_or_default(
          default_return_value
      )
  );
  CHECK_EQUAL(
      expected_return_value,
      mock().return_pointer_value_or_default(default_return_value)
  );
}

TEST(MockReturnValue, WhenNoPointerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  void* default_return_value = reinterpret_cast<void*>(0x10);
  mock().expect_one_call("foo");
  CHECK_EQUAL(
      default_return_value,
      mock().actual_call("foo").return_pointer_value_or_default(
          default_return_value
      )
  );
  CHECK_EQUAL(
      default_return_value,
      mock().return_pointer_value_or_default(default_return_value)
  );
}

TEST(MockReturnValue, WhenAFunctionPointerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  auto default_return_value = reinterpret_cast<void (*)()>(0x777);
  auto expected_return_value = reinterpret_cast<void (*)()>(0x144000);
  mock().expect_one_call("foo").and_return_value(expected_return_value);
  CHECK_EQUAL(
      expected_return_value,
      mock().actual_call("foo").return_function_pointer_value_or_default(
          default_return_value
      )
  );
  CHECK_EQUAL(
      expected_return_value,
      mock().return_function_pointer_value_or_default(default_return_value)
  );
}

TEST(MockReturnValue, WhenNoFunctionPointerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  auto default_return_value = reinterpret_cast<void (*)()>(0x10);
  mock().expect_one_call("foo");
  CHECK_EQUAL(
      default_return_value,
      mock().actual_call("foo").return_function_pointer_value_or_default(
          default_return_value
      )
  );
  CHECK_EQUAL(
      default_return_value,
      mock().return_function_pointer_value_or_default(default_return_value)
  );
}

TEST(MockReturnValue, PointerReturnValue)
{
  void* ptr = reinterpret_cast<void*>(0x00107);
  mock().expect_one_call("foo").and_return_value(ptr);
  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");

  CHECK_EQUAL(ptr, actual_call.return_value().get_pointer_value());
  CHECK_EQUAL(ptr, actual_call.return_pointer_value());
  CHECK_EQUAL(ptr, mock().pointer_return_value());
}

TEST(MockReturnValue, ConstPointerReturnValue)
{
  const void* ptr = reinterpret_cast<const void*>(0x001074);
  mock().expect_one_call("foo").and_return_value(ptr);
  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");
  CHECK_EQUAL(ptr, actual_call.return_value().get_const_pointer_value());
  CHECK_EQUAL(ptr, actual_call.return_const_pointer_value());
  CHECK_EQUAL(ptr, mock().const_pointer_return_value());
}

TEST(MockReturnValue, FunctionPointerReturnValue)
{
  auto ptr = reinterpret_cast<void (*)()>(0x00107);
  mock().expect_one_call("foo").and_return_value(ptr);
  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");

  CHECK_EQUAL(ptr, actual_call.return_value().get_function_pointer_value());
  CHECK_EQUAL(ptr, actual_call.return_function_pointer_value());
  CHECK_EQUAL(ptr, mock().function_pointer_return_value());
}

TEST(MockReturnValue, whenCallingDisabledOrIgnoredActualCallsThenTheyDontReturnPreviousCallsValues)
{
  mock().expect_one_call("boo").ignore_other_parameters().and_return_value(10);
  mock().ignore_other_calls();
  mock().actual_call("boo");
  mock().actual_call("An Ignored Call");

  CHECK(!mock().has_return_value());
}

// --- return_value_as<T>() portability tests ---
// Fixed-width types (uint8_t..uint64_t, size_t, ptrdiff_t, etc.) are typedefs
// for platform-specific fundamental types.  On LP64 (Linux 64-bit) uint64_t
// is unsigned long; on LLP64 (Windows 64-bit) it is unsigned long long.
// return_value_as<T>() dispatches to the widest compatible getter so callers
// need not know which fundamental type the platform chose.

TEST(MockReturnValue, ReturnValueAsUnsignedLongLongFromUnsignedLongStorage)
{
  // Simulates LP64: and_return_value(unsigned long) stores "unsigned long int".
  // return_value_as<unsigned long long>() must still retrieve the value.
  mock().expect_one_call("foo").and_return_value(42UL);
  CHECK_EQUAL(
      42ULL, mock().actual_call("foo").return_value_as<unsigned long long>()
  );
}

TEST(MockReturnValue, ReturnValueAsUnsignedLongLongFromUnsignedLongLongStorage)
{
  // Simulates LLP64: and_return_value(unsigned long long) stores
  // "unsigned long long int". return_value_as<unsigned long long>() retrieves
  // it.
  mock().expect_one_call("foo").and_return_value(42ULL);
  CHECK_EQUAL(
      42ULL, mock().actual_call("foo").return_value_as<unsigned long long>()
  );
}

TEST(MockReturnValue, ReturnValueAsUint64)
{
  // Portable use of uint64_t as a return type regardless of whether the
  // platform defines it as unsigned long or unsigned long long.
  mock().expect_one_call("foo").and_return_value(UINT64_C(0xDEADBEEF));
  CHECK_EQUAL(
      UINT64_C(0xDEADBEEF),
      mock().actual_call("foo").return_value_as<uint64_t>()
  );
}

TEST(MockReturnValue, ReturnValueAsSizeT)
{
  size_t expected = 1024;
  mock().expect_one_call("foo").and_return_value(expected);
  CHECK_EQUAL(expected, mock().actual_call("foo").return_value_as<size_t>());
}

TEST(MockReturnValue, ReturnValueAsInt64FromLongLongStorage)
{
  mock().expect_one_call("foo").and_return_value(INT64_C(-1));
  CHECK_EQUAL(
      INT64_C(-1), mock().actual_call("foo").return_value_as<int64_t>()
  );
}

TEST(MockReturnValue, ReturnValueAsOrDefaultReturnsDefaultWhenNoValueSet)
{
  mock().expect_one_call("foo");
  CHECK_EQUAL(
      UINT32_C(99),
      mock().actual_call("foo").return_value_as_or_default(UINT32_C(99))
  );
}

TEST(MockReturnValue, ReturnValueAsOrDefaultIgnoresDefaultWhenValueSet)
{
  mock().expect_one_call("foo").and_return_value(UINT32_C(7));
  CHECK_EQUAL(
      UINT32_C(7),
      mock().actual_call("foo").return_value_as_or_default(UINT32_C(99))
  );
}
