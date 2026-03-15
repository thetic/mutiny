#include "CppUTest/Extensions/MockSupport.hpp"

#include "CppUTest/CppUTest.h"
#include "CppUTest/CppUTest.hpp"

TEST_GROUP_C_WRAPPER(MockDocumentation_C)
{};

TEST_C_WRAPPER(MockDocumentation_C, CInterface)

TEST_GROUP(MockDocumentation)
{};

namespace {
void
production_code()
{
  cpputest::extensions::mock().actual_call("productionCode");
}

void
parameters_function(int p1, const char* p2)
{
  void* object = reinterpret_cast<void*>(1);
  cpputest::extensions::mock()
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
  cpputest::extensions::mock().expect_one_call("productionCode");
  production_code();
  cpputest::extensions::mock().check_expectations();
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
    cpputest::extensions::mock()
        .actual_call("importantFunction")
        .on_object(this);
  }
};

TEST(MockDocumentation, SimpleScenarioObject)
{
  ClassFromProductionCode* object =
      new ClassFromProductionCodeMock; /* create mock instead of real thing */

  cpputest::extensions::mock()
      .expect_one_call("importantFunction")
      .on_object(object);
  object->important_function();
  cpputest::extensions::mock().check_expectations();

  delete object;
}

TEST(MockDocumentation, parameters)
{
  void* object = reinterpret_cast<void*>(1);
  cpputest::extensions::mock()
      .expect_one_call("function")
      .on_object(object)
      .with_parameter("p1", 2)
      .with_parameter("p2", "hah");
  parameters_function(2, "hah");
}

class MyTypeComparator : public cpputest::extensions::MockNamedValueComparator
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
  cpputest::extensions::mock().install_comparator("myType", comparator);
  cpputest::extensions::mock()
      .expect_one_call("function")
      .with_parameter_of_type("myType", "parameterName", object);
  cpputest::extensions::mock().clear();
  cpputest::extensions::mock().remove_all_comparators_and_copiers();
}

TEST(MockDocumentation, returnValue)
{
  cpputest::extensions::mock().expect_one_call("function").and_return_value(10);
  cpputest::extensions::mock()
      .actual_call("function")
      .return_value()
      .get_int_value();
  int value = cpputest::extensions::mock().return_value().get_int_value();
  LONGS_EQUAL(10, value);
}

TEST(MockDocumentation, setData)
{
  ClassFromProductionCode object;
  cpputest::extensions::mock().set_data("importantValue", 10);
  cpputest::extensions::mock().set_data_object(
      "importantObject", "ClassFromProductionCode", &object);

  ClassFromProductionCode* pobject;
  int value =
      cpputest::extensions::mock().get_data("importantValue").get_int_value();
  pobject = static_cast<ClassFromProductionCode*>(cpputest::extensions::mock()
          .get_data("importantObject")
          .get_object_pointer());

  LONGS_EQUAL(10, value);
  POINTERS_EQUAL(pobject, &object);
}

TEST(MockDocumentation, otherMockSupport)
{
  cpputest::extensions::mock().crash_on_failure();
  //	mock().actualCall("unex");

  cpputest::extensions::mock().expect_one_call("foo");
  cpputest::extensions::mock().ignore_other_calls();

  cpputest::extensions::mock().disable();
  do_something_that_would_otherwise_blow_up_the_mocking_framework();
  cpputest::extensions::mock().enable();

  cpputest::extensions::mock().clear();
}

TEST(MockDocumentation, scope)
{
  cpputest::extensions::mock("xmlparser").expect_one_call("open");
  cpputest::extensions::mock("filesystem").ignore_other_calls();

  cpputest::extensions::mock("xmlparser").actual_call("open");
}
