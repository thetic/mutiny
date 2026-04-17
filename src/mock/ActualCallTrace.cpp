#include "mutiny/mock/ActualCallTrace.hpp"

namespace mu {
namespace tiny {
namespace mock {

ActualCallTrace* ActualCallTrace::instance_ = nullptr;

ActualCall& ActualCallTrace::with_name(const String& name)
{
  trace_buffer_ += "\nFunction name:";
  trace_buffer_ += name;
  return *this;
}

ActualCall& ActualCallTrace::with_call_order(unsigned int call_order)
{
  trace_buffer_ += " withCallOrder:";
  trace_buffer_ += string_from(call_order);
  return *this;
}

void ActualCallTrace::add_parameter_name(const String& name)
{
  trace_buffer_ += " ";
  trace_buffer_ += name;
  trace_buffer_ += ":";
}

ActualCall& ActualCallTrace::with_typed_parameter(NamedValue parameter)
{
  add_parameter_name(parameter.get_name());
  trace_buffer_ += parameter.to_string();
  return *this;
}

ActualCall& ActualCallTrace::with_parameter_of_type(
    const String& type_name,
    const String& name,
    const void* value
)
{
  trace_buffer_ += " ";
  trace_buffer_ += type_name;
  add_parameter_name(name);
  trace_buffer_ += string_from(value);
  return *this;
}

ActualCall& ActualCallTrace::with_parameter_of_type(
    const char* type_name,
    const char* name,
    const void* value
)
{
  return with_parameter_of_type(String(type_name), String(name), value);
}

ActualCall& ActualCallTrace::with_output_parameter(
    const String& name,
    void* output
)
{
  add_parameter_name(name);
  trace_buffer_ += string_from(output);
  return *this;
}

ActualCall& ActualCallTrace::with_output_parameter_of_type(
    const String& type_name,
    const String& name,
    void* output
)
{
  trace_buffer_ += " ";
  trace_buffer_ += type_name;
  add_parameter_name(name);
  trace_buffer_ += string_from(output);
  return *this;
}

bool ActualCallTrace::has_return_value()
{
  return false;
}

NamedValue ActualCallTrace::return_value()
{
  return NamedValue("");
}

ActualCall& ActualCallTrace::on_object(const void* object_ptr)
{
  trace_buffer_ += " onObject:";
  trace_buffer_ += string_from(object_ptr);
  return *this;
}

const char* ActualCallTrace::get_trace_output()
{
  return trace_buffer_.c_str();
}

ActualCallTrace& ActualCallTrace::instance()
{
  if (instance_ == nullptr)
    instance_ = new ActualCallTrace;
  return *instance_;
}

void ActualCallTrace::clear_instance()
{
  delete instance_;
  instance_ = nullptr;
}

} // namespace mock
} // namespace tiny
} // namespace mu
