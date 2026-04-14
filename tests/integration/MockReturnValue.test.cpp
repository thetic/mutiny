#include "mutiny/mock.hpp"
#include "mutiny/test.hpp"

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
      expected_value, actual_call.return_value().get_value<unsigned int>()
  );
  CHECK_EQUAL(expected_value, actual_call.return_value_or_default<unsigned int>(0));

  CHECK_EQUAL(expected_value, mock().return_value().get_value<unsigned int>());
  CHECK_EQUAL(expected_value, mock().return_value<unsigned int>());
}

TEST(MockReturnValue, PositiveIntReturnValueCanBeRetrievedAsUnsignedInt)
{
  int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      static_cast<unsigned int>(expected_value),
      mock().actual_call("foo").return_value().get_value<unsigned int>()
  );
}

TEST(MockReturnValue, IntReturnValueCanBeRetrievedAsLongInt)
{
  int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      expected_value,
      mock().actual_call("foo").return_value().get_value<long int>()
  );
}

TEST(MockReturnValue, UnsignedIntReturnValueCanBeRetrievedAsLongInt)
{
  unsigned int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      static_cast<long int>(expected_value),
      mock().actual_call("foo").return_value().get_value<long int>()
  );
}

TEST(MockReturnValue, PositiveIntReturnValueCanBeRetrievedAsUnsignedLongInt)
{
  int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      static_cast<unsigned long int>(expected_value),
      mock().actual_call("foo").return_value().get_value<unsigned long int>()
  );
}

TEST(MockReturnValue, PositiveLongIntReturnValueCanBeRetrievedAsUnsignedLongInt)
{
  long int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      static_cast<unsigned long int>(expected_value),
      mock().actual_call("foo").return_value().get_value<unsigned long int>()
  );
}

TEST(MockReturnValue, UnsignedIntReturnValueCanBeRetrievedAsUnsignedLongInt)
{
  unsigned int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      expected_value,
      mock().actual_call("foo").return_value().get_value<unsigned long int>()
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
          .get_value<unsigned long long>()
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
          .get_value<unsigned long long>()
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
          .get_value<unsigned long long>()
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
          .get_value<unsigned long long>()
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
          .get_value<unsigned long long>()
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
          .get_value<unsigned long long>()
  );
}

TEST(MockReturnValue, PositiveIntReturnValueCanBeRetrievedAsLongLongInt)
{
  int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      expected_value,
      mock().actual_call("foo").return_value().get_value<long long>()
  );
}

TEST(MockReturnValue, PositiveLongIntReturnValueCanBeRetrievedAsLongLongInt)
{
  long int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      expected_value,
      mock().actual_call("foo").return_value().get_value<long long>()
  );
}

TEST(MockReturnValue, UnsignedIntReturnValueCanBeRetrievedAsLongLongInt)
{
  unsigned int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      static_cast<long long int>(expected_value),
      mock().actual_call("foo").return_value().get_value<long long>()
  );
}

TEST(MockReturnValue, UnsignedLongIntReturnValueCanBeRetrievedAsLongLongInt)
{
  unsigned long int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      static_cast<long long int>(expected_value),
      mock().actual_call("foo").return_value().get_value<long long>()
  );
}

TEST(MockReturnValue, LongLongIntReturnValueCanBeRetrieved)
{
  long long int expected_value = 2LL;
  mock().expect_one_call("foo").and_return_value(expected_value);
  CHECK_EQUAL(
      expected_value,
      mock().actual_call("foo").return_value().get_value<long long>()
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
      mock().actual_call("foo").return_value().get_value<unsigned int>()
  );
  CHECK_EQUAL(expected_value, mock().return_value().get_value<unsigned int>());
  CHECK_EQUAL(
      another_expected_value,
      mock().actual_call("foo").return_value().get_value<unsigned int>()
  );
  CHECK_EQUAL(
      another_expected_value, mock().return_value().get_value<unsigned int>()
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
          .get_value<unsigned int>()
  );
  CHECK_EQUAL(ret_value, mock().return_value().get_value<unsigned int>());
  CHECK_EQUAL(
      another_ret_value,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_value<unsigned int>()
  );
  CHECK_EQUAL(
      another_ret_value, mock().return_value().get_value<unsigned int>()
  );
}

TEST(MockReturnValue, WhenADoubleReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  double default_return_value = 10.7;
  double expected_return_value = default_return_value + 1.3;
  mock().expect_one_call("foo").and_return_value(expected_return_value);

  CHECK_APPROX(
      expected_return_value,
      mock().actual_call("foo").return_value_or_default<double>(
          default_return_value
      ),
      0.05
  );
  CHECK_APPROX(
      expected_return_value,
      mock().return_value_or_default<double>(default_return_value),
      0.05
  );
}

TEST(MockReturnValue, WhenNoDoubleReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  double default_return_value = 7.7;
  mock().expect_one_call("foo");
  CHECK_APPROX(
      default_return_value,
      mock().actual_call("foo").return_value_or_default<double>(
          default_return_value
      ),
      0.05
  );
  CHECK_APPROX(
      default_return_value,
      mock().return_value_or_default<double>(default_return_value),
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
      mock().actual_call("foo").return_value_or_default<unsigned int>(
          default_return_value
      )
  );
  CHECK_EQUAL(
      expected_return_value,
      mock().return_value_or_default<unsigned int>(default_return_value)
  );
}

TEST(MockReturnValue, WhenNoUnsignedIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  unsigned int default_return_value = 10;
  mock().expect_one_call("foo");
  CHECK_EQUAL(
      default_return_value,
      mock().actual_call("foo").return_value_or_default<unsigned int>(
          default_return_value
      )
  );
  CHECK_EQUAL(
      default_return_value,
      mock().return_value_or_default<unsigned int>(default_return_value)
  );
}

TEST(MockReturnValue, WhenAUnsignedLongIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  unsigned long int default_return_value = 121487;
  unsigned long int expected_return_value = default_return_value + 1;
  mock().expect_one_call("foo").and_return_value(expected_return_value);
  CHECK_EQUAL(
      expected_return_value,
      mock().actual_call("foo").return_value_or_default<unsigned long int>(
          default_return_value
      )
  );
  CHECK_EQUAL(
      expected_return_value,
      mock().return_value_or_default<unsigned long int>(default_return_value)
  );
}

TEST(MockReturnValue, WhenNoUnsignedLongIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  unsigned long int default_return_value = 7710144;
  mock().expect_one_call("foo");
  CHECK_EQUAL(
      default_return_value,
      mock().actual_call("foo").return_value_or_default<unsigned long int>(
          default_return_value
      )
  );
  CHECK_EQUAL(
      default_return_value,
      mock().return_value_or_default<unsigned long int>(default_return_value)
  );
}

TEST(MockReturnValue, WhenALongIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  long int default_return_value = 748797;
  long int expected_return_value = default_return_value + 1;
  mock().expect_one_call("foo").and_return_value(expected_return_value);
  CHECK_EQUAL(
      expected_return_value,
      mock().actual_call("foo").return_value_or_default<long int>(
          default_return_value
      )
  );
  CHECK_EQUAL(
      expected_return_value,
      mock().return_value_or_default<long int>(default_return_value)
  );
}

TEST(MockReturnValue, WhenNoLongIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  long int default_return_value = 123659;
  mock().expect_one_call("foo");
  CHECK_EQUAL(
      default_return_value,
      mock().actual_call("foo").return_value_or_default<long int>(
          default_return_value
      )
  );
  CHECK_EQUAL(
      default_return_value,
      mock().return_value_or_default<long int>(default_return_value)
  );
}

TEST(MockReturnValue, WhenAUnsignedLongLongIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  unsigned long long int default_return_value = 2ULL;
  unsigned long long int expected_return_value = default_return_value - 1;
  mock().expect_one_call("foo").and_return_value(expected_return_value);
  CHECK_EQUAL(
      expected_return_value,
      mock().actual_call("foo").return_value_or_default<unsigned long long>(
          default_return_value
      )
  );
  CHECK_EQUAL(
      expected_return_value,
      mock().return_value_or_default<unsigned long long>(
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
      mock().actual_call("foo").return_value_or_default<unsigned long long>(
          default_return_value
      )
  );
  CHECK_EQUAL(
      default_return_value,
      mock().return_value_or_default<unsigned long long>(
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
      mock().actual_call("foo").return_value_or_default<long long>(
          default_return_value
      )
  );
  CHECK_EQUAL(
      expected_return_value,
      mock().return_value_or_default<long long>(default_return_value)
  );
}

TEST(MockReturnValue, WhenNoLongLongIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  long long int default_return_value = 2LL;
  mock().expect_one_call("foo");
  CHECK_EQUAL(
      default_return_value,
      mock().actual_call("foo").return_value_or_default<long long>(
          default_return_value
      )
  );
  CHECK_EQUAL(
      default_return_value,
      mock().return_value_or_default<long long>(default_return_value)
  );
}

TEST(MockReturnValue, WhenABooleanReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  bool default_return_value = true;
  bool expected_return_value = false;
  mock().expect_one_call("foo").and_return_value(expected_return_value);
  CHECK_EQUAL(
      expected_return_value,
      mock().actual_call("foo").return_value_or_default<bool>(
          default_return_value
      )
  );
  CHECK_EQUAL(
      expected_return_value,
      mock().return_value_or_default<bool>(default_return_value)
  );
}

TEST(MockReturnValue, WhenNoBooleanReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  bool default_return_value = true;
  mock().expect_one_call("foo");
  CHECK_EQUAL(
      default_return_value,
      mock().actual_call("foo").return_value_or_default<bool>(
          default_return_value
      )
  );
  CHECK_EQUAL(
      default_return_value,
      mock().return_value_or_default<bool>(default_return_value)
  );
}

TEST(MockReturnValue, WhenAIntegerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  int default_return_value = 777;
  int expected_return_value = default_return_value + 1;
  mock().expect_one_call("foo").and_return_value(expected_return_value);
  CHECK_EQUAL(
      expected_return_value,
      mock().actual_call("foo").return_value_or_default<int>(
          default_return_value
      )
  );
  CHECK_EQUAL(
      expected_return_value,
      mock().return_value_or_default<int>(default_return_value)
  );
}

TEST(MockReturnValue, WhenNoIntegerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  int default_return_value = 777;
  mock().expect_one_call("foo");
  CHECK_EQUAL(
      default_return_value,
      mock().actual_call("foo").return_value_or_default<int>(
          default_return_value
      )
  );
  CHECK_EQUAL(
      default_return_value,
      mock().return_value_or_default<int>(default_return_value)
  );
}

TEST(MockReturnValue, BooleanReturnValue)
{
  bool expected_value = true;
  mock().expect_one_call("foo").and_return_value(true);
  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");

  CHECK_EQUAL(expected_value, actual_call.return_value().get_value<bool>());
  CHECK_EQUAL(expected_value, actual_call.return_value_or_default<bool>(false));

  CHECK_EQUAL(expected_value, mock().return_value().get_value<bool>());
  CHECK_EQUAL(expected_value, mock().return_value<bool>());
}

TEST(MockReturnValue, BooleanReturnValueSetsDifferentValues)
{
  bool expected_value = true;
  bool another_expected_value = false;

  mock().expect_one_call("foo").and_return_value(expected_value);
  mock().expect_one_call("foo").and_return_value(another_expected_value);

  CHECK_EQUAL(
      expected_value, mock().actual_call("foo").return_value().get_value<bool>()
  );
  CHECK_EQUAL(expected_value, mock().return_value().get_value<bool>());
  CHECK_EQUAL(
      another_expected_value,
      mock().actual_call("foo").return_value().get_value<bool>()
  );
  CHECK_EQUAL(another_expected_value, mock().return_value().get_value<bool>());
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
          .get_value<bool>()
  );
  CHECK_EQUAL(ret_value, mock().return_value().get_value<bool>());
  CHECK_EQUAL(
      another_ret_value,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_value<bool>()
  );
  CHECK_EQUAL(another_ret_value, mock().return_value().get_value<bool>());
}

TEST(MockReturnValue, IntegerReturnValue)
{
  int expected_value = 1;
  mock().expect_one_call("foo").and_return_value(1);
  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");

  CHECK_EQUAL(expected_value, actual_call.return_value().get_value<int>());
  CHECK_EQUAL(expected_value, actual_call.return_value_or_default<int>(0));

  CHECK_EQUAL(expected_value, mock().return_value().get_value<int>());
  CHECK_EQUAL(expected_value, mock().return_value<int>());
}

TEST(MockReturnValue, IntegerReturnValueSetsDifferentValues)
{
  int expected_value = 1;
  int another_expected_value = -1;

  mock().expect_one_call("foo").and_return_value(expected_value);
  mock().expect_one_call("foo").and_return_value(another_expected_value);

  CHECK_EQUAL(
      expected_value, mock().actual_call("foo").return_value().get_value<int>()
  );
  CHECK_EQUAL(expected_value, mock().return_value().get_value<int>());
  CHECK_EQUAL(
      another_expected_value,
      mock().actual_call("foo").return_value().get_value<int>()
  );
  CHECK_EQUAL(another_expected_value, mock().return_value().get_value<int>());
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
          .get_value<int>()
  );
  CHECK_EQUAL(ret_value, mock().return_value().get_value<int>());
  CHECK_EQUAL(
      another_ret_value,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_value<int>()
  );
  CHECK_EQUAL(another_ret_value, mock().return_value().get_value<int>());
}

TEST(MockReturnValue, LongIntegerReturnValue)
{
  long int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);

  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");
  CHECK_EQUAL(expected_value, actual_call.return_value().get_value<long int>());
  CHECK_EQUAL(expected_value, actual_call.return_value_or_default<long int>(0));
  CHECK_EQUAL(expected_value, mock().return_value().get_value<long int>());
  CHECK_EQUAL(expected_value, mock().return_value<long int>());
}

TEST(MockReturnValue, LongIntegerReturnValueSetsDifferentValues)
{
  long int expected_value = 1;
  long int another_expected_value = 2;

  mock().expect_one_call("foo").and_return_value(expected_value);
  mock().expect_one_call("foo").and_return_value(another_expected_value);

  CHECK_EQUAL(
      expected_value,
      mock().actual_call("foo").return_value().get_value<long int>()
  );
  CHECK_EQUAL(expected_value, mock().return_value().get_value<long int>());
  CHECK_EQUAL(
      another_expected_value,
      mock().actual_call("foo").return_value().get_value<long int>()
  );
  CHECK_EQUAL(
      another_expected_value, mock().return_value().get_value<long int>()
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
          .get_value<long int>()
  );
  CHECK_EQUAL(ret_value, mock().return_value().get_value<long int>());
  CHECK_EQUAL(
      another_ret_value,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_value<long int>()
  );
  CHECK_EQUAL(another_ret_value, mock().return_value().get_value<long int>());
}

TEST(MockReturnValue, UnsignedLongIntegerReturnValue)
{
  unsigned long int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);

  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");
  CHECK_EQUAL(
      expected_value, actual_call.return_value().get_value<unsigned long int>()
  );
  CHECK_EQUAL(expected_value, actual_call.return_value_or_default<unsigned long int>(0));
  CHECK_EQUAL(
      expected_value, mock().return_value().get_value<unsigned long int>()
  );
  CHECK_EQUAL(expected_value, mock().return_value<unsigned long int>());
}

TEST(MockReturnValue, UnsignedLongIntegerReturnValueSetsDifferentValues)
{
  unsigned long int expected_value = 1;
  unsigned long int another_expected_value = 2;

  mock().expect_one_call("foo").and_return_value(expected_value);
  mock().expect_one_call("foo").and_return_value(another_expected_value);

  CHECK_EQUAL(
      expected_value,
      mock().actual_call("foo").return_value().get_value<unsigned long int>()
  );
  CHECK_EQUAL(
      expected_value, mock().return_value().get_value<unsigned long int>()
  );
  CHECK_EQUAL(
      another_expected_value,
      mock().actual_call("foo").return_value().get_value<unsigned long int>()
  );
  CHECK_EQUAL(
      another_expected_value,
      mock().return_value().get_value<unsigned long int>()
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
          .get_value<unsigned long int>()
  );
  CHECK_EQUAL(ret_value, mock().return_value().get_value<unsigned long int>());
  CHECK_EQUAL(
      another_ret_value,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_value<unsigned long int>()
  );
  CHECK_EQUAL(
      another_ret_value, mock().return_value().get_value<unsigned long int>()
  );
}

TEST(MockReturnValue, LongLongIntegerReturnValue)
{
  long long int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);

  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");
  CHECK_EQUAL(
      expected_value, actual_call.return_value().get_value<long long>()
  );
  CHECK_EQUAL(expected_value, actual_call.return_value_or_default<long long>(0));
  CHECK_EQUAL(expected_value, mock().return_value().get_value<long long>());
  CHECK_EQUAL(expected_value, mock().return_value<long long>());
}

TEST(MockReturnValue, LongLongIntegerReturnValueSetsDifferentValues)
{
  long long int expected_value = 1;
  long long int another_expected_value = 2;

  mock().expect_one_call("foo").and_return_value(expected_value);
  mock().expect_one_call("foo").and_return_value(another_expected_value);

  CHECK_EQUAL(
      expected_value,
      mock().actual_call("foo").return_value().get_value<long long>()
  );
  CHECK_EQUAL(expected_value, mock().return_value().get_value<long long>());
  CHECK_EQUAL(
      another_expected_value,
      mock().actual_call("foo").return_value().get_value<long long>()
  );
  CHECK_EQUAL(
      another_expected_value, mock().return_value().get_value<long long>()
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
          .get_value<long long>()
  );
  CHECK_EQUAL(ret_value, mock().return_value().get_value<long long>());
  CHECK_EQUAL(
      another_ret_value,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_value<long long>()
  );
  CHECK_EQUAL(
      another_ret_value, mock().return_value().get_value<long long>()
  );
}

TEST(MockReturnValue, UnsignedLongLongIntegerReturnValue)
{
  unsigned long long int expected_value = 7;
  mock().expect_one_call("foo").and_return_value(expected_value);

  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");
  CHECK_EQUAL(
      expected_value,
      actual_call.return_value().get_value<unsigned long long>()
  );
  CHECK_EQUAL(
      expected_value, actual_call.return_value_or_default<unsigned long long>(0)
  );
  CHECK_EQUAL(
      expected_value, mock().return_value().get_value<unsigned long long>()
  );
  CHECK_EQUAL(expected_value, mock().return_value<unsigned long long>());
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
          .get_value<unsigned long long>()
  );
  CHECK_EQUAL(
      expected_value, mock().return_value().get_value<unsigned long long>()
  );
  CHECK_EQUAL(
      another_expected_value,
      mock()
          .actual_call("foo")
          .return_value()
          .get_value<unsigned long long>()
  );
  CHECK_EQUAL(
      another_expected_value,
      mock().return_value().get_value<unsigned long long>()
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
          .get_value<unsigned long long>()
  );
  CHECK_EQUAL(
      ret_value, mock().return_value().get_value<unsigned long long>()
  );
  CHECK_EQUAL(
      another_ret_value,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_value<unsigned long long>()
  );
  CHECK_EQUAL(
      another_ret_value,
      mock().return_value().get_value<unsigned long long>()
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
          .get_value<int>()
  );
  CHECK_EQUAL(
      4,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 4)
          .return_value()
          .get_value<int>()
  );
  CHECK_EQUAL(
      1,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 1)
          .return_value()
          .get_value<int>()
  );
  CHECK_EQUAL(
      2,
      mock()
          .actual_call("foo")
          .with_parameter("p1", 2)
          .return_value()
          .get_value<int>()
  );
}

TEST(MockReturnValue, WhenAStringReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  const char* default_return_value = "default";
  const char* expected_return_value = "expected";
  mock().expect_one_call("foo").and_return_value(expected_return_value);
  STRCMP_EQUAL(
      expected_return_value,
      mock().actual_call("foo").return_value_or_default<const char*>(
          default_return_value
      )
  );
  STRCMP_EQUAL(
      expected_return_value,
      mock().return_value_or_default<const char*>(default_return_value)
  );
}

TEST(MockReturnValue, WhenNoStringReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  const char* default_return_value = "default";
  mock().expect_one_call("foo");
  STRCMP_EQUAL(
      default_return_value,
      mock().actual_call("foo").return_value_or_default<const char*>(
          default_return_value
      )
  );
  STRCMP_EQUAL(
      default_return_value,
      mock().return_value_or_default<const char*>(default_return_value)
  );
}

TEST(MockReturnValue, StringReturnValue)
{
  mock().expect_one_call("foo").and_return_value("hello world");
  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");

  STRCMP_EQUAL("hello world", actual_call.return_value().get_value<const char*>());
  STRCMP_EQUAL("hello world", actual_call.return_value_or_default<const char*>(""));
  STRCMP_EQUAL("hello world", mock().return_value<const char*>());
}

TEST(MockReturnValue, DoubleReturnValue)
{
  double expected_return_value = 7.8;
  mock().expect_one_call("foo").and_return_value(expected_return_value);

  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");
  CHECK_APPROX(
      expected_return_value, actual_call.return_value().get_value<double>(), 0.05
  );
  CHECK_APPROX(expected_return_value, actual_call.return_value_or_default<double>(0.0), 0.05);
  CHECK_APPROX(expected_return_value, mock().return_value<double>(), 0.05);
}

TEST(MockReturnValue, WhenAConstPointerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  const void* default_return_value = reinterpret_cast<void*>(0x7778);
  const void* expected_return_value = reinterpret_cast<void*>(0x144010);
  mock().expect_one_call("foo").and_return_value(expected_return_value);
  CHECK_EQUAL(
      expected_return_value,
      mock().actual_call("foo").return_value_or_default<const void*>(
          default_return_value
      )
  );
  CHECK_EQUAL(
      expected_return_value,
      mock().return_value_or_default<const void*>(default_return_value)
  );
}

TEST(MockReturnValue, WhenNoConstPointerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  const void* default_return_value = reinterpret_cast<void*>(0x11);
  mock().expect_one_call("foo");
  CHECK_EQUAL(
      default_return_value,
      mock().actual_call("foo").return_value_or_default<const void*>(
          default_return_value
      )
  );
  CHECK_EQUAL(
      default_return_value,
      mock().return_value_or_default<const void*>(default_return_value)
  );
}

TEST(MockReturnValue, WhenAPointerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  void* default_return_value = reinterpret_cast<void*>(0x777);
  void* expected_return_value = reinterpret_cast<void*>(0x144000);
  mock().expect_one_call("foo").and_return_value(expected_return_value);
  CHECK_EQUAL(
      expected_return_value,
      mock().actual_call("foo").return_value_or_default<void*>(
          default_return_value
      )
  );
  CHECK_EQUAL(
      expected_return_value,
      mock().return_value_or_default<void*>(default_return_value)
  );
}

TEST(MockReturnValue, WhenNoPointerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  void* default_return_value = reinterpret_cast<void*>(0x10);
  mock().expect_one_call("foo");
  CHECK_EQUAL(
      default_return_value,
      mock().actual_call("foo").return_value_or_default<void*>(
          default_return_value
      )
  );
  CHECK_EQUAL(
      default_return_value,
      mock().return_value_or_default<void*>(default_return_value)
  );
}

TEST(MockReturnValue, WhenAFunctionPointerReturnValueIsExpectedAndAlsoThereIsADefaultShouldlIgnoreTheDefault)
{
  auto default_return_value = reinterpret_cast<void (*)()>(0x777);
  auto expected_return_value = reinterpret_cast<void (*)()>(0x144000);
  mock().expect_one_call("foo").and_return_value(expected_return_value);
  CHECK_EQUAL(
      expected_return_value,
      mock().actual_call("foo").return_value_or_default<mu::tiny::mock::ActualCall::FunctionPointerReturnValue>(
          default_return_value
      )
  );
  CHECK_EQUAL(
      expected_return_value,
      mock().return_value_or_default<mu::tiny::mock::Support::FunctionPointerValue>(default_return_value)
  );
}

TEST(MockReturnValue, WhenNoFunctionPointerReturnValueIsExpectedButThereIsADefaultShouldlUseTheDefaultValue)
{
  auto default_return_value = reinterpret_cast<void (*)()>(0x10);
  mock().expect_one_call("foo");
  CHECK_EQUAL(
      default_return_value,
      mock().actual_call("foo").return_value_or_default<mu::tiny::mock::ActualCall::FunctionPointerReturnValue>(
          default_return_value
      )
  );
  CHECK_EQUAL(
      default_return_value,
      mock().return_value_or_default<mu::tiny::mock::Support::FunctionPointerValue>(default_return_value)
  );
}

TEST(MockReturnValue, PointerReturnValue)
{
  void* ptr = reinterpret_cast<void*>(0x00107);
  mock().expect_one_call("foo").and_return_value(ptr);
  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");

  CHECK_EQUAL(ptr, actual_call.return_value().get_value<void*>());
  CHECK_EQUAL(ptr, actual_call.return_value_or_default<void*>(nullptr));
  CHECK_EQUAL(ptr, mock().return_value<void*>());
}

TEST(MockReturnValue, ConstPointerReturnValue)
{
  const void* ptr = reinterpret_cast<const void*>(0x001074);
  mock().expect_one_call("foo").and_return_value(ptr);
  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");
  CHECK_EQUAL(ptr, actual_call.return_value().get_value<const void*>());
  CHECK_EQUAL(ptr, actual_call.return_value_or_default<const void*>(nullptr));
  CHECK_EQUAL(ptr, mock().return_value<const void*>());
}

TEST(MockReturnValue, FunctionPointerReturnValue)
{
  auto ptr = reinterpret_cast<void (*)()>(0x00107);
  mock().expect_one_call("foo").and_return_value(ptr);
  mu::tiny::mock::ActualCall& actual_call = mock().actual_call("foo");

  CHECK_EQUAL(ptr, actual_call.return_value().get_value<mu::tiny::mock::NamedValue::FunctionPointerValue>());
  CHECK_EQUAL(ptr, actual_call.return_value_or_default<mu::tiny::mock::ActualCall::FunctionPointerReturnValue>(nullptr));
  CHECK_EQUAL(ptr, mock().return_value<mu::tiny::mock::Support::FunctionPointerValue>());
}

TEST(MockReturnValue, whenCallingDisabledOrIgnoredActualCallsThenTheyDontReturnPreviousCallsValues)
{
  mock().expect_one_call("boo").ignore_other_parameters().and_return_value(10);
  mock().ignore_other_calls();
  mock().actual_call("boo");
  mock().actual_call("An Ignored Call");

  CHECK(!mock().has_return_value());
}
