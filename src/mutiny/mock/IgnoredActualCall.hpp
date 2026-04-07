#ifndef INCLUDED_MUTINY_MOCK_IGNOREDACTUALCALL_HPP
#define INCLUDED_MUTINY_MOCK_IGNOREDACTUALCALL_HPP

#include "mutiny/mock/ActualCall.hpp"

namespace mu {
namespace tiny {
namespace mock {

class IgnoredActualCall : public ActualCall
{
public:
  ActualCall& with_name(const String&) override { return *this; }
  ActualCall& with_call_order(unsigned int) override { return *this; }
  ActualCall& with_bool_parameter(const String&, bool) override
  {
    return *this;
  }
  ActualCall& with_int_parameter(const String&, int) override { return *this; }
  ActualCall& with_unsigned_int_parameter(const String&, unsigned int) override
  {
    return *this;
  }
  ActualCall& with_long_int_parameter(const String&, long int) override
  {
    return *this;
  }
  ActualCall& with_unsigned_long_int_parameter(
      const String&,
      unsigned long int
  ) override
  {
    return *this;
  }
  ActualCall& with_long_long_int_parameter(const String&, long long) override
  {
    return *this;
  }
  ActualCall& with_unsigned_long_long_int_parameter(
      const String&,
      unsigned long long
  ) override
  {
    return *this;
  }
  ActualCall& with_double_parameter(const String&, double) override
  {
    return *this;
  }
  ActualCall& with_string_parameter(const String&, const char*) override
  {
    return *this;
  }
  ActualCall& with_pointer_parameter(const String&, void*) override
  {
    return *this;
  }
  ActualCall& with_const_pointer_parameter(const String&, const void*) override
  {
    return *this;
  }
  ActualCall& with_function_pointer_parameter(
      const String&,
      void (*)()
  ) override
  {
    return *this;
  }
  ActualCall& with_memory_buffer_parameter(
      const String&,
      const unsigned char*,
      size_t
  ) override
  {
    return *this;
  }
  ActualCall& with_memory_buffer_parameter(
      const char*,
      const unsigned char*,
      size_t
  ) override
  {
    return *this;
  }
  ActualCall& with_parameter_of_type(
      const String&,
      const String&,
      const void*
  ) override
  {
    return *this;
  }
  ActualCall& with_parameter_of_type(
      const char*,
      const char*,
      const void*
  ) override
  {
    return *this;
  }
  ActualCall& with_output_parameter(const String&, void*) override
  {
    return *this;
  }
  ActualCall& with_output_parameter_of_type(
      const String&,
      const String&,
      void*
  ) override
  {
    return *this;
  }

  bool has_return_value() override { return false; }
  NamedValue return_value() override { return NamedValue(""); }

  ActualCall& on_object(const void*) override { return *this; }

  static IgnoredActualCall& instance()
  {
    static IgnoredActualCall call;
    return call;
  }

private:
  bool return_bool_value() override { return false; }
  bool return_bool_value_or_default(bool value) override { return value; }
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
  const char* return_string_value() override { return ""; }
  const char* return_string_value_or_default(const char* value) override
  {
    return value;
  }
  double return_double_value() override { return 0.0; }
  double return_double_value_or_default(double value) override { return value; }
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
};

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
