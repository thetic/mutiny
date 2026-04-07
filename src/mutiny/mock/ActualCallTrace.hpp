#ifndef INCLUDED_MUTINY_MOCK_ACTUALCALLTRACE_HPP
#define INCLUDED_MUTINY_MOCK_ACTUALCALLTRACE_HPP

#include "mutiny/mock/ActualCall.hpp"

namespace mu {
namespace tiny {
namespace mock {

class ActualCallTrace : public ActualCall
{
public:
  ActualCallTrace() = default;
  ~ActualCallTrace() override = default;

  ActualCall& with_name(const String& name) override;
  ActualCall& with_call_order(unsigned int) override;
  ActualCall& with_bool_parameter(const String& name, bool value) override;
  ActualCall& with_int_parameter(const String& name, int value) override;
  ActualCall& with_unsigned_int_parameter(
      const String& name,
      unsigned int value
  ) override;
  ActualCall& with_long_int_parameter(
      const String& name,
      long int value
  ) override;
  ActualCall& with_unsigned_long_int_parameter(
      const String& name,
      unsigned long int value
  ) override;
  ActualCall& with_long_long_int_parameter(
      const String& name,
      long long value
  ) override;
  ActualCall& with_unsigned_long_long_int_parameter(
      const String& name,
      unsigned long long value
  ) override;
  ActualCall& with_double_parameter(const String& name, double value) override;
  ActualCall& with_string_parameter(
      const String& name,
      const char* value
  ) override;
  ActualCall& with_pointer_parameter(const String& name, void* value) override;
  ActualCall& with_const_pointer_parameter(
      const String& name,
      const void* value
  ) override;
  ActualCall& with_function_pointer_parameter(
      const String& name,
      void (*value)()
  ) override;
  ActualCall& with_memory_buffer_parameter(
      const String& name,
      const unsigned char* value,
      size_t size
  ) override;
  ActualCall& with_memory_buffer_parameter(
      const char* name,
      const unsigned char* value,
      size_t size
  ) override;
  ActualCall& with_parameter_of_type(
      const String& type_name,
      const String& name,
      const void* value
  ) override;
  ActualCall& with_parameter_of_type(
      const char* type_name,
      const char* name,
      const void* value
  ) override;
  ActualCall& with_output_parameter(const String& name, void* output) override;
  ActualCall& with_output_parameter_of_type(
      const String& type_name,
      const String& name,
      void* output
  ) override;

  bool has_return_value() override;
  NamedValue return_value() override;

  ActualCall& on_object(const void* object_ptr) override;

  const char* get_trace_output();
  void clear();
  static ActualCallTrace& instance();
  static void clear_instance();

private:
  bool return_bool_value() override;
  bool return_bool_value_or_default(bool) override;
  unsigned long long return_unsigned_long_long_int_value() override;
  unsigned long long return_unsigned_long_long_int_value_or_default(
      unsigned long long
  ) override;
  long long return_long_long_int_value() override;
  long long return_long_long_int_value_or_default(long long) override;
  const char* return_string_value() override;
  const char* return_string_value_or_default(const char*) override;
  double return_double_value() override;
  double return_double_value_or_default(double) override;
  void* return_pointer_value() override;
  void* return_pointer_value_or_default(void*) override;
  const void* return_const_pointer_value() override;
  const void* return_const_pointer_value_or_default(const void*) override;
  FunctionPointerReturnValue return_function_pointer_value() override;
  FunctionPointerReturnValue return_function_pointer_value_or_default(
      void (*)()
  ) override;

  String trace_buffer_;

  static ActualCallTrace* instance_;

  void add_parameter_name(const String& name);
};

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
