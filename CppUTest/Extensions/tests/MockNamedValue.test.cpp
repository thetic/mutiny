#include "CppUTest/Extensions/MockNamedValue.hpp"

#include "CppUTest/Extensions/MockNamedValueComparatorsAndCopiersRepository.hpp"

#include "CppUTest/TestHarness.hpp"

TEST_GROUP(ComparatorsAndCopiersRepository)
{};

class MyComparator : public cpputest::extensions::MockNamedValueComparator
{
public:
  MyComparator() {}
  virtual ~MyComparator() override {}

  virtual bool isEqual(const void*, const void*) override { return false; }
  virtual cpputest::String valueToString(const void*) override { return ""; }
};

class MyCopier : public cpputest::extensions::MockNamedValueCopier
{
public:
  MyCopier() {}
  virtual ~MyCopier() override {}

  virtual void copy(void*, const void*) override {}
};

TEST(ComparatorsAndCopiersRepository, InstallCopierAndRetrieveIt)
{
  MyCopier copier;
  cpputest::extensions::MockNamedValueComparatorsAndCopiersRepository
      repository;
  repository.installCopier("MyType", copier);
  POINTERS_EQUAL(&copier, repository.getCopierForType("MyType"));
  repository.clear();
}

TEST(ComparatorsAndCopiersRepository,
    ComparatorAndCopierByTheSameNameShouldBothBeFound)
{
  MyComparator comparator;
  MyCopier copier;
  cpputest::extensions::MockNamedValueComparatorsAndCopiersRepository
      repository;
  repository.installCopier("MyType", copier);
  repository.installComparator("MyType", comparator);
  POINTERS_EQUAL(&comparator, repository.getComparatorForType("MyType"));
  POINTERS_EQUAL(&copier, repository.getCopierForType("MyType"));
  repository.clear();
}

TEST(ComparatorsAndCopiersRepository,
    InstallComparatorsAndCopiersFromRepository)
{
  MyComparator comparator;
  MyCopier copier;
  cpputest::extensions::MockNamedValueComparatorsAndCopiersRepository source;
  cpputest::extensions::MockNamedValueComparatorsAndCopiersRepository target;

  source.installCopier("MyType", copier);
  source.installComparator("MyType", comparator);

  target.installComparatorsAndCopiers(source);

  POINTERS_EQUAL(&comparator, target.getComparatorForType("MyType"));
  POINTERS_EQUAL(&copier, target.getCopierForType("MyType"));

  source.clear();
  target.clear();
}

TEST_GROUP(MockNamedValue)
{
  cpputest::extensions::MockNamedValue* value;
  void setup() override
  {
    value = new cpputest::extensions::MockNamedValue("param");
  }

  void teardown() override { delete value; }
};

TEST(MockNamedValue, DefaultToleranceUsedWhenNoToleranceGiven)
{
  value->setValue(0.2);
  DOUBLES_EQUAL(cpputest::extensions::MockNamedValue::defaultDoubleTolerance,
      value->getDoubleTolerance(),
      0.0);
}

TEST(MockNamedValue, GivenToleranceUsed)
{
  value->setValue(0.2, 3.2);
  STRCMP_EQUAL("double", value->getType().c_str());
  DOUBLES_EQUAL(0.2, value->getDoubleValue(), 0.0);
  DOUBLES_EQUAL(3.2, value->getDoubleTolerance(), 0.0);
}

TEST(MockNamedValue, DoublesEqualIfWithinTolerance)
{
  value->setValue(5.0, 0.4);
  cpputest::extensions::MockNamedValue other("param2");
  other.setValue(5.3);

  CHECK_TRUE(value->equals(other));
}

TEST(MockNamedValue, DoublesNotEqualIfOutsideTolerance)
{
  value->setValue(5.0, 0.4);
  cpputest::extensions::MockNamedValue other("param2");
  other.setValue(5.5);

  CHECK_FALSE(value->equals(other));
}
