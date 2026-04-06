#include "MockFailureReporter.hpp"

#include "mutiny/test/TestingFixture.hpp"

#include "mutiny/mock.hpp"
#include "mutiny/test.hpp"

using mu::tiny::mock::mock;

TEST_GROUP(Support)
{
  FailureReporterInstaller failure_reporter_installer;

  void teardown() override
  {
    mock().check_expectations();
    CHECK_NO_MOCK_FAILURE();
    FailureReporterForTest::clear_reporter();
    mock().clear();
  }
};

TEST(Support, setDataForUnsignedIntegerValues)
{
  unsigned int expected_data = 7;
  mock().set_data("data", expected_data);
  CHECK_EQUAL(expected_data, mock().get_data("data").get_unsigned_int_value());
}

TEST(Support, setDataForIntegerValues)
{
  int expected_data = 10;
  mock().set_data("data", expected_data);
  CHECK_EQUAL(expected_data, mock().get_data("data").get_int_value());
}

TEST(Support, setDataForBooleanValues)
{
  bool expected_data = true;
  mock().set_data("data", expected_data);
  CHECK_EQUAL(expected_data, mock().get_data("data").get_bool_value());
}

TEST(Support, hasDataBeenSet)
{
  CHECK(!mock().has_data("data"));
  mock().set_data("data", 10);
  CHECK(mock().has_data("data"));
}

TEST(Support, dataCanBeChanged)
{
  mock().set_data("data", 10);
  mock().set_data("data", 15);
  CHECK_EQUAL(15, mock().get_data("data").get_int_value());
}

TEST(Support, uninitializedData)
{
  CHECK_EQUAL(0, mock().get_data("nonexisting").get_int_value());
  STRCMP_EQUAL("int", mock().get_data("nonexisting").get_type().c_str());
}

TEST(Support, setMultipleData)
{
  mock().set_data("data", 1);
  mock().set_data("data2", 10);
  CHECK_EQUAL(1, mock().get_data("data").get_int_value());
  CHECK_EQUAL(10, mock().get_data("data2").get_int_value());
}

TEST(Support, setDataString)
{
  mock().set_data("data", "string");
  STRCMP_EQUAL("string", mock().get_data("data").get_string_value());
}

TEST(Support, setDataDouble)
{
  mock().set_data("data", 1.0);
  DOUBLES_EQUAL(1.0, mock().get_data("data").get_double_value(), 0.05);
}

TEST(Support, setDataLongInt)
{
  long int i = 100;
  mock().set_data("data", i);
  CHECK_EQUAL(i, mock().get_data("data").get_long_int_value());
}

TEST(Support, setDataUnsignedLongInt)
{
  unsigned long int i = 100;
  mock().set_data("data", i);
  CHECK_EQUAL(i, mock().get_data("data").get_unsigned_long_int_value());
}

TEST(Support, setDataPointer)
{
  void* ptr = reinterpret_cast<void*>(0x001);
  mock().set_data("data", ptr);
  POINTERS_EQUAL(ptr, mock().get_data("data").get_pointer_value());
}

TEST(Support, setConstDataPointer)
{
  const void* ptr = reinterpret_cast<const void*>(0x001);
  mock().set_data("data", ptr);
  POINTERS_EQUAL(ptr, mock().get_data("data").get_const_pointer_value());
}

TEST(Support, setDataFunctionPointer)
{
  auto ptr = reinterpret_cast<void (*)()>(0x001);
  mock().set_data("data", ptr);
  FUNCTIONPOINTERS_EQUAL(
      ptr, mock().get_data("data").get_function_pointer_value()
  );
}

TEST(Support, setDataObject)
{
  void* ptr = reinterpret_cast<void*>(0x001);
  mock().set_data_object("data", "type", ptr);
  POINTERS_EQUAL(ptr, mock().get_data("data").get_object_pointer());
  STRCMP_EQUAL("type", mock().get_data("data").get_type().c_str());
}

TEST(Support, setDataConstObject)
{
  void* ptr = reinterpret_cast<void*>(0x011);
  mock().set_data_const_object("data", "type", ptr);
  POINTERS_EQUAL(ptr, mock().get_data("data").get_const_object_pointer());
  STRCMP_EQUAL("type", mock().get_data("data").get_type().c_str());
}

TEST(Support, tracing)
{
  mock().tracing(true);

  mock()
      .actual_call("boo")
      .with_parameter("double", 1.0)
      .with_parameter("int", 1)
      .with_parameter("string", "string");
  mock("scope")
      .actual_call("foo")
      .with_parameter("double", 1.0)
      .with_parameter("int", 1)
      .with_parameter("string", "string");
  mock().check_expectations();

  STRCMP_CONTAINS("boo", mock().get_trace_output());
  STRCMP_CONTAINS("foo", mock().get_trace_output());
}

TEST(Support, tracingWorksHierarchically)
{
  mock("scope").tracing(true);
  mock().tracing(true);

  mock().actual_call("boo");
  mock("scope").actual_call("foo");
  mock().check_expectations();

  STRCMP_CONTAINS("boo", mock().get_trace_output());
  STRCMP_CONTAINS("foo", mock().get_trace_output());
}

TEST(Support, ignoredCallAcceptsAllParameterTypes)
{
  int obj = 0;
  unsigned char buf[] = { 0x01, 0x02 };
  mock().ignore_other_calls();
  mock()
      .actual_call("anything")
      .with_string_parameter("s", "hello")
      .with_pointer_parameter("p", &obj)
      .with_memory_buffer_parameter("buf", buf, sizeof(buf))
      .with_parameter_of_type("MyType", "t", &obj)
      .on_object(&obj);
}
