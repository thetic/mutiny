#include "CppMu/MockNamedValueComparatorsAndCopiersRepository.hpp"

#include "MockFailureReporterForTest.hpp"

#include "CppMu/CppMu.hpp"

class TypeForTestingExpectedFunctionCall
{
public:
  TypeForTestingExpectedFunctionCall(int val) { value = new int(val); }
  virtual ~TypeForTestingExpectedFunctionCall() { delete value; }
  int* value;
};

class TypeForTestingExpectedFunctionCallComparator
  : public cppmu::MockNamedValueComparator
{
public:
  bool is_equal(const void* object1, const void* object2) override
  {
    auto* obj1 =
        static_cast<const TypeForTestingExpectedFunctionCall*>(object1);
    auto* obj2 =
        static_cast<const TypeForTestingExpectedFunctionCall*>(object2);
    return *(obj1->value) == *(obj2->value);
  }
  cppmu::String value_to_string(const void* object) override
  {
    auto* obj = static_cast<const TypeForTestingExpectedFunctionCall*>(object);
    return cppmu::string_from(*(obj->value));
  }
};

class TypeForTestingExpectedFunctionCallCopier
  : public cppmu::MockNamedValueCopier
{
public:
  void copy(void* dst, const void* src) override
  {
    auto* typed_dst = static_cast<TypeForTestingExpectedFunctionCall*>(dst);
    auto* typed_src =
        static_cast<const TypeForTestingExpectedFunctionCall*>(src);
    *(typed_dst->value) = *(typed_src->value);
  }
};

class MyComparator : public cppmu::MockNamedValueComparator
{
public:
  MyComparator() = default;
  ~MyComparator() override = default;

  bool is_equal(const void*, const void*) override { return false; }
  cppmu::String value_to_string(const void*) override { return ""; }
};

class MyCopier : public cppmu::MockNamedValueCopier
{
public:
  MyCopier() = default;
  ~MyCopier() override = default;

  void copy(void*, const void*) override {}
};

TEST_GROUP(MockNamedValueComparatorsAndCopiersRepository)
{
  void teardown() override
  {
    CHECK_NO_MOCK_FAILURE();
    MockFailureReporterForTest::clear_reporter();
  }
};

TEST(MockNamedValueComparatorsAndCopiersRepository, getComparatorForNonExistingName)
{
  cppmu::MockNamedValueComparatorsAndCopiersRepository repository;
  POINTERS_EQUAL(nullptr, repository.get_comparator_for_type("typeName"));
}

TEST(MockNamedValueComparatorsAndCopiersRepository, installComparator)
{
  TypeForTestingExpectedFunctionCallComparator comparator;
  cppmu::MockNamedValueComparatorsAndCopiersRepository repository;
  repository.install_comparator("typeName", comparator);
  POINTERS_EQUAL(&comparator, repository.get_comparator_for_type("typeName"));
}

TEST(MockNamedValueComparatorsAndCopiersRepository, installMultipleComparators)
{
  TypeForTestingExpectedFunctionCallComparator comparator1, comparator2,
      comparator3;
  cppmu::MockNamedValueComparatorsAndCopiersRepository repository;
  repository.install_comparator("type1", comparator1);
  repository.install_comparator("type2", comparator2);
  repository.install_comparator("type3", comparator3);
  POINTERS_EQUAL(&comparator3, repository.get_comparator_for_type("type3"));
  POINTERS_EQUAL(&comparator2, repository.get_comparator_for_type("type2"));
  POINTERS_EQUAL(&comparator1, repository.get_comparator_for_type("type1"));
}

TEST(MockNamedValueComparatorsAndCopiersRepository, getCopierForNonExistingName)
{
  cppmu::MockNamedValueComparatorsAndCopiersRepository repository;
  POINTERS_EQUAL(nullptr, repository.get_copier_for_type("typeName"));
}

TEST(MockNamedValueComparatorsAndCopiersRepository, installCopier)
{
  TypeForTestingExpectedFunctionCallCopier copier;
  cppmu::MockNamedValueComparatorsAndCopiersRepository repository;
  repository.install_copier("typeName", copier);
  POINTERS_EQUAL(&copier, repository.get_copier_for_type("typeName"));
}

TEST(MockNamedValueComparatorsAndCopiersRepository, installMultipleCopiers)
{
  TypeForTestingExpectedFunctionCallCopier copier1, copier2, copier3;
  cppmu::MockNamedValueComparatorsAndCopiersRepository repository;
  repository.install_copier("type1", copier1);
  repository.install_copier("type2", copier2);
  repository.install_copier("type3", copier3);
  POINTERS_EQUAL(&copier3, repository.get_copier_for_type("type3"));
  POINTERS_EQUAL(&copier2, repository.get_copier_for_type("type2"));
  POINTERS_EQUAL(&copier1, repository.get_copier_for_type("type1"));
}

TEST(MockNamedValueComparatorsAndCopiersRepository, installMultipleHandlers)
{
  TypeForTestingExpectedFunctionCallCopier copier1, copier2, copier3;
  TypeForTestingExpectedFunctionCallComparator comparator1, comparator2,
      comparator3;
  cppmu::MockNamedValueComparatorsAndCopiersRepository repository;
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

TEST(MockNamedValueComparatorsAndCopiersRepository, InstallCopierAndRetrieveIt)
{
  MyCopier copier;
  cppmu::MockNamedValueComparatorsAndCopiersRepository repository;
  repository.install_copier("MyType", copier);
  POINTERS_EQUAL(&copier, repository.get_copier_for_type("MyType"));
  repository.clear();
}

TEST(MockNamedValueComparatorsAndCopiersRepository, ComparatorAndCopierByTheSameNameShouldBothBeFound)
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

TEST(MockNamedValueComparatorsAndCopiersRepository, InstallComparatorsAndCopiersFromRepository)
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
