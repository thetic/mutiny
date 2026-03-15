#include "CppUTest/MockCheckedActualCall.hpp"

#include "CppUTest/CppUTest.hpp"
#include "CppUTest/MockCheckedExpectedCall.hpp"
#include "CppUTest/MockFailure.hpp"

#include <string.h>

namespace cpputest {

void
MockCheckedActualCall::set_name(const String& name)
{
  function_name_ = name;
}

const String&
MockCheckedActualCall::get_name() const
{
  return function_name_;
}

MockCheckedActualCall::MockCheckedActualCall(unsigned int call_order,
    MockFailureReporter* reporter,
    const MockExpectedCallsList& all_expectations)
  : call_order_(call_order)
  , reporter_(reporter)
  , state_(ActualCallState::success)
  , expectations_checked_(false)
  , matching_expectation_(nullptr)
  , all_expectations_(all_expectations)
  , output_parameter_expectations_(nullptr)
{
  potentially_matching_expectations_.add_potentially_matching_expectations(
      all_expectations);
}

MockCheckedActualCall::~MockCheckedActualCall()
{
  clean_up_output_parameter_list();
}

void
MockCheckedActualCall::set_mock_failure_reporter(MockFailureReporter* reporter)
{
  reporter_ = reporter;
}

cpputest::TestShell*
MockCheckedActualCall::get_test() const
{
  return reporter_->get_test_to_fail();
}

void
MockCheckedActualCall::fail_with(MockFailure failure)
{
  if (!has_failed()) {
    set_state(ActualCallState::failed);
    reporter_->fail_test(static_cast<MockFailure&&>(failure));
  }
}

void
MockCheckedActualCall::copy_output_parameters(
    MockCheckedExpectedCall* expected_call)
{
  for (MockOutputParametersListNode* p = output_parameter_expectations_; p;
      p = p->next) {
    MockNamedValue output_parameter =
        expected_call->get_output_parameter(p->name);
    MockNamedValueCopier* copier = output_parameter.get_copier();
    if (copier) {
      copier->copy(p->ptr, output_parameter.get_const_object_pointer());
    } else if ((output_parameter.get_type() == "const void*") &&
               (p->type == "void*")) {
      const void* data = output_parameter.get_const_pointer_value();
      size_t size = output_parameter.get_size();
      memcpy(p->ptr, data, size);
    } else if (output_parameter.get_name() != "") {
      fail_with(MockNoWayToCopyCustomTypeFailure(
          get_test(), expected_call->get_output_parameter(p->name).get_type()));
    }
  }
}

void
MockCheckedActualCall::complete_call_when_match_is_found()
{
  // Expectations that don't ignore parameters have higher fulfillment
  // preference than those that ignore parameters

  matching_expectation_ = potentially_matching_expectations_
                              .remove_first_finalized_matching_expectation();
  if (matching_expectation_) {
    copy_output_parameters(matching_expectation_);
    call_has_succeeded();
  } else {
    MockCheckedExpectedCall* matching_expectation_with_ignored_parameters =
        potentially_matching_expectations_.get_first_matching_expectation();
    if (matching_expectation_with_ignored_parameters) {
      copy_output_parameters(matching_expectation_with_ignored_parameters);
    }
  }
}

void
MockCheckedActualCall::call_has_succeeded()
{
  set_state(ActualCallState::success);
}

void
MockCheckedActualCall::discard_currently_matching_expectations()
{
  if (matching_expectation_) {
    matching_expectation_->reset_actual_call_matching_state();
    matching_expectation_ = nullptr;
  }
  potentially_matching_expectations_.only_keep_unmatching_expectations();
}

void
MockCheckedActualCall::set_name_and_check(String name)
{
  function_name_ = static_cast<String&&>(name);
  set_state(ActualCallState::in_progress);

  potentially_matching_expectations_.only_keep_expectations_related_to(
      function_name_);
  if (potentially_matching_expectations_.empty()) {
    fail_with(MockUnexpectedCallHappenedFailure(
        get_test(), function_name_, all_expectations_));
    return;
  }

  complete_call_when_match_is_found();
}

MockActualCall&
MockCheckedActualCall::with_name(const String& name)
{
  set_name_and_check(name);
  return *this;
}

MockActualCall&
MockCheckedActualCall::with_call_order(unsigned int)
{
  return *this;
}

void
MockCheckedActualCall::check_input_parameter(MockNamedValue actual_parameter)
{
  if (has_failed()) {
    return;
  }

  set_state(ActualCallState::in_progress);
  discard_currently_matching_expectations();

  potentially_matching_expectations_
      .only_keep_expectations_with_input_parameter(actual_parameter);

  if (potentially_matching_expectations_.empty()) {
    MockUnexpectedInputParameterFailure failure(get_test(),
        get_name(),
        static_cast<MockNamedValue&&>(actual_parameter),
        all_expectations_);
    fail_with(static_cast<MockFailure&&>(failure));
    return;
  }

  potentially_matching_expectations_.parameter_was_passed(
      actual_parameter.get_name());
  complete_call_when_match_is_found();
}

void
MockCheckedActualCall::check_output_parameter(MockNamedValue output_parameter)
{
  if (has_failed()) {
    return;
  }

  set_state(ActualCallState::in_progress);
  discard_currently_matching_expectations();

  potentially_matching_expectations_
      .only_keep_expectations_with_output_parameter(output_parameter);

  if (potentially_matching_expectations_.empty()) {
    MockUnexpectedOutputParameterFailure failure(get_test(),
        get_name(),
        static_cast<MockNamedValue&&>(output_parameter),
        all_expectations_);
    fail_with(static_cast<MockFailure&&>(failure));
    return;
  }

  potentially_matching_expectations_.output_parameter_was_passed(
      output_parameter.get_name());
  complete_call_when_match_is_found();
}

MockActualCall&
MockCheckedActualCall::with_bool_parameter(const String& name, bool value)
{
  MockNamedValue actual_parameter(name);
  actual_parameter.set_value(value);
  check_input_parameter(static_cast<MockNamedValue&&>(actual_parameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::with_unsigned_int_parameter(const String& name,
    unsigned int value)
{
  MockNamedValue actual_parameter(name);
  actual_parameter.set_value(value);
  check_input_parameter(static_cast<MockNamedValue&&>(actual_parameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::with_int_parameter(const String& name, int value)
{
  MockNamedValue actual_parameter(name);
  actual_parameter.set_value(value);
  check_input_parameter(static_cast<MockNamedValue&&>(actual_parameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::with_unsigned_long_int_parameter(const String& name,
    unsigned long int value)
{
  MockNamedValue actual_parameter(name);
  actual_parameter.set_value(value);
  check_input_parameter(static_cast<MockNamedValue&&>(actual_parameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::with_long_int_parameter(const String& name,
    long int value)
{
  MockNamedValue actual_parameter(name);
  actual_parameter.set_value(value);
  check_input_parameter(static_cast<MockNamedValue&&>(actual_parameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::with_unsigned_long_long_int_parameter(const String& name,
    unsigned long long value)
{
  MockNamedValue actual_parameter(name);
  actual_parameter.set_value(value);
  check_input_parameter(static_cast<MockNamedValue&&>(actual_parameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::with_long_long_int_parameter(const String& name,
    long long value)
{
  MockNamedValue actual_parameter(name);
  actual_parameter.set_value(value);
  check_input_parameter(static_cast<MockNamedValue&&>(actual_parameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::with_double_parameter(const String& name, double value)
{
  MockNamedValue actual_parameter(name);
  actual_parameter.set_value(value);
  check_input_parameter(static_cast<MockNamedValue&&>(actual_parameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::with_string_parameter(const String& name,
    const char* value)
{
  MockNamedValue actual_parameter(name);
  actual_parameter.set_value(value);
  check_input_parameter(static_cast<MockNamedValue&&>(actual_parameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::with_pointer_parameter(const String& name, void* value)
{
  MockNamedValue actual_parameter(name);
  actual_parameter.set_value(value);
  check_input_parameter(static_cast<MockNamedValue&&>(actual_parameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::with_const_pointer_parameter(const String& name,
    const void* value)
{
  MockNamedValue actual_parameter(name);
  actual_parameter.set_value(value);
  check_input_parameter(static_cast<MockNamedValue&&>(actual_parameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::with_function_pointer_parameter(const String& name,
    void (*value)())
{
  MockNamedValue actual_parameter(name);
  actual_parameter.set_value(value);
  check_input_parameter(static_cast<MockNamedValue&&>(actual_parameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::with_memory_buffer_parameter(const String& name,
    const unsigned char* value,
    size_t size)
{
  MockNamedValue actual_parameter(name);
  actual_parameter.set_memory_buffer(value, size);
  check_input_parameter(static_cast<MockNamedValue&&>(actual_parameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::with_parameter_of_type(const String& type,
    const String& name,
    const void* value)
{
  MockNamedValue actual_parameter(name);
  actual_parameter.set_const_object_pointer(type, value);

  if (actual_parameter.get_comparator() == nullptr) {
    {
      MockNamedValue freed(static_cast<MockNamedValue&&>(actual_parameter));
    }
    fail_with(MockNoWayToCompareCustomTypeFailure(get_test(), type));
    return *this;
  }
  check_input_parameter(static_cast<MockNamedValue&&>(actual_parameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::with_output_parameter(const String& name, void* output)
{
  add_output_parameter(name, "void*", output);

  MockNamedValue output_parameter(name);
  output_parameter.set_value(output);
  check_output_parameter(static_cast<MockNamedValue&&>(output_parameter));

  return *this;
}

MockActualCall&
MockCheckedActualCall::with_output_parameter_of_type(const String& type,
    const String& name,
    void* output)
{
  add_output_parameter(name, type, output);

  MockNamedValue output_parameter(name);
  output_parameter.set_const_object_pointer(type, output);
  check_output_parameter(static_cast<MockNamedValue&&>(output_parameter));

  return *this;
}

MockActualCall&
MockCheckedActualCall::with_memory_buffer_parameter(const char* name,
    const unsigned char* value,
    size_t size)
{
  MockNamedValue actual_parameter(name);
  actual_parameter.set_memory_buffer(value, size);
  check_input_parameter(static_cast<MockNamedValue&&>(actual_parameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::with_parameter_of_type(const char* type_name,
    const char* name,
    const void* value)
{
  MockNamedValue actual_parameter(name);
  actual_parameter.set_const_object_pointer(type_name, value);

  if (actual_parameter.get_comparator() == nullptr) {
    String type = actual_parameter.get_type();
    {
      MockNamedValue freed(static_cast<MockNamedValue&&>(actual_parameter));
    }
    fail_with(MockNoWayToCompareCustomTypeFailure(
        get_test(), static_cast<String&&>(type)));
    return *this;
  }
  check_input_parameter(static_cast<MockNamedValue&&>(actual_parameter));
  return *this;
}

bool
MockCheckedActualCall::is_fulfilled() const
{
  return state_ == ActualCallState::success;
}

bool
MockCheckedActualCall::has_failed() const
{
  return state_ == ActualCallState::failed;
}

void
MockCheckedActualCall::check_expectations()
{
  if (expectations_checked_) {
    return;
  }

  expectations_checked_ = true;

  if (state_ != ActualCallState::in_progress) {
    if (state_ == ActualCallState::success) {
      matching_expectation_->call_was_made(call_order_);
    }
    potentially_matching_expectations_.reset_actual_call_matching_state();
    return;
  }

  if (potentially_matching_expectations_.has_finalized_matching_expectations())
    FAIL("Actual call is in progress, but there are finalized matching "
         "expectations when checking expectations. This cannot happen.");

  matching_expectation_ =
      potentially_matching_expectations_.remove_first_matching_expectation();
  if (matching_expectation_) {
    matching_expectation_->finalize_actual_call_match();
    call_has_succeeded();
    matching_expectation_->call_was_made(call_order_);
    potentially_matching_expectations_.reset_actual_call_matching_state();
    return;
  }

  if (potentially_matching_expectations_
          .has_unmatching_expectations_because_of_missing_parameters()) {
    fail_with(MockExpectedParameterDidntHappenFailure(get_test(),
        get_name(),
        all_expectations_,
        potentially_matching_expectations_));
  } else {
    fail_with(MockExpectedObjectDidntHappenFailure(
        get_test(), get_name(), all_expectations_));
  }
}

void
MockCheckedActualCall::set_state(ActualCallState state)
{
  state_ = state;
}

MockNamedValue
MockCheckedActualCall::return_value()
{
  check_expectations();
  if (matching_expectation_)
    return matching_expectation_->return_value();
  return MockNamedValue("no return value");
}

bool
MockCheckedActualCall::return_bool_value_or_default(bool default_value)
{
  if (!has_return_value()) {
    return default_value;
  }
  return return_bool_value();
}

bool
MockCheckedActualCall::return_bool_value()
{
  return return_value().get_bool_value();
}

int
MockCheckedActualCall::return_int_value_or_default(int default_value)
{
  if (!has_return_value()) {
    return default_value;
  }
  return return_int_value();
}

int
MockCheckedActualCall::return_int_value()
{
  return return_value().get_int_value();
}

unsigned long int
MockCheckedActualCall::return_unsigned_long_int_value()
{
  return return_value().get_unsigned_long_int_value();
}

unsigned long int
MockCheckedActualCall::return_unsigned_long_int_value_or_default(
    unsigned long int default_value)
{
  if (!has_return_value()) {
    return default_value;
  }
  return return_unsigned_long_int_value();
}

long int
MockCheckedActualCall::return_long_int_value()
{
  return return_value().get_long_int_value();
}

long int
MockCheckedActualCall::return_long_int_value_or_default(long int default_value)
{
  if (!has_return_value()) {
    return default_value;
  }
  return return_long_int_value();
}

unsigned long long
MockCheckedActualCall::return_unsigned_long_long_int_value()
{
  return return_value().get_unsigned_long_long_int_value();
}

unsigned long long
MockCheckedActualCall::return_unsigned_long_long_int_value_or_default(
    unsigned long long default_value)
{
  if (!has_return_value()) {
    return default_value;
  }
  return return_unsigned_long_long_int_value();
}

long long
MockCheckedActualCall::return_long_long_int_value()
{
  return return_value().get_long_long_int_value();
}

long long
MockCheckedActualCall::return_long_long_int_value_or_default(
    long long default_value)
{
  if (!has_return_value()) {
    return default_value;
  }
  return return_long_long_int_value();
}

double
MockCheckedActualCall::return_double_value()
{
  return return_value().get_double_value();
}

double
MockCheckedActualCall::return_double_value_or_default(double default_value)
{
  if (!has_return_value()) {
    return default_value;
  }
  return return_double_value();
}

unsigned int
MockCheckedActualCall::return_unsigned_int_value()
{
  return return_value().get_unsigned_int_value();
}

unsigned int
MockCheckedActualCall::return_unsigned_int_value_or_default(
    unsigned int default_value)
{
  if (!has_return_value()) {
    return default_value;
  }
  return return_unsigned_int_value();
}

void*
MockCheckedActualCall::return_pointer_value_or_default(void* default_value)
{
  if (!has_return_value()) {
    return default_value;
  }
  return return_pointer_value();
}

void*
MockCheckedActualCall::return_pointer_value()
{
  return return_value().get_pointer_value();
}

const void*
MockCheckedActualCall::return_const_pointer_value()
{
  return return_value().get_const_pointer_value();
}

const void*
MockCheckedActualCall::return_const_pointer_value_or_default(
    const void* default_value)
{
  if (!has_return_value()) {
    return default_value;
  }
  return return_const_pointer_value();
}

MockActualCall::FunctionPointerReturnValue
MockCheckedActualCall::return_function_pointer_value()
{
  return return_value().get_function_pointer_value();
}

MockActualCall::FunctionPointerReturnValue
MockCheckedActualCall::return_function_pointer_value_or_default(
    void (*default_value)())
{
  if (!has_return_value()) {
    return default_value;
  }
  return return_function_pointer_value();
}

const char*
MockCheckedActualCall::return_string_value_or_default(const char* default_value)
{
  if (!has_return_value()) {
    return default_value;
  }
  return return_string_value();
}

const char*
MockCheckedActualCall::return_string_value()
{
  return return_value().get_string_value();
}

bool
MockCheckedActualCall::has_return_value()
{
  return !return_value().get_name().empty();
}

MockActualCall&
MockCheckedActualCall::on_object(const void* object_ptr)
{
  if (has_failed()) {
    return *this;
  }

  // Currently matching expectations are not discarded because the passed object
  // is ignored if not specifically set in the expectation

  potentially_matching_expectations_.only_keep_expectations_on_object(
      object_ptr);

  if ((!matching_expectation_) && potentially_matching_expectations_.empty()) {
    fail_with(MockUnexpectedObjectFailure(
        get_test(), get_name(), object_ptr, all_expectations_));
    return *this;
  }

  potentially_matching_expectations_.was_passed_to_object();

  if (!matching_expectation_) {
    complete_call_when_match_is_found();
  }

  return *this;
}

void
MockCheckedActualCall::add_output_parameter(const String& name,
    const String& type,
    void* ptr)
{
  auto* new_node = new MockOutputParametersListNode(name, type, ptr);

  if (output_parameter_expectations_ == nullptr)
    output_parameter_expectations_ = new_node;
  else {
    MockOutputParametersListNode* last_node = output_parameter_expectations_;
    while (last_node->next)
      last_node = last_node->next;
    last_node->next = new_node;
  }
}

void
MockCheckedActualCall::clean_up_output_parameter_list()
{
  MockOutputParametersListNode* current = output_parameter_expectations_;
  MockOutputParametersListNode* to_be_deleted = nullptr;

  while (current) {
    to_be_deleted = current;
    output_parameter_expectations_ = current = current->next;
    delete to_be_deleted;
  }
}

} // namespace cpputest
