#ifndef INCLUDED_MUTINY_MOCK_ACTUALCALLTRACE_HPP
#define INCLUDED_MUTINY_MOCK_ACTUALCALLTRACE_HPP

#include "mutiny/mock/ActualCall.hpp"

namespace mu {
namespace tiny {
namespace mock {

class ActualCallTrace : public ActualCall
{
public:
  ActualCallTrace() = default;
  ~ActualCallTrace() override = default;

  ActualCall& with_name(const String& name) override;
  ActualCall& with_call_order(unsigned int) override;
  ActualCall& with_typed_parameter(NamedValue parameter) override;
  ActualCall& with_parameter_of_type(
      const String& type_name,
      const String& name,
      const void* value
  ) override;
  ActualCall& with_parameter_of_type(
      const char* type_name,
      const char* name,
      const void* value
  ) override;
  ActualCall& with_output_parameter(const String& name, void* output) override;
  ActualCall& with_output_parameter_of_type(
      const String& type_name,
      const String& name,
      void* output
  ) override;

  bool has_return_value() override;
  NamedValue return_value() override;

  ActualCall& on_object(const void* object_ptr) override;

  const char* get_trace_output();
  void clear();
  static ActualCallTrace& instance();
  static void clear_instance();

private:
  String trace_buffer_;

  static ActualCallTrace* instance_;

  void add_parameter_name(const String& name);
};

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
