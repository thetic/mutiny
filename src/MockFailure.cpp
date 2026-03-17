#include "CppMu/MockFailure.hpp"

#include "CppMu/MockExpectedCall.hpp"
#include "CppMu/MockExpectedCallsList.hpp"
#include "CppMu/MockNamedValue.hpp"
#include "CppMu/TestShell.hpp"

namespace cppmu {

class MockFailureReporterTestTerminator : public TestTerminator
{
public:
  MockFailureReporterTestTerminator(bool crash_on_failure)
    : crash_on_failure_(crash_on_failure)
  {
  }

  void exit_current_test() const override
  {
    if (crash_on_failure_)
      cppmu::TestShell::crash();

    cppmu::TestShell::get_current_test_terminator().exit_current_test();
  }

  ~MockFailureReporterTestTerminator() override = default;

private:
  bool crash_on_failure_;
};

void MockFailureReporter::report_failure(const MockFailure& failure)
{
  if (!get_test_to_fail()->has_failed())
    get_test_to_fail()->add_failure(failure);
}

void MockFailureReporter::exit_test()
{
  MockFailureReporterTestTerminator(crash_on_failure_).exit_current_test();
}

void MockFailureReporter::fail_test(MockFailure failure)
{
  if (!get_test_to_fail()->has_failed()) {
    report_failure(failure);
    {
      MockFailure f(static_cast<MockFailure&&>(failure));
    }
    exit_test();
  }
}

cppmu::TestShell* MockFailureReporter::get_test_to_fail()
{
  return cppmu::TestShell::get_current();
}

MockFailure::MockFailure(cppmu::TestShell* test)
  : TestFailure(
        test,
        "Test failed with MockFailure without an error! Something went "
        "seriously wrong."
    )
{
}

void MockFailure::add_expectations_and_call_history(
    const MockExpectedCallsList& expectations
)
{
  message_ += "\tEXPECTED calls that WERE NOT fulfilled:\n";
  message_ += expectations.unfulfilled_calls_to_string("\t\t");
  message_ += "\n\tEXPECTED calls that WERE fulfilled:\n";
  message_ += expectations.fulfilled_calls_to_string("\t\t");
}

void MockFailure::add_expectations_and_call_history_related_to(
    const String& name,
    const MockExpectedCallsList& expectations
)
{
  MockExpectedCallsList expectations_for_function;
  expectations_for_function.add_expectations_related_to(name, expectations);

  message_ += "\tEXPECTED calls that WERE NOT fulfilled related to function: ";
  message_ += name;
  message_ += "\n";

  message_ += expectations_for_function.unfulfilled_calls_to_string("\t\t");

  message_ += "\n\tEXPECTED calls that WERE fulfilled related to function: ";
  message_ += name;
  message_ += "\n";

  message_ += expectations_for_function.fulfilled_calls_to_string("\t\t");
}

MockExpectedCallsDidntHappenFailure::MockExpectedCallsDidntHappenFailure(
    cppmu::TestShell* test,
    const MockExpectedCallsList& expectations
)
  : MockFailure(test)
{
  message_ = "Mock Failure: Expected call WAS NOT fulfilled.\n";
  add_expectations_and_call_history(expectations);
}

MockUnexpectedCallHappenedFailure::MockUnexpectedCallHappenedFailure(
    cppmu::TestShell* test,
    const String& name,
    const MockExpectedCallsList& expectations
)
  : MockFailure(test)
{
  unsigned int amount_of_actual_calls =
      expectations.amount_of_actual_calls_fulfilled_for(name);
  if (amount_of_actual_calls > 0) {
    String ordinal_number =
        string_from_ordinal_number(amount_of_actual_calls + 1);
    message_ = string_from_format(
        "Mock Failure: Unexpected additional (%s) call to function: ",
        ordinal_number.c_str()
    );
  } else {
    message_ = "Mock Failure: Unexpected call to function: ";
  }
  message_ += name;
  message_ += "\n";
  add_expectations_and_call_history(expectations);
}

MockCallOrderFailure::MockCallOrderFailure(
    cppmu::TestShell* test,
    const MockExpectedCallsList& expectations
)
  : MockFailure(test)
{
  MockExpectedCallsList expectations_for_out_of_order;
  expectations_for_out_of_order.add_expectations(expectations);
  expectations_for_out_of_order.only_keep_out_of_order_expectations();

  message_ = "Mock Failure: Out of order calls";
  message_ += "\n";
  add_expectations_and_call_history(expectations_for_out_of_order);
}

MockUnexpectedInputParameterFailure::MockUnexpectedInputParameterFailure(
    cppmu::TestShell* test,
    const String& function_name,
    MockNamedValue parameter,
    const MockExpectedCallsList& expectations
)
  : MockFailure(test)
{
  MockExpectedCallsList expectations_for_function_with_parameter_name;
  expectations_for_function_with_parameter_name.add_expectations_related_to(
      function_name, expectations
  );
  expectations_for_function_with_parameter_name
      .only_keep_expectations_with_input_parameter_name(parameter.get_name());

  if (expectations_for_function_with_parameter_name.empty()) {
    message_ = "Mock Failure: Unexpected parameter name to function \"";
    message_ += function_name;
    message_ += "\": ";
    message_ += parameter.get_name();
  } else {
    message_ = "Mock Failure: Unexpected parameter value to parameter \"";
    message_ += parameter.get_name();
    message_ += "\" to function \"";
    message_ += function_name;
    message_ += "\": <";
    message_ += string_from(parameter);
    message_ += ">";
  }

  message_ += "\n";
  add_expectations_and_call_history_related_to(function_name, expectations);

  message_ += "\n\tACTUAL unexpected parameter passed to function: ";
  message_ += function_name;
  message_ += "\n";

  message_ += "\t\t";
  message_ += parameter.get_type();
  message_ += " ";
  message_ += parameter.get_name();
  message_ += ": <";
  message_ += string_from(parameter);
  message_ += ">";
}

MockUnexpectedOutputParameterFailure::MockUnexpectedOutputParameterFailure(
    cppmu::TestShell* test,
    const String& function_name,
    MockNamedValue parameter,
    const MockExpectedCallsList& expectations
)
  : MockFailure(test)
{
  MockExpectedCallsList expectations_for_function_with_parameter_name;
  expectations_for_function_with_parameter_name.add_expectations_related_to(
      function_name, expectations
  );
  expectations_for_function_with_parameter_name
      .only_keep_expectations_with_output_parameter_name(parameter.get_name());

  if (expectations_for_function_with_parameter_name.empty()) {
    message_ = "Mock Failure: Unexpected output parameter name to function \"";
    message_ += function_name;
    message_ += "\": ";
    message_ += parameter.get_name();
  } else {
    message_ = "Mock Failure: Unexpected parameter type \"";
    message_ += parameter.get_type();
    message_ += "\" to output parameter \"";
    message_ += parameter.get_name();
    message_ += "\" to function \"";
    message_ += function_name;
    message_ += "\"";
  }

  message_ += "\n";
  add_expectations_and_call_history_related_to(function_name, expectations);

  message_ += "\n\tACTUAL unexpected output parameter passed to function: ";
  message_ += function_name;
  message_ += "\n";

  message_ += "\t\t";
  message_ += parameter.get_type();
  message_ += " ";
  message_ += parameter.get_name();
}

MockExpectedParameterDidntHappenFailure::
    MockExpectedParameterDidntHappenFailure(
        cppmu::TestShell* test,
        const String& function_name,
        const MockExpectedCallsList& all_expectations,
        const MockExpectedCallsList& matching_expectations
    )
  : MockFailure(test)
{
  message_ = "Mock Failure: Expected parameter for function \"";
  message_ += function_name;
  message_ += "\" did not happen.\n";

  message_ += "\tEXPECTED calls with MISSING parameters related to function: ";
  message_ += function_name;
  message_ += "\n";
  message_ += matching_expectations.calls_with_missing_parameters_to_string(
      "\t\t", "\tMISSING parameters: "
  );
  message_ += "\n";

  add_expectations_and_call_history_related_to(function_name, all_expectations);
}

MockNoWayToCompareCustomTypeFailure::MockNoWayToCompareCustomTypeFailure(
    cppmu::TestShell* test,
    String type_name
)
  : MockFailure(test)
{
  message_ = string_from_format(
      "MockFailure: No way to compare type <%s>. "
      "Please install a MockNamedValueComparator.",
      type_name.c_str()
  );
}

MockNoWayToCopyCustomTypeFailure::MockNoWayToCopyCustomTypeFailure(
    cppmu::TestShell* test,
    String type_name
)
  : MockFailure(test)
{
  message_ = string_from_format(
      "MockFailure: No way to copy type <%s>. Please "
      "install a MockNamedValueCopier.",
      type_name.c_str()
  );
}

MockUnexpectedObjectFailure::MockUnexpectedObjectFailure(
    cppmu::TestShell* test,
    const String& function_name,
    const void* actual,
    const MockExpectedCallsList& expectations
)
  : MockFailure(test)
{
  message_ = string_from_format(
      "MockFailure: Function called on an unexpected object: %s\n"
      "\tActual object for call has address: <%p>\n",
      function_name.c_str(),
      actual
  );
  add_expectations_and_call_history_related_to(function_name, expectations);
}

MockExpectedObjectDidntHappenFailure::MockExpectedObjectDidntHappenFailure(
    cppmu::TestShell* test,
    const String& function_name,
    const MockExpectedCallsList& expectations
)
  : MockFailure(test)
{
  message_ = string_from_format(
      "Mock Failure: Expected call on object for "
      "function \"%s\" but it did not happen.\n",
      function_name.c_str()
  );
  add_expectations_and_call_history_related_to(function_name, expectations);
}

} // namespace cppmu
