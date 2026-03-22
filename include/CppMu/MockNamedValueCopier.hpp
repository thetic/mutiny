#ifndef INCLUDED_CPPMU_MOCKNAMEDVALUECOPIER_HPP
#define INCLUDED_CPPMU_MOCKNAMEDVALUECOPIER_HPP

namespace cppmu {

/*
 * MockNamedValueCopier is an interface that needs to be used when creating
 * Copiers. This is needed when copying values of non-native type.
 */

class MockNamedValueCopier
{
public:
  MockNamedValueCopier() = default;
  virtual ~MockNamedValueCopier() = default;

  virtual void copy(void* out, const void* in) = 0;
};

class MockFunctionCopier : public MockNamedValueCopier
{
public:
  using CopyFunction = void (*)(void*, const void*);

  MockFunctionCopier(CopyFunction copier)
    : copier_(copier)
  {
  }

  void copy(void* dst, const void* src) override { copier_(dst, src); }

private:
  CopyFunction copier_;
};

template<typename T>
class TypedMockCopier : public MockNamedValueCopier
{
public:
  void copy(void* dst, const void* src) override
  {
    *static_cast<T*>(dst) = *static_cast<const T*>(src);
  }
};

} // namespace cppmu

#endif
