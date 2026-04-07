#include "mutiny/mock/IgnoredActualCall.hpp"

#include "mutiny/test.hpp"

TEST_GROUP(IgnoredActualCall)
{
  mu::tiny::mock::IgnoredActualCall ignored;
};

TEST(IgnoredActualCall, allWithMethodsReturnSelf)
{
  auto& r01 = ignored.with_name("func");
  CHECK_EQUAL(&ignored, &r01);

  auto& r02 = ignored.with_call_order(0);
  CHECK_EQUAL(&ignored, &r02);

  auto& r03 = ignored.on_object(nullptr);
  CHECK_EQUAL(&ignored, &r03);

  auto& r04 = ignored.with_bool_parameter("umm", true);
  CHECK_EQUAL(&ignored, &r04);

  auto& r05 = ignored.with_int_parameter("bla", 1);
  CHECK_EQUAL(&ignored, &r05);

  auto& r06 = ignored.with_unsigned_int_parameter("foo", 1U);
  CHECK_EQUAL(&ignored, &r06);

  auto& r07 = ignored.with_long_int_parameter("hey", 1L);
  CHECK_EQUAL(&ignored, &r07);

  auto& r08 = ignored.with_unsigned_long_int_parameter("bah", 1UL);
  CHECK_EQUAL(&ignored, &r08);

  auto& r09 = ignored.with_long_long_int_parameter("yo", 1LL);
  CHECK_EQUAL(&ignored, &r09);

  auto& r10 = ignored.with_unsigned_long_long_int_parameter("grr", 1ULL);
  CHECK_EQUAL(&ignored, &r10);

  auto& r11 = ignored.with_double_parameter("hah", 1.1);
  CHECK_EQUAL(&ignored, &r11);

  auto& r12 = ignored.with_string_parameter("goo", "hello");
  CHECK_EQUAL(&ignored, &r12);

  auto& r13 =
      ignored.with_pointer_parameter("pie", static_cast<void*>(nullptr));
  CHECK_EQUAL(&ignored, &r13);

  auto& r14 = ignored.with_const_pointer_parameter(
      "woo", static_cast<const void*>(nullptr)
  );
  CHECK_EQUAL(&ignored, &r14);

  auto& r15 = ignored.with_function_pointer_parameter(
      "fop", static_cast<void (*)()>(nullptr)
  );
  CHECK_EQUAL(&ignored, &r15);

  auto& r16 = ignored.with_memory_buffer_parameter(
      mu::tiny::String("waa"), static_cast<const unsigned char*>(nullptr), 0
  );
  CHECK_EQUAL(&ignored, &r16);

  auto& r17 = ignored.with_memory_buffer_parameter(
      "waa", static_cast<const unsigned char*>(nullptr), 0
  );
  CHECK_EQUAL(&ignored, &r17);

  auto& r18 = ignored.with_parameter_of_type(
      mu::tiny::String("mytype"),
      mu::tiny::String("top"),
      static_cast<const void*>(nullptr)
  );
  CHECK_EQUAL(&ignored, &r18);

  auto& r19 = ignored.with_parameter_of_type(
      "mytype", "top", static_cast<const void*>(nullptr)
  );
  CHECK_EQUAL(&ignored, &r19);

  auto& r20 = ignored.with_output_parameter("bar", static_cast<void*>(nullptr));
  CHECK_EQUAL(&ignored, &r20);

  auto& r21 = ignored.with_output_parameter_of_type(
      mu::tiny::String("mytype"),
      mu::tiny::String("bar"),
      static_cast<void*>(nullptr)
  );
  CHECK_EQUAL(&ignored, &r21);
}

TEST(IgnoredActualCall, returnValuesAreNullDefaults)
{
  CHECK(!ignored.has_return_value());
  ignored.return_value();
  CHECK(!ignored.return_value_as<bool>());
  CHECK(ignored.return_value_as_or_default(true));
  CHECK_EQUAL(0, ignored.return_value_as<int>());
  CHECK_EQUAL(1, ignored.return_value_as_or_default(1));
  CHECK_EQUAL(0U, ignored.return_value_as<unsigned int>());
  CHECK_EQUAL(1U, ignored.return_value_as_or_default(1U));
  CHECK_EQUAL(0L, ignored.return_value_as<long int>());
  CHECK_EQUAL(1L, ignored.return_value_as_or_default(1L));
  CHECK_EQUAL(0UL, ignored.return_value_as<unsigned long int>());
  CHECK_EQUAL(1UL, ignored.return_value_as_or_default(1UL));
  CHECK_EQUAL(0LL, ignored.return_value_as<long long>());
  CHECK_EQUAL(1LL, ignored.return_value_as_or_default(1LL));
  CHECK_EQUAL(0ULL, ignored.return_value_as<unsigned long long>());
  CHECK_EQUAL(1ULL, ignored.return_value_as_or_default(1ULL));
  CHECK_APPROX(0.0, ignored.return_value_as<double>(), 0.001);
  CHECK_APPROX(1.1, ignored.return_value_as_or_default(1.1), 0.001);
  STRCMP_EQUAL("", ignored.return_value_as<const char*>());
  STRCMP_EQUAL("hi", ignored.return_value_as_or_default("hi"));
  CHECK_EQUAL(nullptr, ignored.return_value_as<void*>());
  CHECK_EQUAL(nullptr, ignored.return_value_as_or_default(static_cast<void*>(nullptr)));
  CHECK_EQUAL(nullptr, ignored.return_value_as<const void*>());
  CHECK_EQUAL(nullptr, ignored.return_value_as_or_default(static_cast<const void*>(nullptr)));
  CHECK_EQUAL(
      static_cast<void (*)()>(nullptr),
      ignored.return_value_as<mu::tiny::mock::ActualCall::FunctionPointerReturnValue>()
  );
  CHECK_EQUAL(
      static_cast<void (*)()>(nullptr),
      ignored.return_value_as_or_default(static_cast<void (*)()>(nullptr))
  );
}
