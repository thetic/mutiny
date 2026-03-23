#ifndef INCLUDED_MUTINY_MOCKNAMEDVALUECOMPARATORSANDCOPIERSREPOSITORY_HPP
#define INCLUDED_MUTINY_MOCKNAMEDVALUECOMPARATORSANDCOPIERSREPOSITORY_HPP

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

struct MockNamedValueComparatorsAndCopiersRepositoryNode;
class MockNamedValueComparatorsAndCopiersRepository
{
  MockNamedValueComparatorsAndCopiersRepositoryNode* head_{ nullptr };

public:
  MockNamedValueComparatorsAndCopiersRepository() = default;
  virtual ~MockNamedValueComparatorsAndCopiersRepository();

  virtual void install_comparator(
      const mu::tiny::test::String& name,
      MockNamedValueComparator& comparator
  );
  virtual void install_copier(
      const mu::tiny::test::String& name,
      MockNamedValueCopier& copier
  );
  virtual void install_comparators_and_copiers(
      const MockNamedValueComparatorsAndCopiersRepository& repository
  );
  virtual MockNamedValueComparator* get_comparator_for_type(
      const mu::tiny::test::String& name
  );
  virtual MockNamedValueCopier* get_copier_for_type(
      const mu::tiny::test::String& name
  );

  void clear();
};

}
}
} // namespace mu::tiny::mock

#endif
