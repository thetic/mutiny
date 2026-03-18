#ifndef INCLUDED_CPPMU_MOCKACTUALCALL_HPP
#define INCLUDED_CPPMU_MOCKACTUALCALL_HPP

#include "CppMu/MockNamedValue.hpp"

namespace cppmu {

class MockFailureReporter;
class MockFailure;

class MockActualCall
{
public:
  MockActualCall() = default;
  virtual ~MockActualCall() = default;

  virtual MockActualCall& with_name(const cppmu::String& name) = 0;
  virtual MockActualCall& with_call_order(unsigned int call_order) = 0;
  MockActualCall& with_parameter(const cppmu::String& name, bool value)
  {
    return with_bool_parameter(name, value);
  }
  MockActualCall& with_parameter(const cppmu::String& name, int value)
  {
    return with_int_parameter(name, value);
  }
  MockActualCall& with_parameter(const cppmu::String& name, unsigned int value)
  {
    return with_unsigned_int_parameter(name, value);
  }
  MockActualCall& with_parameter(const cppmu::String& name, long int value)
  {
    return with_long_int_parameter(name, value);
  }
  MockActualCall& with_parameter(
      const cppmu::String& name,
      unsigned long int value
  )
  {
    return with_unsigned_long_int_parameter(name, value);
  }
  MockActualCall& with_parameter(const cppmu::String& name, long long value)
  {
    return with_long_long_int_parameter(name, value);
  }
  MockActualCall& with_parameter(
      const cppmu::String& name,
      unsigned long long value
  )
  {
    return with_unsigned_long_long_int_parameter(name, value);
  }
  MockActualCall& with_parameter(const cppmu::String& name, double value)
  {
    return with_double_parameter(name, value);
  }
  MockActualCall& with_parameter(const cppmu::String& name, const char* value)
  {
    return with_string_parameter(name, value);
  }
  MockActualCall& with_parameter(const cppmu::String& name, void* value)
  {
    return with_pointer_parameter(name, value);
  }
  MockActualCall& with_parameter(const cppmu::String& name, void (*value)())
  {
    return with_function_pointer_parameter(name, value);
  }
  MockActualCall& with_parameter(const cppmu::String& name, const void* value)
  {
    return with_const_pointer_parameter(name, value);
  }
  MockActualCall& with_parameter(
      const cppmu::String& name,
      const unsigned char* value,
      size_t size
  )
  {
    return with_memory_buffer_parameter(name, value, size);
  }
  MockActualCall& with_parameter(
      const char* name,
      const unsigned char* value,
      size_t size
  )
  {
    return with_memory_buffer_parameter(name, value, size);
  }
  virtual MockActualCall& with_parameter_of_type(
      const cppmu::String& type_name,
      const cppmu::String& name,
      const void* value
  ) = 0;
  virtual MockActualCall& with_parameter_of_type(
      const char* type_name,
      const char* name,
      const void* value
  ) = 0;
  virtual MockActualCall& with_output_parameter(
      const cppmu::String& name,
      void* output
  ) = 0;
  virtual MockActualCall& with_output_parameter_of_type(
      const cppmu::String& type_name,
      const cppmu::String& name,
      void* output
  ) = 0;

  virtual MockActualCall& with_bool_parameter(
      const cppmu::String& name,
      bool value
  ) = 0;
  virtual MockActualCall& with_int_parameter(
      const cppmu::String& name,
      int value
  ) = 0;
  virtual MockActualCall& with_unsigned_int_parameter(
      const cppmu::String& name,
      unsigned int value
  ) = 0;
  virtual MockActualCall& with_long_int_parameter(
      const cppmu::String& name,
      long int value
  ) = 0;
  virtual MockActualCall& with_unsigned_long_int_parameter(
      const cppmu::String& name,
      unsigned long int value
  ) = 0;
  virtual MockActualCall& with_long_long_int_parameter(
      const cppmu::String& name,
      long long value
  ) = 0;
  virtual MockActualCall& with_unsigned_long_long_int_parameter(
      const cppmu::String& name,
      unsigned long long value
  ) = 0;
  virtual MockActualCall& with_double_parameter(
      const cppmu::String& name,
      double value
  ) = 0;
  virtual MockActualCall& with_string_parameter(
      const cppmu::String& name,
      const char* value
  ) = 0;
  virtual MockActualCall& with_pointer_parameter(
      const cppmu::String& name,
      void* value
  ) = 0;
  virtual MockActualCall& with_function_pointer_parameter(
      const cppmu::String& name,
      void (*value)()
  ) = 0;
  virtual MockActualCall& with_const_pointer_parameter(
      const cppmu::String& name,
      const void* value
  ) = 0;
  virtual MockActualCall& with_memory_buffer_parameter(
      const cppmu::String& name,
      const unsigned char* value,
      size_t size
  ) = 0;
  virtual MockActualCall& with_memory_buffer_parameter(
      const char* name,
      const unsigned char* value,
      size_t size
  ) = 0;

  virtual bool has_return_value() = 0;
  virtual MockNamedValue return_value() = 0;

  virtual bool return_bool_value_or_default(bool default_value) = 0;
  virtual bool return_bool_value() = 0;

  virtual int return_int_value_or_default(int default_value) = 0;
  virtual int return_int_value() = 0;

  virtual unsigned long int return_unsigned_long_int_value() = 0;
  virtual unsigned long int return_unsigned_long_int_value_or_default(
      unsigned long int default_value
  ) = 0;

  virtual long int return_long_int_value() = 0;
  virtual long int return_long_int_value_or_default(long int default_value) = 0;

  virtual unsigned long long return_unsigned_long_long_int_value() = 0;
  virtual unsigned long long return_unsigned_long_long_int_value_or_default(
      unsigned long long default_value
  ) = 0;

  virtual long long return_long_long_int_value() = 0;
  virtual long long return_long_long_int_value_or_default(
      long long default_value
  ) = 0;

  virtual unsigned int return_unsigned_int_value() = 0;
  virtual unsigned int return_unsigned_int_value_or_default(
      unsigned int default_value
  ) = 0;

  virtual const char* return_string_value_or_default(
      const char* default_value
  ) = 0;
  virtual const char* return_string_value() = 0;

  virtual double return_double_value() = 0;
  virtual double return_double_value_or_default(double default_value) = 0;

  virtual void* return_pointer_value() = 0;
  virtual void* return_pointer_value_or_default(void* default_value) = 0;

  virtual const void* return_const_pointer_value() = 0;
  virtual const void* return_const_pointer_value_or_default(
      const void* default_value
  ) = 0;

  using FunctionPointerReturnValue = void (*)();
  virtual FunctionPointerReturnValue return_function_pointer_value() = 0;
  virtual FunctionPointerReturnValue return_function_pointer_value_or_default(
      void (*default_value)()
  ) = 0;

  virtual MockActualCall& on_object(const void* object_ptr) = 0;
};

} // namespace cppmu

#endif
