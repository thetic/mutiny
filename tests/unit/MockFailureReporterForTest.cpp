#include "MockFailureReporterForTest.hpp"

ExpectedCallsListForTest::~ExpectedCallsListForTest()
{
  delete_all_expectations_and_clear_list();
}

mu::tiny::mock::CheckedExpectedCall* ExpectedCallsListForTest::add_function(
    const mu::tiny::String& name
)
{
  auto* new_call = new mu::tiny::mock::CheckedExpectedCall;
  new_call->with_name(name);
  add_expected_call(new_call);
  return new_call;
}

mu::tiny::mock::CheckedExpectedCall* ExpectedCallsListForTest::add_function(
    unsigned int num_calls,
    const mu::tiny::String& name
)
{
  auto* new_call = new mu::tiny::mock::CheckedExpectedCall(num_calls);
  new_call->with_name(name);
  add_expected_call(new_call);
  return new_call;
}

mu::tiny::mock::CheckedExpectedCall* ExpectedCallsListForTest::
    add_function_ordered(const mu::tiny::String& name, unsigned int order)
{
  auto* new_call = add_function(name);
  new_call->with_call_order(order);
  return new_call;
}
