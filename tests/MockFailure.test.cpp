#include "CppUTest/MockFailure.hpp"

#include "MockFailureReporterForTest.hpp"

#include "CppUTest/CppUTest.hpp"
#include "CppUTest/MockCheckedExpectedCall.hpp"
#include "CppUTest/MockExpectedCallsList.hpp"

TEST_GROUP(MockFailure)
{
  cpputest::MockFailureReporter reporter;

  MockExpectedCallsListForTest::MockExpectedCallsList* list;
  cpputest::MockCheckedExpectedCall* call1;
  cpputest::MockCheckedExpectedCall* call2;
  cpputest::MockCheckedExpectedCall* call3;
  cpputest::MockCheckedExpectedCall* call4;
  cpputest::MockCheckedExpectedCall* call5;

  void setup() override
  {
    list = new MockExpectedCallsListForTest::MockExpectedCallsList;
    call1 = new cpputest::MockCheckedExpectedCall;
    call2 = new cpputest::MockCheckedExpectedCall;
    call3 = new cpputest::MockCheckedExpectedCall;
    call4 = new cpputest::MockCheckedExpectedCall;
    call5 = new cpputest::MockCheckedExpectedCall;
  }
  void teardown() override
  {
    delete list;
    delete call1;
    delete call2;
    delete call3;
    delete call4;
    delete call5;
    CHECK_NO_MOCK_FAILURE();
    MockFailureReporterForTest::clear_reporter();
  }

  void add_three_calls_to_list()
  {
    list->add_expected_call(call1);
    list->add_expected_call(call2);
    list->add_expected_call(call3);
  }

  void add_five_calls_to_list()
  {
    list->add_expected_call(call1);
    list->add_expected_call(call2);
    list->add_expected_call(call3);
    list->add_expected_call(call4);
    list->add_expected_call(call5);
  }

  void check_unexpected_nth_call_message(unsigned int count,
      const char* expected_ordinal)
  {
    MockExpectedCallsListForTest::MockExpectedCallsList call_list;
    cpputest::MockCheckedExpectedCall expected_call_single(1);
    cpputest::MockCheckedExpectedCall expected_call_multi(count - 1);

    expected_call_single.with_name("bar");
    expected_call_multi.with_name("bar");

    if (count > 1) {
      call_list.add_expected_call(&expected_call_single);
      expected_call_single.call_was_made(1);
    }

    if (count > 2) {
      call_list.add_expected_call(&expected_call_multi);
      for (unsigned int i = 1; i < (count - 1); i++) {
        expected_call_multi.call_was_made(i + 1);
      }
    }

    cpputest::MockUnexpectedCallHappenedFailure failure(
        cpputest::TestShell::get_current(), "bar", call_list);

    cpputest::String expected_message = cpputest::string_from_format(
        "Mock Failure: Unexpected additional (%s) call to "
        "function: bar\n\tEXPECTED",
        expected_ordinal);
    STRCMP_CONTAINS(expected_message.c_str(), failure.get_message().c_str());
  }
};

TEST(MockFailure, noErrorFailureSomethingGoneWrong)
{
  cpputest::MockFailure failure((cpputest::TestShell::get_current()));
  STRCMP_EQUAL("Test failed with MockFailure without an error! Something went "
               "seriously wrong.",
      failure.get_message().c_str());
}

TEST(MockFailure, unexpectedCallHappened)
{
  cpputest::MockUnexpectedCallHappenedFailure failure(
      cpputest::TestShell::get_current(), "foobar", *list);
  STRCMP_EQUAL("Mock Failure: Unexpected call to function: foobar\n"
               "\tEXPECTED calls that WERE NOT fulfilled:\n"
               "\t\t<none>\n"
               "\tEXPECTED calls that WERE fulfilled:\n"
               "\t\t<none>",
      failure.get_message().c_str());
}

TEST(MockFailure, expectedCallDidNotHappen)
{
  call1->with_name("foobar");
  call2->with_name("world").with_parameter("boo", 2).with_parameter(
      "hello", "world");
  call3->with_name("haphaphap");
  call3->call_was_made(1);
  add_three_calls_to_list();

  cpputest::MockExpectedCallsDidntHappenFailure failure(
      cpputest::TestShell::get_current(), *list);
  STRCMP_EQUAL(
      "Mock Failure: Expected call WAS NOT fulfilled.\n"
      "\tEXPECTED calls that WERE NOT fulfilled:\n"
      "\t\tfoobar -> no parameters (expected 1 call, called 0 times)\n"
      "\t\tworld -> int boo: <2 (0x2)>, const char* hello: <world> (expected 1 "
      "call, called 0 times)\n"
      "\tEXPECTED calls that WERE fulfilled:\n"
      "\t\thaphaphap -> no parameters (expected 1 call, called 1 time)",
      failure.get_message().c_str());
}

TEST(MockFailure, MockUnexpectedNthAdditionalCallFailure)
{
  check_unexpected_nth_call_message(2, "2nd");
  check_unexpected_nth_call_message(3, "3rd");
  check_unexpected_nth_call_message(4, "4th");
  check_unexpected_nth_call_message(11, "11th");
  check_unexpected_nth_call_message(12, "12th");
  check_unexpected_nth_call_message(13, "13th");
  check_unexpected_nth_call_message(14, "14th");
  check_unexpected_nth_call_message(21, "21st");
  check_unexpected_nth_call_message(22, "22nd");
  check_unexpected_nth_call_message(23, "23rd");
}

TEST(MockFailure, MockUnexpectedInputParameterFailure)
{
  call1->with_name("foo").with_parameter("boo", 2);
  call2->with_name("foo").with_parameter("boo", 3.3);
  call3->with_name("unrelated");
  add_three_calls_to_list();

  cpputest::MockNamedValue actual_parameter("bar");
  actual_parameter.set_value(2);

  cpputest::MockUnexpectedInputParameterFailure failure(
      cpputest::TestShell::get_current(), "foo", actual_parameter, *list);
  STRCMP_EQUAL(
      "Mock Failure: Unexpected parameter name to function \"foo\": bar\n"
      "\tEXPECTED calls that WERE NOT fulfilled related to function: foo\n"
      "\t\tfoo -> int boo: <2 (0x2)> (expected 1 call, called 0 times)\n"
      "\t\tfoo -> double boo: <3.3> (expected 1 call, called 0 times)\n"
      "\tEXPECTED calls that WERE fulfilled related to function: foo\n"
      "\t\t<none>\n"
      "\tACTUAL unexpected parameter passed to function: foo\n"
      "\t\tint bar: <2 (0x2)>",
      failure.get_message().c_str());
}

TEST(MockFailure, MockUnexpectedOutputParameterFailure)
{
  int out1 = 0;
  int out2 = 0;
  call1->with_name("foo").with_output_parameter_returning(
      "boo", &out1, sizeof(out1));
  call2->with_name("foo").with_output_parameter_returning(
      "boo", &out2, sizeof(out2));
  call3->with_name("unrelated");
  add_three_calls_to_list();

  cpputest::MockNamedValue actual_parameter("bar");
  actual_parameter.set_value(reinterpret_cast<void*>(0x123));

  cpputest::MockUnexpectedOutputParameterFailure failure(
      cpputest::TestShell::get_current(), "foo", actual_parameter, *list);
  STRCMP_EQUAL(
      "Mock Failure: Unexpected output parameter name to function \"foo\": "
      "bar\n"
      "\tEXPECTED calls that WERE NOT fulfilled related to function: foo\n"
      "\t\tfoo -> const void* boo: <output> (expected 1 call, called 0 times)\n"
      "\t\tfoo -> const void* boo: <output> (expected 1 call, called 0 times)\n"
      "\tEXPECTED calls that WERE fulfilled related to function: foo\n"
      "\t\t<none>\n"
      "\tACTUAL unexpected output parameter passed to function: foo\n"
      "\t\tvoid* bar",
      failure.get_message().c_str());
}

TEST(MockFailure, MockUnexpectedUnmodifiedOutputParameterFailure)
{
  int out1 = 0;
  call1->with_name("foo").with_output_parameter_returning(
      "boo", &out1, sizeof(out1));
  call2->with_name("foo").with_unmodified_output_parameter("boo");
  call3->with_name("unrelated");
  add_three_calls_to_list();

  cpputest::MockNamedValue actual_parameter("bar");
  actual_parameter.set_value(reinterpret_cast<void*>(0x123));

  cpputest::MockUnexpectedOutputParameterFailure failure(
      cpputest::TestShell::get_current(), "foo", actual_parameter, *list);
  STRCMP_EQUAL(
      "Mock Failure: Unexpected output parameter name to function \"foo\": "
      "bar\n"
      "\tEXPECTED calls that WERE NOT fulfilled related to function: foo\n"
      "\t\tfoo -> const void* boo: <output> (expected 1 call, called 0 times)\n"
      "\t\tfoo -> const void* boo: <output> (expected 1 call, called 0 times)\n"
      "\tEXPECTED calls that WERE fulfilled related to function: foo\n"
      "\t\t<none>\n"
      "\tACTUAL unexpected output parameter passed to function: foo\n"
      "\t\tvoid* bar",
      failure.get_message().c_str());
}

TEST(MockFailure, MockUnexpectedParameterValueFailure)
{
  call1->with_name("foo").with_parameter("boo", 2);
  call2->with_name("foo").with_parameter("boo", 10);
  call3->with_name("unrelated");
  add_three_calls_to_list();

  cpputest::MockNamedValue actual_parameter("boo");
  actual_parameter.set_value(20);

  cpputest::MockUnexpectedInputParameterFailure failure(
      cpputest::TestShell::get_current(), "foo", actual_parameter, *list);
  STRCMP_EQUAL(
      "Mock Failure: Unexpected parameter value to parameter \"boo\" to "
      "function "
      "\"foo\": <20 (0x14)>\n"
      "\tEXPECTED calls that WERE NOT fulfilled related to function: foo\n"
      "\t\tfoo -> int boo: <2 (0x2)> (expected 1 call, called 0 times)\n"
      "\t\tfoo -> int boo: <10 (0xa)> (expected 1 call, called 0 times)\n"
      "\tEXPECTED calls that WERE fulfilled related to function: foo\n"
      "\t\t<none>\n"
      "\tACTUAL unexpected parameter passed to function: foo\n"
      "\t\tint boo: <20 (0x14)>",
      failure.get_message().c_str());
}

TEST(MockFailure, MockExpectedParameterDidntHappenFailure)
{
  call1->with_name("foo").with_parameter("bar", 2).with_parameter("boo", "str");
  call1->input_parameter_was_passed("bar");
  call2->with_name("foo").with_parameter("bar", 10).with_parameter(
      "boo", "bleh");
  call2->call_was_made(1);
  call2->input_parameter_was_passed("bar");
  call2->input_parameter_was_passed("boo");
  call3->with_name("foo")
      .with_parameter("bar", 2)
      .with_parameter("boo", "blah")
      .with_parameter("baa", 0u);
  call3->input_parameter_was_passed("bar");
  call4->with_name("foo").with_parameter("bar", 20);
  call5->with_name("unrelated");
  add_five_calls_to_list();

  MockExpectedCallsListForTest::MockExpectedCallsList matching_calls;
  matching_calls.add_expected_call(call1);
  matching_calls.add_expected_call(call3);

  cpputest::MockExpectedParameterDidntHappenFailure failure(
      cpputest::TestShell::get_current(), "foo", *list, matching_calls);
  STRCMP_EQUAL(
      "Mock Failure: Expected parameter for function \"foo\" did not happen.\n"
      "\tEXPECTED calls with MISSING parameters related to function: foo\n"
      "\t\tfoo -> int bar: <2 (0x2)>, const char* boo: <str> (expected 1 call, "
      "called 0 times)\n"
      "\t\t\tMISSING parameters: const char* boo\n"
      "\t\tfoo -> int bar: <2 (0x2)>, const char* boo: <blah>, unsigned int "
      "baa: "
      "<0 (0x0)> (expected 1 call, called 0 times)\n"
      "\t\t\tMISSING parameters: const char* boo, unsigned int baa\n"
      "\tEXPECTED calls that WERE NOT fulfilled related to function: foo\n"
      "\t\tfoo -> int bar: <2 (0x2)>, const char* boo: <str> (expected 1 call, "
      "called 0 times)\n"
      "\t\tfoo -> int bar: <2 (0x2)>, const char* boo: <blah>, unsigned int "
      "baa: "
      "<0 (0x0)> (expected 1 call, called 0 times)\n"
      "\t\tfoo -> int bar: <20 (0x14)> (expected 1 call, called 0 times)\n"
      "\tEXPECTED calls that WERE fulfilled related to function: foo\n"
      "\t\tfoo -> int bar: <10 (0xa)>, const char* boo: <bleh> (expected 1 "
      "call, "
      "called 1 time)",
      failure.get_message().c_str());
}

TEST(MockFailure, MockNoWayToCompareCustomTypeFailure)
{
  cpputest::MockNoWayToCompareCustomTypeFailure failure(
      cpputest::TestShell::get_current(), "myType");
  STRCMP_EQUAL("MockFailure: No way to compare type <myType>. Please install a "
               "MockNamedValueComparator.",
      failure.get_message().c_str());
}

TEST(MockFailure, MockUnexpectedObjectFailure)
{
  call1->with_name("foo").on_object(reinterpret_cast<void*>(0x02));
  call2->with_name("foo").on_object(reinterpret_cast<void*>(0x03));
  call2->call_was_made(1);
  call2->was_passed_to_object();
  call3->with_name("unrelated");
  add_three_calls_to_list();

  cpputest::MockUnexpectedObjectFailure failure(
      cpputest::TestShell::get_current(),
      "foo",
      reinterpret_cast<void*>(0x1),
      *list);
  STRCMP_EQUAL(
      cpputest::string_from_format(
          "MockFailure: Function called on an unexpected object: foo\n"
          "\tActual object for call has address: <%p>\n"
          "\tEXPECTED calls that WERE NOT fulfilled related to function: foo\n"
          "\t\t(object address: %p)::foo -> no parameters (expected 1 call, "
          "called "
          "0 times)\n"
          "\tEXPECTED calls that WERE fulfilled related to function: foo\n"
          "\t\t(object address: %p)::foo -> no parameters (expected 1 call, "
          "called "
          "1 time)",
          reinterpret_cast<void*>(0x01),
          reinterpret_cast<void*>(0x02),
          reinterpret_cast<void*>(0x03))
          .c_str(),
      failure.get_message().c_str());
}

TEST(MockFailure, MockExpectedObjectDidntHappenFailure)
{
  call1->with_name("foo").on_object(reinterpret_cast<void*>(0x02));
  call2->with_name("foo").on_object(reinterpret_cast<void*>(0x03));
  call2->call_was_made(1);
  call2->was_passed_to_object();
  call3->with_name("unrelated");
  add_three_calls_to_list();

  cpputest::MockExpectedObjectDidntHappenFailure failure(
      cpputest::TestShell::get_current(), "foo", *list);
  STRCMP_EQUAL(
      cpputest::string_from_format(
          "Mock Failure: Expected call on object for function \"foo\" but it "
          "did "
          "not happen.\n"
          "\tEXPECTED calls that WERE NOT fulfilled related to function: foo\n"
          "\t\t(object address: %p)::foo -> no parameters (expected 1 call, "
          "called "
          "0 times)\n"
          "\tEXPECTED calls that WERE fulfilled related to function: foo\n"
          "\t\t(object address: %p)::foo -> no parameters (expected 1 call, "
          "called "
          "1 time)",
          reinterpret_cast<void*>(0x2),
          reinterpret_cast<void*>(0x3))
          .c_str(),
      failure.get_message().c_str());
}
