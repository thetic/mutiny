#ifndef INCLUDED_MUTINY_MOCK_EXPECTEDCALLSLIST_HPP
#define INCLUDED_MUTINY_MOCK_EXPECTEDCALLSLIST_HPP

#include "mutiny/String.hpp"

namespace mu {
namespace tiny {
namespace mock {

class CheckedExpectedCall;
class NamedValue;

class ExpectedCallsList
{

public:
  ExpectedCallsList() = default;
  virtual ~ExpectedCallsList();
  ExpectedCallsList(const ExpectedCallsList&) = delete;

  virtual void delete_all_expectations_and_clear_list();

  virtual unsigned int amount_of_actual_calls_fulfilled_for(
      const String& name
  ) const;
  virtual bool has_unfulfilled_expectations() const;
  virtual bool has_finalized_matching_expectations() const;
  virtual bool
  has_unmatching_expectations_because_of_missing_parameters() const;
  virtual bool has_expectation_with_name(const String& name) const;
  virtual bool has_calls_out_of_order() const;
  virtual bool empty() const;

  virtual void add_expected_call(CheckedExpectedCall* call);
  virtual void add_expectations(const ExpectedCallsList& list);
  virtual void add_expectations_related_to(
      const String& name,
      const ExpectedCallsList& list
  );

  virtual void only_keep_out_of_order_expectations();
  virtual void add_potentially_matching_expectations(
      const ExpectedCallsList& list
  );

  virtual void only_keep_expectations_related_to(const String& name);
  virtual void only_keep_expectations_with_input_parameter(
      const NamedValue& parameter
  );
  virtual void only_keep_expectations_with_input_parameter_name(
      const String& name
  );
  virtual void only_keep_expectations_with_output_parameter(
      const NamedValue& parameter
  );
  virtual void only_keep_expectations_with_output_parameter_name(
      const String& name
  );
  virtual void only_keep_expectations_on_object(const void* object_ptr);
  virtual void only_keep_unmatching_expectations();

  virtual CheckedExpectedCall* remove_first_finalized_matching_expectation();
  virtual CheckedExpectedCall* remove_first_matching_expectation();
  virtual CheckedExpectedCall* get_first_matching_expectation();

  virtual void reset_actual_call_matching_state();
  virtual void was_passed_to_object();
  virtual void parameter_was_passed(const String& parameter_name);
  virtual void output_parameter_was_passed(const String& parameter_name);

  virtual String unfulfilled_calls_to_string(
      const String& line_prefix = ""
  ) const;
  virtual String fulfilled_calls_to_string(
      const String& line_prefix = ""
  ) const;
  virtual String calls_with_missing_parameters_to_string(
      const String& line_prefix,
      const String& missing_parameters_prefix
  ) const;

protected:
  virtual void prune_empty_node_from_list();

  class MockExpectedCallsListNode
  {
  public:
    CheckedExpectedCall* expected_call;

    MockExpectedCallsListNode* next{ nullptr };
    MockExpectedCallsListNode(CheckedExpectedCall* ec)
      : expected_call(ec)

    {
    }
  };

private:
  MockExpectedCallsListNode* head_{ nullptr };
};

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
