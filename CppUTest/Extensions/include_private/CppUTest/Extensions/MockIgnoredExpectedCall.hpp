#ifndef INCLUDED_CPPUTEST_EXTENSIONS_MOCKIGNOREDEXPECTEDCALL_HPP
#define INCLUDED_CPPUTEST_EXTENSIONS_MOCKIGNOREDEXPECTEDCALL_HPP

#include "CppUTest/Extensions/MockExpectedCall.hpp"

namespace cpputest {
namespace extensions {

class MockIgnoredExpectedCall : public MockExpectedCall
{
public:
  virtual MockExpectedCall& with_name(const cpputest::String&) override
  {
    return *this;
  }
  virtual MockExpectedCall& with_call_order(unsigned int) override
  {
    return *this;
  }
  virtual MockExpectedCall& with_call_order(unsigned int, unsigned int) override
  {
    return *this;
  }
  virtual MockExpectedCall& with_bool_parameter(const cpputest::String&,
      bool) override
  {
    return *this;
  }
  virtual MockExpectedCall& with_int_parameter(const cpputest::String&,
      int) override
  {
    return *this;
  }
  virtual MockExpectedCall& with_unsigned_int_parameter(const cpputest::String&,
      unsigned int) override
  {
    return *this;
  }
  virtual MockExpectedCall& with_long_int_parameter(const cpputest::String&,
      long int) override
  {
    return *this;
  }
  virtual MockExpectedCall& with_unsigned_long_int_parameter(
      const cpputest::String&,
      unsigned long int) override
  {
    return *this;
  }
  virtual MockExpectedCall& with_long_long_int_parameter(
      const cpputest::String&,
      long long) override
  {
    return *this;
  }
  virtual MockExpectedCall& with_unsigned_long_long_int_parameter(
      const cpputest::String&,
      unsigned long long) override
  {
    return *this;
  }
  virtual MockExpectedCall& with_double_parameter(const cpputest::String&,
      double) override
  {
    return *this;
  }
  virtual MockExpectedCall& with_double_parameter(const cpputest::String&,
      double,
      double) override
  {
    return *this;
  }
  virtual MockExpectedCall& with_string_parameter(const cpputest::String&,
      const char*) override
  {
    return *this;
  }
  virtual MockExpectedCall& with_pointer_parameter(const cpputest::String&,
      void*) override
  {
    return *this;
  }
  virtual MockExpectedCall& with_const_pointer_parameter(
      const cpputest::String&,
      const void*) override
  {
    return *this;
  }
  virtual MockExpectedCall& with_function_pointer_parameter(
      const cpputest::String&,
      void (*)()) override
  {
    return *this;
  }
  virtual MockExpectedCall& with_memory_buffer_parameter(
      const cpputest::String&,
      const unsigned char*,
      size_t) override
  {
    return *this;
  }
  virtual MockExpectedCall& with_parameter_of_type(const cpputest::String&,
      const cpputest::String&,
      const void*) override
  {
    return *this;
  }
  virtual MockExpectedCall& with_output_parameter_returning(
      const cpputest::String&,
      const void*,
      size_t) override
  {
    return *this;
  }
  virtual MockExpectedCall& with_output_parameter_of_type_returning(
      const cpputest::String&,
      const cpputest::String&,
      const void*) override
  {
    return *this;
  }
  virtual MockExpectedCall& with_unmodified_output_parameter(
      const cpputest::String&) override
  {
    return *this;
  }
  virtual MockExpectedCall& ignore_other_parameters() override { return *this; }

  virtual MockExpectedCall& and_return_value(bool) override { return *this; }
  virtual MockExpectedCall& and_return_value(int) override { return *this; }
  virtual MockExpectedCall& and_return_value(unsigned int) override
  {
    return *this;
  }
  virtual MockExpectedCall& and_return_value(long int) override
  {
    return *this;
  }
  virtual MockExpectedCall& and_return_value(unsigned long int) override
  {
    return *this;
  }
  virtual MockExpectedCall& and_return_value(long long) override
  {
    return *this;
  }
  virtual MockExpectedCall& and_return_value(unsigned long long) override
  {
    return *this;
  }
  virtual MockExpectedCall& and_return_value(double) override { return *this; }
  virtual MockExpectedCall& and_return_value(const char*) override
  {
    return *this;
  }
  virtual MockExpectedCall& and_return_value(void*) override { return *this; }
  virtual MockExpectedCall& and_return_value(const void*) override
  {
    return *this;
  }
  virtual MockExpectedCall& and_return_value(void (*)()) override
  {
    return *this;
  }

  virtual MockExpectedCall& on_object(void*) override { return *this; }

  static MockExpectedCall& instance();
};

} // namespace extensions
} // namespace cpputest

#endif
