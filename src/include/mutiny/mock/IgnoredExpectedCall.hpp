#ifndef INCLUDED_MUTINY_MOCKIGNOREDEXPECTEDCALL_HPP
#define INCLUDED_MUTINY_MOCKIGNOREDEXPECTEDCALL_HPP

#include "mutiny/mock/ExpectedCall.hpp"

namespace mu {
namespace tiny {
namespace mock {

class MockIgnoredExpectedCall : public MockExpectedCall
{
public:
  MockExpectedCall& with_name(const mu::tiny::test::String&) override
  {
    return *this;
  }
  MockExpectedCall& with_call_order(unsigned int) override { return *this; }
  MockExpectedCall& with_call_order(unsigned int, unsigned int) override
  {
    return *this;
  }
  MockExpectedCall& with_bool_parameter(
      const mu::tiny::test::String&,
      bool
  ) override
  {
    return *this;
  }
  MockExpectedCall& with_int_parameter(
      const mu::tiny::test::String&,
      int
  ) override
  {
    return *this;
  }
  MockExpectedCall& with_unsigned_int_parameter(
      const mu::tiny::test::String&,
      unsigned int
  ) override
  {
    return *this;
  }
  MockExpectedCall& with_long_int_parameter(
      const mu::tiny::test::String&,
      long int
  ) override
  {
    return *this;
  }
  MockExpectedCall& with_unsigned_long_int_parameter(
      const mu::tiny::test::String&,
      unsigned long int
  ) override
  {
    return *this;
  }
  MockExpectedCall& with_long_long_int_parameter(
      const mu::tiny::test::String&,
      long long
  ) override
  {
    return *this;
  }
  MockExpectedCall& with_unsigned_long_long_int_parameter(
      const mu::tiny::test::String&,
      unsigned long long
  ) override
  {
    return *this;
  }
  MockExpectedCall& with_double_parameter(
      const mu::tiny::test::String&,
      double
  ) override
  {
    return *this;
  }
  MockExpectedCall& with_double_parameter(
      const mu::tiny::test::String&,
      double,
      double
  ) override
  {
    return *this;
  }
  MockExpectedCall& with_string_parameter(
      const mu::tiny::test::String&,
      const char*
  ) override
  {
    return *this;
  }
  MockExpectedCall& with_pointer_parameter(
      const mu::tiny::test::String&,
      void*
  ) override
  {
    return *this;
  }
  MockExpectedCall& with_const_pointer_parameter(
      const mu::tiny::test::String&,
      const void*
  ) override
  {
    return *this;
  }
  MockExpectedCall& with_function_pointer_parameter(
      const mu::tiny::test::String&,
      void (*)()
  ) override
  {
    return *this;
  }
  MockExpectedCall& with_memory_buffer_parameter(
      const mu::tiny::test::String&,
      const unsigned char*,
      size_t
  ) override
  {
    return *this;
  }
  MockExpectedCall& with_parameter_of_type(
      const mu::tiny::test::String&,
      const mu::tiny::test::String&,
      const void*
  ) override
  {
    return *this;
  }
  MockExpectedCall& with_output_parameter_returning(
      const mu::tiny::test::String&,
      const void*,
      size_t
  ) override
  {
    return *this;
  }
  MockExpectedCall& with_output_parameter_of_type_returning(
      const mu::tiny::test::String&,
      const mu::tiny::test::String&,
      const void*
  ) override
  {
    return *this;
  }
  MockExpectedCall& with_unmodified_output_parameter(
      const mu::tiny::test::String&
  ) override
  {
    return *this;
  }
  MockExpectedCall& ignore_other_parameters() override { return *this; }

  MockExpectedCall& and_return_value(bool) override { return *this; }
  MockExpectedCall& and_return_value(int) override { return *this; }
  MockExpectedCall& and_return_value(unsigned int) override { return *this; }
  MockExpectedCall& and_return_value(long int) override { return *this; }
  MockExpectedCall& and_return_value(unsigned long int) override
  {
    return *this;
  }
  MockExpectedCall& and_return_value(long long) override { return *this; }
  MockExpectedCall& and_return_value(unsigned long long) override
  {
    return *this;
  }
  MockExpectedCall& and_return_value(double) override { return *this; }
  MockExpectedCall& and_return_value(const char*) override { return *this; }
  MockExpectedCall& and_return_value(void*) override { return *this; }
  MockExpectedCall& and_return_value(const void*) override { return *this; }
  MockExpectedCall& and_return_value(void (*)()) override { return *this; }

  MockExpectedCall& on_object(void*) override { return *this; }

  static MockExpectedCall& instance();
};

}
}
} // namespace mu::tiny::mock

#endif
