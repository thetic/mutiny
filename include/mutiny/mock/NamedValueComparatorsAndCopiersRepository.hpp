#ifndef INCLUDED_MUTINY_MOCK_NAMEDVALUECOMPARATORSANDCOPIERSREPOSITORY_HPP
#define INCLUDED_MUTINY_MOCK_NAMEDVALUECOMPARATORSANDCOPIERSREPOSITORY_HPP

#include "mutiny/mock/NamedValueComparator.hpp"
#include "mutiny/mock/NamedValueCopier.hpp"

namespace mu {
namespace tiny {
namespace mock {

/*
 * MockParameterComparatorRepository is a class which stores comparators and
 * copiers which can be used for comparing non-native types
 *
 */

struct NamedValueComparatorsAndCopiersRepositoryNode;
class NamedValueComparatorsAndCopiersRepository
{
  NamedValueComparatorsAndCopiersRepositoryNode* head_{ nullptr };

public:
  NamedValueComparatorsAndCopiersRepository() = default;
  virtual ~NamedValueComparatorsAndCopiersRepository();

  virtual void install_comparator(
      const String& name,
      NamedValueComparator& comparator
  );
  virtual void install_copier(const String& name, NamedValueCopier& copier);
  virtual void install_comparators_and_copiers(
      const NamedValueComparatorsAndCopiersRepository& repository
  );
  virtual NamedValueComparator* get_comparator_for_type(const String& name);
  virtual NamedValueCopier* get_copier_for_type(const String& name);

  void clear();
};

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
