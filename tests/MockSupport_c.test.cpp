#include "MockSupport.h"

#include "CppMu/CppMu.hpp"
#include "CppMu/MockSupport.h"
#include "CppMu/TestTestingFixture.hpp"

namespace {
void
dummy_function_for_mock_c_test()
{
}

void
dummy_function_for_mock_c_test_two()
{
}

int
type_name_is_equal(const void* object1, const void* object2)
{
  return object1 == object2;
}

const char*
type_name_value_to_string(const void* /*object*/)
{
  return "valueToString";
}

void
type_copy(void* dst, const void* src)
{
  *static_cast<int*>(dst) = *static_cast<const int*>(src);
}

bool destructor_was_called = false;

void
failed_call_to_mock_c()
{
  struct SetBoolOnDestruct
  {
    bool& b;
    ~SetBoolOnDestruct() { b = true; }
  } set_one_destructor{ destructor_was_called };
  mock()->actual_call("Not a call");
}

bool cppmu_has_crashed;

void
crash_method()
{
  cppmu_has_crashed = true;
}

void
failing_call_to_mock_c_with_parameter_of_type()
{
  mock()->expect_one_call("bar")->with_parameter_of_type(
      "typeName", "name", reinterpret_cast<const void*>(1));
  mock()->actual_call("bar")->with_parameter_of_type(
      "typeName", "name", reinterpret_cast<const void*>(2));
}

void
call_to_mock_c_with_output_parameter_of_type()
{
  int value1 = 7;
  const int value2 = 9;
  mock()->expect_one_call("bar")->with_output_parameter_of_type_returning(
      "intType", "bla", &value2);
  mock()->actual_call("bar")->with_output_parameter_of_type(
      "intType", "bla", &value1);
  LONGS_EQUAL(value1, value2);
}

void
failing_call_to_mock_c_with_memory_buffer()
{
  unsigned char mem_buffer1[] = { 0x12, 0x15, 0xFF };
  unsigned char mem_buffer2[] = { 0x12, 0x05, 0xFF };
  mock()->expect_one_call("bar")->with_memory_buffer_parameter(
      "name", mem_buffer1, sizeof(mem_buffer1));
  mock()->actual_call("bar")->with_memory_buffer_parameter(
      "name", mem_buffer2, sizeof(mem_buffer2));
}

} // namespace

TEST_GROUP(MockSupport_c)
{
  void teardown() override { mock()->clear(); }
};

TEST(MockSupport_c, OrderObserved)
{
  mock()->strict_order();

  mock()->expect_one_call("foo1");
  mock()->expect_one_call("foo2");
  mock()->actual_call("foo1");
  mock()->actual_call("foo2");

  mock()->check_expectations();
}

TEST(MockSupport_c, hasReturnValue)
{
  mock()->expect_one_call("foo");
  CHECK(mock()->actual_call("foo")->has_return_value() == 0);
  CHECK(mock()->has_return_value() == 0);

  mock()->expect_one_call("foo2")->and_return_int_value(1);
  CHECK(mock()->actual_call("foo2")->has_return_value() != 0);
  CHECK(mock()->has_return_value() != 0);
}

TEST(MockSupport_c, expectAndActualOneCall)
{
  mock()->expect_one_call("boo");
  mock()->actual_call("boo");
  mock()->check_expectations();
}

TEST(MockSupport_c, expectAndActualThreeCalls)
{
  mock()->expect_n_calls(3, "boo");
  mock()->actual_call("boo");
  mock()->actual_call("boo");
  mock()->actual_call("boo");
  mock()->check_expectations();
}

TEST(MockSupport_c, expectNoCall)
{
  mock()->expect_no_call("foo");
  mock()->expect_one_call("bar");
  mock()->actual_call("bar");
  mock()->check_expectations();
}

TEST(MockSupport_c, expectAndActualParameters)
{
  mock()
      ->expect_one_call("boo")
      ->with_int_parameters("integer", 1)
      ->with_double_parameters("double", 1.0)
      ->with_string_parameters("string", "string")
      ->with_pointer_parameters("pointer", reinterpret_cast<void*>(1))
      ->with_function_pointer_parameters(
          "functionPointer", dummy_function_for_mock_c_test);
  mock()
      ->actual_call("boo")
      ->with_int_parameters("integer", 1)
      ->with_double_parameters("double", 1.0)
      ->with_string_parameters("string", "string")
      ->with_pointer_parameters("pointer", reinterpret_cast<void*>(1))
      ->with_function_pointer_parameters(
          "functionPointer", dummy_function_for_mock_c_test);
}

TEST(MockSupport_c, expectAndActualParametersOnObject)
{
  mock()->install_comparator(
      "typeName", type_name_is_equal, type_name_value_to_string);
  mock()->expect_one_call("boo")->with_parameter_of_type(
      "typeName", "name", reinterpret_cast<const void*>(1));
  mock()->actual_call("boo")->with_parameter_of_type(
      "typeName", "name", reinterpret_cast<const void*>(1));
  mock()->check_expectations();
  mock()->remove_all_comparators_and_copiers();
}

TEST(MockSupport_c, boolParameter)
{
  mock()->expect_one_call("foo")->with_bool_parameters("p", 1);
  mock()->actual_call("foo")->with_bool_parameters("p", 1);
}

TEST(MockSupport_c, unsignedIntParameter)
{
  mock()->expect_one_call("foo")->with_unsigned_int_parameters("p", 1);
  mock()->actual_call("foo")->with_unsigned_int_parameters("p", 1);
}

TEST(MockSupport_c, longIntParameter)
{
  mock()->expect_one_call("foo")->with_long_int_parameters("p", 1);
  mock()->actual_call("foo")->with_long_int_parameters("p", 1);
}

TEST(MockSupport_c, unsignedLongIntParameter)
{
  mock()->expect_one_call("foo")->with_unsigned_long_int_parameters("p", 1);
  mock()->actual_call("foo")->with_unsigned_long_int_parameters("p", 1);
}

TEST(MockSupport_c, doubleParameterWithTolerance)
{
  mock()->expect_one_call("foo")->with_double_parameters_and_tolerance(
      "p", 2.0, 0.2);
  mock()->actual_call("foo")->with_double_parameters("p", 1.9);
}

TEST(MockSupport_c, longLongIntParameter)
{
  mock()->expect_one_call("foo")->with_long_long_int_parameters("p", 1);
  mock()->actual_call("foo")->with_long_long_int_parameters("p", 1);
}

TEST(MockSupport_c, unsignedLongLongIntParameter)
{
  mock()->expect_one_call("foo")->with_unsigned_long_long_int_parameters(
      "p", 1);
  mock()->actual_call("foo")->with_unsigned_long_long_int_parameters("p", 1);
}

TEST(MockSupport_c, memoryBufferParameter)
{
  const unsigned char mem_buffer[] = { 1, 2, 3 };
  mock()->expect_one_call("foo")->with_memory_buffer_parameter(
      "out", mem_buffer, sizeof(mem_buffer));
  mock()->actual_call("foo")->with_memory_buffer_parameter(
      "out", mem_buffer, sizeof(mem_buffer));
  mock()->check_expectations();
}

TEST(MockSupport_c, outputParameters)
{
  int param = 1;
  const int retval = 2;
  mock()->expect_one_call("foo")->with_output_parameter_returning(
      "out", &retval, sizeof(retval));
  mock()->actual_call("foo")->with_output_parameter("out", &param);
  mock()->check_expectations();
  LONGS_EQUAL(2, param);
  LONGS_EQUAL(2, retval);
}

TEST(MockSupport_c, unmodifiedOutputParameter)
{
  int param = 1;
  mock()->expect_one_call("foo")->with_unmodified_output_parameter("out");
  mock()->actual_call("foo")->with_output_parameter("out", &param);
  mock()->check_expectations();
  LONGS_EQUAL(1, param);
}

TEST(MockSupport_c, outputParameters_differentType)
{
  long param = 1;
  const long retval = 2;
  mock()->expect_one_call("foo")->with_output_parameter_returning(
      "out", &retval, sizeof(retval));
  mock()->actual_call("foo")->with_output_parameter("out", &param);
  mock()->check_expectations();
  LONGS_EQUAL(2, param);
  LONGS_EQUAL(2, retval);
}

TEST(MockSupport_c, outputParametersOfType)
{
  int param = 1;
  const int retval = 2;
  mock()->install_copier("typeName", type_copy);
  mock()->expect_one_call("foo")->with_output_parameter_of_type_returning(
      "typeName", "out", &retval);
  mock()->actual_call("foo")->with_output_parameter_of_type(
      "typeName", "out", &param);
  LONGS_EQUAL(2, param);
  LONGS_EQUAL(2, retval);
  mock()->check_expectations();
  mock()->remove_all_comparators_and_copiers();
}

TEST(MockSupport_c, ignoreOtherParameters)
{
  mock()
      ->expect_one_call("foo")
      ->with_int_parameters("int", 1)
      ->ignore_other_parameters();
  mock()
      ->actual_call("foo")
      ->with_int_parameters("int", 1)
      ->with_double_parameters("double", 0.01);
  mock()->check_expectations();
}

TEST(MockSupport_c, returnBoolValue)
{
  int expected_value = 1;
  mock()->expect_one_call("boo")->and_return_bool_value(expected_value);
  CHECK_EQUAL(expected_value, mock()->actual_call("boo")->bool_return_value());
  CHECK_EQUAL(expected_value, mock()->bool_return_value());
  LONGS_EQUAL(mockvaluetype_bool, mock()->return_value().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnBoolValueOrDefaultShouldIgnoreTheDefault)
{
  int default_value = 1;
  int expected_value = 0;
  mock()->expect_one_call("foo")->and_return_bool_value(expected_value);
  LONGS_EQUAL(expected_value,
      mock()->actual_call("foo")->return_bool_value_or_default(default_value));
  LONGS_EQUAL(
      expected_value, mock()->return_bool_value_or_default(default_value));
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnBoolValueOrDefaultShouldlUseTheDefaultValue)
{
  int default_value = 1;
  mock()->expect_one_call("foo");
  LONGS_EQUAL(default_value,
      mock()->actual_call("foo")->return_bool_value_or_default(default_value));
  LONGS_EQUAL(
      default_value, mock()->return_bool_value_or_default(default_value));
}

TEST(MockSupport_c, returnIntValue)
{
  int expected_value = -10;
  mock()->expect_one_call("boo")->and_return_int_value(expected_value);
  LONGS_EQUAL(expected_value, mock()->actual_call("boo")->int_return_value());
  LONGS_EQUAL(expected_value, mock()->int_return_value());
  LONGS_EQUAL(mockvaluetype_integer, mock()->return_value().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnIntValueOrDefaultShouldIgnoreTheDefault)
{
  int default_value = -10;
  int expected_value = default_value - 1;
  mock()->expect_one_call("foo")->and_return_int_value(expected_value);
  LONGS_EQUAL(expected_value,
      mock()->actual_call("foo")->return_int_value_or_default(default_value));
  LONGS_EQUAL(
      expected_value, mock()->return_int_value_or_default(default_value));
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnIntValueOrDefaultShouldlUseTheDefaultValue)
{
  int default_value = -10;
  mock()->expect_one_call("foo");
  LONGS_EQUAL(default_value,
      mock()->actual_call("foo")->return_int_value_or_default(default_value));
  LONGS_EQUAL(
      default_value, mock()->return_int_value_or_default(default_value));
}

TEST(MockSupport_c, returnUnsignedIntValue)
{
  unsigned int expected_value = 7;
  mock()->expect_one_call("boo")->and_return_unsigned_int_value(expected_value);
  LONGS_EQUAL(
      expected_value, mock()->actual_call("boo")->unsigned_int_return_value());
  LONGS_EQUAL(expected_value, mock()->unsigned_int_return_value());
  LONGS_EQUAL(mockvaluetype_unsigned_integer, mock()->return_value().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnUnsignedIntValueOrDefaultShouldIgnoreTheDefault)
{
  unsigned int default_value = 10;
  unsigned int expected_value = default_value + 1;
  mock()->expect_one_call("foo")->and_return_unsigned_int_value(expected_value);
  LONGS_EQUAL(expected_value,
      mock()->actual_call("foo")->return_unsigned_int_value_or_default(
          default_value));
  LONGS_EQUAL(expected_value,
      mock()->return_unsigned_int_value_or_default(default_value));
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnUnsignedIntValueOrDefaultShouldlUseTheDefaultValue)
{
  unsigned int default_value = 10;
  mock()->expect_one_call("foo");
  LONGS_EQUAL(default_value,
      mock()->actual_call("foo")->return_unsigned_int_value_or_default(
          default_value));
  LONGS_EQUAL(default_value,
      mock()->return_unsigned_int_value_or_default(default_value));
}

TEST(MockSupport_c, returnLongIntValue)
{
  long int expected_value = -10L;
  mock()->expect_one_call("boo")->and_return_long_int_value(expected_value);
  LONGS_EQUAL(
      expected_value, mock()->actual_call("boo")->long_int_return_value());
  LONGS_EQUAL(expected_value, mock()->long_int_return_value());
  LONGS_EQUAL(mockvaluetype_long_integer, mock()->return_value().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnLongIntValueOrDefaultShouldIgnoreTheDefault)
{
  long int default_value = -10L;
  long int expected_value = default_value - 1L;
  mock()->expect_one_call("foo")->and_return_long_int_value(expected_value);
  LONGS_EQUAL(expected_value,
      mock()->actual_call("foo")->return_long_int_value_or_default(
          default_value));
  LONGS_EQUAL(
      expected_value, mock()->return_long_int_value_or_default(default_value));
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnLongIntValueOrDefaultShouldlUseTheDefaultValue)
{
  long int default_value = -10L;
  mock()->expect_one_call("foo");
  LONGS_EQUAL(default_value,
      mock()->actual_call("foo")->return_long_int_value_or_default(
          default_value));
  LONGS_EQUAL(
      default_value, mock()->return_long_int_value_or_default(default_value));
}

TEST(MockSupport_c, returnUnsignedLongIntValue)
{
  unsigned long int expected_value = 10;
  mock()->expect_one_call("boo")->and_return_unsigned_long_int_value(
      expected_value);
  LONGS_EQUAL(expected_value,
      mock()->actual_call("boo")->unsigned_long_int_return_value());
  LONGS_EQUAL(expected_value, mock()->unsigned_long_int_return_value());
  LONGS_EQUAL(mockvaluetype_unsigned_long_integer, mock()->return_value().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnUnsignedLongIntValueOrDefaultShouldIgnoreTheDefault)
{
  unsigned long int default_value = 10L;
  unsigned long int expected_value = default_value + 1L;
  mock()->expect_one_call("foo")->and_return_unsigned_long_int_value(
      expected_value);
  LONGS_EQUAL(expected_value,
      mock()->actual_call("foo")->return_unsigned_long_int_value_or_default(
          default_value));
  LONGS_EQUAL(expected_value,
      mock()->return_unsigned_long_int_value_or_default(default_value));
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnUnsignedLongIntValueOrDefaultShouldlUseTheDefaultValue)
{
  unsigned long int default_value = 10L;
  mock()->expect_one_call("foo");
  LONGS_EQUAL(default_value,
      mock()->actual_call("foo")->return_unsigned_long_int_value_or_default(
          default_value));
  LONGS_EQUAL(default_value,
      mock()->return_unsigned_long_int_value_or_default(default_value));
}

TEST(MockSupport_c, returnLongLongIntValue)
{
  long long int expected_value = -10L;
  mock()->expect_one_call("boo")->and_return_long_long_int_value(
      expected_value);
  LONGLONGS_EQUAL(
      expected_value, mock()->actual_call("boo")->long_long_int_return_value());
  LONGLONGS_EQUAL(expected_value, mock()->long_long_int_return_value());
  LONGLONGS_EQUAL(mockvaluetype_long_long_integer, mock()->return_value().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnLongLongIntValueOrDefaultShouldIgnoreTheDefault)
{
  long long int default_value = -10L;
  long long int expected_value = default_value - 1L;
  mock()->expect_one_call("foo")->and_return_long_long_int_value(
      expected_value);
  LONGLONGS_EQUAL(expected_value,
      mock()->actual_call("foo")->return_long_long_int_value_or_default(
          default_value));
  LONGLONGS_EQUAL(expected_value,
      mock()->return_long_long_int_value_or_default(default_value));
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnLongLongIntValueOrDefaultShouldlUseTheDefaultValue)
{
  long long int default_value = -10L;
  mock()->expect_one_call("foo");
  LONGLONGS_EQUAL(default_value,
      mock()->actual_call("foo")->return_long_long_int_value_or_default(
          default_value));
  LONGLONGS_EQUAL(default_value,
      mock()->return_long_long_int_value_or_default(default_value));
}

TEST(MockSupport_c, returnUnsignedLongLongIntValue)
{
  unsigned long long int expected_value = 10;
  mock()->expect_one_call("boo")->and_return_unsigned_long_long_int_value(
      expected_value);
  UNSIGNED_LONGLONGS_EQUAL(expected_value,
      mock()->actual_call("boo")->unsigned_long_long_int_return_value());
  UNSIGNED_LONGLONGS_EQUAL(
      expected_value, mock()->unsigned_long_long_int_return_value());
  UNSIGNED_LONGLONGS_EQUAL(
      mockvaluetype_unsigned_long_long_integer, mock()->return_value().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnUnsignedLongLongIntValueOrDefaultShouldIgnoreTheDefault)
{
  unsigned long long int default_value = 10L;
  unsigned long long int expected_value = default_value + 1L;
  mock()->expect_one_call("foo")->and_return_unsigned_long_long_int_value(
      expected_value);
  UNSIGNED_LONGLONGS_EQUAL(expected_value,
      mock()
          ->actual_call("foo")
          ->return_unsigned_long_long_int_value_or_default(default_value));
  UNSIGNED_LONGLONGS_EQUAL(expected_value,
      mock()->return_unsigned_long_long_int_value_or_default(default_value));
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnUnsignedLongLongIntValueOrDefaultShouldlUseTheDefaultValue)
{
  unsigned long long int default_value = 10L;
  mock()->expect_one_call("foo");
  UNSIGNED_LONGLONGS_EQUAL(default_value,
      mock()
          ->actual_call("foo")
          ->return_unsigned_long_long_int_value_or_default(default_value));
  UNSIGNED_LONGLONGS_EQUAL(default_value,
      mock()->return_unsigned_long_long_int_value_or_default(default_value));
}

TEST(MockSupport_c, returnStringValue)
{
  mock()->expect_one_call("boo")->and_return_string_value("hello world");
  STRCMP_EQUAL(
      "hello world", mock()->actual_call("boo")->string_return_value());
  STRCMP_EQUAL("hello world", mock()->string_return_value());
  LONGS_EQUAL(mockvaluetype_string, mock()->return_value().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnStringValueOrDefaultShouldIgnoreTheDefault)
{
  const char default_value[] = "bar";
  const char expected_value[] = "bla";
  mock()->expect_one_call("foo")->and_return_string_value(expected_value);
  STRCMP_EQUAL(expected_value,
      mock()->actual_call("foo")->return_string_value_or_default(
          default_value));
  STRCMP_EQUAL(
      expected_value, mock()->return_string_value_or_default(default_value));
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnStringValueOrDefaultShouldlUseTheDefaultValue)
{
  const char default_value[] = "bar";
  mock()->expect_one_call("foo");
  STRCMP_EQUAL(default_value,
      mock()->actual_call("foo")->return_string_value_or_default(
          default_value));
  STRCMP_EQUAL(
      default_value, mock()->return_string_value_or_default(default_value));
}

TEST(MockSupport_c, returnDoubleValue)
{
  mock()->expect_one_call("boo")->and_return_double_value(1.0);
  DOUBLES_EQUAL(1.0, mock()->actual_call("boo")->double_return_value(), 0.005);
  DOUBLES_EQUAL(1.0, mock()->double_return_value(), 0.005);
  LONGS_EQUAL(mockvaluetype_double, mock()->return_value().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnDoubleValueOrDefaultShouldIgnoreTheDefault)
{
  double default_value = 2.2;
  double expected_value = default_value + 0.1;
  mock()->expect_one_call("foo")->and_return_double_value(expected_value);
  DOUBLES_EQUAL(expected_value,
      mock()->actual_call("foo")->return_double_value_or_default(default_value),
      0.005);
  DOUBLES_EQUAL(expected_value,
      mock()->return_double_value_or_default(default_value),
      0.005);
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnDoubleValueOrDefaultShouldlUseTheDefaultValue)
{
  double default_value = 2.2;
  mock()->expect_one_call("foo");
  DOUBLES_EQUAL(default_value,
      mock()->actual_call("foo")->return_double_value_or_default(default_value),
      0.005);
  DOUBLES_EQUAL(default_value,
      mock()->return_double_value_or_default(default_value),
      0.005);
}

TEST(MockSupport_c, returnPointerValue)
{
  mock()->expect_one_call("boo")->and_return_pointer_value(
      reinterpret_cast<void*>(10));
  POINTERS_EQUAL(reinterpret_cast<void*>(10),
      mock()->actual_call("boo")->pointer_return_value());
  POINTERS_EQUAL(reinterpret_cast<void*>(10), mock()->pointer_return_value());
  LONGS_EQUAL(mockvaluetype_pointer, mock()->return_value().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnPointerValueOrDefaultShouldIgnoreTheDefault)
{
  void* default_value = reinterpret_cast<void*>(10);
  void* expected_value = reinterpret_cast<void*>(27);
  mock()->expect_one_call("foo")->and_return_pointer_value(expected_value);
  POINTERS_EQUAL(expected_value,
      mock()->actual_call("foo")->return_pointer_value_or_default(
          default_value));
  POINTERS_EQUAL(
      expected_value, mock()->return_pointer_value_or_default(default_value));
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnPointerValueOrDefaultShouldlUseTheDefaultValue)
{
  void* default_value = reinterpret_cast<void*>(10);
  mock()->expect_one_call("foo");
  POINTERS_EQUAL(default_value,
      mock()->actual_call("foo")->return_pointer_value_or_default(
          default_value));
  POINTERS_EQUAL(
      default_value, mock()->return_pointer_value_or_default(default_value));
}

TEST(MockSupport_c, returnConstPointerValue)
{
  mock()->expect_one_call("boo")->and_return_const_pointer_value(
      reinterpret_cast<const void*>(10));
  POINTERS_EQUAL(reinterpret_cast<const void*>(10),
      mock()->actual_call("boo")->const_pointer_return_value());
  POINTERS_EQUAL(
      reinterpret_cast<const void*>(10), mock()->const_pointer_return_value());
  LONGS_EQUAL(mockvaluetype_const_pointer, mock()->return_value().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnConstPointerValueOrDefaultShouldIgnoreTheDefault)
{
  const void* default_value = reinterpret_cast<void*>(10);
  const void* expected_value = reinterpret_cast<void*>(27);
  mock()->expect_one_call("foo")->and_return_const_pointer_value(
      expected_value);
  POINTERS_EQUAL(expected_value,
      mock()->actual_call("foo")->return_const_pointer_value_or_default(
          default_value));
  POINTERS_EQUAL(expected_value,
      mock()->return_const_pointer_value_or_default(default_value));
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnConstPointerValueOrDefaultShouldlUseTheDefaultValue)
{
  const void* default_value = reinterpret_cast<void*>(10);
  mock()->expect_one_call("foo");
  POINTERS_EQUAL(default_value,
      mock()->actual_call("foo")->return_const_pointer_value_or_default(
          default_value));
  POINTERS_EQUAL(default_value,
      mock()->return_const_pointer_value_or_default(default_value));
}

TEST(MockSupport_c, returnFunctionPointerValue)
{
  mock()->expect_one_call("boo")->and_return_function_pointer_value(
      dummy_function_for_mock_c_test);
  FUNCTIONPOINTERS_EQUAL(dummy_function_for_mock_c_test,
      mock()->actual_call("boo")->function_pointer_return_value());
  FUNCTIONPOINTERS_EQUAL(
      dummy_function_for_mock_c_test, mock()->function_pointer_return_value());
  LONGS_EQUAL(mockvaluetype_functionpointer, mock()->return_value().type);
}

TEST(MockSupport_c,
    whenReturnValueIsGivenReturnFunctionPointerValueOrDefaultShouldIgnoreTheDefault)
{
  mock()->expect_one_call("foo")->and_return_function_pointer_value(
      dummy_function_for_mock_c_test);
  FUNCTIONPOINTERS_EQUAL(dummy_function_for_mock_c_test,
      mock()->actual_call("foo")->return_function_pointer_value_or_default(
          dummy_function_for_mock_c_test_two));
  FUNCTIONPOINTERS_EQUAL(dummy_function_for_mock_c_test,
      mock()->return_function_pointer_value_or_default(
          dummy_function_for_mock_c_test_two));
}

TEST(MockSupport_c,
    whenNoReturnValueIsGivenReturnFunctionPointerValueOrDefaultShouldlUseTheDefaultValue)
{
  mock()->expect_one_call("foo");
  FUNCTIONPOINTERS_EQUAL(dummy_function_for_mock_c_test_two,
      mock()->actual_call("foo")->return_function_pointer_value_or_default(
          dummy_function_for_mock_c_test_two));
  FUNCTIONPOINTERS_EQUAL(dummy_function_for_mock_c_test_two,
      mock()->return_function_pointer_value_or_default(
          dummy_function_for_mock_c_test_two));
}

TEST(MockSupport_c, MockSupportWithScope)
{
  mock_scope("scope")->expect_one_call("boo");
  LONGS_EQUAL(0, mock_scope("other")->expected_calls_left());
  LONGS_EQUAL(1, mock_scope("scope")->expected_calls_left());
  mock_scope("scope")->actual_call("boo");
}

TEST(MockSupport_c, MockSupportSetBoolData)
{
  mock()->set_bool_data("boolean", 1);
  CHECK_EQUAL(1, mock()->get_data("boolean").value.bool_value);
}

TEST(MockSupport_c, MockSupportSetIntData)
{
  mock()->set_int_data("integer", 10);
  LONGS_EQUAL(10, mock()->get_data("integer").value.int_value);
}

TEST(MockSupport_c, MockSupportSetLongIntData)
{
  long int i = 100;
  mock()->set_long_int_data("long integer", i);
  LONGS_EQUAL(i, mock()->get_data("long integer").value.long_int_value);
}

TEST(MockSupport_c, MockSupportSetUnsignedLongIntData)
{
  unsigned long int i = 100;
  mock()->set_unsigned_long_int_data("unsigned long integer", i);
  UNSIGNED_LONGS_EQUAL(i,
      mock()->get_data("unsigned long integer").value.unsigned_long_int_value);
}

TEST(MockSupport_c, MockSupportSetDoubleData)
{
  mock()->set_double_data("double", 1.0);
  DOUBLES_EQUAL(1.00, mock()->get_data("double").value.double_value, 0.05);
}

TEST(MockSupport_c, MockSupportSetStringData)
{
  mock()->set_string_data("string", "hello world");
  STRCMP_EQUAL("hello world", mock()->get_data("string").value.string_value);
}

TEST(MockSupport_c, MockSupportSetPointerData)
{
  mock()->set_pointer_data("pointer", reinterpret_cast<void*>(1));
  POINTERS_EQUAL(reinterpret_cast<void*>(1),
      mock()->get_data("pointer").value.pointer_value);
}

TEST(MockSupport_c, MockSupportSetConstPointerData)
{
  mock()->set_const_pointer_data(
      "constPointer", reinterpret_cast<const void*>(1));
  POINTERS_EQUAL(reinterpret_cast<const void*>(1),
      mock()->get_data("constPointer").value.const_pointer_value);
}

TEST(MockSupport_c, MockSupportMemoryBufferData)
{
  mock()->set_data_object(
      "name", "const unsigned char*", reinterpret_cast<void*>(0xDEAD));
  POINTERS_EQUAL(reinterpret_cast<void*>(0xDEAD),
      mock()->get_data("name").value.memory_buffer_value);
  LONGS_EQUAL(mockvaluetype_memorybuffer, mock()->get_data("name").type);
}

TEST(MockSupport_c, MockSupportSetFunctionPointerData)
{
  mock()->set_function_pointer_data(
      "functionPointer", dummy_function_for_mock_c_test);
  FUNCTIONPOINTERS_EQUAL(dummy_function_for_mock_c_test,
      mock()->get_data("functionPointer").value.function_pointer_value);
}

TEST(MockSupport_c, MockSupportSetDataObject)
{
  mock()->set_data_object("name", "type", reinterpret_cast<void*>(1));
  POINTERS_EQUAL(
      reinterpret_cast<void*>(1), mock()->get_data("name").value.object_value);
  LONGS_EQUAL(mockvaluetype_object, mock()->get_data("name").type);
}

TEST(MockSupport_c, MockSupportSetDataConstObject)
{
  mock()->set_data_const_object(
      "name", "type", reinterpret_cast<const void*>(5));
  POINTERS_EQUAL(reinterpret_cast<void*>(5),
      mock()->get_data("name").value.const_object_value);
  LONGS_EQUAL(mockvaluetype_const_object, mock()->get_data("name").type);
}

TEST(MockSupport_c, WorksInCFile)
{
  all_mock_support_c_calls();
}

// Silly wrapper because of a test that only fails in Visual C++ due to
// different destructor behaviors
#ifdef _MSC_VER
#define MSC_SWITCHED_TEST(testGroup, testName) IGNORE_TEST(testGroup, testName)
#else
#define MSC_SWITCHED_TEST(testGroup, testName) TEST(testGroup, testName)
#endif

MSC_SWITCHED_TEST(MockSupport_c, NoExceptionsAreThrownWhenAMock_cCallFailed)
{
  cppmu::TestTestingFixture fixture;

  fixture.set_test_function(failed_call_to_mock_c);
  fixture.run_all_tests();

  LONGS_EQUAL(1, fixture.get_failure_count());
  // Odd behavior in Visual C++, destructor still gets called here
  CHECK(!destructor_was_called);
}

TEST(MockSupport_c, shouldCrashOnFailure)
{
  cppmu_has_crashed = false;
  cppmu::TestTestingFixture fixture;
  cppmu::TestShell::set_crash_method(crash_method);
  mock()->crash_on_failure(true);
  fixture.set_test_function(failed_call_to_mock_c);

  fixture.run_all_tests();

  CHECK(cppmu_has_crashed);

  cppmu::TestShell::reset_crash_method();
  mock()->crash_on_failure(false);
}

TEST(MockSupport_c, nextTestShouldNotCrashOnFailure)
{
  mock()->crash_on_failure(false);
  cppmu_has_crashed = false;
  cppmu::TestTestingFixture fixture;
  cppmu::TestShell::set_crash_method(crash_method);
  fixture.set_test_function(failed_call_to_mock_c);

  fixture.run_all_tests();

  CHECK_FALSE(cppmu_has_crashed);

  cppmu::TestShell::reset_crash_method();
}

TEST(MockSupport_c, FailWillNotCrashIfNotEnabled)
{
  cppmu_has_crashed = false;
  cppmu::TestTestingFixture fixture;
  cppmu::TestShell::set_crash_method(crash_method);

  fixture.set_test_function(failed_call_to_mock_c);

  fixture.run_all_tests();

  CHECK_FALSE(cppmu_has_crashed);
  LONGS_EQUAL(1, fixture.get_failure_count());

  cppmu::TestShell::reset_crash_method();
}

TEST(MockSupport_c, FailWillCrashIfEnabled)
{
  cppmu_has_crashed = false;
  cppmu::TestTestingFixture fixture;
  cppmu::TestShell::set_crash_on_fail();
  cppmu::TestShell::set_crash_method(crash_method);

  fixture.set_test_function(failed_call_to_mock_c);

  fixture.run_all_tests();

  CHECK(cppmu_has_crashed);
  LONGS_EQUAL(1, fixture.get_failure_count());

  cppmu::TestShell::restore_default_test_terminator();
  cppmu::TestShell::reset_crash_method();
}

TEST(MockSupport_c, failureWithParameterOfTypeCoversValueToString)
{
  cppmu::TestTestingFixture fixture;
  mock()->install_comparator(
      "typeName", type_name_is_equal, type_name_value_to_string);
  fixture.set_test_function(failing_call_to_mock_c_with_parameter_of_type);
  fixture.run_all_tests();
  fixture.assert_print_contains("typeName name: <valueToString>");
  mock()->remove_all_comparators_and_copiers();
}

TEST(MockSupport_c, successWithOutputParameterOfType)
{
  cppmu::TestTestingFixture fixture;
  mock()->install_copier("intType", type_copy);
  fixture.set_test_function(call_to_mock_c_with_output_parameter_of_type);
  fixture.run_all_tests();
  LONGS_EQUAL(2, fixture.get_check_count());
  LONGS_EQUAL(0, fixture.get_failure_count());
  mock()->remove_all_comparators_and_copiers();
}

TEST(MockSupport_c, expectOneMemBufferParameterAndValueFailsDueToContents)
{
  cppmu::TestTestingFixture fixture;
  fixture.set_test_function(failing_call_to_mock_c_with_memory_buffer);
  fixture.run_all_tests();
  fixture.assert_print_contains(
      "Unexpected parameter value to parameter \"name\" "
      "to function \"bar\": <Size = 3 | HexContents = 12 05 FF>");
}

TEST(MockSupport_c, ignoreOtherCalls)
{
  mock()->expect_one_call("foo");
  mock()->ignore_other_calls();
  mock()->actual_call("foo");
  mock()->actual_call("bar");
  mock()->check_expectations();
}
