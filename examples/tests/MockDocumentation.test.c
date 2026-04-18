#include "mu/tiny/mock.h"
#include "mu/tiny/test.h"

static int equal_method(const void* object1, const void* object2)
{
  return object1 == object2;
}

static const char* to_string_method(const void* object)
{
  (void)object;
  return "string";
}

TEST(MockDocumentation_C, CInterface)
{
  void* object = (void*)0x1;

  mutiny_mock()
      ->expect_one_call("foo")
      ->with_int_parameters("integer", 10)
      ->and_return_double_value(1.11);
  double d = mutiny_mock()
                 ->actual_call("foo")
                 ->with_int_parameters("integer", 10)
                 ->return_value()
                 .value.double_value;
  CHECK_EQUAL_DOUBLE(1.11, d, 0.00001);

  mutiny_mock()->install_comparator("type", equal_method, to_string_method);
  mutiny_mock_scope("scope")->expect_one_call("bar")->with_parameter_of_type(
      "type", "name", object
  );
  mutiny_mock_scope("scope")->actual_call("bar")->with_parameter_of_type(
      "type", "name", object
  );
  mutiny_mock()->remove_all_comparators_and_copiers();

  mutiny_mock()->set_int_data("important", 10);
  mutiny_mock()->check_expectations();
  mutiny_mock()->clear();
}
