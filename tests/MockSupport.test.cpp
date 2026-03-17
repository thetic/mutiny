#include "CppMu/MockSupport.hpp"

#include "MockFailureReporterForTest.hpp"

#include "CppMu/CppMu.hpp"
#include "CppMu/MockExpectedCall.hpp"
#include "CppMu/MockFailure.hpp"
#include "CppMu/TestTestingFixture.hpp"

using cppmu::mock;

TEST_GROUP(MockSupport)
{
  MockExpectedCallsListForTest expectations;
  MockFailureReporterInstaller failure_reporter_installer;

  void teardown() override
  {
    mock().check_expectations();
    CHECK_NO_MOCK_FAILURE();
    MockFailureReporterForTest::clear_reporter();
    mock().clear();
  }
};

TEST(MockSupport, setDataForUnsignedIntegerValues)
{
  unsigned int expected_data = 7;
  mock().set_data("data", expected_data);
  LONGS_EQUAL(expected_data, mock().get_data("data").get_unsigned_int_value());
}

TEST(MockSupport, setDataForIntegerValues)
{
  int expected_data = 10;
  mock().set_data("data", expected_data);
  LONGS_EQUAL(expected_data, mock().get_data("data").get_int_value());
}

TEST(MockSupport, setDataForBooleanValues)
{
  bool expected_data = true;
  mock().set_data("data", expected_data);
  CHECK_EQUAL(expected_data, mock().get_data("data").get_bool_value());
}

TEST(MockSupport, hasDataBeenSet)
{
  CHECK(!mock().has_data("data"));
  mock().set_data("data", 10);
  CHECK(mock().has_data("data"));
}

TEST(MockSupport, dataCanBeChanged)
{
  mock().set_data("data", 10);
  mock().set_data("data", 15);
  LONGS_EQUAL(15, mock().get_data("data").get_int_value());
}

TEST(MockSupport, uninitializedData)
{
  LONGS_EQUAL(0, mock().get_data("nonexisting").get_int_value());
  STRCMP_EQUAL("int", mock().get_data("nonexisting").get_type().c_str());
}

TEST(MockSupport, setMultipleData)
{
  mock().set_data("data", 1);
  mock().set_data("data2", 10);
  LONGS_EQUAL(1, mock().get_data("data").get_int_value());
  LONGS_EQUAL(10, mock().get_data("data2").get_int_value());
}

TEST(MockSupport, setDataString)
{
  mock().set_data("data", "string");
  STRCMP_EQUAL("string", mock().get_data("data").get_string_value());
}

TEST(MockSupport, setDataDouble)
{
  mock().set_data("data", 1.0);
  DOUBLES_EQUAL(1.0, mock().get_data("data").get_double_value(), 0.05);
}

TEST(MockSupport, setDataLongInt)
{
  long int i = 100;
  mock().set_data("data", i);
  LONGS_EQUAL(i, mock().get_data("data").get_long_int_value());
}

TEST(MockSupport, setDataUnsignedLongInt)
{
  unsigned long int i = 100;
  mock().set_data("data", i);
  UNSIGNED_LONGS_EQUAL(
      i, mock().get_data("data").get_unsigned_long_int_value()
  );
}

TEST(MockSupport, setDataPointer)
{
  void* ptr = reinterpret_cast<void*>(0x001);
  mock().set_data("data", ptr);
  POINTERS_EQUAL(ptr, mock().get_data("data").get_pointer_value());
}

TEST(MockSupport, setConstDataPointer)
{
  const void* ptr = reinterpret_cast<const void*>(0x001);
  mock().set_data("data", ptr);
  POINTERS_EQUAL(ptr, mock().get_data("data").get_const_pointer_value());
}

TEST(MockSupport, setDataFunctionPointer)
{
  auto ptr = reinterpret_cast<void (*)()>(0x001);
  mock().set_data("data", ptr);
  FUNCTIONPOINTERS_EQUAL(
      ptr, mock().get_data("data").get_function_pointer_value()
  );
}

TEST(MockSupport, setDataObject)
{
  void* ptr = reinterpret_cast<void*>(0x001);
  mock().set_data_object("data", "type", ptr);
  POINTERS_EQUAL(ptr, mock().get_data("data").get_object_pointer());
  STRCMP_EQUAL("type", mock().get_data("data").get_type().c_str());
}

TEST(MockSupport, setDataConstObject)
{
  void* ptr = reinterpret_cast<void*>(0x011);
  mock().set_data_const_object("data", "type", ptr);
  POINTERS_EQUAL(ptr, mock().get_data("data").get_const_object_pointer());
  STRCMP_EQUAL("type", mock().get_data("data").get_type().c_str());
}

TEST(MockSupport, tracing)
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

TEST(MockSupport, tracingWorksHierarchically)
{
  mock("scope").tracing(true);
  mock().tracing(true);

  mock().actual_call("boo");
  mock("scope").actual_call("foo");
  mock().check_expectations();

  STRCMP_CONTAINS("boo", mock().get_trace_output());
  STRCMP_CONTAINS("foo", mock().get_trace_output());
}
