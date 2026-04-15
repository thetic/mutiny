#include "mutiny/mock/NamedValueComparator.hpp"

#include "mutiny/mock.hpp"
#include "mutiny/test.h"
#include "mutiny/test.hpp"

#include <stdint.h>

TEST_GROUP_C_WRAPPER(MockDocumentation_C)
{};

TEST_C_WRAPPER(MockDocumentation_C, CInterface)

TEST_GROUP(MockDocumentation)
{};

namespace {
void production_code()
{
  mu::tiny::mock::mock().actual_call("productionCode");
}

void parameters_function(int p1, const char* p2)
{
  void* object = reinterpret_cast<void*>(1);
  mu::tiny::mock::mock()
      .actual_call("function")
      .on_object(object)
      .with_parameter("p1", p1)
      .with_parameter("p2", p2);
}

void do_something_that_would_otherwise_blow_up_the_mocking_framework() {}

class ClassFromProductionCode
{
public:
  virtual void important_function() {}
  virtual ~ClassFromProductionCode() = default;
};

class ClassFromProductionCodeMock : public ClassFromProductionCode
{
public:
  void important_function() override
  {
    mu::tiny::mock::mock().actual_call("importantFunction").on_object(this);
  }
};

uint32_t uint32_function(uint32_t p1)
{
  return mu::tiny::mock::mock()
      .actual_call("uint32Function")
      .with_parameter("p1", p1)
      .return_value<uint32_t>();
}

void two_param_function(uint32_t p1, uint32_t p2)
{
  mu::tiny::mock::mock()
      .actual_call("twoParamFunction")
      .with_parameter("p1", p1)
      .with_parameter("p2", p2);
}

int64_t mixed_sizes_function(uint8_t p1)
{
  return mu::tiny::mock::mock()
      .actual_call("mixedSizesFunction")
      .with_parameter("p1", p1)
      .return_value<int64_t>();
}

} // namespace

TEST(MockDocumentation, SimpleScenario)
{
  mu::tiny::mock::mock().expect_one_call("productionCode");
  production_code();
  mu::tiny::mock::mock().check_expectations();
}

TEST(MockDocumentation, SimpleScenarioObject)
{
  ClassFromProductionCode* object =
      new ClassFromProductionCodeMock; /* create mock instead of real thing */

  mu::tiny::mock::mock().expect_one_call("importantFunction").on_object(object);
  object->important_function();
  mu::tiny::mock::mock().check_expectations();

  delete object;
}

TEST(MockDocumentation, parameters)
{
  void* object = reinterpret_cast<void*>(1);
  mu::tiny::mock::mock()
      .expect_one_call("function")
      .on_object(object)
      .with_parameter("p1", 2)
      .with_parameter("p2", "hah");
  parameters_function(2, "hah");
}

class MyTypeComparator : public mu::tiny::mock::NamedValueComparator
{
public:
  bool is_equal(const void* object1, const void* object2) override
  {
    return object1 == object2;
  }
  mu::tiny::String value_to_string(const void* object) override
  {
    return mu::tiny::string_from(object);
  }
};

TEST(MockDocumentation, ObjectParameters)
{
  void* object = reinterpret_cast<void*>(1);
  MyTypeComparator comparator;
  mu::tiny::mock::mock().install_comparator("myType", comparator);
  mu::tiny::mock::mock()
      .expect_one_call("function")
      .with_parameter_of_type("myType", "parameterName", object);
  mu::tiny::mock::mock().clear();
  mu::tiny::mock::mock().remove_all_comparators_and_copiers();
}

TEST(MockDocumentation, returnValue)
{
  mu::tiny::mock::mock().expect_one_call("function").and_return_value(10);
  mu::tiny::mock::mock()
      .actual_call("function")
      .return_value()
      .get_value<int>();
  int value = mu::tiny::mock::mock().return_value().get_value<int>();
  CHECK_EQUAL(10, value);
}

TEST(MockDocumentation, setData)
{
  ClassFromProductionCode object;
  mu::tiny::mock::mock().set_data("importantValue", 10);
  mu::tiny::mock::mock().set_data_object(
      "importantObject", "ClassFromProductionCode", &object
  );

  ClassFromProductionCode* pobject;
  int value =
      mu::tiny::mock::mock().get_data("importantValue").get_value<int>();
  pobject = static_cast<ClassFromProductionCode*>(
      mu::tiny::mock::mock().get_data("importantObject").get_object_pointer()
  );

  CHECK_EQUAL(10, value);
  CHECK_EQUAL(pobject, &object);
}

TEST(MockDocumentation, otherMockSupport)
{
  mu::tiny::mock::mock().crash_on_failure();
  //	mock().actualCall("unex");

  mu::tiny::mock::mock().expect_one_call("foo");
  mu::tiny::mock::mock().ignore_other_calls();

  mu::tiny::mock::mock().disable();
  do_something_that_would_otherwise_blow_up_the_mocking_framework();
  mu::tiny::mock::mock().enable();

  mu::tiny::mock::mock().clear();
}

TEST(MockDocumentation, fixedWidthReturnValue)
{
  mu::tiny::mock::mock()
      .expect_one_call("uint32Function")
      .with_parameter("p1", UINT32_C(0x40000000))
      .and_return_value(UINT32_C(0xDEADBEEF));
  CHECK_EQUAL(UINT32_C(0xDEADBEEF), uint32_function(UINT32_C(0x40000000)));
}

TEST(MockDocumentation, fixedWidthParameters)
{
  mu::tiny::mock::mock()
      .expect_one_call("twoParamFunction")
      .with_parameter("p1", UINT32_C(0x40000000))
      .with_parameter("p2", UINT32_C(0xCAFEBABE));
  two_param_function(UINT32_C(0x40000000), UINT32_C(0xCAFEBABE));
}

TEST(MockDocumentation, fixedWidthMixedSizes)
{
  mu::tiny::mock::mock()
      .expect_one_call("mixedSizesFunction")
      .with_parameter("p1", UINT8_C(3))
      .and_return_value(INT64_C(-4000000000));
  CHECK_EQUAL(INT64_C(-4000000000), mixed_sizes_function(UINT8_C(3)));
}

TEST(MockDocumentation, fixedWidthSetData)
{
  mu::tiny::mock::mock().set_data("calibration", static_cast<int64_t>(42));
  auto value =
      mu::tiny::mock::mock().get_data("calibration").get_value<int64_t>();
  CHECK_EQUAL(42, value);
}

TEST(MockDocumentation, scope)
{
  mu::tiny::mock::mock("xmlparser").expect_one_call("open");
  mu::tiny::mock::mock("filesystem").ignore_other_calls();

  mu::tiny::mock::mock("xmlparser").actual_call("open");
}
