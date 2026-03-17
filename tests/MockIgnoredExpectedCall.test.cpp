#include "CppMu/MockIgnoredExpectedCall.hpp"

#include "CppMu/CppMu.hpp"

TEST_GROUP(MockIgnoredExpectedCall)
{
  cppmu::MockIgnoredExpectedCall ignored;
};

TEST(MockIgnoredExpectedCall, worksAsItShould)
{
  ignored.with_name("func");
  ignored.with_call_order(1);
  ignored.with_call_order(1, 1);
  ignored.on_object(nullptr);
  ignored.with_bool_parameter("umm", true);
  ignored.with_int_parameter("bla", static_cast<int>(1));
  ignored.with_unsigned_int_parameter("foo", static_cast<unsigned int>(1));
  ignored.with_long_int_parameter("hey", static_cast<long int>(1));
  ignored.with_unsigned_long_int_parameter(
      "bah", static_cast<unsigned long int>(1)
  );
  ignored.with_long_long_int_parameter("yo", static_cast<long long int>(1));
  ignored.with_unsigned_long_long_int_parameter(
      "grr", static_cast<unsigned long long int>(1)
  );
  ignored.with_double_parameter("hah", static_cast<double>(1.1f));
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
  ignored.and_return_value(static_cast<double>(1.0f));
  ignored.and_return_value(static_cast<unsigned int>(1));
  ignored.and_return_value(static_cast<int>(1));
  ignored.and_return_value(static_cast<unsigned long int>(1));
  ignored.and_return_value(static_cast<long int>(1));
  ignored.and_return_value(static_cast<unsigned long long int>(1));
  ignored.and_return_value(static_cast<long long int>(1));
  ignored.and_return_value("boo");
  ignored.and_return_value(static_cast<void*>(nullptr));
  ignored.and_return_value(static_cast<const void*>(nullptr));
  ignored.and_return_value(static_cast<void (*)()>(nullptr));
}
