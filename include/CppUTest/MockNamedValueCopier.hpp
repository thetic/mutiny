#ifndef INCLUDED_CPPUTEST_MOCKNAMEDVALUECOPIER_HPP
#define INCLUDED_CPPUTEST_MOCKNAMEDVALUECOPIER_HPP

namespace cpputest {

/*
 * MockNamedValueCopier is an interface that needs to be used when creating
 * Copiers. This is needed when copying values of non-native type.
 */

class MockNamedValueCopier
{
public:
  MockNamedValueCopier() {}
  virtual ~MockNamedValueCopier() {}

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

  virtual void copy(void* dst, const void* src) override { copier_(dst, src); }

private:
  CopyFunction copier_;
};

} // namespace cpputest

#endif
