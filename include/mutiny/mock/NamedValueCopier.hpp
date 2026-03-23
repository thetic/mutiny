#ifndef INCLUDED_MUTINY_MOCKNAMEDVALUECOPIER_HPP
#define INCLUDED_MUTINY_MOCKNAMEDVALUECOPIER_HPP

/**
 * @file MockNamedValueCopier.hpp
 * @brief Interface and helpers for copying mock output parameters of custom
 * types.
 *
 * When a mock expectation is set with
 * with_output_parameter_of_type_returning(), the framework needs to copy the
 * configured value into the caller's output buffer. Implement
 * MockNamedValueCopier for your type and register it with
 * MockSupport::install_copier().
 *
 * Two concrete implementations are provided:
 * - MockFunctionCopier — wraps a plain function pointer
 * - TypedMockCopier<T> — uses T::operator=
 *
 * @code
 * struct PointCopier : mu::tiny::mock::MockNamedValueCopier {
 *   void copy(void* dst, const void* src) override {
 *     *static_cast<Point*>(dst) = *static_cast<const Point*>(src);
 *   }
 * };
 *
 * PointCopier copier;
 * mock().install_copier("Point", copier);
 * @endcode
 *
 * @see MockSupport::install_copier(), MockNamedValueComparator
 */

namespace mu {
namespace tiny {
namespace mock {

/**
 * @brief Abstract interface for deep-copying a custom mock output-parameter
 * value.
 *
 * Implement copy() to transfer the object at @p in into the buffer at @p out.
 * The copier's lifetime must extend at least as long as the test (or
 * MockSupportPlugin scope).
 */
class MockNamedValueCopier
{
public:
  MockNamedValueCopier() = default;
  virtual ~MockNamedValueCopier() = default;

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
 * @brief MockNamedValueCopier backed by a plain function pointer.
 *
 * Convenient when you want to keep copy logic in a standalone function.
 *
 * @code
 * void copy_point(void* dst, const void* src) {
 *   *static_cast<Point*>(dst) = *static_cast<const Point*>(src);
 * }
 *
 * MockFunctionCopier copier(copy_point);
 * mock().install_copier("Point", copier);
 * @endcode
 */
class MockFunctionCopier : public MockNamedValueCopier
{
public:
  /** Function type for the copy operation. */
  using CopyFunction = void (*)(void*, const void*);

  /**
   * @brief Construct from a plain copy function pointer.
   *
   * @param copier  Function that performs the copy.
   */
  MockFunctionCopier(CopyFunction copier)
    : copier_(copier)
  {
  }

  void copy(void* dst, const void* src) override { copier_(dst, src); }

private:
  CopyFunction copier_;
};

/**
 * @brief MockNamedValueCopier that uses T::operator= for the copy.
 *
 * Zero-overhead wrapper for types that are assignable.
 *
 * @tparam T  Type to copy; must be copy-assignable.
 *
 * @code
 * mu::tiny::test::TypedMockCopier<Point> copier;
 * mock().install_copier("Point", copier);
 * @endcode
 */
template<typename T>
class TypedMockCopier : public MockNamedValueCopier
{
public:
  void copy(void* dst, const void* src) override
  {
    *static_cast<T*>(dst) = *static_cast<const T*>(src);
  }
};

}
}
} // namespace mu::tiny::mock

#endif
