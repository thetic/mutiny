#ifndef INCLUDED_CPPUTEST_EXTENSIONS_MOCKNAMEDVALUELIST_HPP
#define INCLUDED_CPPUTEST_EXTENSIONS_MOCKNAMEDVALUELIST_HPP

#include "CppUTest/Extensions/MockNamedValue.hpp"

namespace cpputest {
namespace extensions {

class MockNamedValueListNode
{
public:
  MockNamedValueListNode(MockNamedValue* newValue);

  cpputest::String getName() const;
  cpputest::String getType() const;

  MockNamedValueListNode* next();
  MockNamedValue* item();

  void destroy();
  void setNext(MockNamedValueListNode* node);

private:
  MockNamedValue* data_;
  MockNamedValueListNode* next_;
};

class MockNamedValueList
{
public:
  MockNamedValueList();

  MockNamedValueListNode* begin();

  void add(MockNamedValue* newValue);
  void clear();

  MockNamedValue* getValueByName(const cpputest::String& name);

private:
  MockNamedValueListNode* head_;
};

} // namespace extensions
} // namespace cpputest

#endif
