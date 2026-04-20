#include "mu/tiny/mock.hpp"

#include "mu/tiny/mock/ActualCallTrace.hpp"
#include "mu/tiny/mock/CheckedActualCall.hpp"
#include "mu/tiny/mock/CheckedExpectedCall.hpp"
#include "mu/tiny/mock/ExpectedCall.hpp"
#include "mu/tiny/mock/ExpectedCallsList.hpp"
#include "mu/tiny/mock/Failure.hpp"
#include "mu/tiny/mock/IgnoredActualCall.hpp"
#include "mu/tiny/mock/IgnoredExpectedCall.hpp"
#include "mu/tiny/mock/NamedValueComparatorsAndCopiersRepository.hpp"
#include "mu/tiny/mock/NamedValueList.hpp"

#include "mu/tiny/test/Shell.hpp"

#define MOCK_SUPPORT_SCOPE_PREFIX "!!!$$$MockingSupportScope$$$!!!"

namespace mu {
namespace tiny {
namespace mock {

class Support::Impl
{
  friend class Support;

  FailureReporter default_reporter_;
  FailureReporter* active_reporter_{ nullptr };
  FailureReporter* standard_reporter_;
  ExpectedCallsList expectations_;
  CheckedActualCall* last_actual_function_call_{ nullptr };
  NamedValueComparatorsAndCopiersRepository comparators_and_copiers_repository_;
  NamedValueList data_;

  Impl()
    : standard_reporter_(&default_reporter_)

  {
  }
};

Support& mock(
    StringView mock_name,
    FailureReporter* failure_reporter_for_this_call
)
{
  static Support global_mock;
  Support& mock_support = (!mock_name.empty())
                              ? *global_mock.get_mock_support_scope(mock_name)
                              : global_mock;
  mock_support.set_active_reporter(failure_reporter_for_this_call);
  mock_support.set_default_comparators_and_copiers_repository();
  return mock_support;
}

Support::Support(StringView mock_name)
  : impl_(new Impl())
  , mock_name_(mock_name.data(), mock_name.size())
{
}

Support::~Support()
{
  delete impl_;
}

void Support::crash_on_failure(bool should_crash)
{
  impl_->active_reporter_->crash_on_failure(should_crash);
}

void Support::set_mock_failure_standard_reporter(FailureReporter* reporter)
{
  impl_->standard_reporter_ =
      (reporter != nullptr) ? reporter : &impl_->default_reporter_;

  if (impl_->last_actual_function_call_ != nullptr) {
    impl_->last_actual_function_call_->set_mock_failure_reporter(
        impl_->standard_reporter_
    );
  }

  for (NamedValueListNode* p = impl_->data_.begin(); p != nullptr;
       p = p->next()) {
    if (get_mock_support(p) != nullptr) {
      get_mock_support(p)->set_mock_failure_standard_reporter(
          impl_->standard_reporter_
      );
    }
  }
}

void Support::set_active_reporter(FailureReporter* reporter)
{
  impl_->active_reporter_ =
      (reporter != nullptr) ? reporter : impl_->standard_reporter_;
}

void Support::set_default_comparators_and_copiers_repository()
{
  NamedValue::set_default_comparators_and_copiers_repository(
      &impl_->comparators_and_copiers_repository_
  );
}

void Support::install_comparator(
    StringView type_name,
    NamedValueComparator& comparator
)
{
  impl_->comparators_and_copiers_repository_.install_comparator(
      type_name, comparator
  );

  for (NamedValueListNode* p = impl_->data_.begin(); p != nullptr;
       p = p->next()) {
    if (get_mock_support(p) != nullptr) {
      get_mock_support(p)->install_comparator(type_name, comparator);
    }
  }
}

void Support::install_copier(StringView type_name, NamedValueCopier& copier)
{
  impl_->comparators_and_copiers_repository_.install_copier(type_name, copier);

  for (NamedValueListNode* p = impl_->data_.begin(); p != nullptr;
       p = p->next()) {
    if (get_mock_support(p) != nullptr) {
      get_mock_support(p)->install_copier(type_name, copier);
    }
  }
}

void Support::install_comparators_and_copiers(
    const NamedValueComparatorsAndCopiersRepository& repository
)
{
  impl_->comparators_and_copiers_repository_.install_comparators_and_copiers(
      repository
  );

  for (NamedValueListNode* p = impl_->data_.begin(); p != nullptr;
       p = p->next()) {
    if (get_mock_support(p) != nullptr) {
      get_mock_support(p)->install_comparators_and_copiers(repository);
    }
  }
}

void Support::remove_all_comparators_and_copiers()
{
  impl_->comparators_and_copiers_repository_.clear();
  for (NamedValueListNode* p = impl_->data_.begin(); p != nullptr;
       p = p->next()) {
    if (get_mock_support(p) != nullptr) {
      get_mock_support(p)->remove_all_comparators_and_copiers();
    }
  }
}

void Support::clear()
{
  delete impl_->last_actual_function_call_;
  impl_->last_actual_function_call_ = nullptr;

  tracing_ = false;
  ActualCallTrace::clear_instance();

  impl_->expectations_.delete_all_expectations_and_clear_list();
  ignore_other_calls_ = false;
  enabled_ = true;
  actual_call_order_ = 0;
  expected_call_order_ = 0;
  strict_ordering_ = false;

  for (NamedValueListNode* p = impl_->data_.begin(); p != nullptr;
       p = p->next()) {
    Support* support = get_mock_support(p);
    if (support != nullptr) {
      support->clear();
      delete support;
    }
  }
  impl_->data_.clear();
}

void Support::strict_order()
{
  strict_ordering_ = true;
}

String Support::append_scope_to_name(StringView function_name)
{
  if (mock_name_.empty()) {
    return String(function_name.data(), function_name.size());
  }
  return mock_name_ + "::" + String(function_name.data(), function_name.size());
}

ExpectedCall& Support::expect_one_call(StringView function_name)
{
  return expect_n_calls(1, function_name);
}

void Support::expect_no_call(StringView function_name)
{
  expect_n_calls(0, function_name);
}

ExpectedCall& Support::expect_n_calls(
    unsigned int amount,
    StringView function_name
)
{
  if (!enabled_) {
    return IgnoredExpectedCall::instance();
  }

  count_check();

  auto* call = new CheckedExpectedCall(amount);
  call->with_name(append_scope_to_name(function_name));
  if (strict_ordering_) {
    call->with_call_order(
        expected_call_order_ + 1, expected_call_order_ + amount
    );
    expected_call_order_ += amount;
  }
  impl_->expectations_.add_expected_call(call);
  return *call;
}

CheckedActualCall* Support::create_actual_call()
{
  impl_->last_actual_function_call_ = new CheckedActualCall(
      ++actual_call_order_, impl_->active_reporter_, impl_->expectations_
  );
  return impl_->last_actual_function_call_;
}

bool Support::call_is_ignored(StringView function_name)
{
  return ignore_other_calls_ &&
         !impl_->expectations_.has_expectation_with_name(function_name);
}

ActualCall& Support::actual_call(const char* function_name)
{
  String scope_function_name = append_scope_to_name(function_name);

  if (impl_->last_actual_function_call_ != nullptr) {
    impl_->last_actual_function_call_->check_expectations();
    delete impl_->last_actual_function_call_;
    impl_->last_actual_function_call_ = nullptr;
  }

  if (!enabled_) {
    return IgnoredActualCall::instance();
  }
  if (tracing_) {
    return ActualCallTrace::instance().with_name(scope_function_name);
  }

  if (call_is_ignored(scope_function_name)) {
    return IgnoredActualCall::instance();
  }

  CheckedActualCall* call = create_actual_call();
  call->set_name_and_check(static_cast<String&&>(scope_function_name));
  return *call;
}

void Support::ignore_other_calls()
{
  ignore_other_calls_ = true;

  for (NamedValueListNode* p = impl_->data_.begin(); p != nullptr;
       p = p->next()) {
    if (get_mock_support(p) != nullptr) {
      get_mock_support(p)->ignore_other_calls();
    }
  }
}

void Support::disable()
{
  enabled_ = false;

  for (NamedValueListNode* p = impl_->data_.begin(); p != nullptr;
       p = p->next()) {
    if (get_mock_support(p) != nullptr) {
      get_mock_support(p)->disable();
    }
  }
}

void Support::enable()
{
  enabled_ = true;

  for (NamedValueListNode* p = impl_->data_.begin(); p != nullptr;
       p = p->next()) {
    if (get_mock_support(p) != nullptr) {
      get_mock_support(p)->enable();
    }
  }
}

void Support::tracing(bool enabled)
{
  tracing_ = enabled;

  for (NamedValueListNode* p = impl_->data_.begin(); p != nullptr;
       p = p->next()) {
    if (get_mock_support(p) != nullptr) {
      get_mock_support(p)->tracing(enabled);
    }
  }
}

const char* Support::get_trace_output()
{
  return ActualCallTrace::instance().get_trace_output();
}

bool Support::expected_calls_left()
{
  check_expectations_of_last_actual_call();
  int calls_left =
      static_cast<int>(impl_->expectations_.has_unfulfilled_expectations());

  for (NamedValueListNode* p = impl_->data_.begin(); p != nullptr;
       p = p->next()) {
    if (get_mock_support(p) != nullptr) {
      calls_left +=
          static_cast<int>(get_mock_support(p)->expected_calls_left());
    }
  }

  return calls_left != 0;
}

bool Support::was_last_actual_call_fulfilled()
{
  if ((impl_->last_actual_function_call_ != nullptr) &&
      !impl_->last_actual_function_call_->is_fulfilled()) {
    return false;
  }

  for (NamedValueListNode* p = impl_->data_.begin(); p != nullptr;
       p = p->next()) {
    if ((get_mock_support(p) != nullptr) &&
        !get_mock_support(p)->was_last_actual_call_fulfilled()) {
      return false;
    }
  }

  return true;
}

void Support::fail_test_with_expected_calls_not_fulfilled()
{
  ExpectedCallsList expectations_list;
  expectations_list.add_expectations(impl_->expectations_);

  for (NamedValueListNode* p = impl_->data_.begin(); p != nullptr;
       p = p->next()) {
    if (get_mock_support(p) != nullptr) {
      expectations_list.add_expectations(
          get_mock_support(p)->impl_->expectations_
      );
    }
  }

  ExpectedCallsDidntHappenFailure failure(
      impl_->active_reporter_->get_test_to_fail(), expectations_list
  );
  fail_test(failure);
}

void Support::fail_test_with_out_of_order_calls()
{
  ExpectedCallsList expectations_list;
  expectations_list.add_expectations(impl_->expectations_);

  for (NamedValueListNode* p = impl_->data_.begin(); p != nullptr;
       p = p->next()) {
    if (get_mock_support(p) != nullptr) {
      expectations_list.add_expectations(
          get_mock_support(p)->impl_->expectations_
      );
    }
  }

  CallOrderFailure failure(
      impl_->active_reporter_->get_test_to_fail(), expectations_list
  );
  fail_test(failure);
}

void Support::fail_test(Failure& failure)
{
  clear();
  impl_->active_reporter_->fail_test(static_cast<Failure&&>(failure));
}

void Support::count_check()
{
  test::Shell::get_current()->count_check();
}

void Support::check_expectations_of_last_actual_call()
{
  if (impl_->last_actual_function_call_ != nullptr) {
    impl_->last_actual_function_call_->check_expectations();
  }

  for (NamedValueListNode* p = impl_->data_.begin(); p != nullptr;
       p = p->next()) {
    if ((get_mock_support(p) != nullptr) &&
        (get_mock_support(p)->impl_->last_actual_function_call_ != nullptr)) {
      get_mock_support(p)
          ->impl_->last_actual_function_call_->check_expectations();
    }
  }
}

bool Support::has_calls_out_of_order()
{
  if (impl_->expectations_.has_calls_out_of_order()) {
    return true;
  }
  for (NamedValueListNode* p = impl_->data_.begin(); p != nullptr;
       p = p->next()) {
    if ((get_mock_support(p) != nullptr) &&
        get_mock_support(p)->has_calls_out_of_order()) {
      return true;
    }
  }
  return false;
}

void Support::check_expectations()
{
  check_expectations_of_last_actual_call();

  if (was_last_actual_call_fulfilled() && expected_calls_left()) {
    fail_test_with_expected_calls_not_fulfilled();
  }

  if (has_calls_out_of_order()) {
    fail_test_with_out_of_order_calls();
  }
}

bool Support::has_data(StringView name)
{
  return impl_->data_.get_value_by_name(name) != nullptr;
}

NamedValue* Support::retrieve_data_from_store(StringView name)
{
  NamedValue* new_data = impl_->data_.get_value_by_name(name);
  if (new_data == nullptr) {
    new_data = new NamedValue(name);
    impl_->data_.add(new_data);
  }
  return new_data;
}

void Support::set_data(StringView name, bool value)
{
  NamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_value(value);
}

void Support::set_data(StringView name, unsigned int value)
{
  NamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_value(value);
}

void Support::set_data(StringView name, int value)
{
  NamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_value(value);
}

void Support::set_data(StringView name, long int value)
{
  NamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_value(value);
}

void Support::set_data(StringView name, unsigned long int value)
{
  NamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_value(value);
}

void Support::set_data(StringView name, const char* value)
{
  NamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_value(value);
}

void Support::set_data(StringView name, double value)
{
  NamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_value(value);
}

void Support::set_data(StringView name, void* value)
{
  NamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_value(value);
}

void Support::set_data(StringView name, const void* value)
{
  NamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_value(value);
}

void Support::set_data(StringView name, long long value)
{
  NamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_value(value);
}

void Support::set_data(StringView name, unsigned long long value)
{
  NamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_value(value);
}

void Support::set_data(StringView name, FunctionPointerValue value)
{
  NamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_value(value);
}

void Support::set_data_object(StringView name, StringView type, void* value)
{
  NamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_object_pointer(type, value);
}

void Support::set_data_const_object(
    StringView name,
    StringView type,
    const void* value
)
{
  NamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_const_object_pointer(type, value);
}

NamedValue Support::get_data(StringView name)
{
  NamedValue* value = impl_->data_.get_value_by_name(name);
  if (value == nullptr) {
    return NamedValue("");
  }
  return *value;
}

Support* Support::clone(StringView mock_name)
{
  auto* new_mock = new Support(mock_name);
  new_mock->set_mock_failure_standard_reporter(impl_->standard_reporter_);
  if (ignore_other_calls_) {
    new_mock->ignore_other_calls();
  }

  if (!enabled_) {
    new_mock->disable();
  }

  if (strict_ordering_) {
    new_mock->strict_order();
  }

  new_mock->tracing(tracing_);
  new_mock->install_comparators_and_copiers(
      impl_->comparators_and_copiers_repository_
  );
  return new_mock;
}

Support* Support::get_mock_support_scope(StringView name)
{
  String mocking_support_name = MOCK_SUPPORT_SCOPE_PREFIX;
  mocking_support_name += name.c_str();

  if (has_data(mocking_support_name)) {
    STRCMP_EQUAL("Support", get_data(mocking_support_name).get_type().c_str());
    return static_cast<Support*>(
        get_data(mocking_support_name).get_object_pointer()
    );
  }

  Support* new_mock = clone(name);

  set_data_object(mocking_support_name, "Support", new_mock);
  return new_mock;
}

Support* Support::get_mock_support(NamedValueListNode* node)
{
  if (node->get_type() == "Support" &&
      string_contains(node->get_name(), MOCK_SUPPORT_SCOPE_PREFIX)) {
    return static_cast<Support*>(node->item()->get_object_pointer());
  }
  return nullptr;
}

NamedValue Support::return_value()
{
  if (impl_->last_actual_function_call_ != nullptr) {
    return impl_->last_actual_function_call_->return_value();
  }
  return NamedValue("");
}

bool Support::has_return_value()
{
  if (impl_->last_actual_function_call_ != nullptr) {
    return impl_->last_actual_function_call_->has_return_value();
  }
  return false;
}

} // namespace mock
} // namespace tiny
} // namespace mu
