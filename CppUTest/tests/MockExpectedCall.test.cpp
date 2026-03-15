#include "MockFailureReporterForTest.hpp"

#include "CppUTest/CppUTest.hpp"
#include "CppUTest/MockCheckedExpectedCall.hpp"
#include "CppUTest/MockFailure.hpp"
#include "CppUTest/MockIgnoredExpectedCall.hpp"
#include "CppUTest/MockNamedValueComparatorsAndCopiersRepository.hpp"

class TypeForTestingExpectedFunctionCall
{
public:
  TypeForTestingExpectedFunctionCall(int val) { value = new int(val); }
  virtual ~TypeForTestingExpectedFunctionCall() { delete value; }
  int* value;
};

class TypeForTestingExpectedFunctionCallComparator
  : public cpputest::MockNamedValueComparator
{
public:
  virtual bool is_equal(const void* object1, const void* object2) override
  {
    auto* obj1 =
        static_cast<const TypeForTestingExpectedFunctionCall*>(object1);
    auto* obj2 =
        static_cast<const TypeForTestingExpectedFunctionCall*>(object2);
    return *(obj1->value) == *(obj2->value);
  }
  virtual cpputest::String value_to_string(const void* object) override
  {
    auto* obj = static_cast<const TypeForTestingExpectedFunctionCall*>(object);
    return cpputest::string_from(*(obj->value));
  }
};

class TypeForTestingExpectedFunctionCallCopier
  : public cpputest::MockNamedValueCopier
{
public:
  virtual void copy(void* dst, const void* src) override
  {
    auto* typed_dst = static_cast<TypeForTestingExpectedFunctionCall*>(dst);
    auto* typed_src =
        static_cast<const TypeForTestingExpectedFunctionCall*>(src);
    *(typed_dst->value) = *(typed_src->value);
  }
};

TEST_GROUP(MockNamedValueHandlerRepository)
{
  void teardown() override
  {
    CHECK_NO_MOCK_FAILURE();
    MockFailureReporterForTest::clear_reporter();
  }
};

TEST(MockNamedValueHandlerRepository, getComparatorForNonExistingName)
{
  cpputest::MockNamedValueComparatorsAndCopiersRepository repository;
  POINTERS_EQUAL(nullptr, repository.get_comparator_for_type("typeName"));
}

TEST(MockNamedValueHandlerRepository, installComparator)
{
  TypeForTestingExpectedFunctionCallComparator comparator;
  cpputest::MockNamedValueComparatorsAndCopiersRepository repository;
  repository.install_comparator("typeName", comparator);
  POINTERS_EQUAL(&comparator, repository.get_comparator_for_type("typeName"));
}

TEST(MockNamedValueHandlerRepository, installMultipleComparators)
{
  TypeForTestingExpectedFunctionCallComparator comparator1, comparator2,
      comparator3;
  cpputest::MockNamedValueComparatorsAndCopiersRepository repository;
  repository.install_comparator("type1", comparator1);
  repository.install_comparator("type2", comparator2);
  repository.install_comparator("type3", comparator3);
  POINTERS_EQUAL(&comparator3, repository.get_comparator_for_type("type3"));
  POINTERS_EQUAL(&comparator2, repository.get_comparator_for_type("type2"));
  POINTERS_EQUAL(&comparator1, repository.get_comparator_for_type("type1"));
}

TEST(MockNamedValueHandlerRepository, getCopierForNonExistingName)
{
  cpputest::MockNamedValueComparatorsAndCopiersRepository repository;
  POINTERS_EQUAL(nullptr, repository.get_copier_for_type("typeName"));
}

TEST(MockNamedValueHandlerRepository, installCopier)
{
  TypeForTestingExpectedFunctionCallCopier copier;
  cpputest::MockNamedValueComparatorsAndCopiersRepository repository;
  repository.install_copier("typeName", copier);
  POINTERS_EQUAL(&copier, repository.get_copier_for_type("typeName"));
}

TEST(MockNamedValueHandlerRepository, installMultipleCopiers)
{
  TypeForTestingExpectedFunctionCallCopier copier1, copier2, copier3;
  cpputest::MockNamedValueComparatorsAndCopiersRepository repository;
  repository.install_copier("type1", copier1);
  repository.install_copier("type2", copier2);
  repository.install_copier("type3", copier3);
  POINTERS_EQUAL(&copier3, repository.get_copier_for_type("type3"));
  POINTERS_EQUAL(&copier2, repository.get_copier_for_type("type2"));
  POINTERS_EQUAL(&copier1, repository.get_copier_for_type("type1"));
}

TEST(MockNamedValueHandlerRepository, installMultipleHandlers)
{
  TypeForTestingExpectedFunctionCallCopier copier1, copier2, copier3;
  TypeForTestingExpectedFunctionCallComparator comparator1, comparator2,
      comparator3;
  cpputest::MockNamedValueComparatorsAndCopiersRepository repository;
  repository.install_copier("type1", copier1);
  repository.install_comparator("type1", comparator1);
  repository.install_copier("type2", copier2);
  repository.install_copier("type3", copier3);
  repository.install_comparator("type2", comparator2);
  repository.install_comparator("type3", comparator3);
  POINTERS_EQUAL(&comparator3, repository.get_comparator_for_type("type3"));
  POINTERS_EQUAL(&comparator2, repository.get_comparator_for_type("type2"));
  POINTERS_EQUAL(&comparator1, repository.get_comparator_for_type("type1"));
  POINTERS_EQUAL(&copier3, repository.get_copier_for_type("type3"));
  POINTERS_EQUAL(&copier2, repository.get_copier_for_type("type2"));
  POINTERS_EQUAL(&copier1, repository.get_copier_for_type("type1"));
}

TEST_GROUP(MockExpectedCall)
{
  cpputest::MockCheckedExpectedCall* call;
  cpputest::MockNamedValueComparatorsAndCopiersRepository*
      original_comparator_repository;
  void setup() override
  {
    original_comparator_repository = cpputest::MockNamedValue::
        get_default_comparators_and_copiers_repository();
    call = new cpputest::MockCheckedExpectedCall(1);
    call->with_name("funcName");
  }
  void teardown() override
  {
    cpputest::MockNamedValue::set_default_comparators_and_copiers_repository(
        original_comparator_repository);
    delete call;
    CHECK_NO_MOCK_FAILURE();
    MockFailureReporterForTest::clear_reporter();
  }
};

TEST(MockExpectedCall, callWithoutParameterSetOrNotFound)
{
  STRCMP_EQUAL("", call->get_input_parameter_type("nonexisting").c_str());
  LONGS_EQUAL(0, call->get_input_parameter("nonexisting").get_int_value());
  CHECK(!call->has_input_parameter_with_name("nonexisting"));
}

TEST(MockExpectedCall, callWithUnsignedIntegerParameter)
{
  const cpputest::String param_name = "paramName";
  unsigned int value = 356;
  call->with_parameter(param_name, value);
  STRCMP_EQUAL(
      "unsigned int", call->get_input_parameter_type(param_name).c_str());
  LONGS_EQUAL(
      value, call->get_input_parameter(param_name).get_unsigned_int_value());
  CHECK(call->has_input_parameter_with_name(param_name));
  STRCMP_CONTAINS("funcName -> unsigned int paramName: <356 (0x164)>",
      call->call_to_string().c_str());
}

TEST(MockExpectedCall, callWithIntegerParameter)
{
  const cpputest::String param_name = "paramName";
  int value = 2;
  call->with_parameter(param_name, value);
  STRCMP_EQUAL("int", call->get_input_parameter_type(param_name).c_str());
  LONGS_EQUAL(value, call->get_input_parameter(param_name).get_int_value());
  CHECK(call->has_input_parameter_with_name(param_name));
  STRCMP_CONTAINS(
      "funcName -> int paramName: <2 (0x2)>", call->call_to_string().c_str());
}

TEST(MockExpectedCall, callWithBooleanParameter)
{
  const cpputest::String param_name = "paramName";
  bool value = true;
  call->with_parameter(param_name, value);
  STRCMP_EQUAL("bool", call->get_input_parameter_type(param_name).c_str());
  CHECK_EQUAL(value, call->get_input_parameter(param_name).get_bool_value());
  CHECK(call->has_input_parameter_with_name(param_name));
  STRCMP_CONTAINS(
      "funcName -> bool paramName: <true>", call->call_to_string().c_str());
}

TEST(MockExpectedCall, callWithUnsignedLongIntegerParameter)
{
  const cpputest::String param_name = "paramName";
  unsigned long value = 888;
  call->with_parameter(param_name, value);
  STRCMP_EQUAL(
      "unsigned long int", call->get_input_parameter_type(param_name).c_str());
  LONGS_EQUAL(value,
      call->get_input_parameter(param_name).get_unsigned_long_int_value());
  CHECK(call->has_input_parameter_with_name(param_name));
  STRCMP_CONTAINS("funcName -> unsigned long int paramName: <888 (0x378)>",
      call->call_to_string().c_str());
}

TEST(MockExpectedCall, callWithLongIntegerParameter)
{
  const cpputest::String param_name = "paramName";
  long value = 777;
  call->with_parameter(param_name, value);
  STRCMP_EQUAL("long int", call->get_input_parameter_type(param_name).c_str());
  LONGS_EQUAL(
      value, call->get_input_parameter(param_name).get_long_int_value());
  CHECK(call->has_input_parameter_with_name(param_name));
  STRCMP_CONTAINS("funcName -> long int paramName: <777 (0x309)>",
      call->call_to_string().c_str());
}

TEST(MockExpectedCall, callWithUnsignedLongLongIntegerParameter)
{
  const cpputest::String param_name = "paramName";
  unsigned long long value = 888;
  call->with_parameter(param_name, value);
  STRCMP_EQUAL("unsigned long long int",
      call->get_input_parameter_type(param_name).c_str());
  UNSIGNED_LONGLONGS_EQUAL(value,
      call->get_input_parameter(param_name).get_unsigned_long_long_int_value());
  CHECK(call->has_input_parameter_with_name(param_name));
  STRCMP_CONTAINS("funcName -> unsigned long long int paramName: <888 (0x378)>",
      call->call_to_string().c_str());
}

TEST(MockExpectedCall, callWithLongLongIntegerParameter)
{
  const cpputest::String param_name = "paramName";
  long long value = 777;
  call->with_parameter(param_name, value);
  STRCMP_EQUAL(
      "long long int", call->get_input_parameter_type(param_name).c_str());
  LONGLONGS_EQUAL(
      value, call->get_input_parameter(param_name).get_long_long_int_value());
  CHECK(call->has_input_parameter_with_name(param_name));
  STRCMP_CONTAINS("funcName -> long long int paramName: <777 (0x309)>",
      call->call_to_string().c_str());
}

TEST(MockExpectedCall, callWithDoubleParameter)
{
  const cpputest::String param_name = "paramName";
  double value = 1.2;
  call->with_parameter(param_name, value);
  STRCMP_EQUAL("double", call->get_input_parameter_type(param_name).c_str());
  DOUBLES_EQUAL(
      value, call->get_input_parameter(param_name).get_double_value(), 0.0);
  STRCMP_CONTAINS(
      "funcName -> double paramName: <1.2>", call->call_to_string().c_str());
}

TEST(MockExpectedCall, callWithDoubleParameterAndTolerance)
{
  const cpputest::String param_name = "paramName";
  double value = 1.2;
  double tolerance = 0.2;
  call->with_parameter(param_name, value, tolerance);
  STRCMP_EQUAL("double", call->get_input_parameter_type(param_name).c_str());
  DOUBLES_EQUAL(
      value, call->get_input_parameter(param_name).get_double_value(), 0.0);
  DOUBLES_EQUAL(tolerance,
      call->get_input_parameter(param_name).get_double_tolerance(),
      0.0);
  STRCMP_CONTAINS(
      "funcName -> double paramName: <1.2>", call->call_to_string().c_str());
}

TEST(MockExpectedCall, callWithStringParameter)
{
  const cpputest::String param_name = "paramName";
  const char* value = "hello world";
  call->with_parameter(param_name, value);
  STRCMP_EQUAL(
      "const char*", call->get_input_parameter_type(param_name).c_str());
  STRCMP_EQUAL(value, call->get_input_parameter(param_name).get_string_value());
  STRCMP_CONTAINS("funcName -> const char* paramName: <hello world>",
      call->call_to_string().c_str());
}

TEST(MockExpectedCall, callWithPointerParameter)
{
  const cpputest::String param_name = "paramName";
  void* value = reinterpret_cast<void*>(0x123);
  call->with_parameter(param_name, value);
  STRCMP_EQUAL("void*", call->get_input_parameter_type(param_name).c_str());
  POINTERS_EQUAL(
      value, call->get_input_parameter(param_name).get_pointer_value());
  STRCMP_CONTAINS(
      "funcName -> void* paramName: <0x123>", call->call_to_string().c_str());
}

TEST(MockExpectedCall, callWithConstPointerParameter)
{
  const cpputest::String param_name = "paramName";
  const void* value = reinterpret_cast<const void*>(0x345);
  call->with_parameter(param_name, value);
  STRCMP_EQUAL(
      "const void*", call->get_input_parameter_type(param_name).c_str());
  POINTERS_EQUAL(
      value, call->get_input_parameter(param_name).get_const_pointer_value());
  STRCMP_CONTAINS("funcName -> const void* paramName: <0x345>",
      call->call_to_string().c_str());
}

TEST(MockExpectedCall, callWithFunctionPointerParameter)
{
  const cpputest::String param_name = "paramName";
  auto value = reinterpret_cast<void (*)()>(0xdead);
  call->with_parameter(param_name, value);
  STRCMP_EQUAL(
      "void (*)()", call->get_input_parameter_type(param_name).c_str());
  FUNCTIONPOINTERS_EQUAL(value,
      call->get_input_parameter(param_name).get_function_pointer_value());
  STRCMP_CONTAINS("funcName -> void (*)() paramName: <0xdead>",
      call->call_to_string().c_str());
}

TEST(MockExpectedCall, callWithMemoryBuffer)
{
  const cpputest::String param_name = "paramName";
  const unsigned char value[] = { 0x12, 0xFE, 0xA1 };
  call->with_parameter(param_name, value, sizeof(value));
  STRCMP_EQUAL("const unsigned char*",
      call->get_input_parameter_type(param_name).c_str());
  POINTERS_EQUAL(
      value, call->get_input_parameter(param_name).get_memory_buffer());
  LONGS_EQUAL(sizeof(value), call->get_input_parameter(param_name).get_size());
  STRCMP_CONTAINS("funcName -> const unsigned char* paramName: <Size = 3 | "
                  "HexContents = 12 FE A1>",
      call->call_to_string().c_str());
}

TEST(MockExpectedCall, callWithObjectParameter)
{
  const cpputest::String param_name = "paramName";
  void* value = reinterpret_cast<void*>(0x123);
  call->with_parameter_of_type("ClassName", param_name, value);
  POINTERS_EQUAL(
      value, call->get_input_parameter(param_name).get_const_object_pointer());
  STRCMP_EQUAL("ClassName", call->get_input_parameter_type(param_name).c_str());
  STRCMP_CONTAINS("funcName -> ClassName paramName: <No comparator found for "
                  "type: \"ClassName\">",
      call->call_to_string().c_str());
}

TEST(MockExpectedCall, callWithObjectParameterUnequalComparison)
{
  TypeForTestingExpectedFunctionCall type(1), unequal_type(2);
  cpputest::MockNamedValue parameter("name");
  parameter.set_const_object_pointer("type", &unequal_type);
  call->with_parameter_of_type("type", "name", &type);
  CHECK(!call->has_input_parameter(parameter));
}

TEST(MockExpectedCall,
    callWithObjectParameterEqualComparisonButFailsWithoutRepository)
{
  TypeForTestingExpectedFunctionCall type(1), equal_type(1);
  cpputest::MockNamedValue parameter("name");
  parameter.set_const_object_pointer("type", &equal_type);
  call->with_parameter_of_type("type", "name", &type);
  CHECK(!call->has_input_parameter(parameter));
}

TEST(MockExpectedCall,
    callWithObjectParameterEqualComparisonButFailsWithoutComparator)
{
  cpputest::MockNamedValueComparatorsAndCopiersRepository repository;
  cpputest::MockNamedValue::set_default_comparators_and_copiers_repository(
      &repository);

  TypeForTestingExpectedFunctionCall type(1), equal_type(1);
  cpputest::MockNamedValue parameter("name");
  parameter.set_const_object_pointer("type", &equal_type);
  call->with_parameter_of_type("type", "name", &type);
  CHECK(!call->has_input_parameter(parameter));
}

TEST(MockExpectedCall, callWithObjectParameterEqualComparison)
{
  TypeForTestingExpectedFunctionCallComparator comparator;
  cpputest::MockNamedValueComparatorsAndCopiersRepository repository;
  cpputest::MockNamedValue::set_default_comparators_and_copiers_repository(
      &repository);
  repository.install_comparator("type", comparator);

  TypeForTestingExpectedFunctionCall type(1), equal_type(1);
  cpputest::MockNamedValue parameter("name");
  parameter.set_const_object_pointer("type", &equal_type);

  call->with_parameter_of_type("type", "name", &type);
  CHECK(call->has_input_parameter(parameter));
}

TEST(MockExpectedCall, getParameterValueOfObjectType)
{
  TypeForTestingExpectedFunctionCallComparator comparator;
  cpputest::MockNamedValueComparatorsAndCopiersRepository repository;
  cpputest::MockNamedValue::set_default_comparators_and_copiers_repository(
      &repository);
  repository.install_comparator("type", comparator);

  TypeForTestingExpectedFunctionCall type(1);
  call->with_parameter_of_type("type", "name", &type);
  POINTERS_EQUAL(
      &type, call->get_input_parameter("name").get_const_object_pointer());
  STRCMP_EQUAL("1", call->get_input_parameter_value_string("name").c_str());
}

TEST(MockExpectedCall, getParameterValueOfObjectTypeWithoutRepository)
{
  TypeForTestingExpectedFunctionCall type(1);
  call->with_parameter_of_type("type", "name", &type);
  STRCMP_EQUAL("No comparator found for type: \"type\"",
      call->get_input_parameter_value_string("name").c_str());
}

TEST(MockExpectedCall, getParameterValueOfObjectTypeWithoutComparator)
{
  TypeForTestingExpectedFunctionCall type(1);
  cpputest::MockNamedValueComparatorsAndCopiersRepository repository;
  cpputest::MockNamedValue::set_default_comparators_and_copiers_repository(
      &repository);
  call->with_parameter_of_type("type", "name", &type);
  STRCMP_EQUAL("No comparator found for type: \"type\"",
      call->get_input_parameter_value_string("name").c_str());
}

TEST(MockExpectedCall, callWithTwoUnsignedIntegerParameter)
{
  unsigned int expected_value = 1;
  unsigned int another_expected_value = 2;

  call->with_parameter("unsigned-integer1", expected_value);
  call->with_parameter("unsigned-integer2", another_expected_value);
  STRCMP_EQUAL("unsigned int",
      call->get_input_parameter_type("unsigned-integer1").c_str());
  STRCMP_EQUAL("unsigned int",
      call->get_input_parameter_type("unsigned-integer2").c_str());
  LONGS_EQUAL(expected_value,
      call->get_input_parameter("unsigned-integer1").get_unsigned_int_value());
  LONGS_EQUAL(another_expected_value,
      call->get_input_parameter("unsigned-integer2").get_unsigned_int_value());
}

TEST(MockExpectedCall, callWithTwoIntegerParameter)
{
  int expected_value = 1;
  int another_expected_value = -1;

  call->with_parameter("integer1", expected_value);
  call->with_parameter("integer2", another_expected_value);
  STRCMP_EQUAL("int", call->get_input_parameter_type("integer1").c_str());
  STRCMP_EQUAL("int", call->get_input_parameter_type("integer2").c_str());
  LONGS_EQUAL(
      expected_value, call->get_input_parameter("integer1").get_int_value());
  LONGS_EQUAL(another_expected_value,
      call->get_input_parameter("integer2").get_int_value());
}

TEST(MockExpectedCall, callWithThreeDifferentParameter)
{
  call->with_parameter("integer", 1);
  call->with_parameter("string", "hello world");
  call->with_parameter("double", 0.12);
  STRCMP_EQUAL("int", call->get_input_parameter_type("integer").c_str());
  STRCMP_EQUAL("const char*", call->get_input_parameter_type("string").c_str());
  STRCMP_EQUAL("double", call->get_input_parameter_type("double").c_str());
  LONGS_EQUAL(1, call->get_input_parameter("integer").get_int_value());
  STRCMP_EQUAL(
      "hello world", call->get_input_parameter("string").get_string_value());
  DOUBLES_EQUAL(
      0.12, call->get_input_parameter("double").get_double_value(), 0.05);
}

TEST(MockExpectedCall, singleCallNotMadeIsNotFulfilledButCanMatchActualCalls)
{
  cpputest::MockCheckedExpectedCall expected_call(1);
  expected_call.with_name("name");
  CHECK(!expected_call.is_fulfilled());
  CHECK(expected_call.can_match_actual_calls());
}

TEST(MockExpectedCall, singleCallMadeIsFulFilledAndCannotMatchActualCalls)
{
  cpputest::MockCheckedExpectedCall expected_call(1);
  expected_call.call_was_made(1);
  CHECK(expected_call.is_fulfilled());
  CHECK(!expected_call.can_match_actual_calls());
}

TEST(MockExpectedCall, multiCallNotMadeIsNotFulfilledButCanMatchActualCalls)
{
  cpputest::MockCheckedExpectedCall expected_call(2);
  expected_call.with_name("name");
  CHECK(!expected_call.is_fulfilled());
  CHECK(expected_call.can_match_actual_calls());
}

TEST(MockExpectedCall,
    multiCallNotMadeExpectedTimesIsNotFulfilledButCanMatchActualCalls)
{
  cpputest::MockCheckedExpectedCall expected_call(2);
  expected_call.with_name("name");
  expected_call.call_was_made(1);
  CHECK(!expected_call.is_fulfilled());
  CHECK(expected_call.can_match_actual_calls());
}

TEST(MockExpectedCall,
    multiCallsMadeExpectedTimesIsFulfilledAndCannotMatchActualCalls)
{
  cpputest::MockCheckedExpectedCall expected_call(3);
  expected_call.with_name("name");
  expected_call.call_was_made(1);
  expected_call.call_was_made(2);
  expected_call.call_was_made(3);
  CHECK(expected_call.is_fulfilled());
  CHECK(!expected_call.can_match_actual_calls());
}

TEST(MockExpectedCall,
    multiCallsMadeMoreThanExpectedTimesIsNotFulfilledAndCannotMatchActualCalls)
{
  cpputest::MockCheckedExpectedCall expected_call(3);
  expected_call.with_name("name");
  expected_call.call_was_made(1);
  expected_call.call_was_made(2);
  expected_call.call_was_made(3);
  expected_call.call_was_made(4);
  CHECK(!expected_call.is_fulfilled());
  CHECK(!expected_call.can_match_actual_calls());
}

TEST(MockExpectedCall, callsWithoutParameterAlwaysMatch)
{
  cpputest::MockCheckedExpectedCall expected_call(1);
  CHECK(expected_call.is_matching_actual_call());
}

TEST(MockExpectedCall, callsWithParameterNotFulfilledDontMatch)
{
  cpputest::MockCheckedExpectedCall expected_call(1);
  expected_call.with_parameter("para", 1);
  CHECK(!expected_call.is_matching_actual_call());
}

TEST(MockExpectedCall, callsWithParameterFulfilledDoMatch)
{
  cpputest::MockCheckedExpectedCall expected_call(1);
  expected_call.with_parameter("para", 1);
  expected_call.input_parameter_was_passed("para");
  CHECK(expected_call.is_matching_actual_call());
}

TEST(MockExpectedCall, callsWithSomeParametersNotFulfilledDontMatch)
{
  cpputest::MockCheckedExpectedCall expected_call(1);
  expected_call.with_parameter("para", 1).with_parameter("two", 2);
  expected_call.input_parameter_was_passed("para");
  CHECK(!expected_call.is_matching_actual_call());
}

TEST(MockExpectedCall, toStringForNoParametersSingleCallNotCalled)
{
  cpputest::MockCheckedExpectedCall expected_call(1);
  expected_call.with_name("name");
  STRCMP_EQUAL("name -> no parameters (expected 1 call, called 0 times)",
      expected_call.call_to_string().c_str());
}

TEST(MockExpectedCall,
    toStringForNoParametersMultiCallCalledLessThanExpectedTimes)
{
  cpputest::MockCheckedExpectedCall expected_call(2);
  expected_call.with_name("name");
  expected_call.call_was_made(1);
  STRCMP_EQUAL("name -> no parameters (expected 2 calls, called 1 time)",
      expected_call.call_to_string().c_str());
}

TEST(MockExpectedCall, toStringForNoParametersMultiCallCalledExpectedTimes)
{
  cpputest::MockCheckedExpectedCall expected_call(2);
  expected_call.with_name("name");
  expected_call.call_was_made(1);
  expected_call.call_was_made(2);
  STRCMP_EQUAL("name -> no parameters (expected 2 calls, called 2 times)",
      expected_call.call_to_string().c_str());
}

TEST(MockExpectedCall, toStringForIgnoredParameters)
{
  cpputest::MockCheckedExpectedCall expected_call(1);
  expected_call.with_name("name");
  expected_call.ignore_other_parameters();
  STRCMP_EQUAL(
      "name -> all parameters ignored (expected 1 call, called 0 times)",
      expected_call.call_to_string().c_str());
}

TEST(MockExpectedCall, toStringForMultipleInputParameters)
{
  int int_value = 10;
  unsigned int uint_value = 7;

  cpputest::MockCheckedExpectedCall expected_call(1);
  expected_call.with_name("name");
  expected_call.with_parameter("string", "value");
  expected_call.with_parameter("integer", int_value);
  expected_call.with_parameter("unsigned-integer", uint_value);
  expected_call.call_was_made(1);
  STRCMP_EQUAL("name -> const char* string: <value>, int integer: <10 (0xa)>, "
               "unsigned int unsigned-integer: <7 (0x7)> "
               "(expected 1 call, called 1 time)",
      expected_call.call_to_string().c_str());
}

TEST(MockExpectedCall, toStringForMultipleInputAndOutputParameters)
{
  int int_value = 10;
  unsigned int uint_value = 7;
  unsigned char buffer_value[3];

  cpputest::MockCheckedExpectedCall expected_call(1);
  expected_call.with_name("name");
  expected_call.with_parameter("string", "value");
  expected_call.with_parameter("integer", int_value);
  expected_call.with_parameter("unsigned-integer", uint_value);
  expected_call.with_output_parameter_returning(
      "buffer", buffer_value, sizeof(buffer_value));
  expected_call.call_was_made(1);
  STRCMP_EQUAL("name -> const char* string: <value>, int integer: <10 (0xa)>, "
               "unsigned int unsigned-integer: <7 (0x7)>, "
               "const void* buffer: <output> (expected 1 call, called 1 time)",
      expected_call.call_to_string().c_str());
}

TEST(MockExpectedCall, toStringForMultipleOutputParameters)
{
  unsigned char buffer_value[3];

  cpputest::MockCheckedExpectedCall expected_call(1);
  expected_call.with_name("name");
  expected_call.with_output_parameter_returning(
      "buffer1", buffer_value, sizeof(buffer_value));
  expected_call.with_output_parameter_returning(
      "buffer2", buffer_value, sizeof(buffer_value));
  expected_call.call_was_made(1);
  STRCMP_EQUAL("name -> const void* buffer1: <output>, const void* buffer2: "
               "<output> (expected 1 call, called 1 time)",
      expected_call.call_to_string().c_str());
}

TEST(MockExpectedCall, toStringForUnmodifiedOutputParameter)
{
  cpputest::MockCheckedExpectedCall expected_call(1);
  expected_call.with_name("name");
  expected_call.with_unmodified_output_parameter("buffer1");
  expected_call.call_was_made(1);
  STRCMP_EQUAL(
      "name -> const void* buffer1: <output> (expected 1 call, called 1 time)",
      expected_call.call_to_string().c_str());
}

TEST(MockExpectedCall, toStringForParameterAndIgnored)
{
  cpputest::MockCheckedExpectedCall expected_call(1);
  expected_call.with_name("name");
  expected_call.with_parameter("string", "value");
  expected_call.ignore_other_parameters();
  expected_call.call_was_made(1);
  STRCMP_EQUAL("name -> const char* string: <value>, other parameters are "
               "ignored (expected 1 call, called 1 time)",
      expected_call.call_to_string().c_str());
}

TEST(MockExpectedCall, toStringForCallOrderSingle)
{
  cpputest::MockCheckedExpectedCall expected_call(1);
  expected_call.with_name("name");
  expected_call.with_call_order(2);
  expected_call.call_was_made(1);
  STRCMP_EQUAL("name -> expected call order: <2> -> no parameters (expected 1 "
               "call, called 1 time)",
      expected_call.call_to_string().c_str());
}

TEST(MockExpectedCall, toStringForCallOrderMultiple)
{
  cpputest::MockCheckedExpectedCall expected_call(5);
  expected_call.with_name("name");
  expected_call.with_call_order(5, 9);
  expected_call.call_was_made(5);
  expected_call.call_was_made(6);
  expected_call.call_was_made(7);
  expected_call.call_was_made(8);
  expected_call.call_was_made(9);
  STRCMP_EQUAL("name -> expected calls order: <5..9> -> no parameters "
               "(expected 5 calls, called 5 times)",
      expected_call.call_to_string().c_str());
}

TEST(MockExpectedCall, callOrderIsFulfilledButWithWrongOrderSingle)
{
  call->with_name("name");
  call->with_call_order(2);
  call->call_was_made(1);
  CHECK(call->is_fulfilled());
  CHECK(call->is_out_of_order());
}

TEST(MockExpectedCall, callOrderIsFulfilledButWithWrongOrderMultipleTooEarly)
{
  cpputest::MockCheckedExpectedCall expected_call(3);
  expected_call.with_name("name");
  expected_call.with_call_order(10, 12);
  expected_call.call_was_made(9);
  expected_call.call_was_made(10);
  expected_call.call_was_made(11);
  CHECK(expected_call.is_fulfilled());
  CHECK(expected_call.is_out_of_order());
}

TEST(MockExpectedCall, callOrderIsFulfilledButWithWrongOrderMultipleTooLate)
{
  cpputest::MockCheckedExpectedCall expected_call(3);
  expected_call.with_name("name");
  expected_call.with_call_order(10, 12);
  expected_call.call_was_made(11);
  expected_call.call_was_made(12);
  expected_call.call_was_made(13);
  CHECK(expected_call.is_fulfilled());
  CHECK(expected_call.is_out_of_order());
}

TEST(MockExpectedCall, callOrderIsFulfilledSingle)
{
  call->with_name("name");
  call->with_call_order(1);
  call->call_was_made(1);
  CHECK(call->is_fulfilled());
  CHECK_FALSE(call->is_out_of_order());
}

TEST(MockExpectedCall, callOrderIsFulfilledMultiple)
{
  cpputest::MockCheckedExpectedCall expected_call(4);
  expected_call.with_name("name");
  expected_call.with_call_order(150, 153);
  expected_call.call_was_made(150);
  expected_call.call_was_made(151);
  expected_call.call_was_made(152);
  expected_call.call_was_made(153);
  CHECK(expected_call.is_fulfilled());
  CHECK_FALSE(expected_call.is_out_of_order());
}

TEST(MockExpectedCall, hasOutputParameter)
{
  const int value = 1;
  call->with_output_parameter_returning("foo", &value, sizeof(value));
  cpputest::MockNamedValue foo("foo");
  foo.set_value(&value);
  CHECK(call->has_output_parameter(foo));
}

TEST(MockExpectedCall, hasUnmodifiedOutputParameter)
{
  call->with_unmodified_output_parameter("foo");
  cpputest::MockNamedValue foo("foo");
  foo.set_value(static_cast<const void*>(nullptr));
  foo.set_size(0);
  CHECK(call->has_output_parameter(foo));
}

TEST(MockExpectedCall, hasNoOutputParameter)
{
  call->with_int_parameter("foo", static_cast<int>(1));
  cpputest::MockNamedValue foo("foo");
  foo.set_value(static_cast<int>(1));
  CHECK_FALSE(call->has_output_parameter(foo));
}

TEST(MockExpectedCall, hasOutputParameterOfType)
{
  TypeForTestingExpectedFunctionCall object(6789);
  call->with_output_parameter_of_type_returning(
      "TypeForTestingExpectedFunctionCall", "foo", &object);
  cpputest::MockNamedValue foo("foo");
  foo.set_const_object_pointer("TypeForTestingExpectedFunctionCall", &object);
  CHECK(call->has_output_parameter(foo));
}

TEST(MockExpectedCall, hasNoOutputParameterOfTypeSameTypeButInput)
{
  TypeForTestingExpectedFunctionCall object(543);
  call->with_parameter_of_type(
      "TypeForTestingExpectedFunctionCall", "foo", &object);
  cpputest::MockNamedValue foo("foo");
  foo.set_const_object_pointer("TypeForTestingExpectedFunctionCall", &object);
  CHECK_FALSE(call->has_output_parameter(foo));
}

TEST(MockExpectedCall, hasNoOutputParameterOfTypeDifferentType)
{
  TypeForTestingExpectedFunctionCall object(543);
  call->with_output_parameter_of_type_returning(
      "TypeForTestingExpectedFunctionCall", "foo", &object);
  cpputest::MockNamedValue foo("foo");
  foo.set_const_object_pointer(
      "OtherTypeForTestingExpectedFunctionCall", &object);
  CHECK_FALSE(call->has_output_parameter(foo));
}

TEST_GROUP(MockIgnoredExpectedCall)
{
  cpputest::MockIgnoredExpectedCall ignored;
};

TEST(MockIgnoredExpectedCall, worksAsItShould)
{
  ignored.with_name("func");
  ignored.with_call_order(1);
  ignored.with_call_order(1, 1);
  ignored.on_object(nullptr);
  ignored.with_bool_parameter("umm", true);
  ignored.with_int_parameter("bla", static_cast<int>(1));
  ignored.with_unsigned_int_parameter("foo", static_cast<unsigned int>(1));
  ignored.with_long_int_parameter("hey", static_cast<long int>(1));
  ignored.with_unsigned_long_int_parameter(
      "bah", static_cast<unsigned long int>(1));
  ignored.with_long_long_int_parameter("yo", static_cast<long long int>(1));
  ignored.with_unsigned_long_long_int_parameter(
      "grr", static_cast<unsigned long long int>(1));
  ignored.with_double_parameter("hah", static_cast<double>(1.1f));
  ignored.with_double_parameter("gah", 2.1, 0.3);
  ignored.with_string_parameter("goo", "hello");
  ignored.with_pointer_parameter("pie", static_cast<void*>(nullptr));
  ignored.with_const_pointer_parameter(
      "woo", static_cast<const void*>(nullptr));
  ignored.with_function_pointer_parameter(
      "fop", static_cast<void (*)()>(nullptr));
  ignored.with_memory_buffer_parameter(
      "waa", static_cast<const unsigned char*>(nullptr), 0);
  ignored.with_parameter_of_type(
      "mytype", "top", static_cast<const void*>(nullptr));
  ignored.with_output_parameter_returning(
      "bar", static_cast<void*>(nullptr), 1);
  ignored.with_output_parameter_of_type_returning(
      "mytype", "bar", static_cast<const void*>(nullptr));
  ignored.with_unmodified_output_parameter("unmod");
  ignored.ignore_other_parameters();
  ignored.and_return_value(true);
  ignored.and_return_value(static_cast<double>(1.0f));
  ignored.and_return_value(static_cast<unsigned int>(1));
  ignored.and_return_value(static_cast<int>(1));
  ignored.and_return_value(static_cast<unsigned long int>(1));
  ignored.and_return_value(static_cast<long int>(1));
  ignored.and_return_value(static_cast<unsigned long long int>(1));
  ignored.and_return_value(static_cast<long long int>(1));
  ignored.and_return_value("boo");
  ignored.and_return_value(static_cast<void*>(nullptr));
  ignored.and_return_value(static_cast<const void*>(nullptr));
  ignored.and_return_value(static_cast<void (*)()>(nullptr));
}
