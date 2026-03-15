#ifndef D_MockNamedValueCopier_h
#define D_MockNamedValueCopier_h

namespace cpputest {
namespace extensions {

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
  using copyFunction = void (*)(void*, const void*);

  MockFunctionCopier(copyFunction copier)
    : copier_(copier)
  {
  }

  virtual void copy(void* dst, const void* src) override { copier_(dst, src); }

private:
  copyFunction copier_;
};

} // namespace extensions
} // namespace cpputest

#endif
