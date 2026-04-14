#ifndef INCLUDED_MUTINY_MOCK_CHECKEDACTUALCALL_HPP
#define INCLUDED_MUTINY_MOCK_CHECKEDACTUALCALL_HPP

#include "mutiny/mock/ActualCall.hpp"
#include "mutiny/mock/ExpectedCallsList.hpp"
#include "mutiny/mock/Failure.hpp"

namespace mu {
namespace tiny {
namespace mock {

class CheckedActualCall : public ActualCall
{
public:
  CheckedActualCall(
      unsigned int call_order,
      FailureReporter* reporter,
      const ExpectedCallsList& expectations
  );
  ~CheckedActualCall() override;

  ActualCall& with_name(const String& name) override;
  ActualCall& with_call_order(unsigned int) override;
  ActualCall& with_typed_parameter(NamedValue parameter) override;
  ActualCall& with_parameter_of_type(
      const String& type,
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
      const String& type,
      const String& name,
      void* output
  ) override;

  bool has_return_value() override;
  NamedValue return_value() override;

  ActualCall& on_object(const void* object_ptr) override;

  virtual bool is_fulfilled() const;
  virtual bool has_failed() const;

  virtual void check_expectations();

  virtual void set_mock_failure_reporter(FailureReporter* reporter);
  void set_name_and_check(String name);

protected:
  const String& get_name() const;
  virtual test::Shell* get_test() const;
  virtual void call_has_succeeded();
  virtual void copy_output_parameters(CheckedExpectedCall* call);
  virtual void complete_call_when_match_is_found();
  void fail_with(Failure failure);
  virtual void check_input_parameter(NamedValue actual_parameter);
  virtual void check_output_parameter(NamedValue output_parameter);
  virtual void discard_currently_matching_expectations();

  enum class MutinyActualCallState
  {
    in_progress,
    failed,
    success
  };
  virtual void set_state(MutinyActualCallState state);

private:
  String function_name_;
  unsigned int call_order_;
  FailureReporter* reporter_;

  MutinyActualCallState state_{ MutinyActualCallState::success };
  bool expectations_checked_{ false };
  CheckedExpectedCall* matching_expectation_{ nullptr };

  ExpectedCallsList potentially_matching_expectations_;
  const ExpectedCallsList& all_expectations_;

  class MockOutputParametersListNode
  {
  public:
    String name;
    String type;
    void* ptr;

    MockOutputParametersListNode* next{ nullptr };
    MockOutputParametersListNode(const String& n, const String& t, void* p)
      : name(n)
      , type(t)
      , ptr(p)

    {
    }
  };

  MockOutputParametersListNode* output_parameter_expectations_{ nullptr };

  virtual void add_output_parameter(
      const String& name,
      const String& type,
      void* ptr
  );
  void clean_up_output_parameter_list();
};

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
