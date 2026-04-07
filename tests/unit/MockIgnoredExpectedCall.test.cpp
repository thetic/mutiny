#include "mutiny/mock/IgnoredExpectedCall.hpp"

#include "mutiny/test.hpp"

TEST_GROUP(IgnoredExpectedCall)
{
  mu::tiny::mock::IgnoredExpectedCall ignored;
};

TEST(IgnoredExpectedCall, worksAsItShould)
{
  ignored.with_name("func");
  ignored.with_call_order(1);
  ignored.with_call_order(1, 1);
  ignored.on_object(nullptr);
  ignored.with_bool_parameter("umm", true);
  ignored.with_int_parameter("bla", 1);
  ignored.with_unsigned_int_parameter("foo", 1U);
  ignored.with_long_int_parameter("hey", 1L);
  ignored.with_unsigned_long_int_parameter("bah", 1UL);
  ignored.with_long_long_int_parameter("yo", 1LL);
  ignored.with_unsigned_long_long_int_parameter("grr", 1ULL);
  ignored.with_double_parameter("hah", 1.1);
  ignored.with_double_parameter("gah", 2.1, 0.3);
  ignored.with_string_parameter("goo", "hello");
  ignored.with_pointer_parameter("pie", static_cast<void*>(nullptr));
  ignored.with_const_pointer_parameter(
      "woo", static_cast<const void*>(nullptr)
  );
  ignored.with_function_pointer_parameter(
      "fop", static_cast<void (*)()>(nullptr)
  );
  ignored.with_memory_buffer_parameter(
      "waa", static_cast<const unsigned char*>(nullptr), 0
  );
  ignored.with_parameter_of_type(
      "mytype", "top", static_cast<const void*>(nullptr)
  );
  ignored.with_output_parameter_returning(
      "bar", static_cast<void*>(nullptr), 1
  );
  ignored.with_output_parameter_of_type_returning(
      "mytype", "bar", static_cast<const void*>(nullptr)
  );
  ignored.with_unmodified_output_parameter("unmod");
  ignored.ignore_other_parameters();
  ignored.and_return_value(true);
  ignored.and_return_value(1.0);
  ignored.and_return_value(1U);
  ignored.and_return_value(1);
  ignored.and_return_value(1UL);
  ignored.and_return_value(1L);
  ignored.and_return_value(1ULL);
  ignored.and_return_value(1LL);
  ignored.and_return_value("boo");
  ignored.and_return_value(static_cast<void*>(nullptr));
  ignored.and_return_value(static_cast<const void*>(nullptr));
  ignored.and_return_value(static_cast<void (*)()>(nullptr));
}
