#include "CppUTestExt/MockNamedValueComparatorsAndCopiersRepository.hpp"

namespace cpputest {
namespace extensions {

struct MockNamedValueComparatorsAndCopiersRepositoryNode
{
  MockNamedValueComparatorsAndCopiersRepositoryNode(const String& name,
      MockNamedValueComparator* comparator,
      MockNamedValueComparatorsAndCopiersRepositoryNode* next)
    : name_(name)
    , comparator_(comparator)
    , copier_(nullptr)
    , next_(next)
  {
  }
  MockNamedValueComparatorsAndCopiersRepositoryNode(const String& name,
      MockNamedValueCopier* copier,
      MockNamedValueComparatorsAndCopiersRepositoryNode* next)
    : name_(name)
    , comparator_(nullptr)
    , copier_(copier)
    , next_(next)
  {
  }
  MockNamedValueComparatorsAndCopiersRepositoryNode(const String& name,
      MockNamedValueComparator* comparator,
      MockNamedValueCopier* copier,
      MockNamedValueComparatorsAndCopiersRepositoryNode* next)
    : name_(name)
    , comparator_(comparator)
    , copier_(copier)
    , next_(next)
  {
  }
  String name_;
  MockNamedValueComparator* comparator_;
  MockNamedValueCopier* copier_;
  MockNamedValueComparatorsAndCopiersRepositoryNode* next_;
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
    MockNamedValueComparatorsAndCopiersRepositoryNode* next = head_->next_;
    delete head_;
    head_ = next;
  }
}

void
MockNamedValueComparatorsAndCopiersRepository::installComparator(
    const String& name,
    MockNamedValueComparator& comparator)
{
  head_ = new MockNamedValueComparatorsAndCopiersRepositoryNode(
      name, &comparator, head_);
}

void
MockNamedValueComparatorsAndCopiersRepository::installCopier(const String& name,
    MockNamedValueCopier& copier)
{
  head_ = new MockNamedValueComparatorsAndCopiersRepositoryNode(
      name, &copier, head_);
}

MockNamedValueComparator*
MockNamedValueComparatorsAndCopiersRepository::getComparatorForType(
    const String& name)
{
  for (MockNamedValueComparatorsAndCopiersRepositoryNode* p = head_; p;
      p = p->next_)
    if (p->name_ == name && p->comparator_)
      return p->comparator_;
  return nullptr;
}

MockNamedValueCopier*
MockNamedValueComparatorsAndCopiersRepository::getCopierForType(
    const String& name)
{
  for (MockNamedValueComparatorsAndCopiersRepositoryNode* p = head_; p;
      p = p->next_)
    if (p->name_ == name && p->copier_)
      return p->copier_;
  return nullptr;
}

void
MockNamedValueComparatorsAndCopiersRepository::installComparatorsAndCopiers(
    const MockNamedValueComparatorsAndCopiersRepository& repository)
{
  for (MockNamedValueComparatorsAndCopiersRepositoryNode* p = repository.head_;
      p;
      p = p->next_)
    head_ = new MockNamedValueComparatorsAndCopiersRepositoryNode(
        p->name_, p->comparator_, p->copier_, head_);
}

} // namespace extensions
} // namespace cpputest
