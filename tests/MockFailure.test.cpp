#include "MockFailureReporterForTest.hpp"

#include "mutiny/mock/CheckedExpectedCall.hpp"
#include "mutiny/mock/ExpectedCallsList.hpp"
#include "mutiny/mock/Failure.hpp"

#include "mutiny/test.hpp"

TEST_GROUP(MockFailure)
{
  mu::tiny::mock::FailureReporter reporter;

  ExpectedCallsListForTest::ExpectedCallsList* list;
  mu::tiny::mock::CheckedExpectedCall* call1;
  mu::tiny::mock::CheckedExpectedCall* call2;
  mu::tiny::mock::CheckedExpectedCall* call3;
  mu::tiny::mock::CheckedExpectedCall* call4;
  mu::tiny::mock::CheckedExpectedCall* call5;

  void setup() override
  {
    list = new ExpectedCallsListForTest::ExpectedCallsList;
    call1 = new mu::tiny::mock::CheckedExpectedCall;
    call2 = new mu::tiny::mock::CheckedExpectedCall;
    call3 = new mu::tiny::mock::CheckedExpectedCall;
    call4 = new mu::tiny::mock::CheckedExpectedCall;
    call5 = new mu::tiny::mock::CheckedExpectedCall;
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
    FailureReporterForTest::clear_reporter();
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

  void check_unexpected_nth_call_message(
      unsigned int count,
      const char* expected_ordinal
  )
  {
    ExpectedCallsListForTest::ExpectedCallsList call_list;
    mu::tiny::mock::CheckedExpectedCall expected_call_single(1);
    mu::tiny::mock::CheckedExpectedCall expected_call_multi(count - 1);

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

    mu::tiny::mock::UnexpectedCallHappenedFailure failure(
        mu::tiny::test::Shell::get_current(), "bar", call_list
    );

    mu::tiny::String expected_message = mu::tiny::string_from_format(
        "Mock Failure: Unexpected additional (%s) call to "
        "function: bar\n\tEXPECTED",
        expected_ordinal
    );
    STRCMP_CONTAINS(expected_message.c_str(), failure.get_message().c_str());
  }
};

TEST(MockFailure, noErrorFailureSomethingGoneWrong)
{
  mu::tiny::mock::Failure failure((mu::tiny::test::Shell::get_current()));
  STRCMP_EQUAL(
      "Test failed with Failure without an error! Something went "
      "seriously wrong.",
      failure.get_message().c_str()
  );
}

TEST(MockFailure, unexpectedCallHappened)
{
  mu::tiny::mock::UnexpectedCallHappenedFailure failure(
      mu::tiny::test::Shell::get_current(), "foobar", *list
  );
  STRCMP_EQUAL(
      "Mock Failure: Unexpected call to function: foobar\n"
      "\tEXPECTED calls that WERE NOT fulfilled:\n"
      "\t\t<none>\n"
      "\tEXPECTED calls that WERE fulfilled:\n"
      "\t\t<none>",
      failure.get_message().c_str()
  );
}

TEST(MockFailure, expectedCallDidNotHappen)
{
  call1->with_name("foobar");
  call2->with_name("world").with_parameter("boo", 2).with_parameter(
      "hello", "world"
  );
  call3->with_name("haphaphap");
  call3->call_was_made(1);
  add_three_calls_to_list();

  mu::tiny::mock::ExpectedCallsDidntHappenFailure failure(
      mu::tiny::test::Shell::get_current(), *list
  );
  STRCMP_EQUAL(
      "Mock Failure: Expected call WAS NOT fulfilled.\n"
      "\tEXPECTED calls that WERE NOT fulfilled:\n"
      "\t\tfoobar -> no parameters (expected 1 call, called 0 times)\n"
      "\t\tworld -> int boo: <2 (0x2)>, const char* hello: <world> (expected 1 "
      "call, called 0 times)\n"
      "\tEXPECTED calls that WERE fulfilled:\n"
      "\t\thaphaphap -> no parameters (expected 1 call, called 1 time)",
      failure.get_message().c_str()
  );
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

TEST(MockFailure, UnexpectedInputParameterFailure)
{
  call1->with_name("foo").with_parameter("boo", 2);
  call2->with_name("foo").with_parameter("boo", 3.3);
  call3->with_name("unrelated");
  add_three_calls_to_list();

  mu::tiny::mock::NamedValue actual_parameter("bar");
  actual_parameter.set_value(2);

  mu::tiny::mock::UnexpectedInputParameterFailure failure(
      mu::tiny::test::Shell::get_current(), "foo", actual_parameter, *list
  );
  STRCMP_EQUAL(
      "Mock Failure: Unexpected parameter name to function \"foo\": bar\n"
      "\tEXPECTED calls that WERE NOT fulfilled related to function: foo\n"
      "\t\tfoo -> int boo: <2 (0x2)> (expected 1 call, called 0 times)\n"
      "\t\tfoo -> double boo: <3.3> (expected 1 call, called 0 times)\n"
      "\tEXPECTED calls that WERE fulfilled related to function: foo\n"
      "\t\t<none>\n"
      "\tACTUAL unexpected parameter passed to function: foo\n"
      "\t\tint bar: <2 (0x2)>",
      failure.get_message().c_str()
  );
}

TEST(MockFailure, UnexpectedOutputParameterFailure)
{
  int out1 = 0;
  int out2 = 0;
  call1->with_name("foo").with_output_parameter_returning(
      "boo", &out1, sizeof(out1)
  );
  call2->with_name("foo").with_output_parameter_returning(
      "boo", &out2, sizeof(out2)
  );
  call3->with_name("unrelated");
  add_three_calls_to_list();

  mu::tiny::mock::NamedValue actual_parameter("bar");
  actual_parameter.set_value(reinterpret_cast<void*>(0x123));

  mu::tiny::mock::UnexpectedOutputParameterFailure failure(
      mu::tiny::test::Shell::get_current(), "foo", actual_parameter, *list
  );
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
      failure.get_message().c_str()
  );
}

TEST(MockFailure, MockUnexpectedUnmodifiedOutputParameterFailure)
{
  int out1 = 0;
  call1->with_name("foo").with_output_parameter_returning(
      "boo", &out1, sizeof(out1)
  );
  call2->with_name("foo").with_unmodified_output_parameter("boo");
  call3->with_name("unrelated");
  add_three_calls_to_list();

  mu::tiny::mock::NamedValue actual_parameter("bar");
  actual_parameter.set_value(reinterpret_cast<void*>(0x123));

  mu::tiny::mock::UnexpectedOutputParameterFailure failure(
      mu::tiny::test::Shell::get_current(), "foo", actual_parameter, *list
  );
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
      failure.get_message().c_str()
  );
}

TEST(MockFailure, MockUnexpectedParameterValueFailure)
{
  call1->with_name("foo").with_parameter("boo", 2);
  call2->with_name("foo").with_parameter("boo", 10);
  call3->with_name("unrelated");
  add_three_calls_to_list();

  mu::tiny::mock::NamedValue actual_parameter("boo");
  actual_parameter.set_value(20);

  mu::tiny::mock::UnexpectedInputParameterFailure failure(
      mu::tiny::test::Shell::get_current(), "foo", actual_parameter, *list
  );
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
      failure.get_message().c_str()
  );
}

TEST(MockFailure, ExpectedParameterDidntHappenFailure)
{
  call1->with_name("foo").with_parameter("bar", 2).with_parameter("boo", "str");
  call1->input_parameter_was_passed("bar");
  call2->with_name("foo").with_parameter("bar", 10).with_parameter(
      "boo", "bleh"
  );
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

  ExpectedCallsListForTest::ExpectedCallsList matching_calls;
  matching_calls.add_expected_call(call1);
  matching_calls.add_expected_call(call3);

  mu::tiny::mock::ExpectedParameterDidntHappenFailure failure(
      mu::tiny::test::Shell::get_current(), "foo", *list, matching_calls
  );
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
      failure.get_message().c_str()
  );
}

TEST(MockFailure, MockExpectedParameterDidntHappenWithTwoMissingOutputParams)
{
  int val1 = 1;
  int val2 = 2;
  call1->with_name("foo")
      .with_output_parameter_returning("out1", &val1, sizeof(val1))
      .with_output_parameter_returning("out2", &val2, sizeof(val2));

  ExpectedCallsListForTest::ExpectedCallsList matching_calls;
  matching_calls.add_expected_call(call1);
  list->add_expected_call(call1);

  mu::tiny::mock::ExpectedParameterDidntHappenFailure failure(
      mu::tiny::test::Shell::get_current(), "foo", *list, matching_calls
  );
  STRCMP_CONTAINS(
      "MISSING parameters: const void* out1, const void* out2",
      failure.get_message().c_str()
  );
}

TEST(MockFailure, NoWayToCompareCustomTypeFailure)
{
  mu::tiny::mock::NoWayToCompareCustomTypeFailure failure(
      mu::tiny::test::Shell::get_current(), "myType"
  );
  STRCMP_EQUAL(
      "Failure: No way to compare type <myType>. Please install a "
      "NamedValueComparator.",
      failure.get_message().c_str()
  );
}

TEST(MockFailure, UnexpectedObjectFailure)
{
  call1->with_name("foo").on_object(reinterpret_cast<void*>(0x02));
  call2->with_name("foo").on_object(reinterpret_cast<void*>(0x03));
  call2->call_was_made(1);
  call2->was_passed_to_object();
  call3->with_name("unrelated");
  add_three_calls_to_list();

  mu::tiny::mock::UnexpectedObjectFailure failure(
      mu::tiny::test::Shell::get_current(),
      "foo",
      reinterpret_cast<void*>(0x1),
      *list
  );
  STRCMP_EQUAL(
      mu::tiny::string_from_format(
          "Failure: Function called on an unexpected object: foo\n"
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
          reinterpret_cast<void*>(0x03)
      )
          .c_str(),
      failure.get_message().c_str()
  );
}

TEST(MockFailure, ExpectedObjectDidntHappenFailure)
{
  call1->with_name("foo").on_object(reinterpret_cast<void*>(0x02));
  call2->with_name("foo").on_object(reinterpret_cast<void*>(0x03));
  call2->call_was_made(1);
  call2->was_passed_to_object();
  call3->with_name("unrelated");
  add_three_calls_to_list();

  mu::tiny::mock::ExpectedObjectDidntHappenFailure failure(
      mu::tiny::test::Shell::get_current(), "foo", *list
  );
  STRCMP_EQUAL(
      mu::tiny::string_from_format(
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
          reinterpret_cast<void*>(0x3)
      )
          .c_str(),
      failure.get_message().c_str()
  );
}
