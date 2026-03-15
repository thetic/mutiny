#include "CppUTest/CppUTest.h"
#include "CppUTest/CppUTest.hpp"
#include "CppUTest/MockSupport.hpp"

TEST_GROUP_C_WRAPPER(MockDocumentation_C)
{};

TEST_C_WRAPPER(MockDocumentation_C, CInterface)

TEST_GROUP(MockDocumentation)
{};

namespace {
void
production_code()
{
  cpputest::mock().actual_call("productionCode");
}

void
parameters_function(int p1, const char* p2)
{
  void* object = reinterpret_cast<void*>(1);
  cpputest::mock()
      .actual_call("function")
      .on_object(object)
      .with_parameter("p1", p1)
      .with_parameter("p2", p2);
}

void
do_something_that_would_otherwise_blow_up_the_mocking_framework()
{
}

} // namespace

TEST(MockDocumentation, SimpleScenario)
{
  cpputest::mock().expect_one_call("productionCode");
  production_code();
  cpputest::mock().check_expectations();
}

class ClassFromProductionCode
{
public:
  virtual void important_function() {}
  virtual ~ClassFromProductionCode() {}
};

class ClassFromProductionCodeMock : public ClassFromProductionCode
{
public:
  virtual void important_function() override
  {
    cpputest::mock().actual_call("importantFunction").on_object(this);
  }
};

TEST(MockDocumentation, SimpleScenarioObject)
{
  ClassFromProductionCode* object =
      new ClassFromProductionCodeMock; /* create mock instead of real thing */

  cpputest::mock().expect_one_call("importantFunction").on_object(object);
  object->important_function();
  cpputest::mock().check_expectations();

  delete object;
}

TEST(MockDocumentation, parameters)
{
  void* object = reinterpret_cast<void*>(1);
  cpputest::mock()
      .expect_one_call("function")
      .on_object(object)
      .with_parameter("p1", 2)
      .with_parameter("p2", "hah");
  parameters_function(2, "hah");
}

class MyTypeComparator : public cpputest::MockNamedValueComparator
{
public:
  virtual bool is_equal(const void* object1, const void* object2) override
  {
    return object1 == object2;
  }
  virtual cpputest::String value_to_string(const void* object) override
  {
    return cpputest::string_from(object);
  }
};

TEST(MockDocumentation, ObjectParameters)
{
  void* object = reinterpret_cast<void*>(1);
  MyTypeComparator comparator;
  cpputest::mock().install_comparator("myType", comparator);
  cpputest::mock()
      .expect_one_call("function")
      .with_parameter_of_type("myType", "parameterName", object);
  cpputest::mock().clear();
  cpputest::mock().remove_all_comparators_and_copiers();
}

TEST(MockDocumentation, returnValue)
{
  cpputest::mock().expect_one_call("function").and_return_value(10);
  cpputest::mock().actual_call("function").return_value().get_int_value();
  int value = cpputest::mock().return_value().get_int_value();
  LONGS_EQUAL(10, value);
}

TEST(MockDocumentation, setData)
{
  ClassFromProductionCode object;
  cpputest::mock().set_data("importantValue", 10);
  cpputest::mock().set_data_object(
      "importantObject", "ClassFromProductionCode", &object);

  ClassFromProductionCode* pobject;
  int value = cpputest::mock().get_data("importantValue").get_int_value();
  pobject = static_cast<ClassFromProductionCode*>(
      cpputest::mock().get_data("importantObject").get_object_pointer());

  LONGS_EQUAL(10, value);
  POINTERS_EQUAL(pobject, &object);
}

TEST(MockDocumentation, otherMockSupport)
{
  cpputest::mock().crash_on_failure();
  //	mock().actualCall("unex");

  cpputest::mock().expect_one_call("foo");
  cpputest::mock().ignore_other_calls();

  cpputest::mock().disable();
  do_something_that_would_otherwise_blow_up_the_mocking_framework();
  cpputest::mock().enable();

  cpputest::mock().clear();
}

TEST(MockDocumentation, scope)
{
  cpputest::mock("xmlparser").expect_one_call("open");
  cpputest::mock("filesystem").ignore_other_calls();

  cpputest::mock("xmlparser").actual_call("open");
}
