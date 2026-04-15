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
  ExpectedCall& with_typed_parameter(NamedValue) override { return *this; }
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

  ExpectedCall& and_return_typed_value(NamedValue) override { return *this; }

  ExpectedCall& on_object(void*) override { return *this; }

  static ExpectedCall& instance();
};

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
