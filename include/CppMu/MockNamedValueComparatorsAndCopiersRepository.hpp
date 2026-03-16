#ifndef INCLUDED_CPPMU_MOCKNAMEDVALUECOMPARATORSANDCOPIERSREPOSITORY_HPP
#define INCLUDED_CPPMU_MOCKNAMEDVALUECOMPARATORSANDCOPIERSREPOSITORY_HPP

#include "CppMu/MockNamedValueComparator.hpp"
#include "CppMu/MockNamedValueCopier.hpp"

namespace cppmu {

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
  MockNamedValueComparatorsAndCopiersRepository();
  virtual ~MockNamedValueComparatorsAndCopiersRepository();

  virtual void install_comparator(const cppmu::String& name,
      MockNamedValueComparator& comparator);
  virtual void install_copier(const cppmu::String& name,
      MockNamedValueCopier& copier);
  virtual void install_comparators_and_copiers(
      const MockNamedValueComparatorsAndCopiersRepository& repository);
  virtual MockNamedValueComparator* get_comparator_for_type(
      const cppmu::String& name);
  virtual MockNamedValueCopier* get_copier_for_type(const cppmu::String& name);

  void clear();
};

} // namespace cppmu

#endif
