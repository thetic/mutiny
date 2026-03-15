#ifndef INCLUDED_CPPUTEST_EXTENSIONS_MOCKNAMEDVALUECOMPARATOR_HPP
#define INCLUDED_CPPUTEST_EXTENSIONS_MOCKNAMEDVALUECOMPARATOR_HPP

#include "CppUTest/String.hpp"

namespace cpputest {
namespace extensions {

/*
 * MockNamedValueComparator is an interface that needs to be used when creating
 * Comparators. This is needed when comparing values of non-native type.
 */

class MockNamedValueComparator
{
public:
  MockNamedValueComparator() {}
  virtual ~MockNamedValueComparator() {}

  virtual bool isEqual(const void* object1, const void* object2) = 0;
  virtual cpputest::String valueToString(const void* object) = 0;
};

class MockFunctionComparator : public MockNamedValueComparator
{
public:
  using isEqualFunction = bool (*)(const void*, const void*);
  using valueToStringFunction = cpputest::String (*)(const void*);

  MockFunctionComparator(isEqualFunction equal,
      valueToStringFunction valToString)
    : equal_(equal)
    , valueToString_(valToString)
  {
  }

  virtual bool isEqual(const void* object1, const void* object2) override
  {
    return equal_(object1, object2);
  }
  virtual cpputest::String valueToString(const void* object) override
  {
    return valueToString_(object);
  }

private:
  isEqualFunction equal_;
  valueToStringFunction valueToString_;
};

} // namespace extensions
} // namespace cpputest

#endif
