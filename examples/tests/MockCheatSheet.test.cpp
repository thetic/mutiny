#include "mutiny/mock.hpp"
#include "mutiny/test.hpp"

using mu::tiny::mock::mock;

namespace {
/* Stubbed out product code using linker, function pointer, or overriding */
int foo(const char* param_string, int param_int)
{
  /* Tell mutiny Mocking what we mock. Also return recorded value */
  return mock()
      .actual_call("Foo")
      .with_parameter("param_string", param_string)
      .with_parameter("param_int", param_int)
      .return_value()
      .get_value_as<int>();
}

void bar(double param_double, const char* param_string)
{
  mock()
      .actual_call("Bar")
      .with_parameter("param_double", param_double)
      .with_parameter("param_string", param_string);
}

/* Production code calls to the methods we stubbed */
int production_code_foo_calls()
{
  int return_value;
  return_value = foo("value_string", 10);
  (void)return_value;
  return_value = foo("value_string", 10);
  return return_value;
}

void production_code_bar_calls()
{
  bar(1.5, "more");
  bar(1.5, "more");
}

} // namespace

TEST_GROUP(MockCheatSheet)
{
  void teardown() override
  {
    /* Check expectations. Alternatively use SupportPlugin */
    mock().check_expectations();

    mock().clear();
  }
};

TEST(MockCheatSheet, foo)
{
  /* Record 2 calls to Foo. Return different values on each call */
  mock()
      .expect_one_call("Foo")
      .with_parameter("param_string", "value_string")
      .with_parameter("param_int", 10)
      .and_return_value(30);
  mock().expect_one_call("Foo").ignore_other_parameters().and_return_value(50);

  /* Call production code */
  production_code_foo_calls();
}

TEST(MockCheatSheet, bar)
{
  /* Expect 2 calls on Bar. Check only one parameter */
  mock()
      .expect_n_calls(2, "Bar")
      .with_parameter("param_double", 1.5)
      .ignore_other_parameters();

  /* And the production code call */
  production_code_bar_calls();
}
