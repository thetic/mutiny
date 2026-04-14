#include "mutiny/mock/IgnoredExpectedCall.hpp"

#include "mutiny/test.hpp"

TEST_GROUP(IgnoredExpectedCall)
{
  mu::tiny::mock::IgnoredExpectedCall ignored;
};

TEST(IgnoredExpectedCall, withNameReturnsSelf)
{
  auto& result = ignored.with_name("func");
  CHECK_EQUAL(&ignored, &result);
}

TEST(IgnoredExpectedCall, withCallOrderReturnsSelf)
{
  auto& r01 = ignored.with_call_order(1);
  CHECK_EQUAL(&ignored, &r01);

  auto& r02 = ignored.with_call_order(1, 1);
  CHECK_EQUAL(&ignored, &r02);
}

TEST(IgnoredExpectedCall, onObjectReturnsSelf)
{
  auto& result = ignored.on_object(nullptr);
  CHECK_EQUAL(&ignored, &result);
}

TEST(IgnoredExpectedCall, withTypedParameterReturnsSelf)
{
  mu::tiny::mock::NamedValue nv("p");
  nv.set_value(42);
  auto& result = ignored.with_typed_parameter(
      static_cast<mu::tiny::mock::NamedValue&&>(nv)
  );
  CHECK_EQUAL(&ignored, &result);
}

TEST(IgnoredExpectedCall, withParameterOverloadsReturnSelf)
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

  auto& r09 = ignored.with_parameter("d_tol", 2.1, 0.3);
  CHECK_EQUAL(&ignored, &r09);

  auto& r10 = ignored.with_parameter("s", "hello");
  CHECK_EQUAL(&ignored, &r10);

  auto& r11 = ignored.with_parameter("p", static_cast<void*>(nullptr));
  CHECK_EQUAL(&ignored, &r11);

  auto& r12 =
      ignored.with_parameter("cp", static_cast<const void*>(nullptr));
  CHECK_EQUAL(&ignored, &r12);

  auto& r13 =
      ignored.with_parameter("fp", static_cast<void (*)()>(nullptr));
  CHECK_EQUAL(&ignored, &r13);

  auto& r14 = ignored.with_parameter(
      mu::tiny::String("mb"), static_cast<const unsigned char*>(nullptr), 0
  );
  CHECK_EQUAL(&ignored, &r14);
}

TEST(IgnoredExpectedCall, withParameterOfTypeReturnsSelf)
{
  auto& result = ignored.with_parameter_of_type(
      "mytype", "top", static_cast<const void*>(nullptr)
  );
  CHECK_EQUAL(&ignored, &result);
}

TEST(IgnoredExpectedCall, withOutputParameterReturningReturnsSelf)
{
  auto& result = ignored.with_output_parameter_returning(
      "bar", static_cast<void*>(nullptr), 1
  );
  CHECK_EQUAL(&ignored, &result);
}

TEST(IgnoredExpectedCall, withOutputParameterOfTypeReturningReturnsSelf)
{
  auto& result = ignored.with_output_parameter_of_type_returning(
      "mytype", "bar", static_cast<const void*>(nullptr)
  );
  CHECK_EQUAL(&ignored, &result);
}

TEST(IgnoredExpectedCall, withUnmodifiedOutputParameterReturnsSelf)
{
  auto& result = ignored.with_unmodified_output_parameter("unmod");
  CHECK_EQUAL(&ignored, &result);
}

TEST(IgnoredExpectedCall, ignoreOtherParametersReturnsSelf)
{
  auto& result = ignored.ignore_other_parameters();
  CHECK_EQUAL(&ignored, &result);
}

TEST(IgnoredExpectedCall, andReturnTypedValueReturnsSelf)
{
  mu::tiny::mock::NamedValue nv("returnValue");
  nv.set_value(42);
  auto& result = ignored.and_return_typed_value(
      static_cast<mu::tiny::mock::NamedValue&&>(nv)
  );
  CHECK_EQUAL(&ignored, &result);
}

TEST(IgnoredExpectedCall, andReturnValueOverloadsReturnSelf)
{
  auto& r01 = ignored.and_return_value(true);
  CHECK_EQUAL(&ignored, &r01);

  auto& r02 = ignored.and_return_value(1);
  CHECK_EQUAL(&ignored, &r02);

  auto& r03 = ignored.and_return_value(1U);
  CHECK_EQUAL(&ignored, &r03);

  auto& r04 = ignored.and_return_value(1L);
  CHECK_EQUAL(&ignored, &r04);

  auto& r05 = ignored.and_return_value(1UL);
  CHECK_EQUAL(&ignored, &r05);

  auto& r06 = ignored.and_return_value(1LL);
  CHECK_EQUAL(&ignored, &r06);

  auto& r07 = ignored.and_return_value(1ULL);
  CHECK_EQUAL(&ignored, &r07);

  auto& r08 = ignored.and_return_value(1.0);
  CHECK_EQUAL(&ignored, &r08);

  auto& r09 = ignored.and_return_value("boo");
  CHECK_EQUAL(&ignored, &r09);

  auto& r10 = ignored.and_return_value(static_cast<void*>(nullptr));
  CHECK_EQUAL(&ignored, &r10);

  auto& r11 = ignored.and_return_value(static_cast<const void*>(nullptr));
  CHECK_EQUAL(&ignored, &r11);

  auto& r12 = ignored.and_return_value(static_cast<void (*)()>(nullptr));
  CHECK_EQUAL(&ignored, &r12);
}
