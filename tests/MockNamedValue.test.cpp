#include "CppMu/MockNamedValue.hpp"

#include "CppMu/CppMu.hpp"
#include "CppMu/MockNamedValueComparatorsAndCopiersRepository.hpp"

TEST_GROUP(ComparatorsAndCopiersRepository)
{};

class MyComparator : public cppmu::MockNamedValueComparator
{
public:
  MyComparator() {}
  virtual ~MyComparator() override {}

  virtual bool is_equal(const void*, const void*) override { return false; }
  virtual cppmu::String value_to_string(const void*) override { return ""; }
};

class MyCopier : public cppmu::MockNamedValueCopier
{
public:
  MyCopier() {}
  virtual ~MyCopier() override {}

  virtual void copy(void*, const void*) override {}
};

TEST(ComparatorsAndCopiersRepository, InstallCopierAndRetrieveIt)
{
  MyCopier copier;
  cppmu::MockNamedValueComparatorsAndCopiersRepository repository;
  repository.install_copier("MyType", copier);
  POINTERS_EQUAL(&copier, repository.get_copier_for_type("MyType"));
  repository.clear();
}

TEST(ComparatorsAndCopiersRepository,
    ComparatorAndCopierByTheSameNameShouldBothBeFound)
{
  MyComparator comparator;
  MyCopier copier;
  cppmu::MockNamedValueComparatorsAndCopiersRepository repository;
  repository.install_copier("MyType", copier);
  repository.install_comparator("MyType", comparator);
  POINTERS_EQUAL(&comparator, repository.get_comparator_for_type("MyType"));
  POINTERS_EQUAL(&copier, repository.get_copier_for_type("MyType"));
  repository.clear();
}

TEST(ComparatorsAndCopiersRepository,
    InstallComparatorsAndCopiersFromRepository)
{
  MyComparator comparator;
  MyCopier copier;
  cppmu::MockNamedValueComparatorsAndCopiersRepository source;
  cppmu::MockNamedValueComparatorsAndCopiersRepository target;

  source.install_copier("MyType", copier);
  source.install_comparator("MyType", comparator);

  target.install_comparators_and_copiers(source);

  POINTERS_EQUAL(&comparator, target.get_comparator_for_type("MyType"));
  POINTERS_EQUAL(&copier, target.get_copier_for_type("MyType"));

  source.clear();
  target.clear();
}

TEST_GROUP(MockNamedValue)
{
  cppmu::MockNamedValue* value;
  void setup() override { value = new cppmu::MockNamedValue("param"); }

  void teardown() override { delete value; }
};

TEST(MockNamedValue, DefaultToleranceUsedWhenNoToleranceGiven)
{
  value->set_value(0.2);
  DOUBLES_EQUAL(cppmu::MockNamedValue::default_double_tolerance,
      value->get_double_tolerance(),
      0.0);
}

TEST(MockNamedValue, GivenToleranceUsed)
{
  value->set_value(0.2, 3.2);
  STRCMP_EQUAL("double", value->get_type().c_str());
  DOUBLES_EQUAL(0.2, value->get_double_value(), 0.0);
  DOUBLES_EQUAL(3.2, value->get_double_tolerance(), 0.0);
}

TEST(MockNamedValue, DoublesEqualIfWithinTolerance)
{
  value->set_value(5.0, 0.4);
  cppmu::MockNamedValue other("param2");
  other.set_value(5.3);

  CHECK_TRUE(value->equals(other));
}

TEST(MockNamedValue, DoublesNotEqualIfOutsideTolerance)
{
  value->set_value(5.0, 0.4);
  cppmu::MockNamedValue other("param2");
  other.set_value(5.5);

  CHECK_FALSE(value->equals(other));
}
