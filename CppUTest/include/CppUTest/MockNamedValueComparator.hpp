#ifndef INCLUDED_CPPUTEST_MOCKNAMEDVALUECOMPARATOR_HPP
#define INCLUDED_CPPUTEST_MOCKNAMEDVALUECOMPARATOR_HPP

#include "CppUTest/String.hpp"

namespace cpputest {

/*
 * MockNamedValueComparator is an interface that needs to be used when creating
 * Comparators. This is needed when comparing values of non-native type.
 */

class MockNamedValueComparator
{
public:
  MockNamedValueComparator() {}
  virtual ~MockNamedValueComparator() {}

  virtual bool is_equal(const void* object1, const void* object2) = 0;
  virtual cpputest::String value_to_string(const void* object) = 0;
};

class MockFunctionComparator : public MockNamedValueComparator
{
public:
  using IsEqualFunction = bool (*)(const void*, const void*);
  using ValueToStringFunction = cpputest::String (*)(const void*);

  MockFunctionComparator(IsEqualFunction equal,
      ValueToStringFunction val_to_string)
    : equal_(equal)
    , value_to_string_(val_to_string)
  {
  }

  virtual bool is_equal(const void* object1, const void* object2) override
  {
    return equal_(object1, object2);
  }
  virtual cpputest::String value_to_string(const void* object) override
  {
    return value_to_string_(object);
  }

private:
  IsEqualFunction equal_;
  ValueToStringFunction value_to_string_;
};

} // namespace cpputest

#endif
