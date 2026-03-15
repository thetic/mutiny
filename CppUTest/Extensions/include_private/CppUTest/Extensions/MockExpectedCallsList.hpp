#ifndef INCLUDED_CPPUTEST_EXTENSIONS_MOCKEXPECTEDCALLSLIST_HPP
#define INCLUDED_CPPUTEST_EXTENSIONS_MOCKEXPECTEDCALLSLIST_HPP

#include "CppUTest/String.hpp"

namespace cpputest {
namespace extensions {

class MockCheckedExpectedCall;
class MockNamedValue;

class MockExpectedCallsList
{

public:
  MockExpectedCallsList();
  virtual ~MockExpectedCallsList();
  virtual void delete_all_expectations_and_clear_list();

  virtual unsigned int size() const;
  virtual unsigned int amount_of_actual_calls_fulfilled_for(
      const cpputest::String& name) const;
  virtual unsigned int amount_of_unfulfilled_expectations() const;
  virtual bool has_unfulfilled_expectations() const;
  virtual bool has_finalized_matching_expectations() const;
  virtual bool has_unmatching_expectations_because_of_missing_parameters()
      const;
  virtual bool has_expectation_with_name(const cpputest::String& name) const;
  virtual bool has_calls_out_of_order() const;
  virtual bool empty() const;

  virtual void add_expected_call(MockCheckedExpectedCall* call);
  virtual void add_expectations(const MockExpectedCallsList& list);
  virtual void add_expectations_related_to(const cpputest::String& name,
      const MockExpectedCallsList& list);

  virtual void only_keep_out_of_order_expectations();
  virtual void add_potentially_matching_expectations(
      const MockExpectedCallsList& list);

  virtual void only_keep_expectations_related_to(const cpputest::String& name);
  virtual void only_keep_expectations_with_input_parameter(
      const MockNamedValue& parameter);
  virtual void only_keep_expectations_with_input_parameter_name(
      const cpputest::String& name);
  virtual void only_keep_expectations_with_output_parameter(
      const MockNamedValue& parameter);
  virtual void only_keep_expectations_with_output_parameter_name(
      const cpputest::String& name);
  virtual void only_keep_expectations_on_object(const void* object_ptr);
  virtual void only_keep_unmatching_expectations();

  virtual MockCheckedExpectedCall*
  remove_first_finalized_matching_expectation();
  virtual MockCheckedExpectedCall* remove_first_matching_expectation();
  virtual MockCheckedExpectedCall* get_first_matching_expectation();

  virtual void reset_actual_call_matching_state();
  virtual void was_passed_to_object();
  virtual void parameter_was_passed(const cpputest::String& parameter_name);
  virtual void output_parameter_was_passed(
      const cpputest::String& parameter_name);

  virtual cpputest::String unfulfilled_calls_to_string(
      const cpputest::String& line_prefix = "") const;
  virtual cpputest::String fulfilled_calls_to_string(
      const cpputest::String& line_prefix = "") const;
  virtual cpputest::String calls_with_missing_parameters_to_string(
      const cpputest::String& line_prefix,
      const cpputest::String& missing_parameters_prefix) const;

protected:
  virtual void prune_empty_node_from_list();

  class MockExpectedCallsListNode
  {
  public:
    MockCheckedExpectedCall* expected_call;

    MockExpectedCallsListNode* next;
    MockExpectedCallsListNode(MockCheckedExpectedCall* ec)
      : expected_call(ec)
      , next(nullptr)
    {
    }
  };

private:
  MockExpectedCallsListNode* head_;

  MockExpectedCallsList(const MockExpectedCallsList&);
};

} // namespace extensions
} // namespace cpputest

#endif
