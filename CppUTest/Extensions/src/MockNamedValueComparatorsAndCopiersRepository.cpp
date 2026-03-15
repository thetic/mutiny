#include "CppUTest/Extensions/MockNamedValueComparatorsAndCopiersRepository.hpp"

namespace cpputest {
namespace extensions {

struct MockNamedValueComparatorsAndCopiersRepositoryNode
{
  MockNamedValueComparatorsAndCopiersRepositoryNode(const String& name,
      MockNamedValueComparator* comparator,
      MockNamedValueComparatorsAndCopiersRepositoryNode* next)
    : name(name)
    , comparator(comparator)
    , copier(nullptr)
    , next(next)
  {
  }
  MockNamedValueComparatorsAndCopiersRepositoryNode(const String& name,
      MockNamedValueCopier* copier,
      MockNamedValueComparatorsAndCopiersRepositoryNode* next)
    : name(name)
    , comparator(nullptr)
    , copier(copier)
    , next(next)
  {
  }
  MockNamedValueComparatorsAndCopiersRepositoryNode(const String& name,
      MockNamedValueComparator* comparator,
      MockNamedValueCopier* copier,
      MockNamedValueComparatorsAndCopiersRepositoryNode* next)
    : name(name)
    , comparator(comparator)
    , copier(copier)
    , next(next)
  {
  }
  String name;
  MockNamedValueComparator* comparator;
  MockNamedValueCopier* copier;
  MockNamedValueComparatorsAndCopiersRepositoryNode* next;
};

MockNamedValueComparatorsAndCopiersRepository::
    MockNamedValueComparatorsAndCopiersRepository()
  : head_(nullptr)
{
}

MockNamedValueComparatorsAndCopiersRepository::
    ~MockNamedValueComparatorsAndCopiersRepository()
{
  clear();
}

void
MockNamedValueComparatorsAndCopiersRepository::clear()
{
  while (head_) {
    MockNamedValueComparatorsAndCopiersRepositoryNode* next = head_->next;
    delete head_;
    head_ = next;
  }
}

void
MockNamedValueComparatorsAndCopiersRepository::install_comparator(
    const String& name,
    MockNamedValueComparator& comparator)
{
  head_ = new MockNamedValueComparatorsAndCopiersRepositoryNode(
      name, &comparator, head_);
}

void
MockNamedValueComparatorsAndCopiersRepository::install_copier(
    const String& name,
    MockNamedValueCopier& copier)
{
  head_ = new MockNamedValueComparatorsAndCopiersRepositoryNode(
      name, &copier, head_);
}

MockNamedValueComparator*
MockNamedValueComparatorsAndCopiersRepository::get_comparator_for_type(
    const String& name)
{
  for (MockNamedValueComparatorsAndCopiersRepositoryNode* p = head_; p;
      p = p->next)
    if (p->name == name && p->comparator)
      return p->comparator;
  return nullptr;
}

MockNamedValueCopier*
MockNamedValueComparatorsAndCopiersRepository::get_copier_for_type(
    const String& name)
{
  for (MockNamedValueComparatorsAndCopiersRepositoryNode* p = head_; p;
      p = p->next)
    if (p->name == name && p->copier)
      return p->copier;
  return nullptr;
}

void
MockNamedValueComparatorsAndCopiersRepository::install_comparators_and_copiers(
    const MockNamedValueComparatorsAndCopiersRepository& repository)
{
  for (MockNamedValueComparatorsAndCopiersRepositoryNode* p = repository.head_;
      p;
      p = p->next)
    head_ = new MockNamedValueComparatorsAndCopiersRepositoryNode(
        p->name, p->comparator, p->copier, head_);
}

} // namespace extensions
} // namespace cpputest
