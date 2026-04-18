/**
 * @file
 * @brief Interface and helpers for copying mock output parameters of custom
 * types.
 *
 * When a mock expectation is set with @ref
 * mu::tiny::mock::ExpectedCall::with_output_parameter_of_type_returning(), the
 * framework needs to copy the configured value into the caller's output
 * buffer. Implement @ref mu::tiny::mock::NamedValueCopier for your type and
 * register it with @ref mu::tiny::mock::Support::install_copier().
 *
 * See also @ref mu::tiny::mock::NamedValueComparator for expected-parameter
 * matching.
 */

#ifndef INCLUDED_MU_TINY_MOCK_NAMEDVALUECOPIER_HPP
#define INCLUDED_MU_TINY_MOCK_NAMEDVALUECOPIER_HPP

#include "mu/tiny/export.h"

namespace mu {
namespace tiny {
namespace mock {

/**
 * @brief Abstract interface for deep-copying a custom mock output-parameter
 * value.
 *
 * Implement @ref copy() to transfer the object at @p in into the buffer at
 * @p out. The copier's lifetime must extend at least as long as the test (or
 * @ref SupportPlugin scope).
 */
class MUTINY_EXPORT NamedValueCopier
{
public:
  NamedValueCopier() = default;
  virtual ~NamedValueCopier() = default;

  /**
   * @brief Copy the object at @p in into the buffer at @p out.
   *
   * Both pointers reference objects of the type associated with this copier.
   *
   * @param out  Destination buffer (already allocated by the caller).
   * @param in   Source object to copy from.
   */
  virtual void copy(void* out, const void* in) = 0;
};

/**
 * @brief NamedValueCopier backed by a plain function pointer.
 *
 * Convenient when you want to keep copy logic in a standalone function.
 */
class MUTINY_EXPORT FunctionCopier : public NamedValueCopier
{
public:
  /** Function type for the copy operation. */
  using CopyFunction = void (*)(void*, const void*);

  /**
   * @brief Construct from a plain copy function pointer.
   *
   * @param copier  Function that performs the copy.
   */
  FunctionCopier(CopyFunction copier)
    : copier_(copier)
  {
  }

  void copy(void* dst, const void* src) override { copier_(dst, src); }

private:
  CopyFunction copier_;
};

/**
 * @brief NamedValueCopier that uses T::operator= for the copy.
 *
 * Zero-overhead wrapper for types that are assignable.
 *
 * @tparam T  Type to copy; must be copy-assignable.
 */
template<typename T>
class TypedMockCopier : public NamedValueCopier
{
public:
  void copy(void* dst, const void* src) override
  {
    *static_cast<T*>(dst) = *static_cast<const T*>(src);
  }
};

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
