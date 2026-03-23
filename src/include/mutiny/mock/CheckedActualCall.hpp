#ifndef INCLUDED_MUTINY_MOCKCHECKEDACTUALCALL_HPP
#define INCLUDED_MUTINY_MOCKCHECKEDACTUALCALL_HPP

#include "mutiny/mock/ActualCall.hpp"
#include "mutiny/mock/ExpectedCallsList.hpp"
#include "mutiny/mock/Failure.hpp"

namespace mu {
namespace tiny {
namespace mock {

class MockCheckedActualCall : public MockActualCall
{
public:
  MockCheckedActualCall(
      unsigned int call_order,
      MockFailureReporter* reporter,
      const MockExpectedCallsList& expectations
  );
  ~MockCheckedActualCall() override;

  MockActualCall& with_name(const mu::tiny::test::String& name) override;
  MockActualCall& with_call_order(unsigned int) override;
  MockActualCall& with_bool_parameter(
      const mu::tiny::test::String& name,
      bool value
  ) override;
  MockActualCall& with_int_parameter(
      const mu::tiny::test::String& name,
      int value
  ) override;
  MockActualCall& with_unsigned_int_parameter(
      const mu::tiny::test::String& name,
      unsigned int value
  ) override;
  MockActualCall& with_long_int_parameter(
      const mu::tiny::test::String& name,
      long int value
  ) override;
  MockActualCall& with_unsigned_long_int_parameter(
      const mu::tiny::test::String& name,
      unsigned long int value
  ) override;
  MockActualCall& with_long_long_int_parameter(
      const mu::tiny::test::String& name,
      long long value
  ) override;
  MockActualCall& with_unsigned_long_long_int_parameter(
      const mu::tiny::test::String& name,
      unsigned long long value
  ) override;
  MockActualCall& with_double_parameter(
      const mu::tiny::test::String& name,
      double value
  ) override;
  MockActualCall& with_string_parameter(
      const mu::tiny::test::String& name,
      const char* value
  ) override;
  MockActualCall& with_pointer_parameter(
      const mu::tiny::test::String& name,
      void* value
  ) override;
  MockActualCall& with_const_pointer_parameter(
      const mu::tiny::test::String& name,
      const void* value
  ) override;
  MockActualCall& with_function_pointer_parameter(
      const mu::tiny::test::String& name,
      void (*value)()
  ) override;
  MockActualCall& with_memory_buffer_parameter(
      const mu::tiny::test::String& name,
      const unsigned char* value,
      size_t size
  ) override;
  MockActualCall& with_memory_buffer_parameter(
      const char* name,
      const unsigned char* value,
      size_t size
  ) override;
  MockActualCall& with_parameter_of_type(
      const mu::tiny::test::String& type,
      const mu::tiny::test::String& name,
      const void* value
  ) override;
  MockActualCall& with_parameter_of_type(
      const char* type_name,
      const char* name,
      const void* value
  ) override;
  MockActualCall& with_output_parameter(
      const mu::tiny::test::String& name,
      void* output
  ) override;
  MockActualCall& with_output_parameter_of_type(
      const mu::tiny::test::String& type,
      const mu::tiny::test::String& name,
      void* output
  ) override;

  bool has_return_value() override;
  MockNamedValue return_value() override;

  bool return_bool_value_or_default(bool default_value) override;
  bool return_bool_value() override;

  int return_int_value_or_default(int default_value) override;
  int return_int_value() override;

  unsigned long int return_unsigned_long_int_value() override;
  unsigned long int return_unsigned_long_int_value_or_default(
      unsigned long int
  ) override;

  long int return_long_int_value() override;
  long int return_long_int_value_or_default(long int default_value) override;

  unsigned long long return_unsigned_long_long_int_value() override;
  unsigned long long return_unsigned_long_long_int_value_or_default(
      unsigned long long default_value
  ) override;

  long long return_long_long_int_value() override;
  long long return_long_long_int_value_or_default(
      long long default_value
  ) override;

  unsigned int return_unsigned_int_value() override;
  unsigned int return_unsigned_int_value_or_default(
      unsigned int default_value
  ) override;

  const char* return_string_value_or_default(
      const char* default_value
  ) override;
  const char* return_string_value() override;

  double return_double_value() override;
  double return_double_value_or_default(double default_value) override;

  const void* return_const_pointer_value() override;
  const void* return_const_pointer_value_or_default(
      const void* default_value
  ) override;

  void* return_pointer_value() override;
  void* return_pointer_value_or_default(void*) override;

  FunctionPointerReturnValue return_function_pointer_value() override;
  FunctionPointerReturnValue return_function_pointer_value_or_default(
      void (*)()
  ) override;

  MockActualCall& on_object(const void* object_ptr) override;

  virtual bool is_fulfilled() const;
  virtual bool has_failed() const;

  virtual void check_expectations();

  virtual void set_mock_failure_reporter(MockFailureReporter* reporter);
  void set_name_and_check(mu::tiny::test::String name);

protected:
  const mu::tiny::test::String& get_name() const;
  virtual mu::tiny::test::TestShell* get_test() const;
  virtual void call_has_succeeded();
  virtual void copy_output_parameters(MockCheckedExpectedCall* call);
  virtual void complete_call_when_match_is_found();
  void fail_with(MockFailure failure);
  virtual void check_input_parameter(MockNamedValue actual_parameter);
  virtual void check_output_parameter(MockNamedValue output_parameter);
  virtual void discard_currently_matching_expectations();

  enum class ActualCallState
  {
    in_progress,
    failed,
    success
  };
  virtual void set_state(ActualCallState state);

private:
  mu::tiny::test::String function_name_;
  unsigned int call_order_;
  MockFailureReporter* reporter_;

  ActualCallState state_{ ActualCallState::success };
  bool expectations_checked_{ false };
  MockCheckedExpectedCall* matching_expectation_{ nullptr };

  MockExpectedCallsList potentially_matching_expectations_;
  const MockExpectedCallsList& all_expectations_;

  class MockOutputParametersListNode
  {
  public:
    mu::tiny::test::String name;
    mu::tiny::test::String type;
    void* ptr;

    MockOutputParametersListNode* next{ nullptr };
    MockOutputParametersListNode(
        const mu::tiny::test::String& n,
        const mu::tiny::test::String& t,
        void* p
    )
      : name(n)
      , type(t)
      , ptr(p)

    {
    }
  };

  MockOutputParametersListNode* output_parameter_expectations_{ nullptr };

  virtual void add_output_parameter(
      const mu::tiny::test::String& name,
      const mu::tiny::test::String& type,
      void* ptr
  );
  void clean_up_output_parameter_list();
};

}
}
} // namespace mu::tiny::mock

#endif
