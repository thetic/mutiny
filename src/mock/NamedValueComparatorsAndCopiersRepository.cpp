#include "mu/tiny/mock/NamedValueComparatorsAndCopiersRepository.hpp"

namespace mu {
namespace tiny {
namespace mock {

class NamedValueComparatorsAndCopiersRepositoryNode
{
public:
  NamedValueComparatorsAndCopiersRepositoryNode(
      StringView n,
      NamedValueComparator* cmp,
      NamedValueComparatorsAndCopiersRepositoryNode* nx
  )
    : name(n.data(), n.size())
    , comparator(cmp)
    , copier(nullptr)
    , next(nx)
  {
  }
  NamedValueComparatorsAndCopiersRepositoryNode(
      StringView n,
      NamedValueCopier* cp,
      NamedValueComparatorsAndCopiersRepositoryNode* nx
  )
    : name(n.data(), n.size())
    , comparator(nullptr)
    , copier(cp)
    , next(nx)
  {
  }
  NamedValueComparatorsAndCopiersRepositoryNode(
      const String& n,
      NamedValueComparator* cmp,
      NamedValueCopier* cp,
      NamedValueComparatorsAndCopiersRepositoryNode* nx
  )
    : name(n)
    , comparator(cmp)
    , copier(cp)
    , next(nx)
  {
  }
  String name;
  NamedValueComparator* comparator;
  NamedValueCopier* copier;
  NamedValueComparatorsAndCopiersRepositoryNode* next;
};

NamedValueComparatorsAndCopiersRepository::
    ~NamedValueComparatorsAndCopiersRepository()
{
  clear();
}

void NamedValueComparatorsAndCopiersRepository::clear()
{
  while (head_) {
    NamedValueComparatorsAndCopiersRepositoryNode* next = head_->next;
    delete head_;
    head_ = next;
  }
}

void NamedValueComparatorsAndCopiersRepository::install_comparator(
    StringView name,
    NamedValueComparator& comparator
)
{
  head_ = new NamedValueComparatorsAndCopiersRepositoryNode(
      name, &comparator, head_
  );
}

void NamedValueComparatorsAndCopiersRepository::install_copier(
    StringView name,
    NamedValueCopier& copier
)
{
  head_ =
      new NamedValueComparatorsAndCopiersRepositoryNode(name, &copier, head_);
}

NamedValueComparator* NamedValueComparatorsAndCopiersRepository::
    get_comparator_for_type(StringView name)
{
  for (NamedValueComparatorsAndCopiersRepositoryNode* p = head_; p; p = p->next)
    if (p->name == name && p->comparator)
      return p->comparator;
  return nullptr;
}

NamedValueCopier* NamedValueComparatorsAndCopiersRepository::
    get_copier_for_type(StringView name)
{
  for (NamedValueComparatorsAndCopiersRepositoryNode* p = head_; p; p = p->next)
    if (p->name == name && p->copier)
      return p->copier;
  return nullptr;
}

void NamedValueComparatorsAndCopiersRepository::install_comparators_and_copiers(
    const NamedValueComparatorsAndCopiersRepository& repository
)
{
  for (NamedValueComparatorsAndCopiersRepositoryNode* p = repository.head_; p;
       p = p->next)
    head_ = new NamedValueComparatorsAndCopiersRepositoryNode(
        p->name, p->comparator, p->copier, head_
    );
}

} // namespace mock
} // namespace tiny
} // namespace mu
