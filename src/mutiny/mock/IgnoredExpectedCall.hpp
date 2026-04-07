#ifndef INCLUDED_MUTINY_MOCK_IGNOREDEXPECTEDCALL_HPP
#define INCLUDED_MUTINY_MOCK_IGNOREDEXPECTEDCALL_HPP

#include "mutiny/mock/ExpectedCall.hpp"

namespace mu {
namespace tiny {
namespace mock {

class IgnoredExpectedCall : public ExpectedCall
{
public:
  ExpectedCall& with_name(const String&) override { return *this; }
  ExpectedCall& with_call_order(unsigned int) override { return *this; }
  ExpectedCall& with_call_order(unsigned int, unsigned int) override
  {
    return *this;
  }
  ExpectedCall& with_parameter_of_type(
      const String&,
      const String&,
      const void*
  ) override
  {
    return *this;
  }
  ExpectedCall& with_output_parameter_returning(
      const String&,
      const void*,
      size_t
  ) override
  {
    return *this;
  }
  ExpectedCall& with_output_parameter_of_type_returning(
      const String&,
      const String&,
      const void*
  ) override
  {
    return *this;
  }
  ExpectedCall& with_unmodified_output_parameter(const String&) override
  {
    return *this;
  }
  ExpectedCall& ignore_other_parameters() override { return *this; }

  using ExpectedCall::and_return_value;
  ExpectedCall& and_return_value(bool) override { return *this; }
  ExpectedCall& and_return_value(long long) override { return *this; }
  ExpectedCall& and_return_value(unsigned long long) override { return *this; }
  ExpectedCall& and_return_value(double) override { return *this; }
  ExpectedCall& and_return_value(const char*) override { return *this; }
  ExpectedCall& and_return_value(void*) override { return *this; }
  ExpectedCall& and_return_value(const void*) override { return *this; }
  ExpectedCall& and_return_value(void (*)()) override { return *this; }

  ExpectedCall& on_object(void*) override { return *this; }

  static ExpectedCall& instance();

private:
  // NVI back-end: ignored implementations of typed pure virtuals
  ExpectedCall& with_bool_parameter(const String&, bool) override
  {
    return *this;
  }
  ExpectedCall& with_int_parameter(const String&, int) override
  {
    return *this;
  }
  ExpectedCall& with_unsigned_int_parameter(
      const String&,
      unsigned int
  ) override
  {
    return *this;
  }
  ExpectedCall& with_long_int_parameter(const String&, long int) override
  {
    return *this;
  }
  ExpectedCall& with_unsigned_long_int_parameter(
      const String&,
      unsigned long int
  ) override
  {
    return *this;
  }
  ExpectedCall& with_long_long_int_parameter(const String&, long long) override
  {
    return *this;
  }
  ExpectedCall& with_unsigned_long_long_int_parameter(
      const String&,
      unsigned long long
  ) override
  {
    return *this;
  }
  ExpectedCall& with_double_parameter(const String&, double) override
  {
    return *this;
  }
  ExpectedCall& with_double_parameter(const String&, double, double) override
  {
    return *this;
  }
  ExpectedCall& with_string_parameter(const String&, const char*) override
  {
    return *this;
  }
  ExpectedCall& with_pointer_parameter(const String&, void*) override
  {
    return *this;
  }
  ExpectedCall& with_const_pointer_parameter(
      const String&,
      const void*
  ) override
  {
    return *this;
  }
  ExpectedCall& with_function_pointer_parameter(
      const String&,
      void (*)()
  ) override
  {
    return *this;
  }
  ExpectedCall& with_memory_buffer_parameter(
      const String&,
      const unsigned char*,
      size_t
  ) override
  {
    return *this;
  }
};

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
