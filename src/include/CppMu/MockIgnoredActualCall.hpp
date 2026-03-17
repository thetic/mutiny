#ifndef INCLUDED_CPPMU_MOCKIGNOREDACTUALCALL_HPP
#define INCLUDED_CPPMU_MOCKIGNOREDACTUALCALL_HPP

#include "CppMu/MockActualCall.hpp"

namespace cppmu {

class MockIgnoredActualCall : public MockActualCall
{
public:
  MockActualCall& with_name(const cppmu::String&) override { return *this; }
  MockActualCall& with_call_order(unsigned int) override { return *this; }
  MockActualCall& with_bool_parameter(const cppmu::String&, bool) override
  {
    return *this;
  }
  MockActualCall& with_int_parameter(const cppmu::String&, int) override
  {
    return *this;
  }
  MockActualCall& with_unsigned_int_parameter(
      const cppmu::String&,
      unsigned int
  ) override
  {
    return *this;
  }
  MockActualCall& with_long_int_parameter(
      const cppmu::String&,
      long int
  ) override
  {
    return *this;
  }
  MockActualCall& with_unsigned_long_int_parameter(
      const cppmu::String&,
      unsigned long int
  ) override
  {
    return *this;
  }
  MockActualCall& with_long_long_int_parameter(
      const cppmu::String&,
      long long
  ) override
  {
    return *this;
  }
  MockActualCall& with_unsigned_long_long_int_parameter(
      const cppmu::String&,
      unsigned long long
  ) override
  {
    return *this;
  }
  MockActualCall& with_double_parameter(const cppmu::String&, double) override
  {
    return *this;
  }
  MockActualCall& with_string_parameter(
      const cppmu::String&,
      const char*
  ) override
  {
    return *this;
  }
  MockActualCall& with_pointer_parameter(const cppmu::String&, void*) override
  {
    return *this;
  }
  MockActualCall& with_const_pointer_parameter(
      const cppmu::String&,
      const void*
  ) override
  {
    return *this;
  }
  MockActualCall& with_function_pointer_parameter(
      const cppmu::String&,
      void (*)()
  ) override
  {
    return *this;
  }
  MockActualCall& with_memory_buffer_parameter(
      const cppmu::String&,
      const unsigned char*,
      size_t
  ) override
  {
    return *this;
  }
  MockActualCall& with_memory_buffer_parameter(
      const char*,
      const unsigned char*,
      size_t
  ) override
  {
    return *this;
  }
  MockActualCall& with_parameter_of_type(
      const cppmu::String&,
      const cppmu::String&,
      const void*
  ) override
  {
    return *this;
  }
  MockActualCall& with_parameter_of_type(
      const char*,
      const char*,
      const void*
  ) override
  {
    return *this;
  }
  MockActualCall& with_output_parameter(const cppmu::String&, void*) override
  {
    return *this;
  }
  MockActualCall& with_output_parameter_of_type(
      const cppmu::String&,
      const cppmu::String&,
      void*
  ) override
  {
    return *this;
  }

  bool has_return_value() override { return false; }
  MockNamedValue return_value() override { return MockNamedValue(""); }

  bool return_bool_value_or_default(bool value) override { return value; }
  bool return_bool_value() override { return false; }

  int return_int_value() override { return 0; }
  int return_int_value_or_default(int value) override { return value; }

  unsigned long int return_unsigned_long_int_value() override { return 0; }
  unsigned long int return_unsigned_long_int_value_or_default(
      unsigned long int value
  ) override
  {
    return value;
  }

  long int return_long_int_value() override { return 0; }
  long int return_long_int_value_or_default(long int value) override
  {
    return value;
  }

  unsigned long long return_unsigned_long_long_int_value() override
  {
    return 0;
  }

  unsigned long long return_unsigned_long_long_int_value_or_default(
      unsigned long long value
  ) override
  {
    return value;
  }

  long long return_long_long_int_value() override { return 0; }

  long long return_long_long_int_value_or_default(long long value) override
  {
    return value;
  }

  unsigned int return_unsigned_int_value() override { return 0; }
  unsigned int return_unsigned_int_value_or_default(unsigned int value) override
  {
    return value;
  }

  double return_double_value() override { return 0.0; }
  double return_double_value_or_default(double value) override { return value; }

  const char* return_string_value() override { return ""; }
  const char* return_string_value_or_default(const char* value) override
  {
    return value;
  }

  void* return_pointer_value() override { return nullptr; }
  void* return_pointer_value_or_default(void* value) override { return value; }

  const void* return_const_pointer_value() override { return nullptr; }
  const void* return_const_pointer_value_or_default(const void* value) override
  {
    return value;
  }

  FunctionPointerReturnValue return_function_pointer_value() override
  {
    return nullptr;
  }
  FunctionPointerReturnValue return_function_pointer_value_or_default(
      void (*value)()
  ) override
  {
    return value;
  }

  MockActualCall& on_object(const void*) override { return *this; }

  static MockIgnoredActualCall& instance()
  {
    static MockIgnoredActualCall call;
    return call;
  }
};

} // namespace cppmu

#endif
