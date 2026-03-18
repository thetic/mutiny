#ifndef INCLUDED_CPPMU_MOCKSUPPORT_HPP
#define INCLUDED_CPPMU_MOCKSUPPORT_HPP

#include "CppMu/MockActualCall.hpp"
#include "CppMu/MockExpectedCall.hpp"

namespace cppmu {

class MockCheckedActualCall;
class MockFailure;
class MockFailureReporter;
class MockNamedValueComparatorsAndCopiersRepository;
class MockNamedValueListNode;

/* This allows access to "the global" mocking support for easier testing */
class MockSupport
{
public:
  MockSupport(const cppmu::String& mock_name = "");
  virtual ~MockSupport();

  virtual void strict_order();
  virtual MockExpectedCall& expect_one_call(const cppmu::String& function_name);
  virtual void expect_no_call(const cppmu::String& function_name);
  virtual MockExpectedCall& expect_n_calls(
      unsigned int amount,
      const cppmu::String& function_name
  );
  virtual MockActualCall& actual_call(const char* function_name);
  virtual bool has_return_value();
  virtual MockNamedValue return_value();
  virtual bool bool_return_value();
  virtual bool return_bool_value_or_default(bool default_value);
  virtual int int_return_value();
  virtual int return_int_value_or_default(int default_value);
  virtual unsigned int unsigned_int_return_value();
  virtual long int long_int_return_value();
  virtual long int return_long_int_value_or_default(long int default_value);
  virtual unsigned long int unsigned_long_int_return_value();
  virtual unsigned long int return_unsigned_long_int_value_or_default(
      unsigned long int default_value
  );
  virtual long long long_long_int_return_value();
  virtual long long return_long_long_int_value_or_default(
      long long default_value
  );
  virtual unsigned long long unsigned_long_long_int_return_value();
  virtual unsigned long long return_unsigned_long_long_int_value_or_default(
      unsigned long long default_value
  );
  virtual unsigned int return_unsigned_int_value_or_default(
      unsigned int default_value
  );
  virtual const char* string_return_value();
  virtual const char* return_string_value_or_default(const char* default_value);
  virtual double return_double_value_or_default(double default_value);
  virtual double double_return_value();
  virtual void* pointer_return_value();
  virtual void* return_pointer_value_or_default(void* default_value);
  virtual const void* return_const_pointer_value_or_default(
      const void* default_value
  );
  virtual const void* const_pointer_return_value();
  virtual void (*return_function_pointer_value_or_default(void (*default_value)()))();
  virtual void (*function_pointer_return_value())();

  bool has_data(const cppmu::String& name);
  void set_data(const cppmu::String& name, bool value);
  void set_data(const cppmu::String& name, int value);
  void set_data(const cppmu::String& name, unsigned int value);
  void set_data(const cppmu::String& name, long int value);
  void set_data(const cppmu::String& name, unsigned long int value);
  void set_data(const cppmu::String& name, const char* value);
  void set_data(const cppmu::String& name, double value);
  void set_data(const cppmu::String& name, void* value);
  void set_data(const cppmu::String& name, const void* value);
  void set_data(const cppmu::String& name, void (*value)());
  void set_data_object(
      const cppmu::String& name,
      const cppmu::String& type,
      void* value
  );
  void set_data_const_object(
      const cppmu::String& name,
      const cppmu::String& type,
      const void* value
  );
  MockNamedValue get_data(const cppmu::String& name);

  MockSupport* get_mock_support_scope(const cppmu::String& name);

  const char* get_trace_output();

  virtual void disable();
  virtual void enable();
  virtual void tracing(bool enabled);
  virtual void ignore_other_calls();

  virtual void check_expectations();
  virtual bool expected_calls_left();

  virtual void clear();
  virtual void crash_on_failure(bool should_fail = true);

  virtual void set_mock_failure_standard_reporter(
      MockFailureReporter* reporter
  );
  virtual void set_active_reporter(MockFailureReporter* active_reporter);
  virtual void set_default_comparators_and_copiers_repository();

  virtual void install_comparator(
      const cppmu::String& type_name,
      MockNamedValueComparator& comparator
  );
  virtual void install_copier(
      const cppmu::String& type_name,
      MockNamedValueCopier& copier
  );
  virtual void install_comparators_and_copiers(
      const MockNamedValueComparatorsAndCopiersRepository& repository
  );
  virtual void remove_all_comparators_and_copiers();

protected:
  MockSupport* clone(const cppmu::String& mock_name);
  virtual MockCheckedActualCall* create_actual_call();
  virtual void fail_test(MockFailure& failure);
  void count_check();

private:
  class Impl;
  Impl* impl_;

  unsigned int actual_call_order_{ 0 };
  unsigned int expected_call_order_{ 0 };
  bool strict_ordering_{ false };
  bool ignore_other_calls_{ false };
  bool enabled_{ true };
  const cppmu::String mock_name_;
  bool tracing_{ false };

  void check_expectations_of_last_actual_call();
  bool was_last_actual_call_fulfilled();
  void fail_test_with_expected_calls_not_fulfilled();
  void fail_test_with_out_of_order_calls();

  MockNamedValue* retrieve_data_from_store(const cppmu::String& name);

  MockSupport* get_mock_support(MockNamedValueListNode* node);

  bool call_is_ignored(const cppmu::String& function_name);
  bool has_calls_out_of_order();

  cppmu::String append_scope_to_name(const cppmu::String& function_name);
};

MockSupport& mock(
    const cppmu::String& mock_name = "",
    MockFailureReporter* failure_reporter_for_this_call = nullptr
);

} // namespace cppmu

#endif
