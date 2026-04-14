#include "mutiny/mock/IgnoredActualCall.hpp"

#include "mutiny/test.hpp"

TEST_GROUP(IgnoredActualCall)
{
  mu::tiny::mock::IgnoredActualCall ignored;
};

TEST(IgnoredActualCall, withNameReturnsSelf)
{
  auto& result = ignored.with_name("func");
  CHECK_EQUAL(&ignored, &result);
}

TEST(IgnoredActualCall, withCallOrderReturnsSelf)
{
  auto& result = ignored.with_call_order(0);
  CHECK_EQUAL(&ignored, &result);
}

TEST(IgnoredActualCall, onObjectReturnsSelf)
{
  auto& result = ignored.on_object(nullptr);
  CHECK_EQUAL(&ignored, &result);
}

TEST(IgnoredActualCall, withTypedParameterReturnsSelf)
{
  mu::tiny::mock::NamedValue nv("p");
  nv.set_value(42);
  auto& result = ignored.with_typed_parameter(static_cast<mu::tiny::mock::NamedValue&&>(nv));
  CHECK_EQUAL(&ignored, &result);
}

TEST(IgnoredActualCall, withParameterOverloadsReturnSelf)
{
  auto& r01 = ignored.with_parameter("b", true);
  CHECK_EQUAL(&ignored, &r01);

  auto& r02 = ignored.with_parameter("i", 1);
  CHECK_EQUAL(&ignored, &r02);

  auto& r03 = ignored.with_parameter("u", 1U);
  CHECK_EQUAL(&ignored, &r03);

  auto& r04 = ignored.with_parameter("l", 1L);
  CHECK_EQUAL(&ignored, &r04);

  auto& r05 = ignored.with_parameter("ul", 1UL);
  CHECK_EQUAL(&ignored, &r05);

  auto& r06 = ignored.with_parameter("ll", 1LL);
  CHECK_EQUAL(&ignored, &r06);

  auto& r07 = ignored.with_parameter("ull", 1ULL);
  CHECK_EQUAL(&ignored, &r07);

  auto& r08 = ignored.with_parameter("d", 1.1);
  CHECK_EQUAL(&ignored, &r08);

  auto& r09 = ignored.with_parameter("s", "hello");
  CHECK_EQUAL(&ignored, &r09);

  auto& r10 = ignored.with_parameter("p", static_cast<void*>(nullptr));
  CHECK_EQUAL(&ignored, &r10);

  auto& r11 =
      ignored.with_parameter("cp", static_cast<const void*>(nullptr));
  CHECK_EQUAL(&ignored, &r11);

  auto& r12 =
      ignored.with_parameter("fp", static_cast<void (*)()>(nullptr));
  CHECK_EQUAL(&ignored, &r12);

  auto& r13 = ignored.with_parameter(
      mu::tiny::String("mb"), static_cast<const unsigned char*>(nullptr), 0
  );
  CHECK_EQUAL(&ignored, &r13);

  auto& r14 = ignored.with_parameter(
      "mb2", static_cast<const unsigned char*>(nullptr), 0
  );
  CHECK_EQUAL(&ignored, &r14);
}

TEST(IgnoredActualCall, withParameterOfTypeReturnsSelf)
{
  auto& r01 = ignored.with_parameter_of_type(
      mu::tiny::String("mytype"),
      mu::tiny::String("top"),
      static_cast<const void*>(nullptr)
  );
  CHECK_EQUAL(&ignored, &r01);

  auto& r02 = ignored.with_parameter_of_type(
      "mytype", "top", static_cast<const void*>(nullptr)
  );
  CHECK_EQUAL(&ignored, &r02);
}

TEST(IgnoredActualCall, withOutputParameterReturnsSelf)
{
  auto& result =
      ignored.with_output_parameter("bar", static_cast<void*>(nullptr));
  CHECK_EQUAL(&ignored, &result);
}

TEST(IgnoredActualCall, withOutputParameterOfTypeReturnsSelf)
{
  auto& result = ignored.with_output_parameter_of_type(
      mu::tiny::String("mytype"),
      mu::tiny::String("bar"),
      static_cast<void*>(nullptr)
  );
  CHECK_EQUAL(&ignored, &result);
}

TEST(IgnoredActualCall, hasReturnValueReturnsFalse)
{
  CHECK(!ignored.has_return_value());
}

TEST(IgnoredActualCall, returnValueReturnsEmptyNamedValue)
{
  ignored.return_value();
}

TEST(IgnoredActualCall, returnTypedValuesAreNullDefaults)
{
  CHECK(!ignored.return_bool_value());
  CHECK_EQUAL(0, ignored.return_int_value());
  CHECK_EQUAL(0U, ignored.return_unsigned_int_value());
  CHECK_EQUAL(0L, ignored.return_long_int_value());
  CHECK_EQUAL(0UL, ignored.return_unsigned_long_int_value());
  CHECK_EQUAL(0LL, ignored.return_long_long_int_value());
  CHECK_EQUAL(0ULL, ignored.return_unsigned_long_long_int_value());
  CHECK_APPROX(0.0, ignored.return_double_value(), 0.001);
  STRCMP_EQUAL("", ignored.return_string_value());
  CHECK_EQUAL(nullptr, ignored.return_pointer_value());
  CHECK_EQUAL(nullptr, ignored.return_const_pointer_value());
  CHECK_EQUAL(
      static_cast<void (*)()>(nullptr), ignored.return_function_pointer_value()
  );
}

TEST(IgnoredActualCall, returnTypedValueOrDefaultReturnsDefault)
{
  CHECK(ignored.return_bool_value_or_default(true));
  CHECK_EQUAL(1, ignored.return_int_value_or_default(1));
  CHECK_EQUAL(1U, ignored.return_unsigned_int_value_or_default(1));
  CHECK_EQUAL(1L, ignored.return_long_int_value_or_default(1));
  CHECK_EQUAL(1UL, ignored.return_unsigned_long_int_value_or_default(1));
  CHECK_EQUAL(1LL, ignored.return_long_long_int_value_or_default(1));
  CHECK_EQUAL(1ULL, ignored.return_unsigned_long_long_int_value_or_default(1));
  CHECK_APPROX(1.1, ignored.return_double_value_or_default(1.1), 0.001);
  STRCMP_EQUAL("hi", ignored.return_string_value_or_default("hi"));
  CHECK_EQUAL(nullptr, ignored.return_pointer_value_or_default(nullptr));
  CHECK_EQUAL(nullptr, ignored.return_const_pointer_value_or_default(nullptr));
  CHECK_EQUAL(
      static_cast<void (*)()>(nullptr),
      ignored.return_function_pointer_value_or_default(nullptr)
  );
}
