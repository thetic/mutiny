#include "MockSupport.h"

#include "CppMu/MockSupport.h"

static int type_name_is_equal(const void* object1, const void* object2)
{
  return object1 == object2;
}

static const char* type_name_value_to_string(const void* object)
{
  return (const char*)object;
}

void cppmu_all_mock_support_c_calls(void)
{
  cppmu_mock()->strict_order();
  cppmu_mock()->expect_one_call("boo");
  cppmu_mock()->expect_no_call("bla");
  cppmu_mock()->expect_n_calls(1, "foo");
  cppmu_mock()->actual_call("boo");
  cppmu_mock()->actual_call("foo");
  cppmu_mock()->check_expectations();

  cppmu_mock()
      ->expect_one_call("boo")
      ->with_int_parameters("integer", 1)
      ->with_bool_parameters("bool", 1)
      ->with_unsigned_int_parameters("unsigned", 1)
      ->with_long_int_parameters("long int", (long int)-1)
      ->with_unsigned_long_int_parameters(
          "unsigned long int", (unsigned long int)1
      )
      ->with_long_long_int_parameters("long long int", (long long int)-1)
      ->with_unsigned_long_long_int_parameters(
          "unsigned long long int", (unsigned long long int)1
      )
      ->with_double_parameters("double", 1.0)
      ->with_double_parameters_and_tolerance("doubleWithTolerance", 1.0, 1.0)
      ->with_string_parameters("string", "string")
      ->with_pointer_parameters("pointer", (void*)1)
      ->with_const_pointer_parameters("constpointer", (const void*)1)
      ->with_function_pointer_parameters("functionpointer", (void (*)(void))1)
      ->with_memory_buffer_parameter("name", (void*)1, 0UL)
      ->ignore_other_parameters();

  cppmu_mock()
      ->actual_call("boo")
      ->with_int_parameters("integer", 1)
      ->with_bool_parameters("bool", 1)
      ->with_unsigned_int_parameters("unsigned", 1)
      ->with_long_int_parameters("long int", (long int)-1)
      ->with_unsigned_long_int_parameters(
          "unsigned long int", (unsigned long int)1
      )
      ->with_long_long_int_parameters("long long int", (long long int)-1)
      ->with_unsigned_long_long_int_parameters(
          "unsigned long long int", (unsigned long long int)1
      )
      ->with_double_parameters("double", 1.0)
      ->with_double_parameters("doubleWithTolerance", 0.0)
      ->with_string_parameters("string", "string")
      ->with_pointer_parameters("pointer", (void*)1)
      ->with_const_pointer_parameters("constpointer", (const void*)1)
      ->with_function_pointer_parameters("functionpointer", (void (*)(void))1)
      ->with_memory_buffer_parameter("name", (void*)1, 0UL)
      ->has_return_value();

  cppmu_mock()->disable();
  cppmu_mock()
      ->expect_one_call("boo")
      ->with_parameter_of_type("type", "name", (void*)1)
      ->with_output_parameter_returning("name", (void*)1, 0UL)
      ->with_output_parameter_of_type_returning("type", "name", (void*)1);
  cppmu_mock()
      ->actual_call("boo")
      ->with_parameter_of_type("type", "name", (void*)1)
      ->with_output_parameter("name", (void*)1)
      ->with_output_parameter_of_type("type", "name", (void*)1);
  cppmu_mock()->enable();

  cppmu_mock()->clear();

  cppmu_mock()->install_comparator(
      "typeName", type_name_is_equal, type_name_value_to_string
  );
  cppmu_mock()->expect_one_call("boo")->with_parameter_of_type(
      "typeName", "name", (void*)1
  );
  cppmu_mock()->actual_call("boo")->with_parameter_of_type(
      "typeName", "name", (void*)1
  );
  cppmu_mock()->clear();
  cppmu_mock()->remove_all_comparators_and_copiers();

  cppmu_mock()->expect_one_call("boo")->and_return_bool_value(1);
  cppmu_mock()->actual_call("boo")->bool_return_value();
  cppmu_mock()->bool_return_value();

  cppmu_mock()->expect_one_call("boo")->and_return_int_value(-10);
  cppmu_mock()->actual_call("boo")->int_return_value();
  cppmu_mock()->int_return_value();
  cppmu_mock()->return_value();

  cppmu_mock()->expect_one_call("boo2")->and_return_unsigned_int_value(1);
  cppmu_mock()->actual_call("boo2")->unsigned_int_return_value();
  cppmu_mock()->unsigned_int_return_value();

  cppmu_mock()->expect_one_call("boo3")->and_return_long_int_value(1);
  cppmu_mock()->actual_call("boo3")->long_int_return_value();
  cppmu_mock()->long_int_return_value();

  cppmu_mock()->expect_one_call("boo3")->and_return_unsigned_long_int_value(1);
  cppmu_mock()->actual_call("boo3")->unsigned_long_int_return_value();
  cppmu_mock()->unsigned_long_int_return_value();

  cppmu_mock()->expect_one_call("mgrgrgr1")->and_return_long_long_int_value(1);
  cppmu_mock()->actual_call("mgrgrgr1")->long_long_int_return_value();
  cppmu_mock()->long_long_int_return_value();

  cppmu_mock()
      ->expect_one_call("mgrgrgr2")
      ->and_return_unsigned_long_long_int_value(1);
  cppmu_mock()->actual_call("mgrgrgr2")->unsigned_long_long_int_return_value();
  cppmu_mock()->unsigned_long_long_int_return_value();

  cppmu_mock()->expect_one_call("boo4")->and_return_double_value(1.0);
  cppmu_mock()->actual_call("boo4")->double_return_value();
  cppmu_mock()->double_return_value();

  cppmu_mock()->expect_one_call("boo5")->and_return_string_value("hello world");
  cppmu_mock()->actual_call("boo5")->string_return_value();
  cppmu_mock()->string_return_value();

  cppmu_mock()->expect_one_call("boo6")->and_return_pointer_value((void*)10);
  cppmu_mock()->actual_call("boo6")->pointer_return_value();
  cppmu_mock()->pointer_return_value();

  cppmu_mock()->expect_one_call("boo7")->and_return_const_pointer_value(
      (void*)10
  );
  cppmu_mock()->actual_call("boo7")->const_pointer_return_value();
  cppmu_mock()->const_pointer_return_value();

  cppmu_mock()->expect_one_call("boo8")->and_return_function_pointer_value(
      (void (*)(void))10
  );
  cppmu_mock()->actual_call("boo8")->function_pointer_return_value();
  cppmu_mock()->function_pointer_return_value();

  cppmu_mock()->set_bool_data("bool", 1);
  cppmu_mock()->expect_one_call("bla")->with_bool_parameters("bool", 1);
  cppmu_mock()->actual_call("bla")->with_bool_parameters(
      "bool", cppmu_mock()->get_data("bool").value.bool_value
  );

  cppmu_mock()->set_int_data("int", 5);
  cppmu_mock()->expect_one_call("bla")->with_int_parameters("int", 5);
  cppmu_mock()->actual_call("bla")->with_int_parameters(
      "int", cppmu_mock()->get_data("int").value.int_value
  );

  cppmu_mock()->set_string_data("string", "lol");
  cppmu_mock()->expect_one_call("bla")->with_string_parameters("str", "lol");
  cppmu_mock()->actual_call("bla")->with_string_parameters(
      "str", cppmu_mock()->get_data("string").value.string_value
  );

  cppmu_mock()->set_double_data("double", 0.001);
  cppmu_mock()->expect_one_call("bla")->with_double_parameters("double", 0.001);
  cppmu_mock()->actual_call("bla")->with_double_parameters(
      "double", cppmu_mock()->get_data("double").value.double_value
  );

  cppmu_mock()->set_pointer_data("ptr", (void*)1);
  cppmu_mock()->expect_one_call("bla")->with_pointer_parameters(
      "ptr", (void*)1
  );
  cppmu_mock()->actual_call("bla")->with_pointer_parameters(
      "ptr", cppmu_mock()->get_data("ptr").value.pointer_value
  );

  cppmu_mock()->set_const_pointer_data("cptr", (const void*)1);
  cppmu_mock()->expect_one_call("bla")->with_const_pointer_parameters(
      "cptr", (const void*)1
  );
  cppmu_mock()->actual_call("bla")->with_const_pointer_parameters(
      "cptr", cppmu_mock()->get_data("ptr").value.const_pointer_value
  );

  cppmu_mock()->set_function_pointer_data("ptr", (void (*)(void))1);
  cppmu_mock()->expect_one_call("bla")->with_function_pointer_parameters(
      "ptr", (void (*)(void))1
  );
  cppmu_mock()->actual_call("bla")->with_function_pointer_parameters(
      "ptr", cppmu_mock()->get_data("ptr").value.function_pointer_value
  );

  cppmu_mock()->clear();

  cppmu_mock()->has_return_value();
  cppmu_mock()->return_bool_value_or_default(1);
  cppmu_mock()->return_int_value_or_default(-1);
  cppmu_mock()->return_unsigned_int_value_or_default(1);
  cppmu_mock()->return_long_int_value_or_default(-1L);
  cppmu_mock()->return_unsigned_long_int_value_or_default(1L);
  cppmu_mock()->return_long_long_int_value_or_default(-1LL);
  cppmu_mock()->return_unsigned_long_long_int_value_or_default(1ULL);
  cppmu_mock()->return_string_value_or_default("");
  cppmu_mock()->return_double_value_or_default(0.01);
  cppmu_mock()->return_pointer_value_or_default(0);
  cppmu_mock()->return_const_pointer_value_or_default(0);
  cppmu_mock()->return_function_pointer_value_or_default(0);

  cppmu_mock()->disable();
  cppmu_mock()->actual_call("disabled");
  cppmu_mock()->enable();
  cppmu_mock()->check_expectations();

  cppmu_mock()->set_int_data("bla1", -2);
  cppmu_mock()->set_unsigned_int_data("bla2", 2);
  cppmu_mock()->set_double_data("bla3", 0.035);
  cppmu_mock()->set_string_data("bla4", "abc");
  cppmu_mock()->set_pointer_data("bla", (void*)2);
  cppmu_mock()->set_const_pointer_data("bla", (const void*)2);
  cppmu_mock()->set_function_pointer_data("bla", (void (*)(void))2);
  cppmu_mock()->set_data_object("bla", "type", (void*)2);
  cppmu_mock()->get_data("bla");

  cppmu_mock_scope("scope")->expect_one_call("boo");
  cppmu_mock_scope("other")->expected_calls_left();
  cppmu_mock_scope("scope")->expected_calls_left();
  cppmu_mock_scope("scope")->actual_call("boo");
}
