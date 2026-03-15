#ifndef INCLUDED_CPPUTEST_EXTENSIONS_MOCKEXPECTEDCALL_HPP
#define INCLUDED_CPPUTEST_EXTENSIONS_MOCKEXPECTEDCALL_HPP

#include "CppUTest/String.hpp"

namespace cpputest {
namespace extensions {

class MockNamedValue;

cpputest::String
string_from(const MockNamedValue& parameter);

class MockExpectedCall
{
public:
  MockExpectedCall();
  virtual ~MockExpectedCall();

  virtual MockExpectedCall& with_name(const cpputest::String& name) = 0;
  virtual MockExpectedCall& with_call_order(unsigned int) = 0;
  virtual MockExpectedCall& with_call_order(unsigned int, unsigned int) = 0;
  MockExpectedCall& with_parameter(const cpputest::String& name, bool value)
  {
    return with_bool_parameter(name, value);
  }
  MockExpectedCall& with_parameter(const cpputest::String& name, int value)
  {
    return with_int_parameter(name, value);
  }
  MockExpectedCall& with_parameter(const cpputest::String& name,
      unsigned int value)
  {
    return with_unsigned_int_parameter(name, value);
  }
  MockExpectedCall& with_parameter(const cpputest::String& name, long int value)
  {
    return with_long_int_parameter(name, value);
  }
  MockExpectedCall& with_parameter(const cpputest::String& name,
      unsigned long int value)
  {
    return with_unsigned_long_int_parameter(name, value);
  }
  MockExpectedCall& with_parameter(const cpputest::String& name,
      long long value)
  {
    return with_long_long_int_parameter(name, value);
  }
  MockExpectedCall& with_parameter(const cpputest::String& name,
      unsigned long long value)
  {
    return with_unsigned_long_long_int_parameter(name, value);
  }
  MockExpectedCall& with_parameter(const cpputest::String& name, double value)
  {
    return with_double_parameter(name, value);
  }
  MockExpectedCall& with_parameter(const cpputest::String& name,
      double value,
      double tolerance)
  {
    return with_double_parameter(name, value, tolerance);
  }
  MockExpectedCall& with_parameter(const cpputest::String& name,
      const char* value)
  {
    return with_string_parameter(name, value);
  }
  MockExpectedCall& with_parameter(const cpputest::String& name, void* value)
  {
    return with_pointer_parameter(name, value);
  }
  MockExpectedCall& with_parameter(const cpputest::String& name,
      const void* value)
  {
    return with_const_pointer_parameter(name, value);
  }
  MockExpectedCall& with_parameter(const cpputest::String& name,
      void (*value)())
  {
    return with_function_pointer_parameter(name, value);
  }
  MockExpectedCall& with_parameter(const cpputest::String& name,
      const unsigned char* value,
      size_t size)
  {
    return with_memory_buffer_parameter(name, value, size);
  }
  virtual MockExpectedCall& with_parameter_of_type(
      const cpputest::String& type_name,
      const cpputest::String& name,
      const void* value) = 0;
  virtual MockExpectedCall& with_output_parameter_returning(
      const cpputest::String& name,
      const void* value,
      size_t size) = 0;
  virtual MockExpectedCall& with_output_parameter_of_type_returning(
      const cpputest::String& type_name,
      const cpputest::String& name,
      const void* value) = 0;
  virtual MockExpectedCall& with_unmodified_output_parameter(
      const cpputest::String& name) = 0;
  virtual MockExpectedCall& ignore_other_parameters() = 0;

  virtual MockExpectedCall& with_bool_parameter(const cpputest::String& name,
      bool value) = 0;
  virtual MockExpectedCall& with_int_parameter(const cpputest::String& name,
      int value) = 0;
  virtual MockExpectedCall& with_unsigned_int_parameter(
      const cpputest::String& name,
      unsigned int value) = 0;
  virtual MockExpectedCall& with_long_int_parameter(
      const cpputest::String& name,
      long int value) = 0;
  virtual MockExpectedCall& with_unsigned_long_int_parameter(
      const cpputest::String& name,
      unsigned long int value) = 0;
  virtual MockExpectedCall& with_long_long_int_parameter(
      const cpputest::String& name,
      long long value) = 0;
  virtual MockExpectedCall& with_unsigned_long_long_int_parameter(
      const cpputest::String& name,
      unsigned long long value) = 0;
  virtual MockExpectedCall& with_double_parameter(const cpputest::String& name,
      double value) = 0;
  virtual MockExpectedCall& with_double_parameter(const cpputest::String& name,
      double value,
      double tolerance) = 0;
  virtual MockExpectedCall& with_string_parameter(const cpputest::String& name,
      const char* value) = 0;
  virtual MockExpectedCall& with_pointer_parameter(const cpputest::String& name,
      void* value) = 0;
  virtual MockExpectedCall& with_function_pointer_parameter(
      const cpputest::String& name,
      void (*value)()) = 0;
  virtual MockExpectedCall& with_const_pointer_parameter(
      const cpputest::String& name,
      const void* value) = 0;
  virtual MockExpectedCall& with_memory_buffer_parameter(
      const cpputest::String& name,
      const unsigned char* value,
      size_t size) = 0;
  virtual MockExpectedCall& and_return_value(bool value) = 0;
  virtual MockExpectedCall& and_return_value(int value) = 0;
  virtual MockExpectedCall& and_return_value(unsigned int value) = 0;
  virtual MockExpectedCall& and_return_value(long int value) = 0;
  virtual MockExpectedCall& and_return_value(unsigned long int value) = 0;
  virtual MockExpectedCall& and_return_value(long long value) = 0;
  virtual MockExpectedCall& and_return_value(unsigned long long value) = 0;
  virtual MockExpectedCall& and_return_value(double value) = 0;
  virtual MockExpectedCall& and_return_value(const char* value) = 0;
  virtual MockExpectedCall& and_return_value(void* value) = 0;
  virtual MockExpectedCall& and_return_value(const void* value) = 0;
  virtual MockExpectedCall& and_return_value(void (*value)()) = 0;

  virtual MockExpectedCall& on_object(void* object_ptr) = 0;
};

} // namespace extensions
} // namespace cpputest

#endif
