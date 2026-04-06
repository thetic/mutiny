#include "mutiny/mock/IgnoredActualCall.hpp"

#include "mutiny/test.hpp"

TEST_GROUP(IgnoredActualCall)
{
  mu::tiny::mock::IgnoredActualCall ignored;
};

TEST(IgnoredActualCall, allWithMethodsReturnSelf)
{
  auto& r01 = ignored.with_name("func");
  POINTERS_EQUAL(&ignored, &r01);

  auto& r02 = ignored.with_call_order(0);
  POINTERS_EQUAL(&ignored, &r02);

  auto& r03 = ignored.on_object(nullptr);
  POINTERS_EQUAL(&ignored, &r03);

  auto& r04 = ignored.with_bool_parameter("umm", true);
  POINTERS_EQUAL(&ignored, &r04);

  auto& r05 = ignored.with_int_parameter("bla", static_cast<int>(1));
  POINTERS_EQUAL(&ignored, &r05);

  auto& r06 =
      ignored.with_unsigned_int_parameter("foo", static_cast<unsigned int>(1));
  POINTERS_EQUAL(&ignored, &r06);

  auto& r07 = ignored.with_long_int_parameter("hey", static_cast<long int>(1));
  POINTERS_EQUAL(&ignored, &r07);

  auto& r08 = ignored.with_unsigned_long_int_parameter(
      "bah", static_cast<unsigned long int>(1)
  );
  POINTERS_EQUAL(&ignored, &r08);

  auto& r09 =
      ignored.with_long_long_int_parameter("yo", static_cast<long long>(1));
  POINTERS_EQUAL(&ignored, &r09);

  auto& r10 = ignored.with_unsigned_long_long_int_parameter(
      "grr", static_cast<unsigned long long>(1)
  );
  POINTERS_EQUAL(&ignored, &r10);

  auto& r11 = ignored.with_double_parameter("hah", static_cast<double>(1.1));
  POINTERS_EQUAL(&ignored, &r11);

  auto& r12 = ignored.with_string_parameter("goo", "hello");
  POINTERS_EQUAL(&ignored, &r12);

  auto& r13 =
      ignored.with_pointer_parameter("pie", static_cast<void*>(nullptr));
  POINTERS_EQUAL(&ignored, &r13);

  auto& r14 = ignored.with_const_pointer_parameter(
      "woo", static_cast<const void*>(nullptr)
  );
  POINTERS_EQUAL(&ignored, &r14);

  auto& r15 = ignored.with_function_pointer_parameter(
      "fop", static_cast<void (*)()>(nullptr)
  );
  POINTERS_EQUAL(&ignored, &r15);

  auto& r16 = ignored.with_memory_buffer_parameter(
      mu::tiny::String("waa"), static_cast<const unsigned char*>(nullptr), 0
  );
  POINTERS_EQUAL(&ignored, &r16);

  auto& r17 = ignored.with_memory_buffer_parameter(
      "waa", static_cast<const unsigned char*>(nullptr), 0
  );
  POINTERS_EQUAL(&ignored, &r17);

  auto& r18 = ignored.with_parameter_of_type(
      mu::tiny::String("mytype"),
      mu::tiny::String("top"),
      static_cast<const void*>(nullptr)
  );
  POINTERS_EQUAL(&ignored, &r18);

  auto& r19 = ignored.with_parameter_of_type(
      "mytype", "top", static_cast<const void*>(nullptr)
  );
  POINTERS_EQUAL(&ignored, &r19);

  auto& r20 = ignored.with_output_parameter("bar", static_cast<void*>(nullptr));
  POINTERS_EQUAL(&ignored, &r20);

  auto& r21 = ignored.with_output_parameter_of_type(
      mu::tiny::String("mytype"),
      mu::tiny::String("bar"),
      static_cast<void*>(nullptr)
  );
  POINTERS_EQUAL(&ignored, &r21);
}

TEST(IgnoredActualCall, returnValuesAreNullDefaults)
{
  CHECK_FALSE(ignored.has_return_value());
  ignored.return_value();
  CHECK_FALSE(ignored.return_bool_value());
  CHECK(ignored.return_bool_value_or_default(true));
  CHECK_EQUAL(0, ignored.return_int_value());
  CHECK_EQUAL(1, ignored.return_int_value_or_default(1));
  CHECK_EQUAL(
      static_cast<unsigned int>(0), ignored.return_unsigned_int_value()
  );
  CHECK_EQUAL(
      static_cast<unsigned int>(1),
      ignored.return_unsigned_int_value_or_default(1)
  );
  CHECK_EQUAL(static_cast<long int>(0), ignored.return_long_int_value());
  CHECK_EQUAL(
      static_cast<long int>(1), ignored.return_long_int_value_or_default(1)
  );
  CHECK_EQUAL(
      static_cast<unsigned long int>(0),
      ignored.return_unsigned_long_int_value()
  );
  CHECK_EQUAL(
      static_cast<unsigned long int>(1),
      ignored.return_unsigned_long_int_value_or_default(1)
  );
  CHECK_EQUAL(static_cast<long long>(0), ignored.return_long_long_int_value());
  CHECK_EQUAL(
      static_cast<long long>(1),
      ignored.return_long_long_int_value_or_default(1)
  );
  CHECK_EQUAL(
      static_cast<unsigned long long>(0),
      ignored.return_unsigned_long_long_int_value()
  );
  CHECK_EQUAL(
      static_cast<unsigned long long>(1),
      ignored.return_unsigned_long_long_int_value_or_default(1)
  );
  CHECK_APPROX(0.0, ignored.return_double_value(), 0.001);
  CHECK_APPROX(1.1, ignored.return_double_value_or_default(1.1), 0.001);
  STRCMP_EQUAL("", ignored.return_string_value());
  STRCMP_EQUAL("hi", ignored.return_string_value_or_default("hi"));
  POINTERS_EQUAL(nullptr, ignored.return_pointer_value());
  POINTERS_EQUAL(nullptr, ignored.return_pointer_value_or_default(nullptr));
  POINTERS_EQUAL(nullptr, ignored.return_const_pointer_value());
  POINTERS_EQUAL(
      nullptr, ignored.return_const_pointer_value_or_default(nullptr)
  );
  CHECK_EQUAL(
      static_cast<void (*)()>(nullptr), ignored.return_function_pointer_value()
  );
  CHECK_EQUAL(
      static_cast<void (*)()>(nullptr),
      ignored.return_function_pointer_value_or_default(nullptr)
  );
}
