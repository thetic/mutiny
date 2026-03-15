#include "CppUTest/Extensions/MockCheckedExpectedCall.hpp"

#include "CppUTest/TestHarness.hpp"

namespace cpputest {
namespace extensions {

String
string_from(const MockNamedValue& parameter)
{
  return parameter.to_string();
}

void
MockCheckedExpectedCall::set_name(const String& name)
{
  function_name_ = name;
}

String
MockCheckedExpectedCall::get_name() const
{
  return function_name_;
}

MockCheckedExpectedCall::MockCheckedExpectedCall()
  : ignore_other_parameters_(false)
  , is_actual_call_match_finalized_(false)
  , initial_expected_call_order_(no_expected_call_order)
  , final_expected_call_order_(no_expected_call_order)
  , out_of_order_(false)
  , return_value_("")
  , object_ptr_(nullptr)
  , is_specific_object_expected_(false)
  , was_passed_to_object_(true)
  , actual_calls_(0)
  , expected_calls_(1)
{
  input_parameters_ = new MockNamedValueList();
  output_parameters_ = new MockNamedValueList();
}

MockCheckedExpectedCall::MockCheckedExpectedCall(unsigned int num_calls)
  : ignore_other_parameters_(false)
  , is_actual_call_match_finalized_(false)
  , initial_expected_call_order_(no_expected_call_order)
  , final_expected_call_order_(no_expected_call_order)
  , out_of_order_(false)
  , return_value_("")
  , object_ptr_(nullptr)
  , is_specific_object_expected_(false)
  , was_passed_to_object_(true)
  , actual_calls_(0)
  , expected_calls_(num_calls)
{
  input_parameters_ = new MockNamedValueList();
  output_parameters_ = new MockNamedValueList();
}

MockCheckedExpectedCall::~MockCheckedExpectedCall()
{
  input_parameters_->clear();
  delete input_parameters_;
  output_parameters_->clear();
  delete output_parameters_;
}

MockExpectedCall&
MockCheckedExpectedCall::with_name(const String& name)
{
  set_name(name);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::with_bool_parameter(const String& name, bool value)
{
  MockNamedValue* new_parameter = new MockExpectedFunctionParameter(name);
  input_parameters_->add(new_parameter);
  new_parameter->set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::with_unsigned_int_parameter(const String& name,
    unsigned int value)
{
  MockNamedValue* new_parameter = new MockExpectedFunctionParameter(name);
  input_parameters_->add(new_parameter);
  new_parameter->set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::with_int_parameter(const String& name, int value)
{
  MockNamedValue* new_parameter = new MockExpectedFunctionParameter(name);
  input_parameters_->add(new_parameter);
  new_parameter->set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::with_long_int_parameter(const String& name,
    long int value)
{
  MockNamedValue* new_parameter = new MockExpectedFunctionParameter(name);
  input_parameters_->add(new_parameter);
  new_parameter->set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::with_unsigned_long_int_parameter(const String& name,
    unsigned long int value)
{
  MockNamedValue* new_parameter = new MockExpectedFunctionParameter(name);
  input_parameters_->add(new_parameter);
  new_parameter->set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::with_long_long_int_parameter(const String& name,
    long long value)
{
  MockNamedValue* new_parameter = new MockExpectedFunctionParameter(name);
  input_parameters_->add(new_parameter);
  new_parameter->set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::with_unsigned_long_long_int_parameter(
    const String& name,
    unsigned long long value)
{
  MockNamedValue* new_parameter = new MockExpectedFunctionParameter(name);
  input_parameters_->add(new_parameter);
  new_parameter->set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::with_double_parameter(const String& name, double value)
{
  MockNamedValue* new_parameter = new MockExpectedFunctionParameter(name);
  input_parameters_->add(new_parameter);
  new_parameter->set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::with_double_parameter(const String& name,
    double value,
    double tolerance)
{
  MockNamedValue* new_parameter = new MockExpectedFunctionParameter(name);
  input_parameters_->add(new_parameter);
  new_parameter->set_value(value, tolerance);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::with_string_parameter(const String& name,
    const char* value)
{
  MockNamedValue* new_parameter = new MockExpectedFunctionParameter(name);
  input_parameters_->add(new_parameter);
  new_parameter->set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::with_pointer_parameter(const String& name, void* value)
{
  MockNamedValue* new_parameter = new MockExpectedFunctionParameter(name);
  input_parameters_->add(new_parameter);
  new_parameter->set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::with_const_pointer_parameter(const String& name,
    const void* value)
{
  MockNamedValue* new_parameter = new MockExpectedFunctionParameter(name);
  input_parameters_->add(new_parameter);
  new_parameter->set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::with_function_pointer_parameter(const String& name,
    void (*value)())
{
  MockNamedValue* new_parameter = new MockExpectedFunctionParameter(name);
  input_parameters_->add(new_parameter);
  new_parameter->set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::with_memory_buffer_parameter(const String& name,
    const unsigned char* value,
    size_t size)
{
  MockNamedValue* new_parameter = new MockExpectedFunctionParameter(name);
  input_parameters_->add(new_parameter);
  new_parameter->set_memory_buffer(value, size);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::with_parameter_of_type(const String& type,
    const String& name,
    const void* value)
{
  MockNamedValue* new_parameter = new MockExpectedFunctionParameter(name);
  input_parameters_->add(new_parameter);
  new_parameter->set_const_object_pointer(type, value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::with_output_parameter_returning(const String& name,
    const void* value,
    size_t size)
{
  MockNamedValue* new_parameter = new MockExpectedFunctionParameter(name);
  output_parameters_->add(new_parameter);
  new_parameter->set_value(value);
  new_parameter->set_size(size);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::with_output_parameter_of_type_returning(
    const String& type,
    const String& name,
    const void* value)
{
  MockNamedValue* new_parameter = new MockExpectedFunctionParameter(name);
  output_parameters_->add(new_parameter);
  new_parameter->set_const_object_pointer(type, value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::with_unmodified_output_parameter(const String& name)
{
  return with_output_parameter_returning(name, nullptr, 0);
}

String
MockCheckedExpectedCall::get_input_parameter_type(const String& name)
{
  MockNamedValue* p = input_parameters_->get_value_by_name(name);
  return (p) ? p->get_type() : cpputest::string_from("");
}

bool
MockCheckedExpectedCall::has_input_parameter_with_name(const String& name)
{
  MockNamedValue* p = input_parameters_->get_value_by_name(name);
  return p != nullptr;
}

bool
MockCheckedExpectedCall::has_output_parameter_with_name(const String& name)
{
  MockNamedValue* p = output_parameters_->get_value_by_name(name);
  return p != nullptr;
}

MockNamedValue
MockCheckedExpectedCall::get_input_parameter(const String& name)
{
  MockNamedValue* p = input_parameters_->get_value_by_name(name);
  return (p) ? *p : MockNamedValue("");
}

MockNamedValue
MockCheckedExpectedCall::get_output_parameter(const String& name)
{
  MockNamedValue* p = output_parameters_->get_value_by_name(name);
  return (p) ? *p : MockNamedValue("");
}

bool
MockCheckedExpectedCall::are_parameters_matching_actual_call()
{
  MockNamedValueListNode* p;
  for (p = input_parameters_->begin(); p; p = p->next())
    if (!item(p)->is_matching_actual_call())
      return false;
  for (p = output_parameters_->begin(); p; p = p->next())
    if (!item(p)->is_matching_actual_call())
      return false;
  return true;
}

MockExpectedCall&
MockCheckedExpectedCall::ignore_other_parameters()
{
  ignore_other_parameters_ = true;
  return *this;
}

bool
MockCheckedExpectedCall::is_fulfilled()
{
  return (actual_calls_ == expected_calls_);
}

bool
MockCheckedExpectedCall::can_match_actual_calls()
{
  return (actual_calls_ < expected_calls_);
}

bool
MockCheckedExpectedCall::is_matching_actual_call_and_finalized()
{
  return is_matching_actual_call() &&
         (!ignore_other_parameters_ || is_actual_call_match_finalized_);
}

bool
MockCheckedExpectedCall::is_matching_actual_call()
{
  return are_parameters_matching_actual_call() && was_passed_to_object_;
}

void
MockCheckedExpectedCall::call_was_made(unsigned int call_order)
{
  actual_calls_++;

  if ((initial_expected_call_order_ != no_expected_call_order) &&
      ((call_order < initial_expected_call_order_) ||
          (call_order > final_expected_call_order_))) {
    out_of_order_ = true;
  }

  reset_actual_call_matching_state();
}

void
MockCheckedExpectedCall::finalize_actual_call_match()
{
  is_actual_call_match_finalized_ = true;
}

void
MockCheckedExpectedCall::was_passed_to_object()
{
  was_passed_to_object_ = true;
}

void
MockCheckedExpectedCall::reset_actual_call_matching_state()
{
  was_passed_to_object_ = !is_specific_object_expected_;
  is_actual_call_match_finalized_ = false;

  MockNamedValueListNode* p;

  for (p = input_parameters_->begin(); p; p = p->next())
    item(p)->set_matches_actual_call(false);
  for (p = output_parameters_->begin(); p; p = p->next())
    item(p)->set_matches_actual_call(false);
}

void
MockCheckedExpectedCall::input_parameter_was_passed(const String& name)
{
  for (MockNamedValueListNode* p = input_parameters_->begin(); p;
      p = p->next()) {
    if (p->get_name() == name)
      item(p)->set_matches_actual_call(true);
  }
}

void
MockCheckedExpectedCall::output_parameter_was_passed(const String& name)
{
  for (MockNamedValueListNode* p = output_parameters_->begin(); p;
      p = p->next()) {
    if (p->get_name() == name)
      item(p)->set_matches_actual_call(true);
  }
}

String
MockCheckedExpectedCall::get_input_parameter_value_string(const String& name)
{
  MockNamedValue* p = input_parameters_->get_value_by_name(name);
  return (p) ? string_from(*p) : cpputest::string_from("failed");
}

bool
MockCheckedExpectedCall::has_input_parameter(const MockNamedValue& parameter)
{
  MockNamedValue* p =
      input_parameters_->get_value_by_name(parameter.get_name());
  return (p) ? p->equals(parameter) : ignore_other_parameters_;
}

bool
MockCheckedExpectedCall::has_output_parameter(const MockNamedValue& parameter)
{
  MockNamedValue* p =
      output_parameters_->get_value_by_name(parameter.get_name());
  return (p) ? p->compatible_for_copying(parameter) : ignore_other_parameters_;
}

String
MockCheckedExpectedCall::call_to_string()
{
  String str;
  if (is_specific_object_expected_)
    str = string_from_format("(object address: %p)::", object_ptr_);

  str += get_name();
  str += " -> ";
  if (initial_expected_call_order_ != no_expected_call_order) {
    if (initial_expected_call_order_ == final_expected_call_order_) {
      str += string_from_format(
          "expected call order: <%u> -> ", initial_expected_call_order_);
    } else {
      str += string_from_format("expected calls order: <%u..%u> -> ",
          initial_expected_call_order_,
          final_expected_call_order_);
    }
  }

  if (input_parameters_->begin() == nullptr &&
      output_parameters_->begin() == nullptr) {
    str +=
        (ignore_other_parameters_) ? "all parameters ignored" : "no parameters";
  } else {
    MockNamedValueListNode* p;

    for (p = input_parameters_->begin(); p; p = p->next()) {
      str += string_from_format("%s %s: <%s>",
          p->get_type().c_str(),
          p->get_name().c_str(),
          get_input_parameter_value_string(p->get_name()).c_str());
      if (p->next())
        str += ", ";
    }

    if (input_parameters_->begin() && output_parameters_->begin()) {
      str += ", ";
    }

    for (p = output_parameters_->begin(); p; p = p->next()) {
      str += string_from_format(
          "%s %s: <output>", p->get_type().c_str(), p->get_name().c_str());
      if (p->next())
        str += ", ";
    }

    if (ignore_other_parameters_)
      str += ", other parameters are ignored";
  }

  str += string_from_format(" (expected %u call%s, called %u time%s)",
      expected_calls_,
      (expected_calls_ == 1) ? "" : "s",
      actual_calls_,
      (actual_calls_ == 1) ? "" : "s");

  return str;
}

String
MockCheckedExpectedCall::missing_parameters_to_string()
{
  String str;
  MockNamedValueListNode* p;

  for (p = input_parameters_->begin(); p; p = p->next()) {
    if (!item(p)->is_matching_actual_call()) {
      if (str != "")
        str += ", ";
      str += string_from_format(
          "%s %s", p->get_type().c_str(), p->get_name().c_str());
    }
  }
  for (p = output_parameters_->begin(); p; p = p->next()) {
    if (!item(p)->is_matching_actual_call()) {
      if (str != "")
        str += ", ";
      str += string_from_format(
          "%s %s", p->get_type().c_str(), p->get_name().c_str());
    }
  }
  return str;
}

bool
MockCheckedExpectedCall::relates_to(const String& function_name)
{
  return function_name == get_name();
}

bool
MockCheckedExpectedCall::relates_to_object(const void* object_ptr) const
{
  return (!is_specific_object_expected_) || (object_ptr_ == object_ptr);
}

MockCheckedExpectedCall::MockExpectedFunctionParameter*
MockCheckedExpectedCall::item(MockNamedValueListNode* node)
{
  return static_cast<MockExpectedFunctionParameter*>(node->item());
}

MockCheckedExpectedCall::MockExpectedFunctionParameter::
    MockExpectedFunctionParameter(const String& name)
  : MockNamedValue(name)
  , matches_actual_call_(false)
{
}

void
MockCheckedExpectedCall::MockExpectedFunctionParameter::set_matches_actual_call(
    bool b)
{
  matches_actual_call_ = b;
}

bool
MockCheckedExpectedCall::MockExpectedFunctionParameter::
    is_matching_actual_call() const
{
  return matches_actual_call_;
}

MockExpectedCall&
MockCheckedExpectedCall::and_return_value(bool value)
{
  return_value_.set_name("returnValue");
  return_value_.set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::and_return_value(unsigned int value)
{
  return_value_.set_name("returnValue");
  return_value_.set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::and_return_value(int value)
{
  return_value_.set_name("returnValue");
  return_value_.set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::and_return_value(long int value)
{
  return_value_.set_name("returnValue");
  return_value_.set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::and_return_value(unsigned long int value)
{
  return_value_.set_name("returnValue");
  return_value_.set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::and_return_value(long long value)
{
  return_value_.set_name("returnValue");
  return_value_.set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::and_return_value(unsigned long long value)
{
  return_value_.set_name("returnValue");
  return_value_.set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::and_return_value(const char* value)
{
  return_value_.set_name("returnValue");
  return_value_.set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::and_return_value(double value)
{
  return_value_.set_name("returnValue");
  return_value_.set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::and_return_value(void* value)
{
  return_value_.set_name("returnValue");
  return_value_.set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::and_return_value(const void* value)
{
  return_value_.set_name("returnValue");
  return_value_.set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::and_return_value(void (*value)())
{
  return_value_.set_name("returnValue");
  return_value_.set_value(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::on_object(void* object_ptr)
{
  is_specific_object_expected_ = true;
  was_passed_to_object_ = false;
  object_ptr_ = object_ptr;
  return *this;
}

MockNamedValue
MockCheckedExpectedCall::return_value()
{
  return return_value_;
}

MockExpectedCall&
MockCheckedExpectedCall::with_call_order(unsigned int initial_call_order,
    unsigned int final_call_order)
{
  initial_expected_call_order_ = initial_call_order;
  final_expected_call_order_ = final_call_order;
  return *this;
}

bool
MockCheckedExpectedCall::is_out_of_order() const
{
  return out_of_order_;
}

unsigned int
MockCheckedExpectedCall::get_actual_calls_fulfilled() const
{
  return actual_calls_;
}

} // namespace extensions
} // namespace cpputest
