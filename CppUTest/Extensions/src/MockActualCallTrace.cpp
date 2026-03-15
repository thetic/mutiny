#include "CppUTest/Extensions/MockActualCallTrace.hpp"

#include "CppUTest/TestHarness.hpp"

namespace cpputest {
namespace extensions {

MockActualCallTrace* MockActualCallTrace::instance_ = nullptr;

MockActualCallTrace::MockActualCallTrace() {}

MockActualCallTrace::~MockActualCallTrace() {}

MockActualCall&
MockActualCallTrace::with_name(const String& name)
{
  trace_buffer_ += "\nFunction name:";
  trace_buffer_ += name;
  return *this;
}

MockActualCall&
MockActualCallTrace::with_call_order(unsigned int call_order)
{
  trace_buffer_ += " withCallOrder:";
  trace_buffer_ += cpputest::string_from(call_order);
  return *this;
}

void
MockActualCallTrace::add_parameter_name(const String& name)
{
  trace_buffer_ += " ";
  trace_buffer_ += name;
  trace_buffer_ += ":";
}

MockActualCall&
MockActualCallTrace::with_bool_parameter(const String& name, bool value)
{
  add_parameter_name(name);
  trace_buffer_ += cpputest::string_from(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::with_unsigned_int_parameter(const String& name,
    unsigned int value)
{
  add_parameter_name(name);
  trace_buffer_ += cpputest::string_from(value) + " " +
                   brackets_formatted_hex_string_from(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::with_int_parameter(const String& name, int value)
{
  add_parameter_name(name);
  trace_buffer_ += cpputest::string_from(value) + " " +
                   brackets_formatted_hex_string_from(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::with_unsigned_long_int_parameter(const String& name,
    unsigned long int value)
{
  add_parameter_name(name);
  trace_buffer_ += cpputest::string_from(value) + " " +
                   brackets_formatted_hex_string_from(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::with_long_int_parameter(const String& name, long int value)
{
  add_parameter_name(name);
  trace_buffer_ += cpputest::string_from(value) + " " +
                   brackets_formatted_hex_string_from(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::with_unsigned_long_long_int_parameter(const String& name,
    unsigned long long value)
{
  add_parameter_name(name);
  trace_buffer_ += cpputest::string_from(value) + " " +
                   brackets_formatted_hex_string_from(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::with_long_long_int_parameter(const String& name,
    long long value)
{
  add_parameter_name(name);
  trace_buffer_ += cpputest::string_from(value) + " " +
                   brackets_formatted_hex_string_from(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::with_double_parameter(const String& name, double value)
{
  add_parameter_name(name);
  trace_buffer_ += cpputest::string_from(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::with_string_parameter(const String& name,
    const char* value)
{
  add_parameter_name(name);
  trace_buffer_ += cpputest::string_from(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::with_pointer_parameter(const String& name, void* value)
{
  add_parameter_name(name);
  trace_buffer_ += cpputest::string_from(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::with_const_pointer_parameter(const String& name,
    const void* value)
{
  add_parameter_name(name);
  trace_buffer_ += cpputest::string_from(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::with_function_pointer_parameter(const String& name,
    void (*value)())
{
  add_parameter_name(name);
  trace_buffer_ += cpputest::string_from(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::with_memory_buffer_parameter(const String& name,
    const unsigned char* value,
    size_t size)
{
  add_parameter_name(name);
  trace_buffer_ += string_from_binary_with_size_or_null(value, size);
  return *this;
}

MockActualCall&
MockActualCallTrace::with_parameter_of_type(const String& type_name,
    const String& name,
    const void* value)
{
  trace_buffer_ += " ";
  trace_buffer_ += type_name;
  add_parameter_name(name);
  trace_buffer_ += cpputest::string_from(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::with_memory_buffer_parameter(const char* name,
    const unsigned char* value,
    size_t size)
{
  return with_memory_buffer_parameter(String(name), value, size);
}

MockActualCall&
MockActualCallTrace::with_parameter_of_type(const char* type_name,
    const char* name,
    const void* value)
{
  return with_parameter_of_type(String(type_name), String(name), value);
}

MockActualCall&
MockActualCallTrace::with_output_parameter(const String& name, void* output)
{
  add_parameter_name(name);
  trace_buffer_ += cpputest::string_from(output);
  return *this;
}

MockActualCall&
MockActualCallTrace::with_output_parameter_of_type(const String& type_name,
    const String& name,
    void* output)
{
  trace_buffer_ += " ";
  trace_buffer_ += type_name;
  add_parameter_name(name);
  trace_buffer_ += cpputest::string_from(output);
  return *this;
}

bool
MockActualCallTrace::has_return_value()
{
  return false;
}

MockNamedValue
MockActualCallTrace::return_value()
{
  return MockNamedValue("");
}

long int
MockActualCallTrace::return_long_int_value()
{
  return 0;
}

unsigned long int
MockActualCallTrace::return_unsigned_long_int_value()
{
  return 0;
}

unsigned long int
MockActualCallTrace::return_unsigned_long_int_value_or_default(unsigned long)
{
  return 0;
}

long int
MockActualCallTrace::return_long_int_value_or_default(long int)
{
  return 0;
}

long long
MockActualCallTrace::return_long_long_int_value()
{
  return 0;
}

unsigned long long
MockActualCallTrace::return_unsigned_long_long_int_value()
{
  return 0;
}

unsigned long long
MockActualCallTrace::return_unsigned_long_long_int_value_or_default(
    unsigned long long)
{
  return 0;
}

long long
MockActualCallTrace::return_long_long_int_value_or_default(long long)
{
  return 0;
}

bool
MockActualCallTrace::return_bool_value()
{
  return false;
}

bool
MockActualCallTrace::return_bool_value_or_default(bool)
{
  return false;
}

int
MockActualCallTrace::return_int_value()
{
  return 0;
}

double
MockActualCallTrace::return_double_value()
{
  return 0.0;
}

double
MockActualCallTrace::return_double_value_or_default(double)
{
  return return_double_value();
}

unsigned int
MockActualCallTrace::return_unsigned_int_value()
{
  return 0;
}

void*
MockActualCallTrace::return_pointer_value()
{
  return nullptr;
}

const void*
MockActualCallTrace::return_const_pointer_value()
{
  return nullptr;
}

MockActualCall::FunctionPointerReturnValue
MockActualCallTrace::return_function_pointer_value()
{
  return nullptr;
}

const void*
MockActualCallTrace::return_const_pointer_value_or_default(const void*)
{
  return return_const_pointer_value();
}

void*
MockActualCallTrace::return_pointer_value_or_default(void*)
{
  return return_pointer_value();
}

MockActualCall::FunctionPointerReturnValue
MockActualCallTrace::return_function_pointer_value_or_default(void (*)())
{
  return return_function_pointer_value();
}

const char*
MockActualCallTrace::return_string_value()
{
  return "";
}

const char*
MockActualCallTrace::return_string_value_or_default(const char*)
{
  return return_string_value();
}

int
MockActualCallTrace::return_int_value_or_default(int)
{
  return 0;
}

unsigned int
MockActualCallTrace::return_unsigned_int_value_or_default(unsigned int)
{
  return return_unsigned_int_value();
}

MockActualCall&
MockActualCallTrace::on_object(const void* object_ptr)
{
  trace_buffer_ += " onObject:";
  trace_buffer_ += cpputest::string_from(object_ptr);
  return *this;
}

void
MockActualCallTrace::clear()
{
  trace_buffer_ = "";
}

const char*
MockActualCallTrace::get_trace_output()
{
  return trace_buffer_.c_str();
}

MockActualCallTrace&
MockActualCallTrace::instance()
{
  if (instance_ == nullptr)
    instance_ = new MockActualCallTrace;
  return *instance_;
}

void
MockActualCallTrace::clear_instance()
{
  delete instance_;
  instance_ = nullptr;
}

} // namespace extensions
} // namespace cpputest
