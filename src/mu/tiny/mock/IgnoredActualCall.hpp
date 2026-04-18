#ifndef INCLUDED_MU_TINY_MOCK_IGNOREDACTUALCALL_HPP
#define INCLUDED_MU_TINY_MOCK_IGNOREDACTUALCALL_HPP

#include "mu/tiny/mock/ActualCall.hpp"

namespace mu {
namespace tiny {
namespace mock {

class IgnoredActualCall : public ActualCall
{
public:
  ActualCall& with_name(const String&) override { return *this; }
  ActualCall& with_call_order(unsigned int) override { return *this; }

  ActualCall& with_typed_parameter(NamedValue) override { return *this; }

  ActualCall& with_parameter_of_type(
      const String&,
      const String&,
      const void*
  ) override
  {
    return *this;
  }
  ActualCall& with_parameter_of_type(
      const char*,
      const char*,
      const void*
  ) override
  {
    return *this;
  }
  ActualCall& with_output_parameter(const String&, void*) override
  {
    return *this;
  }
  ActualCall& with_output_parameter_of_type(
      const String&,
      const String&,
      void*
  ) override
  {
    return *this;
  }

  bool has_return_value() override { return false; }
  NamedValue return_value() override { return NamedValue(""); }

  ActualCall& on_object(const void*) override { return *this; }

  static IgnoredActualCall& instance()
  {
    static IgnoredActualCall call;
    return call;
  }
};

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
