#include "CppMu/MockSupport.h"

#include "CppMu/CppMu.hpp"
#include "CppMu/MockActualCallTrace.hpp"
#include "CppMu/MockCheckedActualCall.hpp"
#include "CppMu/MockCheckedExpectedCall.hpp"
#include "CppMu/MockExpectedCall.hpp"
#include "CppMu/MockExpectedCallsList.hpp"
#include "CppMu/MockFailure.hpp"
#include "CppMu/MockIgnoredActualCall.hpp"
#include "CppMu/MockIgnoredExpectedCall.hpp"
#include "CppMu/MockNamedValueComparatorsAndCopiersRepository.hpp"
#include "CppMu/MockNamedValueList.hpp"
#include "CppMu/MockSupport.hpp"

#define MOCK_SUPPORT_SCOPE_PREFIX "!!!$$$MockingSupportScope$$$!!!"

namespace cppmu {

class MockSupport::Impl
{
  friend class MockSupport;

  MockFailureReporter default_reporter_;
  MockFailureReporter* active_reporter_{ nullptr };
  MockFailureReporter* standard_reporter_;
  MockExpectedCallsList expectations_;
  MockCheckedActualCall* last_actual_function_call_{ nullptr };
  MockNamedValueComparatorsAndCopiersRepository
      comparators_and_copiers_repository_;
  MockNamedValueList data_;

  Impl()
    : standard_reporter_(&default_reporter_)

  {
  }
};

namespace {
MockSupport global_mock;
} // namespace

MockSupport&
mock(const String& mock_name,
    MockFailureReporter* failure_reporter_for_this_call)
{
  MockSupport& mock_support =
      (mock_name != "") ? *global_mock.get_mock_support_scope(mock_name)
                        : global_mock;
  mock_support.set_active_reporter(failure_reporter_for_this_call);
  mock_support.set_default_comparators_and_copiers_repository();
  return mock_support;
}

MockSupport::MockSupport(const String& mock_name)
  : impl_(new Impl())
  , mock_name_(mock_name)

{
}

MockSupport::~MockSupport()
{
  delete impl_;
}

void
MockSupport::crash_on_failure(bool should_crash)
{
  impl_->active_reporter_->crash_on_failure(should_crash);
}

void
MockSupport::set_mock_failure_standard_reporter(MockFailureReporter* reporter)
{
  impl_->standard_reporter_ =
      (reporter != nullptr) ? reporter : &impl_->default_reporter_;

  if (impl_->last_actual_function_call_)
    impl_->last_actual_function_call_->set_mock_failure_reporter(
        impl_->standard_reporter_);

  for (MockNamedValueListNode* p = impl_->data_.begin(); p; p = p->next())
    if (get_mock_support(p))
      get_mock_support(p)->set_mock_failure_standard_reporter(
          impl_->standard_reporter_);
}

void
MockSupport::set_active_reporter(MockFailureReporter* reporter)
{
  impl_->active_reporter_ = (reporter) ? reporter : impl_->standard_reporter_;
}

void
MockSupport::set_default_comparators_and_copiers_repository()
{
  MockNamedValue::set_default_comparators_and_copiers_repository(
      &impl_->comparators_and_copiers_repository_);
}

void
MockSupport::install_comparator(const String& type_name,
    MockNamedValueComparator& comparator)
{
  impl_->comparators_and_copiers_repository_.install_comparator(
      type_name, comparator);

  for (MockNamedValueListNode* p = impl_->data_.begin(); p; p = p->next())
    if (get_mock_support(p))
      get_mock_support(p)->install_comparator(type_name, comparator);
}

void
MockSupport::install_copier(const String& type_name,
    MockNamedValueCopier& copier)
{
  impl_->comparators_and_copiers_repository_.install_copier(type_name, copier);

  for (MockNamedValueListNode* p = impl_->data_.begin(); p; p = p->next())
    if (get_mock_support(p))
      get_mock_support(p)->install_copier(type_name, copier);
}

void
MockSupport::install_comparators_and_copiers(
    const MockNamedValueComparatorsAndCopiersRepository& repository)
{
  impl_->comparators_and_copiers_repository_.install_comparators_and_copiers(
      repository);

  for (MockNamedValueListNode* p = impl_->data_.begin(); p; p = p->next())
    if (get_mock_support(p))
      get_mock_support(p)->install_comparators_and_copiers(repository);
}

void
MockSupport::remove_all_comparators_and_copiers()
{
  impl_->comparators_and_copiers_repository_.clear();
  for (MockNamedValueListNode* p = impl_->data_.begin(); p; p = p->next())
    if (get_mock_support(p))
      get_mock_support(p)->remove_all_comparators_and_copiers();
}

void
MockSupport::clear()
{
  delete impl_->last_actual_function_call_;
  impl_->last_actual_function_call_ = nullptr;

  tracing_ = false;
  MockActualCallTrace::clear_instance();

  impl_->expectations_.delete_all_expectations_and_clear_list();
  ignore_other_calls_ = false;
  enabled_ = true;
  actual_call_order_ = 0;
  expected_call_order_ = 0;
  strict_ordering_ = false;

  for (MockNamedValueListNode* p = impl_->data_.begin(); p; p = p->next()) {
    MockSupport* support = get_mock_support(p);
    if (support) {
      support->clear();
      delete support;
    }
  }
  impl_->data_.clear();
}

void
MockSupport::strict_order()
{
  strict_ordering_ = true;
}

String
MockSupport::append_scope_to_name(const String& function_name)
{
  if (mock_name_.empty())
    return function_name;
  return mock_name_ + "::" + function_name;
}

MockExpectedCall&
MockSupport::expect_one_call(const String& function_name)
{
  return expect_n_calls(1, function_name);
}

void
MockSupport::expect_no_call(const String& function_name)
{
  expect_n_calls(0, function_name);
}

MockExpectedCall&
MockSupport::expect_n_calls(unsigned int amount, const String& function_name)
{
  if (!enabled_)
    return MockIgnoredExpectedCall::instance();

  count_check();

  auto* call = new MockCheckedExpectedCall(amount);
  call->with_name(append_scope_to_name(function_name));
  if (strict_ordering_) {
    call->with_call_order(
        expected_call_order_ + 1, expected_call_order_ + amount);
    expected_call_order_ += amount;
  }
  impl_->expectations_.add_expected_call(call);
  return *call;
}

MockCheckedActualCall*
MockSupport::create_actual_call()
{
  impl_->last_actual_function_call_ = new MockCheckedActualCall(
      ++actual_call_order_, impl_->active_reporter_, impl_->expectations_);
  return impl_->last_actual_function_call_;
}

bool
MockSupport::call_is_ignored(const String& function_name)
{
  return ignore_other_calls_ &&
         !impl_->expectations_.has_expectation_with_name(function_name);
}

MockActualCall&
MockSupport::actual_call(const char* function_name)
{
  String scope_function_name = append_scope_to_name(function_name);

  if (impl_->last_actual_function_call_) {
    impl_->last_actual_function_call_->check_expectations();
    delete impl_->last_actual_function_call_;
    impl_->last_actual_function_call_ = nullptr;
  }

  if (!enabled_)
    return MockIgnoredActualCall::instance();
  if (tracing_)
    return MockActualCallTrace::instance().with_name(scope_function_name);

  if (call_is_ignored(scope_function_name)) {
    return MockIgnoredActualCall::instance();
  }

  MockCheckedActualCall* call = create_actual_call();
  call->set_name_and_check(static_cast<String&&>(scope_function_name));
  return *call;
}

MockActualCall&
MockSupport::actual_call(const String& function_name)
{
  String scope_function_name = append_scope_to_name(function_name);

  if (impl_->last_actual_function_call_) {
    impl_->last_actual_function_call_->check_expectations();
    delete impl_->last_actual_function_call_;
    impl_->last_actual_function_call_ = nullptr;
  }

  if (!enabled_)
    return MockIgnoredActualCall::instance();
  if (tracing_)
    return MockActualCallTrace::instance().with_name(scope_function_name);

  if (call_is_ignored(scope_function_name)) {
    return MockIgnoredActualCall::instance();
  }

  MockCheckedActualCall* call = create_actual_call();
  call->set_name_and_check(static_cast<String&&>(scope_function_name));
  return *call;
}

void
MockSupport::ignore_other_calls()
{
  ignore_other_calls_ = true;

  for (MockNamedValueListNode* p = impl_->data_.begin(); p; p = p->next())
    if (get_mock_support(p))
      get_mock_support(p)->ignore_other_calls();
}

void
MockSupport::disable()
{
  enabled_ = false;

  for (MockNamedValueListNode* p = impl_->data_.begin(); p; p = p->next())
    if (get_mock_support(p))
      get_mock_support(p)->disable();
}

void
MockSupport::enable()
{
  enabled_ = true;

  for (MockNamedValueListNode* p = impl_->data_.begin(); p; p = p->next())
    if (get_mock_support(p))
      get_mock_support(p)->enable();
}

void
MockSupport::tracing(bool enabled)
{
  tracing_ = enabled;

  for (MockNamedValueListNode* p = impl_->data_.begin(); p; p = p->next())
    if (get_mock_support(p))
      get_mock_support(p)->tracing(enabled);
}

const char*
MockSupport::get_trace_output()
{
  return MockActualCallTrace::instance().get_trace_output();
}

bool
MockSupport::expected_calls_left()
{
  check_expectations_of_last_actual_call();
  int calls_left = impl_->expectations_.has_unfulfilled_expectations();

  for (MockNamedValueListNode* p = impl_->data_.begin(); p; p = p->next())
    if (get_mock_support(p))
      calls_left += get_mock_support(p)->expected_calls_left();

  return calls_left != 0;
}

bool
MockSupport::was_last_actual_call_fulfilled()
{
  if (impl_->last_actual_function_call_ &&
      !impl_->last_actual_function_call_->is_fulfilled())
    return false;

  for (MockNamedValueListNode* p = impl_->data_.begin(); p; p = p->next())
    if (get_mock_support(p) &&
        !get_mock_support(p)->was_last_actual_call_fulfilled())
      return false;

  return true;
}

void
MockSupport::fail_test_with_expected_calls_not_fulfilled()
{
  MockExpectedCallsList expectations_list;
  expectations_list.add_expectations(impl_->expectations_);

  for (MockNamedValueListNode* p = impl_->data_.begin(); p; p = p->next())
    if (get_mock_support(p))
      expectations_list.add_expectations(
          get_mock_support(p)->impl_->expectations_);

  MockExpectedCallsDidntHappenFailure failure(
      impl_->active_reporter_->get_test_to_fail(), expectations_list);
  fail_test(failure);
}

void
MockSupport::fail_test_with_out_of_order_calls()
{
  MockExpectedCallsList expectations_list;
  expectations_list.add_expectations(impl_->expectations_);

  for (MockNamedValueListNode* p = impl_->data_.begin(); p; p = p->next())
    if (get_mock_support(p))
      expectations_list.add_expectations(
          get_mock_support(p)->impl_->expectations_);

  MockCallOrderFailure failure(
      impl_->active_reporter_->get_test_to_fail(), expectations_list);
  fail_test(failure);
}

void
MockSupport::fail_test(MockFailure& failure)
{
  clear();
  impl_->active_reporter_->fail_test(static_cast<MockFailure&&>(failure));
}

void
MockSupport::count_check()
{
  cppmu::TestShell::get_current()->count_check();
}

void
MockSupport::check_expectations_of_last_actual_call()
{
  if (impl_->last_actual_function_call_)
    impl_->last_actual_function_call_->check_expectations();

  for (MockNamedValueListNode* p = impl_->data_.begin(); p; p = p->next())
    if (get_mock_support(p) &&
        get_mock_support(p)->impl_->last_actual_function_call_)
      get_mock_support(p)
          ->impl_->last_actual_function_call_->check_expectations();
}

bool
MockSupport::has_calls_out_of_order()
{
  if (impl_->expectations_.has_calls_out_of_order()) {
    return true;
  }
  for (MockNamedValueListNode* p = impl_->data_.begin(); p; p = p->next())
    if (get_mock_support(p) && get_mock_support(p)->has_calls_out_of_order()) {
      return true;
    }
  return false;
}

void
MockSupport::check_expectations()
{
  check_expectations_of_last_actual_call();

  if (was_last_actual_call_fulfilled() && expected_calls_left())
    fail_test_with_expected_calls_not_fulfilled();

  if (has_calls_out_of_order())
    fail_test_with_out_of_order_calls();
}

bool
MockSupport::has_data(const String& name)
{
  return impl_->data_.get_value_by_name(name) != nullptr;
}

MockNamedValue*
MockSupport::retrieve_data_from_store(const String& name)
{
  MockNamedValue* new_data = impl_->data_.get_value_by_name(name);
  if (new_data == nullptr) {
    new_data = new MockNamedValue(name);
    impl_->data_.add(new_data);
  }
  return new_data;
}

void
MockSupport::set_data(const String& name, bool value)
{
  MockNamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_value(value);
}

void
MockSupport::set_data(const String& name, unsigned int value)
{
  MockNamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_value(value);
}

void
MockSupport::set_data(const String& name, int value)
{
  MockNamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_value(value);
}

void
MockSupport::set_data(const String& name, long int value)
{
  MockNamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_value(value);
}

void
MockSupport::set_data(const String& name, unsigned long int value)
{
  MockNamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_value(value);
}

void
MockSupport::set_data(const String& name, const char* value)
{
  MockNamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_value(value);
}

void
MockSupport::set_data(const String& name, double value)
{
  MockNamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_value(value);
}

void
MockSupport::set_data(const String& name, void* value)
{
  MockNamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_value(value);
}

void
MockSupport::set_data(const String& name, const void* value)
{
  MockNamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_value(value);
}

void
MockSupport::set_data(const String& name, void (*value)())
{
  MockNamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_value(value);
}

void
MockSupport::set_data_object(const String& name,
    const String& type,
    void* value)
{
  MockNamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_object_pointer(type, value);
}

void
MockSupport::set_data_const_object(const String& name,
    const String& type,
    const void* value)
{
  MockNamedValue* new_data = retrieve_data_from_store(name);
  new_data->set_const_object_pointer(type, value);
}

MockNamedValue
MockSupport::get_data(const String& name)
{
  MockNamedValue* value = impl_->data_.get_value_by_name(name);
  if (value == nullptr)
    return MockNamedValue("");
  return *value;
}

MockSupport*
MockSupport::clone(const String& mock_name)
{
  auto* new_mock = new MockSupport(mock_name);
  new_mock->set_mock_failure_standard_reporter(impl_->standard_reporter_);
  if (ignore_other_calls_)
    new_mock->ignore_other_calls();

  if (!enabled_)
    new_mock->disable();

  if (strict_ordering_)
    new_mock->strict_order();

  new_mock->tracing(tracing_);
  new_mock->install_comparators_and_copiers(
      impl_->comparators_and_copiers_repository_);
  return new_mock;
}

MockSupport*
MockSupport::get_mock_support_scope(const String& name)
{
  String mocking_support_name = MOCK_SUPPORT_SCOPE_PREFIX;
  mocking_support_name += name;

  if (has_data(mocking_support_name)) {
    STRCMP_EQUAL(
        "MockSupport", get_data(mocking_support_name).get_type().c_str());
    return static_cast<MockSupport*>(
        get_data(mocking_support_name).get_object_pointer());
  }

  MockSupport* new_mock = clone(name);

  set_data_object(mocking_support_name, "MockSupport", new_mock);
  return new_mock;
}

MockSupport*
MockSupport::get_mock_support(MockNamedValueListNode* node)
{
  if (node->get_type() == "MockSupport" &&
      string_contains(node->get_name(), MOCK_SUPPORT_SCOPE_PREFIX))
    return static_cast<MockSupport*>(node->item()->get_object_pointer());
  return nullptr;
}

MockNamedValue
MockSupport::return_value()
{
  if (impl_->last_actual_function_call_)
    return impl_->last_actual_function_call_->return_value();
  return MockNamedValue("");
}

bool
MockSupport::bool_return_value()
{
  return return_value().get_bool_value();
}

unsigned int
MockSupport::unsigned_int_return_value()
{
  return return_value().get_unsigned_int_value();
}

int
MockSupport::int_return_value()
{
  return return_value().get_int_value();
}

const char*
MockSupport::return_string_value_or_default(const char* default_value)
{
  if (has_return_value()) {
    return string_return_value();
  }
  return default_value;
}

double
MockSupport::return_double_value_or_default(double default_value)
{
  if (has_return_value()) {
    return double_return_value();
  }
  return default_value;
}

long int
MockSupport::return_long_int_value_or_default(long int default_value)
{
  if (has_return_value()) {
    return long_int_return_value();
  }
  return default_value;
}

bool
MockSupport::return_bool_value_or_default(bool default_value)
{
  if (has_return_value()) {
    return bool_return_value();
  }
  return default_value;
}

int
MockSupport::return_int_value_or_default(int default_value)
{
  if (has_return_value()) {
    return int_return_value();
  }
  return default_value;
}

unsigned int
MockSupport::return_unsigned_int_value_or_default(unsigned int default_value)
{
  if (has_return_value()) {
    return unsigned_int_return_value();
  }
  return default_value;
}

unsigned long int
MockSupport::return_unsigned_long_int_value_or_default(
    unsigned long int default_value)
{
  if (has_return_value()) {
    return unsigned_long_int_return_value();
  }
  return default_value;
}

long int
MockSupport::long_int_return_value()
{
  return return_value().get_long_int_value();
}

unsigned long int
MockSupport::unsigned_long_int_return_value()
{
  return return_value().get_unsigned_long_int_value();
}

long long
MockSupport::long_long_int_return_value()
{
  return return_value().get_long_long_int_value();
}

unsigned long long
MockSupport::unsigned_long_long_int_return_value()
{
  return return_value().get_unsigned_long_long_int_value();
}

long long
MockSupport::return_long_long_int_value_or_default(long long default_value)
{
  if (has_return_value()) {
    return long_long_int_return_value();
  }
  return default_value;
}

unsigned long long
MockSupport::return_unsigned_long_long_int_value_or_default(
    unsigned long long default_value)
{
  if (has_return_value()) {
    return unsigned_long_long_int_return_value();
  }
  return default_value;
}

const char*
MockSupport::string_return_value()
{
  return return_value().get_string_value();
}

double
MockSupport::double_return_value()
{
  return return_value().get_double_value();
}

void*
MockSupport::return_pointer_value_or_default(void* default_value)
{
  if (has_return_value()) {
    return pointer_return_value();
  }
  return default_value;
}

const void*
MockSupport::return_const_pointer_value_or_default(const void* default_value)
{
  if (has_return_value()) {
    return const_pointer_return_value();
  }
  return default_value;
}

void (*MockSupport::return_function_pointer_value_or_default(
    void (*default_value)()))()
{
  if (has_return_value()) {
    return function_pointer_return_value();
  }
  return default_value;
}

void*
MockSupport::pointer_return_value()
{
  return return_value().get_pointer_value();
}

const void*
MockSupport::const_pointer_return_value()
{
  return return_value().get_const_pointer_value();
}

void (*MockSupport::function_pointer_return_value())()
{
  return return_value().get_function_pointer_value();
}

bool
MockSupport::has_return_value()
{
  if (impl_->last_actual_function_call_)
    return impl_->last_actual_function_call_->has_return_value();
  return false;
}

class MockFailureReporterTestTerminatorForInCOnlyCode : public TestTerminator
{
public:
  MockFailureReporterTestTerminatorForInCOnlyCode(bool crash_on_failure)
    : crash_on_failure_(crash_on_failure)
  {
  }

  void exit_current_test() const override
  {
    if (crash_on_failure_)
      cppmu::TestShell::crash();

    TestShell::get_current_test_terminator_without_exceptions()
        .exit_current_test();
  }
  ~MockFailureReporterTestTerminatorForInCOnlyCode() override = default;

private:
  bool crash_on_failure_;
};

class MockFailureReporterForInCOnlyCode : public MockFailureReporter
{
public:
  void fail_test(MockFailure failure) override
  {
    if (!get_test_to_fail()->has_failed()) {
      get_test_to_fail()->add_failure(failure);
      {
        MockFailure freed(static_cast<MockFailure&&>(failure));
      }
      exit_test();
    }
  }

  void exit_test() override
  {
    MockFailureReporterTestTerminatorForInCOnlyCode(crash_on_failure_)
        .exit_current_test();
  }
};

namespace {
MockSupport* current_mock_support = nullptr;
MockExpectedCall* expected_call = nullptr;
MockActualCall* actual_call = nullptr;
MockFailureReporterForInCOnlyCode failure_reporter_for_c;
} // namespace

} // namespace cppmu

namespace c {

class MockCFunctionComparatorNode : public cppmu::MockNamedValueComparator
{
public:
  MockCFunctionComparatorNode(MockCFunctionComparatorNode* nx,
      MockTypeEqualFunction eq,
      MockTypeValueToStringFunction ts)
    : next(nx)
    , equal(eq)
    , to_string(ts)
  {
  }
  ~MockCFunctionComparatorNode() override = default;

  bool is_equal(const void* object1, const void* object2) override
  {
    return equal(object1, object2) != 0;
  }
  cppmu::String value_to_string(const void* object) override
  {
    return cppmu::String(to_string(object));
  }

  MockCFunctionComparatorNode* next;
  MockTypeEqualFunction equal;
  MockTypeValueToStringFunction to_string;
};

namespace {
MockCFunctionComparatorNode* comparator_list = nullptr;

class MockCFunctionCopierNode : public cppmu::MockNamedValueCopier
{
public:
  MockCFunctionCopierNode(MockCFunctionCopierNode* nx, MockTypeCopyFunction cp)
    : next(nx)
    , copier(cp)
  {
  }
  ~MockCFunctionCopierNode() override = default;

  void copy(void* dst, const void* src) override { copier(dst, src); }

  MockCFunctionCopierNode* next;
  MockTypeCopyFunction copier;
};

MockCFunctionCopierNode* copier_list = nullptr;
} // namespace

void
strict_order();
struct MockExpectedCall*
expect_one_call(const char* name);
void
expect_no_call(const char* name);
struct MockExpectedCall*
expect_n_calls(const unsigned int number, const char* name);
struct MockActualCall*
actual_call(const char* name);
void
disable();
void
enable();
void
ignore_other_calls();
void
set_bool_data(const char* name, int value);
void
set_int_data(const char* name, int value);
void
set_unsigned_int_data(const char* name, unsigned int value);
void
set_long_int_data(const char* name, long int value);
void
set_unsigned_long_int_data(const char* name, unsigned long int value);
void
set_double_data(const char* name, double value);
void
set_string_data(const char* name, const char* value);
void
set_pointer_data(const char* name, void* value);
void
set_const_pointer_data(const char* name, const void* value);
void
set_function_pointer_data(const char* name, void (*value)());
void
set_data_object(const char* name, const char* type, void* value);
void
set_data_const_object(const char* name, const char* type, const void* value);
struct MockValue
get_data(const char* name);
int
has_return_value();

void
check_expectations();
int
expected_calls_left();
void
clear();
void
crash_on_failure(unsigned should_crash);

struct MockExpectedCall*
with_bool_parameters(const char* name, int value);
struct MockExpectedCall*
with_int_parameters(const char* name, int value);
struct MockExpectedCall*
with_unsigned_int_parameters(const char* name, unsigned int value);
struct MockExpectedCall*
with_long_int_parameters(const char* name, long int value);
struct MockExpectedCall*
with_unsigned_long_int_parameters(const char* name, unsigned long int value);
struct MockExpectedCall*
with_long_long_int_parameters(const char* name, long long value);
struct MockExpectedCall*
with_unsigned_long_long_int_parameters(const char* name,
    unsigned long long value);
struct MockExpectedCall*
with_double_parameters(const char* name, double value);
struct MockExpectedCall*
with_double_parameters_and_tolerance(const char* name,
    double value,
    double tolerance);
struct MockExpectedCall*
with_string_parameters(const char* name, const char* value);
struct MockExpectedCall*
with_pointer_parameters(const char* name, void* value);
struct MockExpectedCall*
with_const_pointer_parameters(const char* name, const void* value);
struct MockExpectedCall*
with_function_pointer_parameters(const char* name, void (*value)());
struct MockExpectedCall*
with_memory_buffer_parameters(const char* name,
    const unsigned char* value,
    size_t size);
struct MockExpectedCall*
with_parameter_of_type(const char* type, const char* name, const void* value);
struct MockExpectedCall*
with_output_parameter_returning(const char* name,
    const void* value,
    size_t size);
struct MockExpectedCall*
with_output_parameter_of_type_returning(const char* type,
    const char* name,
    const void* value);
struct MockExpectedCall*
with_unmodified_output_parameter(const char* name);
struct MockExpectedCall*
ignore_other_parameters();
struct MockExpectedCall*
and_return_bool_value(int value);
struct MockExpectedCall*
and_return_int_value(int value);
struct MockExpectedCall*
and_return_unsigned_int_value(unsigned int value);
struct MockExpectedCall*
and_return_long_int_value(long int value);
struct MockExpectedCall*
and_return_unsigned_long_int_value(unsigned long int value);
struct MockExpectedCall*
and_return_long_long_int_value(long long value);
struct MockExpectedCall*
and_return_unsigned_long_long_int_value(unsigned long long value);
struct MockExpectedCall*
and_return_double_value(double value);
struct MockExpectedCall*
and_return_string_value(const char* value);
struct MockExpectedCall*
and_return_pointer_value(void* value);
struct MockExpectedCall*
and_return_const_pointer_value(const void* value);
struct MockExpectedCall*
and_return_function_pointer_value(void (*value)());

struct MockActualCall*
with_actual_bool_parameters(const char* name, int value);
struct MockActualCall*
with_actual_int_parameters(const char* name, int value);
struct MockActualCall*
with_actual_unsigned_int_parameters(const char* name, unsigned int value);
struct MockActualCall*
with_actual_long_int_parameters(const char* name, long int value);
struct MockActualCall*
with_actual_unsigned_long_int_parameters(const char* name,
    unsigned long int value);
struct MockActualCall*
with_actual_long_long_int_parameters(const char* name, long long value);
struct MockActualCall*
with_actual_unsigned_long_long_int_parameters(const char* name,
    unsigned long long value);
struct MockActualCall*
with_actual_double_parameters(const char* name, double value);
struct MockActualCall*
with_actual_string_parameters(const char* name, const char* value);
struct MockActualCall*
with_actual_pointer_parameters(const char* name, void* value);
struct MockActualCall*
with_actual_const_pointer_parameters(const char* name, const void* value);
struct MockActualCall*
with_actual_function_pointer_parameters(const char* name, void (*value)());
struct MockActualCall*
with_actual_memory_buffer_parameters(const char* name,
    const unsigned char* value,
    size_t size);
struct MockActualCall*
with_actual_parameter_of_type(const char* type,
    const char* name,
    const void* value);
struct MockActualCall*
with_actual_output_parameter(const char* name, void* value);
struct MockActualCall*
with_actual_output_parameter_of_type(const char* type,
    const char* name,
    void* value);
struct MockValue
return_value();
int
bool_return_value();
int
return_bool_value_or_default(int default_value);
int
int_return_value();
int
return_int_value_or_default(int default_value);
unsigned int
unsigned_int_return_value();
unsigned int
return_unsigned_int_value_or_default(unsigned int default_value);
long int
long_int_return_value();
long int
return_long_int_value_or_default(long int default_value);
unsigned long int
unsigned_long_int_return_value();
unsigned long int
return_unsigned_long_int_value_or_default(unsigned long int default_value);
long long
long_long_int_return_value();
long long
return_long_long_int_value_or_default(long long default_value);
unsigned long long
unsigned_long_long_int_return_value();
unsigned long long
return_unsigned_long_long_int_value_or_default(
    unsigned long long default_value);
const char*
string_return_value();
const char*
return_string_value_or_default(const char* default_value);
double
double_return_value();
double
return_double_value_or_default(double default_value);
void*
pointer_return_value();
void*
return_pointer_value_or_default(void* default_value);
const void*
const_pointer_return_value();
const void*
return_const_pointer_value_or_default(const void* default_value);
void (*function_pointer_return_value())();
void (*return_function_pointer_value_or_default(void (*default_value)()))();

namespace {
void
install_comparator(const char* type_name,
    MockTypeEqualFunction is_equal,
    MockTypeValueToStringFunction value_to_string)
{
  comparator_list = new MockCFunctionComparatorNode(
      comparator_list, is_equal, value_to_string);
  cppmu::current_mock_support->install_comparator(type_name, *comparator_list);
}

void
install_copier(const char* type_name, MockTypeCopyFunction copier)
{
  copier_list = new MockCFunctionCopierNode(copier_list, copier);
  cppmu::current_mock_support->install_copier(type_name, *copier_list);
}

void
remove_all_comparators_and_copiers()
{
  while (comparator_list) {
    MockCFunctionComparatorNode* next = comparator_list->next;
    delete comparator_list;
    comparator_list = next;
  }
  while (copier_list) {
    MockCFunctionCopierNode* next = copier_list->next;
    delete copier_list;
    copier_list = next;
  }
  cppmu::current_mock_support->remove_all_comparators_and_copiers();
}
} // namespace

namespace {
struct MockExpectedCall g_expected_call = {
  with_bool_parameters,
  with_int_parameters,
  with_unsigned_int_parameters,
  with_long_int_parameters,
  with_unsigned_long_int_parameters,
  with_long_long_int_parameters,
  with_unsigned_long_long_int_parameters,
  with_double_parameters,
  with_double_parameters_and_tolerance,
  with_string_parameters,
  with_pointer_parameters,
  with_const_pointer_parameters,
  with_function_pointer_parameters,
  with_memory_buffer_parameters,
  with_parameter_of_type,
  with_output_parameter_returning,
  with_output_parameter_of_type_returning,
  with_unmodified_output_parameter,
  ignore_other_parameters,
  and_return_bool_value,
  and_return_unsigned_int_value,
  and_return_int_value,
  and_return_long_int_value,
  and_return_unsigned_long_int_value,
  and_return_long_long_int_value,
  and_return_unsigned_long_long_int_value,
  and_return_double_value,
  and_return_string_value,
  and_return_pointer_value,
  and_return_const_pointer_value,
  and_return_function_pointer_value,
};

struct MockActualCall g_actual_call = { with_actual_bool_parameters,
  with_actual_int_parameters,
  with_actual_unsigned_int_parameters,
  with_actual_long_int_parameters,
  with_actual_unsigned_long_int_parameters,
  with_actual_long_long_int_parameters,
  with_actual_unsigned_long_long_int_parameters,
  with_actual_double_parameters,
  with_actual_string_parameters,
  with_actual_pointer_parameters,
  with_actual_const_pointer_parameters,
  with_actual_function_pointer_parameters,
  with_actual_memory_buffer_parameters,
  with_actual_parameter_of_type,
  with_actual_output_parameter,
  with_actual_output_parameter_of_type,
  has_return_value,
  return_value,
  bool_return_value,
  return_bool_value_or_default,
  int_return_value,
  return_int_value_or_default,
  unsigned_int_return_value,
  return_unsigned_int_value_or_default,
  long_int_return_value,
  return_long_int_value_or_default,
  unsigned_long_int_return_value,
  return_unsigned_long_int_value_or_default,
  long_long_int_return_value,
  return_long_long_int_value_or_default,
  unsigned_long_long_int_return_value,
  return_unsigned_long_long_int_value_or_default,
  string_return_value,
  return_string_value_or_default,
  double_return_value,
  return_double_value_or_default,
  pointer_return_value,
  return_pointer_value_or_default,
  const_pointer_return_value,
  return_const_pointer_value_or_default,
  function_pointer_return_value,
  return_function_pointer_value_or_default };
} // namespace

struct MockExpectedCall*
with_bool_parameters(const char* name, int value)
{
  cppmu::expected_call =
      &cppmu::expected_call->with_parameter(name, (value != 0));
  return &g_expected_call;
}

struct MockExpectedCall*
with_int_parameters(const char* name, int value)
{
  cppmu::expected_call = &cppmu::expected_call->with_parameter(name, value);
  return &g_expected_call;
}

struct MockExpectedCall*
with_unsigned_int_parameters(const char* name, unsigned int value)
{
  cppmu::expected_call = &cppmu::expected_call->with_parameter(name, value);
  return &g_expected_call;
}

struct MockExpectedCall*
with_long_int_parameters(const char* name, long int value)
{
  cppmu::expected_call = &cppmu::expected_call->with_parameter(name, value);
  return &g_expected_call;
}

struct MockExpectedCall*
with_unsigned_long_int_parameters(const char* name, unsigned long int value)
{
  cppmu::expected_call = &cppmu::expected_call->with_parameter(name, value);
  return &g_expected_call;
}

struct MockExpectedCall*
with_long_long_int_parameters(const char* name, long long value)
{
  cppmu::expected_call = &cppmu::expected_call->with_parameter(name, value);
  return &g_expected_call;
}

struct MockExpectedCall*
with_unsigned_long_long_int_parameters(const char* name,
    unsigned long long value)
{
  cppmu::expected_call = &cppmu::expected_call->with_parameter(name, value);
  return &g_expected_call;
}

struct MockExpectedCall*
with_double_parameters(const char* name, double value)
{
  cppmu::expected_call = &cppmu::expected_call->with_parameter(name, value);
  return &g_expected_call;
}

struct MockExpectedCall*
with_double_parameters_and_tolerance(const char* name,
    double value,
    double tolerance)
{
  cppmu::expected_call =
      &cppmu::expected_call->with_parameter(name, value, tolerance);
  return &g_expected_call;
}

struct MockExpectedCall*
with_string_parameters(const char* name, const char* value)
{
  cppmu::expected_call = &cppmu::expected_call->with_parameter(name, value);
  return &g_expected_call;
}

struct MockExpectedCall*
with_pointer_parameters(const char* name, void* value)
{
  cppmu::expected_call = &cppmu::expected_call->with_parameter(name, value);
  return &g_expected_call;
}

struct MockExpectedCall*
with_const_pointer_parameters(const char* name, const void* value)
{
  cppmu::expected_call = &cppmu::expected_call->with_parameter(name, value);
  return &g_expected_call;
}

struct MockExpectedCall*
with_function_pointer_parameters(const char* name, void (*value)())
{
  cppmu::expected_call = &cppmu::expected_call->with_parameter(name, value);
  return &g_expected_call;
}

struct MockExpectedCall*
with_memory_buffer_parameters(const char* name,
    const unsigned char* value,
    size_t size)
{
  cppmu::expected_call =
      &cppmu::expected_call->with_parameter(name, value, size);
  return &g_expected_call;
}

struct MockExpectedCall*
with_parameter_of_type(const char* type, const char* name, const void* value)
{
  cppmu::expected_call =
      &cppmu::expected_call->with_parameter_of_type(type, name, value);
  return &g_expected_call;
}

struct MockExpectedCall*
with_output_parameter_returning(const char* name,
    const void* value,
    size_t size)
{
  cppmu::expected_call =
      &cppmu::expected_call->with_output_parameter_returning(name, value, size);
  return &g_expected_call;
}

struct MockExpectedCall*
with_output_parameter_of_type_returning(const char* type,
    const char* name,
    const void* value)
{
  cppmu::expected_call =
      &cppmu::expected_call->with_output_parameter_of_type_returning(
          type, name, value);
  return &g_expected_call;
}

struct MockExpectedCall*
with_unmodified_output_parameter(const char* name)
{
  cppmu::expected_call =
      &cppmu::expected_call->with_unmodified_output_parameter(name);
  return &g_expected_call;
}

struct MockExpectedCall*
ignore_other_parameters()
{
  cppmu::expected_call = &cppmu::expected_call->ignore_other_parameters();
  return &g_expected_call;
}

struct MockExpectedCall*
and_return_bool_value(int value)
{
  cppmu::expected_call = &cppmu::expected_call->and_return_value(value != 0);
  return &g_expected_call;
}

struct MockExpectedCall*
and_return_unsigned_int_value(unsigned int value)
{
  cppmu::expected_call = &cppmu::expected_call->and_return_value(value);
  return &g_expected_call;
}

struct MockExpectedCall*
and_return_int_value(int value)
{
  cppmu::expected_call = &cppmu::expected_call->and_return_value(value);
  return &g_expected_call;
}

struct MockExpectedCall*
and_return_long_int_value(long int value)
{
  cppmu::expected_call = &cppmu::expected_call->and_return_value(value);
  return &g_expected_call;
}

struct MockExpectedCall*
and_return_unsigned_long_int_value(unsigned long int value)
{
  cppmu::expected_call = &cppmu::expected_call->and_return_value(value);
  return &g_expected_call;
}

struct MockExpectedCall*
and_return_long_long_int_value(long long value)
{
  cppmu::expected_call = &cppmu::expected_call->and_return_value(value);
  return &g_expected_call;
}

struct MockExpectedCall*
and_return_unsigned_long_long_int_value(unsigned long long value)
{
  cppmu::expected_call = &cppmu::expected_call->and_return_value(value);
  return &g_expected_call;
}

struct MockExpectedCall*
and_return_double_value(double value)
{
  cppmu::expected_call = &cppmu::expected_call->and_return_value(value);
  return &g_expected_call;
}

struct MockExpectedCall*
and_return_string_value(const char* value)
{
  cppmu::expected_call = &cppmu::expected_call->and_return_value(value);
  return &g_expected_call;
}

struct MockExpectedCall*
and_return_pointer_value(void* value)
{
  cppmu::expected_call = &cppmu::expected_call->and_return_value(value);
  return &g_expected_call;
}

struct MockExpectedCall*
and_return_const_pointer_value(const void* value)
{
  cppmu::expected_call = &cppmu::expected_call->and_return_value(value);
  return &g_expected_call;
}

struct MockExpectedCall*
and_return_function_pointer_value(void (*value)())
{
  cppmu::expected_call = &cppmu::expected_call->and_return_value(value);
  return &g_expected_call;
}

namespace {
struct MockValue
get_mock_value_c_from_named_value(const cppmu::MockNamedValue& named_value)
{
  using cppmu::str_cmp;
  struct MockValue return_value;
  if (str_cmp(named_value.get_type().c_str(), "bool") == 0) {
    return_value.type = mockvaluetype_bool;
    return_value.value.bool_value = named_value.get_bool_value() ? 1 : 0;
  } else if (str_cmp(named_value.get_type().c_str(), "int") == 0) {
    return_value.type = mockvaluetype_integer;
    return_value.value.int_value = named_value.get_int_value();
  } else if (str_cmp(named_value.get_type().c_str(), "unsigned int") == 0) {
    return_value.type = mockvaluetype_unsigned_integer;
    return_value.value.unsigned_int_value =
        named_value.get_unsigned_int_value();
  } else if (str_cmp(named_value.get_type().c_str(), "long int") == 0) {
    return_value.type = mockvaluetype_long_integer;
    return_value.value.long_int_value = named_value.get_long_int_value();
  } else if (str_cmp(named_value.get_type().c_str(), "unsigned long int") ==
             0) {
    return_value.type = mockvaluetype_unsigned_long_integer;
    return_value.value.unsigned_long_int_value =
        named_value.get_unsigned_long_int_value();
  } else if (str_cmp(named_value.get_type().c_str(), "long long int") == 0) {
    return_value.type = mockvaluetype_long_long_integer;
    return_value.value.long_long_int_value =
        named_value.get_long_long_int_value();
  } else if (str_cmp(named_value.get_type().c_str(),
                 "unsigned long long int") == 0) {
    return_value.type = mockvaluetype_unsigned_long_long_integer;
    return_value.value.unsigned_long_long_int_value =
        named_value.get_unsigned_long_long_int_value();
  } else if (str_cmp(named_value.get_type().c_str(), "double") == 0) {
    return_value.type = mockvaluetype_double;
    return_value.value.double_value = named_value.get_double_value();
  } else if (str_cmp(named_value.get_type().c_str(), "const char*") == 0) {
    return_value.type = mockvaluetype_string;
    return_value.value.string_value = named_value.get_string_value();
  } else if (str_cmp(named_value.get_type().c_str(), "void*") == 0) {
    return_value.type = mockvaluetype_pointer;
    return_value.value.pointer_value = named_value.get_pointer_value();
  } else if (str_cmp(named_value.get_type().c_str(), "const void*") == 0) {
    return_value.type = mockvaluetype_const_pointer;
    return_value.value.const_pointer_value =
        named_value.get_const_pointer_value();
  } else if (str_cmp(named_value.get_type().c_str(), "void (*)()") == 0) {
    return_value.type = mockvaluetype_functionpointer;
    return_value.value.function_pointer_value =
        named_value.get_function_pointer_value();
  } else if (str_cmp(named_value.get_type().c_str(), "const unsigned char*") ==
             0) {
    return_value.type = mockvaluetype_memorybuffer;
    return_value.value.memory_buffer_value = named_value.get_memory_buffer();
  } else if (named_value.is_const_object()) {
    return_value.type = mockvaluetype_const_object;
    return_value.value.const_object_value =
        named_value.get_const_object_pointer();
  } else {
    return_value.type = mockvaluetype_object;
    return_value.value.object_value = named_value.get_object_pointer();
  }
  return return_value;
}
} // namespace

void
strict_order()
{
  cppmu::current_mock_support->strict_order();
}

struct MockExpectedCall*
expect_one_call(const char* name)
{
  cppmu::expected_call = &cppmu::current_mock_support->expect_one_call(name);
  return &g_expected_call;
}

void
expect_no_call(const char* name)
{
  cppmu::current_mock_support->expect_no_call(name);
}

struct MockExpectedCall*
expect_n_calls(const unsigned int number, const char* name)
{
  cppmu::expected_call =
      &cppmu::current_mock_support->expect_n_calls(number, name);
  return &g_expected_call;
}

struct MockActualCall*
actual_call(const char* name)
{
  cppmu::actual_call = &cppmu::current_mock_support->actual_call(name);
  return &g_actual_call;
}

struct MockActualCall*
with_actual_bool_parameters(const char* name, int value)
{
  cppmu::actual_call = &cppmu::actual_call->with_parameter(name, (value != 0));
  return &g_actual_call;
}

struct MockActualCall*
with_actual_int_parameters(const char* name, int value)
{
  cppmu::actual_call = &cppmu::actual_call->with_parameter(name, value);
  return &g_actual_call;
}

struct MockActualCall*
with_actual_unsigned_int_parameters(const char* name, unsigned int value)
{
  cppmu::actual_call = &cppmu::actual_call->with_parameter(name, value);
  return &g_actual_call;
}

struct MockActualCall*
with_actual_long_int_parameters(const char* name, long int value)
{
  cppmu::actual_call = &cppmu::actual_call->with_parameter(name, value);
  return &g_actual_call;
}

struct MockActualCall*
with_actual_unsigned_long_int_parameters(const char* name,
    unsigned long int value)
{
  cppmu::actual_call = &cppmu::actual_call->with_parameter(name, value);
  return &g_actual_call;
}

struct MockActualCall*
with_actual_long_long_int_parameters(const char* name, long long value)
{
  cppmu::actual_call = &cppmu::actual_call->with_parameter(name, value);
  return &g_actual_call;
}

struct MockActualCall*
with_actual_unsigned_long_long_int_parameters(const char* name,
    unsigned long long value)
{
  cppmu::actual_call = &cppmu::actual_call->with_parameter(name, value);
  return &g_actual_call;
}

struct MockActualCall*
with_actual_double_parameters(const char* name, double value)
{
  cppmu::actual_call = &cppmu::actual_call->with_parameter(name, value);
  return &g_actual_call;
}

struct MockActualCall*
with_actual_string_parameters(const char* name, const char* value)
{
  cppmu::actual_call = &cppmu::actual_call->with_parameter(name, value);
  return &g_actual_call;
}

struct MockActualCall*
with_actual_pointer_parameters(const char* name, void* value)
{
  cppmu::actual_call = &cppmu::actual_call->with_parameter(name, value);
  return &g_actual_call;
}

struct MockActualCall*
with_actual_const_pointer_parameters(const char* name, const void* value)
{
  cppmu::actual_call = &cppmu::actual_call->with_parameter(name, value);
  return &g_actual_call;
}

struct MockActualCall*
with_actual_function_pointer_parameters(const char* name, void (*value)())
{
  cppmu::actual_call = &cppmu::actual_call->with_parameter(name, value);
  return &g_actual_call;
}

struct MockActualCall*
with_actual_memory_buffer_parameters(const char* name,
    const unsigned char* value,
    size_t size)
{
  cppmu::actual_call = &cppmu::actual_call->with_parameter(name, value, size);
  return &g_actual_call;
}

struct MockActualCall*
with_actual_parameter_of_type(const char* type,
    const char* name,
    const void* value)
{
  cppmu::actual_call =
      &cppmu::actual_call->with_parameter_of_type(type, name, value);
  return &g_actual_call;
}

struct MockActualCall*
with_actual_output_parameter(const char* name, void* value)
{
  cppmu::actual_call = &cppmu::actual_call->with_output_parameter(name, value);
  return &g_actual_call;
}

struct MockActualCall*
with_actual_output_parameter_of_type(const char* type,
    const char* name,
    void* value)
{
  cppmu::actual_call =
      &cppmu::actual_call->with_output_parameter_of_type(type, name, value);
  return &g_actual_call;
}

struct MockValue
return_value()
{
  return get_mock_value_c_from_named_value(cppmu::actual_call->return_value());
}

int
bool_return_value()
{
  return cppmu::actual_call->return_bool_value() ? 1 : 0;
}

int
return_bool_value_or_default(int default_value)
{
  if (!has_return_value()) {
    return default_value;
  }
  return bool_return_value();
}

int
int_return_value()
{
  return cppmu::actual_call->return_int_value();
}

int
return_int_value_or_default(int default_value)
{
  if (!has_return_value()) {
    return default_value;
  }
  return int_return_value();
}

unsigned int
unsigned_int_return_value()
{
  return cppmu::actual_call->return_unsigned_int_value();
}

unsigned int
return_unsigned_int_value_or_default(unsigned int default_value)
{
  if (!has_return_value()) {
    return default_value;
  }
  return unsigned_int_return_value();
}

long int
long_int_return_value()
{
  return cppmu::actual_call->return_long_int_value();
}

long int
return_long_int_value_or_default(long int default_value)
{
  if (!has_return_value()) {
    return default_value;
  }
  return long_int_return_value();
}

unsigned long int
unsigned_long_int_return_value()
{
  return cppmu::actual_call->return_unsigned_long_int_value();
}

unsigned long int
return_unsigned_long_int_value_or_default(unsigned long int default_value)
{
  if (!has_return_value()) {
    return default_value;
  }
  return unsigned_long_int_return_value();
}

long long
long_long_int_return_value()
{
  return cppmu::actual_call->return_long_long_int_value();
}

long long
return_long_long_int_value_or_default(long long default_value)
{
  if (!has_return_value()) {
    return default_value;
  }
  return long_long_int_return_value();
}

unsigned long long
unsigned_long_long_int_return_value()
{
  return cppmu::actual_call->return_unsigned_long_long_int_value();
}

unsigned long long
return_unsigned_long_long_int_value_or_default(unsigned long long default_value)
{
  if (!has_return_value()) {
    return default_value;
  }
  return unsigned_long_long_int_return_value();
}

const char*
string_return_value()
{
  return cppmu::actual_call->return_string_value();
}

const char*
return_string_value_or_default(const char* default_value)
{
  if (!has_return_value()) {
    return default_value;
  }
  return string_return_value();
}

double
double_return_value()
{
  return cppmu::actual_call->return_double_value();
}

double
return_double_value_or_default(double default_value)
{
  if (!has_return_value()) {
    return default_value;
  }
  return double_return_value();
}

void*
pointer_return_value()
{
  return cppmu::actual_call->return_pointer_value();
}

void*
return_pointer_value_or_default(void* default_value)
{
  if (!has_return_value()) {
    return default_value;
  }
  return pointer_return_value();
}

const void*
const_pointer_return_value()
{
  return cppmu::actual_call->return_const_pointer_value();
}

const void*
return_const_pointer_value_or_default(const void* default_value)
{
  if (!has_return_value()) {
    return default_value;
  }
  return const_pointer_return_value();
}

void (*function_pointer_return_value())()
{
  return cppmu::actual_call->return_function_pointer_value();
}

void (*return_function_pointer_value_or_default(void (*default_value)()))()
{
  if (!has_return_value()) {
    return default_value;
  }
  return function_pointer_return_value();
}

void
disable()
{
  cppmu::current_mock_support->disable();
}

void
enable()
{
  cppmu::current_mock_support->enable();
}

void
ignore_other_calls()
{
  cppmu::current_mock_support->ignore_other_calls();
}

void
set_bool_data(const char* name, int value)
{
  cppmu::current_mock_support->set_data(name, (value != 0));
}

void
set_int_data(const char* name, int value)
{
  cppmu::current_mock_support->set_data(name, value);
}

void
set_unsigned_int_data(const char* name, unsigned int value)
{
  cppmu::current_mock_support->set_data(name, value);
}

void
set_long_int_data(const char* name, long int value)
{
  cppmu::current_mock_support->set_data(name, value);
}

void
set_unsigned_long_int_data(const char* name, unsigned long int value)
{
  cppmu::current_mock_support->set_data(name, value);
}

void
set_double_data(const char* name, double value)
{
  cppmu::current_mock_support->set_data(name, value);
}

void
set_string_data(const char* name, const char* value)
{
  cppmu::current_mock_support->set_data(name, value);
}

void
set_pointer_data(const char* name, void* value)
{
  cppmu::current_mock_support->set_data(name, value);
}

void
set_const_pointer_data(const char* name, const void* value)
{
  cppmu::current_mock_support->set_data(name, value);
}

void
set_function_pointer_data(const char* name, void (*value)())
{
  cppmu::current_mock_support->set_data(name, value);
}

void
set_data_object(const char* name, const char* type, void* value)
{
  cppmu::current_mock_support->set_data_object(name, type, value);
}

void
set_data_const_object(const char* name, const char* type, const void* value)
{
  cppmu::current_mock_support->set_data_const_object(name, type, value);
}

struct MockValue
get_data(const char* name)
{
  return get_mock_value_c_from_named_value(
      cppmu::current_mock_support->get_data(name));
}

int
has_return_value()
{
  return cppmu::current_mock_support->has_return_value();
}

void
check_expectations()
{
  cppmu::current_mock_support->check_expectations();
}

int
expected_calls_left()
{
  return cppmu::current_mock_support->expected_calls_left();
}

void
clear()
{
  cppmu::current_mock_support->clear();
}

void
crash_on_failure(unsigned should_crash)
{
  cppmu::current_mock_support->crash_on_failure(0 != should_crash);
}

namespace {
struct MockSupport g_mock_support = { strict_order,
  expect_one_call,
  expect_no_call,
  expect_n_calls,
  actual_call,
  has_return_value,
  return_value,
  bool_return_value,
  return_bool_value_or_default,
  int_return_value,
  return_int_value_or_default,
  unsigned_int_return_value,
  return_unsigned_int_value_or_default,
  long_int_return_value,
  return_long_int_value_or_default,
  unsigned_long_int_return_value,
  return_unsigned_long_int_value_or_default,
  long_long_int_return_value,
  return_long_long_int_value_or_default,
  unsigned_long_long_int_return_value,
  return_unsigned_long_long_int_value_or_default,
  string_return_value,
  return_string_value_or_default,
  double_return_value,
  return_double_value_or_default,
  pointer_return_value,
  return_pointer_value_or_default,
  const_pointer_return_value,
  return_const_pointer_value_or_default,
  function_pointer_return_value,
  return_function_pointer_value_or_default,
  set_bool_data,
  set_int_data,
  set_unsigned_int_data,
  set_long_int_data,
  set_unsigned_long_int_data,
  set_string_data,
  set_double_data,
  set_pointer_data,
  set_const_pointer_data,
  set_function_pointer_data,
  set_data_object,
  set_data_const_object,
  get_data,
  disable,
  enable,
  ignore_other_calls,
  check_expectations,
  expected_calls_left,
  clear,
  crash_on_failure,
  install_comparator,
  install_copier,
  remove_all_comparators_and_copiers };
} // namespace

} // namespace c

struct MockSupport*
mock()
{
  cppmu::current_mock_support = &mock("", &cppmu::failure_reporter_for_c);
  return &c::g_mock_support;
}

struct MockSupport*
mock_scope(const char* scope)
{
  cppmu::current_mock_support = &mock(scope, &cppmu::failure_reporter_for_c);
  return &c::g_mock_support;
}
