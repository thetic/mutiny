#ifndef INCLUDED_MUTINY_MOCKACTUALCALLTRACE_HPP
#define INCLUDED_MUTINY_MOCKACTUALCALLTRACE_HPP

#include "mutiny/mock/ActualCall.hpp"

namespace mu {
namespace tiny {
namespace mock {

class MockActualCallTrace : public MockActualCall
{
public:
  MockActualCallTrace() = default;
  ~MockActualCallTrace() override = default;

  MockActualCall& with_name(const mu::tiny::test::String& name) override;
  MockActualCall& with_call_order(unsigned int) override;
  MockActualCall& with_bool_parameter(
      const mu::tiny::test::String& name,
      bool value
  ) override;
  MockActualCall& with_int_parameter(
      const mu::tiny::test::String& name,
      int value
  ) override;
  MockActualCall& with_unsigned_int_parameter(
      const mu::tiny::test::String& name,
      unsigned int value
  ) override;
  MockActualCall& with_long_int_parameter(
      const mu::tiny::test::String& name,
      long int value
  ) override;
  MockActualCall& with_unsigned_long_int_parameter(
      const mu::tiny::test::String& name,
      unsigned long int value
  ) override;
  MockActualCall& with_long_long_int_parameter(
      const mu::tiny::test::String& name,
      long long value
  ) override;
  MockActualCall& with_unsigned_long_long_int_parameter(
      const mu::tiny::test::String& name,
      unsigned long long value
  ) override;
  MockActualCall& with_double_parameter(
      const mu::tiny::test::String& name,
      double value
  ) override;
  MockActualCall& with_string_parameter(
      const mu::tiny::test::String& name,
      const char* value
  ) override;
  MockActualCall& with_pointer_parameter(
      const mu::tiny::test::String& name,
      void* value
  ) override;
  MockActualCall& with_const_pointer_parameter(
      const mu::tiny::test::String& name,
      const void* value
  ) override;
  MockActualCall& with_function_pointer_parameter(
      const mu::tiny::test::String& name,
      void (*value)()
  ) override;
  MockActualCall& with_memory_buffer_parameter(
      const mu::tiny::test::String& name,
      const unsigned char* value,
      size_t size
  ) override;
  MockActualCall& with_memory_buffer_parameter(
      const char* name,
      const unsigned char* value,
      size_t size
  ) override;
  MockActualCall& with_parameter_of_type(
      const mu::tiny::test::String& type_name,
      const mu::tiny::test::String& name,
      const void* value
  ) override;
  MockActualCall& with_parameter_of_type(
      const char* type_name,
      const char* name,
      const void* value
  ) override;
  MockActualCall& with_output_parameter(
      const mu::tiny::test::String& name,
      void* output
  ) override;
  MockActualCall& with_output_parameter_of_type(
      const mu::tiny::test::String& type_name,
      const mu::tiny::test::String& name,
      void* output
  ) override;

  bool has_return_value() override;
  MockNamedValue return_value() override;

  bool return_bool_value_or_default(bool default_value) override;
  bool return_bool_value() override;

  int return_int_value_or_default(int default_value) override;
  int return_int_value() override;

  unsigned long int return_unsigned_long_int_value() override;
  unsigned long int return_unsigned_long_int_value_or_default(
      unsigned long int
  ) override;

  long int return_long_int_value() override;
  long int return_long_int_value_or_default(long int default_value) override;

  unsigned long long return_unsigned_long_long_int_value() override;
  unsigned long long return_unsigned_long_long_int_value_or_default(
      unsigned long long default_value
  ) override;

  long long return_long_long_int_value() override;
  long long return_long_long_int_value_or_default(
      long long default_value
  ) override;

  unsigned int return_unsigned_int_value() override;
  unsigned int return_unsigned_int_value_or_default(
      unsigned int default_value
  ) override;

  const char* return_string_value_or_default(
      const char* default_value
  ) override;
  const char* return_string_value() override;

  double return_double_value() override;
  double return_double_value_or_default(double default_value) override;

  void* return_pointer_value() override;
  void* return_pointer_value_or_default(void*) override;

  const void* return_const_pointer_value() override;
  const void* return_const_pointer_value_or_default(
      const void* default_value
  ) override;

  FunctionPointerReturnValue return_function_pointer_value() override;
  FunctionPointerReturnValue return_function_pointer_value_or_default(
      void (*)()
  ) override;

  MockActualCall& on_object(const void* object_ptr) override;

  const char* get_trace_output();
  void clear();
  static MockActualCallTrace& instance();
  static void clear_instance();

private:
  mu::tiny::test::String trace_buffer_;

  static MockActualCallTrace* instance_;

  void add_parameter_name(const mu::tiny::test::String& name);
};

}
}
} // namespace mu::tiny::mock

#endif
