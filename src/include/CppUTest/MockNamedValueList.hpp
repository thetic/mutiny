#ifndef INCLUDED_CPPUTEST_MOCKNAMEDVALUELIST_HPP
#define INCLUDED_CPPUTEST_MOCKNAMEDVALUELIST_HPP

#include "CppUTest/MockNamedValue.hpp"

namespace cpputest {

class MockNamedValueListNode
{
public:
  MockNamedValueListNode(MockNamedValue* new_value);

  cpputest::String get_name() const;
  cpputest::String get_type() const;

  MockNamedValueListNode* next();
  MockNamedValue* item();

  void destroy();
  void set_next(MockNamedValueListNode* node);

private:
  MockNamedValue* data_;
  MockNamedValueListNode* next_;
};

class MockNamedValueList
{
public:
  MockNamedValueList();

  MockNamedValueListNode* begin();

  void add(MockNamedValue* new_value);
  void clear();

  MockNamedValue* get_value_by_name(const cpputest::String& name);

private:
  MockNamedValueListNode* head_;
};

} // namespace cpputest

#endif
