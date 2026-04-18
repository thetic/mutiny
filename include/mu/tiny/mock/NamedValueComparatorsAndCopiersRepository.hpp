/**
 * @file
 * @brief Repository of custom type comparators and copiers for mock parameter
 * matching.
 *
 * Stores @ref mu::tiny::mock::NamedValueComparator and @ref
 * mu::tiny::mock::NamedValueCopier instances keyed by type name. Populated via
 * @ref mu::tiny::mock::Support::install_comparator() and @ref
 * mu::tiny::mock::Support::install_copier().
 */

#ifndef INCLUDED_MU_TINY_MOCK_NAMEDVALUECOMPARATORSANDCOPIERSREPOSITORY_HPP
#define INCLUDED_MU_TINY_MOCK_NAMEDVALUECOMPARATORSANDCOPIERSREPOSITORY_HPP

#include "mu/tiny/mock/NamedValueComparator.hpp"
#include "mu/tiny/mock/NamedValueCopier.hpp"

#include "mu/tiny/export.h"

namespace mu {
namespace tiny {
namespace mock {

class NamedValueComparatorsAndCopiersRepositoryNode;

/**
 * @brief Stores @ref NamedValueComparator and @ref NamedValueCopier instances
 * keyed by type name.
 *
 * Install comparators and copiers via @ref install_comparator() /
 * @ref install_copier() before running tests that pass custom types as mock
 * parameters. @ref Support delegates to this repository when comparing or
 * copying parameter values of custom types.
 */
class MUTINY_EXPORT NamedValueComparatorsAndCopiersRepository
{
  NamedValueComparatorsAndCopiersRepositoryNode* head_{ nullptr };

public:
  NamedValueComparatorsAndCopiersRepository() = default;
  virtual ~NamedValueComparatorsAndCopiersRepository();

  /**
   * @brief Register a comparator for a custom type.
   *
   * @param name        Type name used as the lookup key.
   * @param comparator  Comparator to register.
   */
  virtual void install_comparator(
      const String& name,
      NamedValueComparator& comparator
  );

  /**
   * @brief Register a copier for a custom type.
   *
   * @param name    Type name used as the lookup key.
   * @param copier  Copier to register.
   */
  virtual void install_copier(const String& name, NamedValueCopier& copier);

  /**
   * @brief Copy all comparators and copiers from @p repository into this one.
   *
   * @param repository  Source repository to merge from.
   */
  virtual void install_comparators_and_copiers(
      const NamedValueComparatorsAndCopiersRepository& repository
  );

  /**
   * @brief Look up the comparator registered for a type.
   *
   * @param name  Type name to look up.
   * @return Pointer to the registered NamedValueComparator, or nullptr.
   */
  virtual NamedValueComparator* get_comparator_for_type(const String& name);

  /**
   * @brief Look up the copier registered for a type.
   *
   * @param name  Type name to look up.
   * @return Pointer to the registered NamedValueCopier, or nullptr.
   */
  virtual NamedValueCopier* get_copier_for_type(const String& name);

  /** @brief Remove all installed comparators and copiers. */
  void clear();
};

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
