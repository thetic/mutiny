#ifndef INCLUDED_CPPUTEST_EXTENSIONS_MOCKIGNOREDACTUALCALL_HPP
#define INCLUDED_CPPUTEST_EXTENSIONS_MOCKIGNOREDACTUALCALL_HPP

#include "CppUTest/Extensions/MockActualCall.hpp"

namespace cpputest {
namespace extensions {

class MockIgnoredActualCall : public MockActualCall
{
public:
  virtual MockActualCall& with_name(const cpputest::String&) override
  {
    return *this;
  }
  virtual MockActualCall& with_call_order(unsigned int) override
  {
    return *this;
  }
  virtual MockActualCall& with_bool_parameter(const cpputest::String&,
      bool) override
  {
    return *this;
  }
  virtual MockActualCall& with_int_parameter(const cpputest::String&,
      int) override
  {
    return *this;
  }
  virtual MockActualCall& with_unsigned_int_parameter(const cpputest::String&,
      unsigned int) override
  {
    return *this;
  }
  virtual MockActualCall& with_long_int_parameter(const cpputest::String&,
      long int) override
  {
    return *this;
  }
  virtual MockActualCall& with_unsigned_long_int_parameter(
      const cpputest::String&,
      unsigned long int) override
  {
    return *this;
  }
  virtual MockActualCall& with_long_long_int_parameter(const cpputest::String&,
      long long) override
  {
    return *this;
  }
  virtual MockActualCall& with_unsigned_long_long_int_parameter(
      const cpputest::String&,
      unsigned long long) override
  {
    return *this;
  }
  virtual MockActualCall& with_double_parameter(const cpputest::String&,
      double) override
  {
    return *this;
  }
  virtual MockActualCall& with_string_parameter(const cpputest::String&,
      const char*) override
  {
    return *this;
  }
  virtual MockActualCall& with_pointer_parameter(const cpputest::String&,
      void*) override
  {
    return *this;
  }
  virtual MockActualCall& with_const_pointer_parameter(const cpputest::String&,
      const void*) override
  {
    return *this;
  }
  virtual MockActualCall& with_function_pointer_parameter(
      const cpputest::String&,
      void (*)()) override
  {
    return *this;
  }
  virtual MockActualCall& with_memory_buffer_parameter(const cpputest::String&,
      const unsigned char*,
      size_t) override
  {
    return *this;
  }
  virtual MockActualCall& with_memory_buffer_parameter(const char*,
      const unsigned char*,
      size_t) override
  {
    return *this;
  }
  virtual MockActualCall& with_parameter_of_type(const cpputest::String&,
      const cpputest::String&,
      const void*) override
  {
    return *this;
  }
  virtual MockActualCall& with_parameter_of_type(const char*,
      const char*,
      const void*) override
  {
    return *this;
  }
  virtual MockActualCall& with_output_parameter(const cpputest::String&,
      void*) override
  {
    return *this;
  }
  virtual MockActualCall& with_output_parameter_of_type(const cpputest::String&,
      const cpputest::String&,
      void*) override
  {
    return *this;
  }

  virtual bool has_return_value() override { return false; }
  virtual MockNamedValue return_value() override { return MockNamedValue(""); }

  virtual bool return_bool_value_or_default(bool value) override
  {
    return value;
  }
  virtual bool return_bool_value() override { return false; }

  virtual int return_int_value() override { return 0; }
  virtual int return_int_value_or_default(int value) override { return value; }

  virtual unsigned long int return_unsigned_long_int_value() override
  {
    return 0;
  }
  virtual unsigned long int return_unsigned_long_int_value_or_default(
      unsigned long int value) override
  {
    return value;
  }

  virtual long int return_long_int_value() override { return 0; }
  virtual long int return_long_int_value_or_default(long int value) override
  {
    return value;
  }

  virtual unsigned long long return_unsigned_long_long_int_value() override
  {
    return 0;
  }

  virtual unsigned long long return_unsigned_long_long_int_value_or_default(
      unsigned long long value) override
  {
    return value;
  }

  virtual long long return_long_long_int_value() override { return 0; }

  virtual long long return_long_long_int_value_or_default(
      long long value) override
  {
    return value;
  }

  virtual unsigned int return_unsigned_int_value() override { return 0; }
  virtual unsigned int return_unsigned_int_value_or_default(
      unsigned int value) override
  {
    return value;
  }

  virtual double return_double_value() override { return 0.0; }
  virtual double return_double_value_or_default(double value) override
  {
    return value;
  }

  virtual const char* return_string_value() override { return ""; }
  virtual const char* return_string_value_or_default(const char* value) override
  {
    return value;
  }

  virtual void* return_pointer_value() override { return nullptr; }
  virtual void* return_pointer_value_or_default(void* value) override
  {
    return value;
  }

  virtual const void* return_const_pointer_value() override { return nullptr; }
  virtual const void* return_const_pointer_value_or_default(
      const void* value) override
  {
    return value;
  }

  virtual FunctionPointerReturnValue return_function_pointer_value() override
  {
    return nullptr;
  }
  virtual FunctionPointerReturnValue return_function_pointer_value_or_default(
      void (*value)()) override
  {
    return value;
  }

  virtual MockActualCall& on_object(const void*) override { return *this; }

  static MockIgnoredActualCall& instance()
  {
    static MockIgnoredActualCall call;
    return call;
  }
};

} // namespace extensions
} // namespace cpputest

#endif
