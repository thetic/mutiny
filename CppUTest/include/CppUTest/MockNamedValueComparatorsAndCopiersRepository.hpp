#ifndef INCLUDED_CPPUTEST_MOCKNAMEDVALUECOMPARATORSANDCOPIERSREPOSITORY_HPP
#define INCLUDED_CPPUTEST_MOCKNAMEDVALUECOMPARATORSANDCOPIERSREPOSITORY_HPP

#include "CppUTest/MockNamedValueComparator.hpp"
#include "CppUTest/MockNamedValueCopier.hpp"

namespace cpputest {

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

  virtual void install_comparator(const cpputest::String& name,
      MockNamedValueComparator& comparator);
  virtual void install_copier(const cpputest::String& name,
      MockNamedValueCopier& copier);
  virtual void install_comparators_and_copiers(
      const MockNamedValueComparatorsAndCopiersRepository& repository);
  virtual MockNamedValueComparator* get_comparator_for_type(
      const cpputest::String& name);
  virtual MockNamedValueCopier* get_copier_for_type(
      const cpputest::String& name);

  void clear();
};

} // namespace cpputest

#endif
