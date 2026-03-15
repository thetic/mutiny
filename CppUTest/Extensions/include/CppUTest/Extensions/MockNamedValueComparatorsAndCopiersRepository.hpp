#ifndef INCLUDED_CPPUTEST_EXTENSIONS_MOCKNAMEDVALUECOMPARATORSANDCOPIERSREPOSITORY_HPP
#define INCLUDED_CPPUTEST_EXTENSIONS_MOCKNAMEDVALUECOMPARATORSANDCOPIERSREPOSITORY_HPP

#include "CppUTest/Extensions/MockNamedValueComparator.hpp"
#include "CppUTest/Extensions/MockNamedValueCopier.hpp"

namespace cpputest {
namespace extensions {

/*
 * MockParameterComparatorRepository is a class which stores comparators and
 * copiers which can be used for comparing non-native types
 *
 */

struct MockNamedValueComparatorsAndCopiersRepositoryNode;
class MockNamedValueComparatorsAndCopiersRepository
{
  MockNamedValueComparatorsAndCopiersRepositoryNode* head_;

public:
  MockNamedValueComparatorsAndCopiersRepository();
  virtual ~MockNamedValueComparatorsAndCopiersRepository();

  virtual void installComparator(const cpputest::String& name,
      MockNamedValueComparator& comparator);
  virtual void installCopier(const cpputest::String& name,
      MockNamedValueCopier& copier);
  virtual void installComparatorsAndCopiers(
      const MockNamedValueComparatorsAndCopiersRepository& repository);
  virtual MockNamedValueComparator* getComparatorForType(
      const cpputest::String& name);
  virtual MockNamedValueCopier* getCopierForType(const cpputest::String& name);

  void clear();
};

} // namespace extensions
} // namespace cpputest

#endif
