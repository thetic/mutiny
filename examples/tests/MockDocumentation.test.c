#include "CppMu/CppMu.h"
#include "CppMu/MockSupport.h"

static int
equal_method(const void* object1, const void* object2)
{
  return object1 == object2;
}

static const char*
to_string_method(const void* object)
{
  (void)object;
  return "string";
}

TEST(MockDocumentation_C, CInterface)
{
  void* object = (void*)0x1;

  mock()
      ->expect_one_call("foo")
      ->with_int_parameters("integer", 10)
      ->and_return_double_value(1.11);
  double d = mock()
                 ->actual_call("foo")
                 ->with_int_parameters("integer", 10)
                 ->return_value()
                 .value.double_value;
  CHECK_EQUAL_REAL(1.11, d, 0.00001);

  mock()->install_comparator("type", equal_method, to_string_method);
  mock_scope("scope")->expect_one_call("bar")->with_parameter_of_type(
      "type", "name", object);
  mock_scope("scope")->actual_call("bar")->with_parameter_of_type(
      "type", "name", object);
  mock()->remove_all_comparators_and_copiers();

  mock()->set_int_data("important", 10);
  mock()->check_expectations();
  mock()->clear();
}
