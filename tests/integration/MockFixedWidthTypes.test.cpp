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

// ---------------------------------------------------------------------------
// End-to-end: mock functions that take and return fixed-width types
// ---------------------------------------------------------------------------

namespace {

int8_t mock_read_int8(uint8_t addr)
{
  return mock()
      .actual_call("read_int8")
      .with_parameter("addr", addr)
      .return_value<int8_t>();
}

uint16_t mock_read_uint16(uint32_t addr)
{
  return mock()
      .actual_call("read_uint16")
      .with_parameter("addr", addr)
      .return_value<uint16_t>();
}

uint32_t mock_read_uint32(uint32_t addr)
{
  return mock()
      .actual_call("read_uint32")
      .with_parameter("addr", addr)
      .return_value<uint32_t>();
}

int64_t mock_read_int64(uint32_t addr)
{
  return mock()
      .actual_call("read_int64")
      .with_parameter("addr", addr)
      .return_value<int64_t>();
}

void mock_write_uint64(uint32_t addr, uint64_t value)
{
  mock()
      .actual_call("write_uint64")
      .with_parameter("addr", addr)
      .with_parameter("value", value);
}

} // namespace

TEST(MockFixedWidthTypes, int8EndToEnd)
{
  int8_t expected = -42;
  mock()
      .expect_one_call("read_int8")
      .with_parameter("addr", static_cast<uint8_t>(0x10))
      .and_return_value(expected);
  CHECK_EQUAL(expected, mock_read_int8(0x10));
}

TEST(MockFixedWidthTypes, uint16EndToEnd)
{
  uint16_t expected = 50000;
  mock()
      .expect_one_call("read_uint16")
      .with_parameter("addr", static_cast<uint32_t>(0x20))
      .and_return_value(expected);
  CHECK_EQUAL(expected, mock_read_uint16(0x20));
}

TEST(MockFixedWidthTypes, uint32EndToEnd)
{
  uint32_t expected = 0xDEADBEEF;
  mock()
      .expect_one_call("read_uint32")
      .with_parameter("addr", static_cast<uint32_t>(0x40000000))
      .and_return_value(expected);
  CHECK_EQUAL(expected, mock_read_uint32(0x40000000));
}

TEST(MockFixedWidthTypes, int64EndToEnd)
{
  int64_t expected = -4000000000LL;
  mock()
      .expect_one_call("read_int64")
      .with_parameter("addr", static_cast<uint32_t>(0x80))
      .and_return_value(expected);
  CHECK_EQUAL(expected, mock_read_int64(0x80));
}

TEST(MockFixedWidthTypes, uint64ParameterEndToEnd)
{
  uint64_t value = 0xFFFFAAAAFFFFAAAAULL;
  mock()
      .expect_one_call("write_uint64")
      .with_parameter("addr", static_cast<uint32_t>(0x90))
      .with_parameter("value", value);
  mock_write_uint64(0x90, value);
}

// ---------------------------------------------------------------------------
// return_value<T>() for every fixed-width type
// ---------------------------------------------------------------------------

TEST(MockFixedWidthTypes, returnValueInt8)
{
  int8_t expected = -128;
  mock().expect_one_call("foo").and_return_value(expected);
  auto actual = mock().actual_call("foo").return_value<int8_t>();
  CHECK_EQUAL(expected, actual);
}

TEST(MockFixedWidthTypes, returnValueUint8)
{
  uint8_t expected = 255;
  mock().expect_one_call("foo").and_return_value(expected);
  auto actual = mock().actual_call("foo").return_value<uint8_t>();
  CHECK_EQUAL(expected, actual);
}

TEST(MockFixedWidthTypes, returnValueInt16)
{
  int16_t expected = -32768;
  mock().expect_one_call("foo").and_return_value(expected);
  auto actual = mock().actual_call("foo").return_value<int16_t>();
  CHECK_EQUAL(expected, actual);
}

TEST(MockFixedWidthTypes, returnValueUint16)
{
  uint16_t expected = 65535;
  mock().expect_one_call("foo").and_return_value(expected);
  auto actual = mock().actual_call("foo").return_value<uint16_t>();
  CHECK_EQUAL(expected, actual);
}

TEST(MockFixedWidthTypes, returnValueInt32)
{
  int32_t expected = -2000000000L;
  mock().expect_one_call("foo").and_return_value(expected);
  auto actual = mock().actual_call("foo").return_value<int32_t>();
  CHECK_EQUAL(expected, actual);
}

TEST(MockFixedWidthTypes, returnValueUint32)
{
  uint32_t expected = 4000000000U;
  mock().expect_one_call("foo").and_return_value(expected);
  auto actual = mock().actual_call("foo").return_value<uint32_t>();
  CHECK_EQUAL(expected, actual);
}

TEST(MockFixedWidthTypes, returnValueInt64)
{
  int64_t expected = -4000000000LL;
  mock().expect_one_call("foo").and_return_value(expected);
  auto actual = mock().actual_call("foo").return_value<int64_t>();
  CHECK_EQUAL(expected, actual);
}

TEST(MockFixedWidthTypes, returnValueUint64)
{
  uint64_t expected = 0xDEADBEEFDEADBEEFULL;
  mock().expect_one_call("foo").and_return_value(expected);
  auto actual = mock().actual_call("foo").return_value<uint64_t>();
  CHECK_EQUAL(expected, actual);
}

// ---------------------------------------------------------------------------
// return_value_or_default<T>() for every fixed-width type — value set
// ---------------------------------------------------------------------------

TEST(MockFixedWidthTypes, returnValueOrDefaultInt8WhenSet)
{
  int8_t expected = -1;
  mock().expect_one_call("foo").and_return_value(expected);
  auto actual =
      mock().actual_call("foo").return_value_or_default<int8_t>(0);
  CHECK_EQUAL(expected, actual);
}

TEST(MockFixedWidthTypes, returnValueOrDefaultUint16WhenSet)
{
  uint16_t expected = 12345;
  mock().expect_one_call("foo").and_return_value(expected);
  auto actual =
      mock().actual_call("foo").return_value_or_default<uint16_t>(0);
  CHECK_EQUAL(expected, actual);
}

TEST(MockFixedWidthTypes, returnValueOrDefaultInt64WhenSet)
{
  int64_t expected = -4000000000LL;
  mock().expect_one_call("foo").and_return_value(expected);
  auto actual =
      mock().actual_call("foo").return_value_or_default<int64_t>(0);
  CHECK_EQUAL(expected, actual);
}

TEST(MockFixedWidthTypes, returnValueOrDefaultUint64WhenSet)
{
  uint64_t expected = 0xDEADBEEFDEADBEEFULL;
  mock().expect_one_call("foo").and_return_value(expected);
  auto actual =
      mock().actual_call("foo").return_value_or_default<uint64_t>(0);
  CHECK_EQUAL(expected, actual);
}

// ---------------------------------------------------------------------------
// return_value_or_default<T>() — no value set, falls back to default
// ---------------------------------------------------------------------------

TEST(MockFixedWidthTypes, returnValueOrDefaultInt8Fallback)
{
  int8_t fallback = -99;
  mock().expect_one_call("foo");
  auto actual =
      mock().actual_call("foo").return_value_or_default<int8_t>(fallback);
  CHECK_EQUAL(fallback, actual);
}

TEST(MockFixedWidthTypes, returnValueOrDefaultUint32Fallback)
{
  uint32_t fallback = 0xCAFEBABE;
  mock().expect_one_call("foo");
  auto actual =
      mock().actual_call("foo").return_value_or_default<uint32_t>(fallback);
  CHECK_EQUAL(fallback, actual);
}

TEST(MockFixedWidthTypes, returnValueOrDefaultInt64Fallback)
{
  int64_t fallback = -9999999999LL;
  mock().expect_one_call("foo");
  auto actual =
      mock().actual_call("foo").return_value_or_default<int64_t>(fallback);
  CHECK_EQUAL(fallback, actual);
}

// ---------------------------------------------------------------------------
// get_value<T>() on NamedValue for every fixed-width type
// ---------------------------------------------------------------------------

TEST(MockFixedWidthTypes, getValueInt8)
{
  int8_t expected = -42;
  mock().expect_one_call("foo").and_return_value(expected);
  auto nv = mock().actual_call("foo").return_value();
  CHECK_EQUAL(expected, nv.get_value<int8_t>());
}

TEST(MockFixedWidthTypes, getValueUint8)
{
  uint8_t expected = 255;
  mock().expect_one_call("foo").and_return_value(expected);
  auto nv = mock().actual_call("foo").return_value();
  CHECK_EQUAL(expected, nv.get_value<uint8_t>());
}

TEST(MockFixedWidthTypes, getValueInt16)
{
  int16_t expected = -32768;
  mock().expect_one_call("foo").and_return_value(expected);
  auto nv = mock().actual_call("foo").return_value();
  CHECK_EQUAL(expected, nv.get_value<int16_t>());
}

TEST(MockFixedWidthTypes, getValueUint16)
{
  uint16_t expected = 65535;
  mock().expect_one_call("foo").and_return_value(expected);
  auto nv = mock().actual_call("foo").return_value();
  CHECK_EQUAL(expected, nv.get_value<uint16_t>());
}

TEST(MockFixedWidthTypes, getValueInt32)
{
  int32_t expected = -2000000000L;
  mock().expect_one_call("foo").and_return_value(expected);
  auto nv = mock().actual_call("foo").return_value();
  CHECK_EQUAL(expected, nv.get_value<int32_t>());
}

TEST(MockFixedWidthTypes, getValueUint32)
{
  uint32_t expected = 4000000000U;
  mock().expect_one_call("foo").and_return_value(expected);
  auto nv = mock().actual_call("foo").return_value();
  CHECK_EQUAL(expected, nv.get_value<uint32_t>());
}

TEST(MockFixedWidthTypes, getValueInt64)
{
  int64_t expected = -4000000000LL;
  mock().expect_one_call("foo").and_return_value(expected);
  auto nv = mock().actual_call("foo").return_value();
  CHECK_EQUAL(expected, nv.get_value<int64_t>());
}

TEST(MockFixedWidthTypes, getValueUint64)
{
  uint64_t expected = 0xDEADBEEFDEADBEEFULL;
  mock().expect_one_call("foo").and_return_value(expected);
  auto nv = mock().actual_call("foo").return_value();
  CHECK_EQUAL(expected, nv.get_value<uint64_t>());
}

// ---------------------------------------------------------------------------
// Support::return_value<T>() and return_value_or_default<T>()
// ---------------------------------------------------------------------------

TEST(MockFixedWidthTypes, supportReturnValueInt32)
{
  int32_t expected = -100000;
  mock().expect_one_call("foo").and_return_value(expected);
  mock().actual_call("foo");
  CHECK_EQUAL(expected, mock().return_value<int32_t>());
}

TEST(MockFixedWidthTypes, supportReturnValueUint64)
{
  uint64_t expected = 0xFFFFAAAAFFFFAAAAULL;
  mock().expect_one_call("foo").and_return_value(expected);
  mock().actual_call("foo");
  CHECK_EQUAL(expected, mock().return_value<uint64_t>());
}

TEST(MockFixedWidthTypes, supportReturnValueOrDefaultFallback)
{
  uint32_t fallback = 99;
  mock().expect_one_call("foo");
  mock().actual_call("foo");
  CHECK_EQUAL(fallback, mock().return_value_or_default<uint32_t>(fallback));
}

// ---------------------------------------------------------------------------
// set_data / get_data with long long, unsigned long long, and fixed-width
// ---------------------------------------------------------------------------

TEST(MockFixedWidthTypes, setDataLongLong)
{
  long long expected = 0x7FFFAAAAFFFFAAAALL;
  mock().set_data("data", expected);
  CHECK_EQUAL(expected, mock().get_data("data").get_value<long long>());
}

TEST(MockFixedWidthTypes, setDataUnsignedLongLong)
{
  unsigned long long expected = 0xFFFFAAAAFFFFAAAAULL;
  mock().set_data("data", expected);
  CHECK_EQUAL(
      expected,
      mock().get_data("data").get_value<unsigned long long>()
  );
}

TEST(MockFixedWidthTypes, setDataInt64RetrieveViaTemplate)
{
  int64_t expected = -4000000000LL;
  mock().set_data("data", expected);
  CHECK_EQUAL(expected, mock().get_data("data").get_value<int64_t>());
}

TEST(MockFixedWidthTypes, setDataUint32RetrieveViaTemplate)
{
  uint32_t expected = 0xDEADBEEF;
  mock().set_data("data", expected);
  CHECK_EQUAL(expected, mock().get_data("data").get_value<uint32_t>());
}
