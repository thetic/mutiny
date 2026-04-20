#include "MockFailureReporter.hpp"

#include "mu/tiny/mock/NamedValueComparatorsAndCopiersRepository.hpp"

#include "mu/tiny/test.hpp"

class TypeForTestingExpectedFunctionCall
{
public:
  TypeForTestingExpectedFunctionCall(int val) { value = new int(val); }
  virtual ~TypeForTestingExpectedFunctionCall() { delete value; }
  int* value;
};

class TypeForTestingExpectedFunctionCallComparator
  : public mu::tiny::mock::NamedValueComparator
{
public:
  bool is_equal(const void* object1, const void* object2) override
  {
    const auto* obj1 =
        static_cast<const TypeForTestingExpectedFunctionCall*>(object1);
    const auto* obj2 =
        static_cast<const TypeForTestingExpectedFunctionCall*>(object2);
    return *(obj1->value) == *(obj2->value);
  }
  mu::tiny::String value_to_string(const void* object) override
  {
    const auto* obj =
        static_cast<const TypeForTestingExpectedFunctionCall*>(object);
    return mu::tiny::string_from(*(obj->value));
  }
};

class TypeForTestingExpectedFunctionCallCopier
  : public mu::tiny::mock::NamedValueCopier
{
public:
  void copy(void* dst, const void* src) override
  {
    auto* typed_dst = static_cast<TypeForTestingExpectedFunctionCall*>(dst);
    const auto* typed_src =
        static_cast<const TypeForTestingExpectedFunctionCall*>(src);
    *(typed_dst->value) = *(typed_src->value);
  }
};

class MyComparator : public mu::tiny::mock::NamedValueComparator
{
public:
  MyComparator() = default;
  ~MyComparator() override = default;

  bool is_equal(const void* /*object1*/, const void* /*object2*/) override
  {
    return false;
  }
  mu::tiny::String value_to_string(const void* /*object*/) override
  {
    return "";
  }
};

class MyCopier : public mu::tiny::mock::NamedValueCopier
{
public:
  MyCopier() = default;
  ~MyCopier() override = default;

  void copy(void* /*out*/, const void* /*in*/) override {}
};

TEST_GROUP(NamedValueComparatorsAndCopiersRepository)
{
  void teardown() override
  {
    CHECK_NO_MOCK_FAILURE();
    FailureReporterForTest::clear_reporter();
  }
};

TEST(NamedValueComparatorsAndCopiersRepository, getComparatorForNonExistingName)
{
  mu::tiny::mock::NamedValueComparatorsAndCopiersRepository repository;
  CHECK_EQUAL(nullptr, repository.get_comparator_for_type("typeName"));
}

TEST(NamedValueComparatorsAndCopiersRepository, installComparator)
{
  TypeForTestingExpectedFunctionCallComparator comparator;
  mu::tiny::mock::NamedValueComparatorsAndCopiersRepository repository;
  repository.install_comparator("typeName", comparator);
  CHECK_EQUAL(&comparator, repository.get_comparator_for_type("typeName"));
}

TEST(NamedValueComparatorsAndCopiersRepository, installMultipleComparators)
{
  mu::tiny::mock::NamedValueComparatorsAndCopiersRepository repository;

  TypeForTestingExpectedFunctionCallComparator comparator1;
  repository.install_comparator("type1", comparator1);

  TypeForTestingExpectedFunctionCallComparator comparator2;
  repository.install_comparator("type2", comparator2);

  TypeForTestingExpectedFunctionCallComparator comparator3;
  repository.install_comparator("type3", comparator3);

  CHECK_EQUAL(&comparator3, repository.get_comparator_for_type("type3"));
  CHECK_EQUAL(&comparator2, repository.get_comparator_for_type("type2"));
  CHECK_EQUAL(&comparator1, repository.get_comparator_for_type("type1"));
}

TEST(NamedValueComparatorsAndCopiersRepository, getCopierForNonExistingName)
{
  mu::tiny::mock::NamedValueComparatorsAndCopiersRepository repository;
  CHECK_EQUAL(nullptr, repository.get_copier_for_type("typeName"));
}

TEST(NamedValueComparatorsAndCopiersRepository, installCopier)
{
  TypeForTestingExpectedFunctionCallCopier copier;
  mu::tiny::mock::NamedValueComparatorsAndCopiersRepository repository;
  repository.install_copier("typeName", copier);
  CHECK_EQUAL(&copier, repository.get_copier_for_type("typeName"));
}

TEST(NamedValueComparatorsAndCopiersRepository, installMultipleCopiers)
{
  mu::tiny::mock::NamedValueComparatorsAndCopiersRepository repository;

  TypeForTestingExpectedFunctionCallCopier copier1;
  repository.install_copier("type1", copier1);

  TypeForTestingExpectedFunctionCallCopier copier2;
  repository.install_copier("type2", copier2);

  TypeForTestingExpectedFunctionCallCopier copier3;
  repository.install_copier("type3", copier3);

  CHECK_EQUAL(&copier3, repository.get_copier_for_type("type3"));
  CHECK_EQUAL(&copier2, repository.get_copier_for_type("type2"));
  CHECK_EQUAL(&copier1, repository.get_copier_for_type("type1"));
}

TEST(NamedValueComparatorsAndCopiersRepository, installMultipleHandlers)
{
  mu::tiny::mock::NamedValueComparatorsAndCopiersRepository repository;

  TypeForTestingExpectedFunctionCallCopier copier1;
  TypeForTestingExpectedFunctionCallComparator comparator1;
  repository.install_copier("type1", copier1);
  repository.install_comparator("type1", comparator1);

  TypeForTestingExpectedFunctionCallCopier copier2;
  TypeForTestingExpectedFunctionCallComparator comparator2;
  repository.install_copier("type2", copier2);
  repository.install_comparator("type2", comparator2);

  TypeForTestingExpectedFunctionCallCopier copier3;
  TypeForTestingExpectedFunctionCallComparator comparator3;
  repository.install_copier("type3", copier3);
  repository.install_comparator("type3", comparator3);

  CHECK_EQUAL(&comparator3, repository.get_comparator_for_type("type3"));
  CHECK_EQUAL(&comparator2, repository.get_comparator_for_type("type2"));
  CHECK_EQUAL(&comparator1, repository.get_comparator_for_type("type1"));
  CHECK_EQUAL(&copier3, repository.get_copier_for_type("type3"));
  CHECK_EQUAL(&copier2, repository.get_copier_for_type("type2"));
  CHECK_EQUAL(&copier1, repository.get_copier_for_type("type1"));
}

TEST(NamedValueComparatorsAndCopiersRepository, InstallCopierAndRetrieveIt)
{
  MyCopier copier;
  mu::tiny::mock::NamedValueComparatorsAndCopiersRepository repository;
  repository.install_copier("MyType", copier);
  CHECK_EQUAL(&copier, repository.get_copier_for_type("MyType"));
  repository.clear();
}

TEST(NamedValueComparatorsAndCopiersRepository, ComparatorAndCopierByTheSameNameShouldBothBeFound)
{
  MyComparator comparator;
  MyCopier copier;
  mu::tiny::mock::NamedValueComparatorsAndCopiersRepository repository;
  repository.install_copier("MyType", copier);
  repository.install_comparator("MyType", comparator);
  CHECK_EQUAL(&comparator, repository.get_comparator_for_type("MyType"));
  CHECK_EQUAL(&copier, repository.get_copier_for_type("MyType"));
  repository.clear();
}

TEST(NamedValueComparatorsAndCopiersRepository, InstallComparatorsAndCopiersFromRepository)
{
  MyComparator comparator;
  MyCopier copier;
  mu::tiny::mock::NamedValueComparatorsAndCopiersRepository source;
  mu::tiny::mock::NamedValueComparatorsAndCopiersRepository target;

  source.install_copier("MyType", copier);
  source.install_comparator("MyType", comparator);

  target.install_comparators_and_copiers(source);

  CHECK_EQUAL(&comparator, target.get_comparator_for_type("MyType"));
  CHECK_EQUAL(&copier, target.get_copier_for_type("MyType"));

  source.clear();
  target.clear();
}
