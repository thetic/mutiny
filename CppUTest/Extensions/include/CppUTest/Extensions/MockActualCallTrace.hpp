#ifndef INCLUDED_CPPUTEST_EXTENSIONS_MOCKACTUALCALLTRACE_HPP
#define INCLUDED_CPPUTEST_EXTENSIONS_MOCKACTUALCALLTRACE_HPP

#include "CppUTest/Extensions/MockActualCall.hpp"

namespace cpputest {
namespace extensions {

class MockActualCallTrace : public MockActualCall
{
public:
  MockActualCallTrace();
  virtual ~MockActualCallTrace() override;

  virtual MockActualCall& with_name(const cpputest::String& name) override;
  virtual MockActualCall& with_call_order(unsigned int) override;
  virtual MockActualCall& with_bool_parameter(const cpputest::String& name,
      bool value) override;
  virtual MockActualCall& with_int_parameter(const cpputest::String& name,
      int value) override;
  virtual MockActualCall& with_unsigned_int_parameter(
      const cpputest::String& name,
      unsigned int value) override;
  virtual MockActualCall& with_long_int_parameter(const cpputest::String& name,
      long int value) override;
  virtual MockActualCall& with_unsigned_long_int_parameter(
      const cpputest::String& name,
      unsigned long int value) override;
  virtual MockActualCall& with_long_long_int_parameter(
      const cpputest::String& name,
      long long value) override;
  virtual MockActualCall& with_unsigned_long_long_int_parameter(
      const cpputest::String& name,
      unsigned long long value) override;
  virtual MockActualCall& with_double_parameter(const cpputest::String& name,
      double value) override;
  virtual MockActualCall& with_string_parameter(const cpputest::String& name,
      const char* value) override;
  virtual MockActualCall& with_pointer_parameter(const cpputest::String& name,
      void* value) override;
  virtual MockActualCall& with_const_pointer_parameter(
      const cpputest::String& name,
      const void* value) override;
  virtual MockActualCall& with_function_pointer_parameter(
      const cpputest::String& name,
      void (*value)()) override;
  virtual MockActualCall& with_memory_buffer_parameter(
      const cpputest::String& name,
      const unsigned char* value,
      size_t size) override;
  virtual MockActualCall& with_memory_buffer_parameter(const char* name,
      const unsigned char* value,
      size_t size) override;
  virtual MockActualCall& with_parameter_of_type(
      const cpputest::String& type_name,
      const cpputest::String& name,
      const void* value) override;
  virtual MockActualCall& with_parameter_of_type(const char* type_name,
      const char* name,
      const void* value) override;
  virtual MockActualCall& with_output_parameter(const cpputest::String& name,
      void* output) override;
  virtual MockActualCall& with_output_parameter_of_type(
      const cpputest::String& type_name,
      const cpputest::String& name,
      void* output) override;

  virtual bool has_return_value() override;
  virtual MockNamedValue return_value() override;

  virtual bool return_bool_value_or_default(bool default_value) override;
  virtual bool return_bool_value() override;

  virtual int return_int_value_or_default(int default_value) override;
  virtual int return_int_value() override;

  virtual unsigned long int return_unsigned_long_int_value() override;
  virtual unsigned long int return_unsigned_long_int_value_or_default(
      unsigned long int) override;

  virtual long int return_long_int_value() override;
  virtual long int return_long_int_value_or_default(
      long int default_value) override;

  virtual unsigned long long return_unsigned_long_long_int_value() override;
  virtual unsigned long long return_unsigned_long_long_int_value_or_default(
      unsigned long long default_value) override;

  virtual long long return_long_long_int_value() override;
  virtual long long return_long_long_int_value_or_default(
      long long default_value) override;

  virtual unsigned int return_unsigned_int_value() override;
  virtual unsigned int return_unsigned_int_value_or_default(
      unsigned int default_value) override;

  virtual const char* return_string_value_or_default(
      const char* default_value) override;
  virtual const char* return_string_value() override;

  virtual double return_double_value() override;
  virtual double return_double_value_or_default(double default_value) override;

  virtual void* return_pointer_value() override;
  virtual void* return_pointer_value_or_default(void*) override;

  virtual const void* return_const_pointer_value() override;
  virtual const void* return_const_pointer_value_or_default(
      const void* default_value) override;

  virtual FunctionPointerReturnValue return_function_pointer_value() override;
  virtual FunctionPointerReturnValue return_function_pointer_value_or_default(
      void (*)()) override;

  virtual MockActualCall& on_object(const void* object_ptr) override;

  const char* get_trace_output();
  void clear();
  static MockActualCallTrace& instance();
  static void clear_instance();

private:
  cpputest::String trace_buffer_;

  static MockActualCallTrace* instance_;

  void add_parameter_name(const cpputest::String& name);
};

} // namespace extensions
} // namespace cpputest

#endif
