#include "mutiny/mock.hpp"
#include "mutiny/test.hpp"

#include <stdint.h>

using mu::tiny::mock::mock;

TEST_GROUP(MockFixedWidthTypes)
{
  void teardown() override
  {
    mock().check_expectations();
    mock().clear();
  }
};

TEST(MockFixedWidthTypes, int8Parameter)
{
  int8_t value = -42;
  mock().expect_one_call("foo").with_parameter("p", value);
  mock().actual_call("foo").with_parameter("p", value);
}

TEST(MockFixedWidthTypes, uint8Parameter)
{
  uint8_t value = 200;
  mock().expect_one_call("foo").with_parameter("p", value);
  mock().actual_call("foo").with_parameter("p", value);
}

TEST(MockFixedWidthTypes, int16Parameter)
{
  int16_t value = -1000;
  mock().expect_one_call("foo").with_parameter("p", value);
  mock().actual_call("foo").with_parameter("p", value);
}

TEST(MockFixedWidthTypes, uint16Parameter)
{
  uint16_t value = 50000;
  mock().expect_one_call("foo").with_parameter("p", value);
  mock().actual_call("foo").with_parameter("p", value);
}

TEST(MockFixedWidthTypes, int32Parameter)
{
  int32_t value = -100000;
  mock().expect_one_call("foo").with_parameter("p", value);
  mock().actual_call("foo").with_parameter("p", value);
}

TEST(MockFixedWidthTypes, uint32Parameter)
{
  uint32_t value = 3000000000U;
  mock().expect_one_call("foo").with_parameter("p", value);
  mock().actual_call("foo").with_parameter("p", value);
}

TEST(MockFixedWidthTypes, int64Parameter)
{
  int64_t value = -4000000000LL;
  mock().expect_one_call("foo").with_parameter("p", value);
  mock().actual_call("foo").with_parameter("p", value);
}

TEST(MockFixedWidthTypes, uint64Parameter)
{
  uint64_t value = 0xFFFFAAAAFFFFAAAAULL;
  mock().expect_one_call("foo").with_parameter("p", value);
  mock().actual_call("foo").with_parameter("p", value);
}

TEST(MockFixedWidthTypes, int32ReturnValueViaTemplate)
{
  int32_t expected = 42;
  mock().expect_one_call("foo").and_return_value(expected);
  auto actual =
      mock().actual_call("foo").return_value<int32_t>();
  CHECK_EQUAL(expected, actual);
}

TEST(MockFixedWidthTypes, uint32ReturnValueViaTemplate)
{
  uint32_t expected = 3000000000U;
  mock().expect_one_call("foo").and_return_value(expected);
  auto actual =
      mock().actual_call("foo").return_value<uint32_t>();
  CHECK_EQUAL(expected, actual);
}

TEST(MockFixedWidthTypes, int64ReturnValueViaTemplate)
{
  int64_t expected = -4000000000LL;
  mock().expect_one_call("foo").and_return_value(expected);
  auto actual =
      mock().actual_call("foo").return_value<int64_t>();
  CHECK_EQUAL(expected, actual);
}

TEST(MockFixedWidthTypes, uint64ReturnValueViaTemplate)
{
  uint64_t expected = 0xDEADBEEFDEADBEEFULL;
  mock().expect_one_call("foo").and_return_value(expected);
  auto actual =
      mock().actual_call("foo").return_value<uint64_t>();
  CHECK_EQUAL(expected, actual);
}

TEST(MockFixedWidthTypes, int8ReturnValueViaTemplate)
{
  int8_t expected = -42;
  mock().expect_one_call("foo").and_return_value(expected);
  auto actual =
      mock().actual_call("foo").return_value<int8_t>();
  CHECK_EQUAL(expected, actual);
}

TEST(MockFixedWidthTypes, uint8ReturnValueViaTemplate)
{
  uint8_t expected = 200;
  mock().expect_one_call("foo").and_return_value(expected);
  auto actual =
      mock().actual_call("foo").return_value<uint8_t>();
  CHECK_EQUAL(expected, actual);
}

TEST(MockFixedWidthTypes, returnValueOrDefaultWithInt32)
{
  int32_t expected = 42;
  mock().expect_one_call("foo").and_return_value(expected);
  auto actual =
      mock().actual_call("foo").return_value_or_default<int32_t>(-1);
  CHECK_EQUAL(expected, actual);
}

TEST(MockFixedWidthTypes, returnValueOrDefaultFallsBackWithInt32)
{
  int32_t fallback = -1;
  mock().expect_one_call("foo");
  auto actual =
      mock().actual_call("foo").return_value_or_default<int32_t>(fallback);
  CHECK_EQUAL(fallback, actual);
}

TEST(MockFixedWidthTypes, namedValueGetValueTemplate)
{
  int32_t expected = 42;
  mock().expect_one_call("foo").and_return_value(expected);
  mu::tiny::mock::NamedValue nv =
      mock().actual_call("foo").return_value();
  CHECK_EQUAL(expected, nv.get_value<int32_t>());
}

TEST(MockFixedWidthTypes, supportReturnValueTemplate)
{
  int64_t expected = -4000000000LL;
  mock().expect_one_call("foo").and_return_value(expected);
  mock().actual_call("foo");
  auto actual = mock().return_value<int64_t>();
  CHECK_EQUAL(expected, actual);
}

TEST(MockFixedWidthTypes, supportReturnValueOrDefaultTemplate)
{
  uint32_t fallback = 99;
  mock().expect_one_call("foo");
  mock().actual_call("foo");
  auto actual = mock().return_value_or_default<uint32_t>(fallback);
  CHECK_EQUAL(fallback, actual);
}

TEST(MockFixedWidthTypes, setDataLongLong)
{
  long long expected = 0x7FFFAAAAFFFFAAAALL;
  mock().set_data("data", expected);
  CHECK_EQUAL(expected, mock().get_data("data").get_long_long_int_value());
}

TEST(MockFixedWidthTypes, setDataUnsignedLongLong)
{
  unsigned long long expected = 0xFFFFAAAAFFFFAAAAULL;
  mock().set_data("data", expected);
  CHECK_EQUAL(
      expected,
      mock().get_data("data").get_unsigned_long_long_int_value()
  );
}

TEST(MockFixedWidthTypes, getDataWithFixedWidthGetter)
{
  int64_t expected = -4000000000LL;
  mock().set_data("data", expected);
  CHECK_EQUAL(expected, mock().get_data("data").get_value<int64_t>());
}
