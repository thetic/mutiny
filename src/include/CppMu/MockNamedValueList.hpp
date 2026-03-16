#ifndef INCLUDED_CPPMU_MOCKNAMEDVALUELIST_HPP
#define INCLUDED_CPPMU_MOCKNAMEDVALUELIST_HPP

#include "CppMu/MockNamedValue.hpp"

namespace cppmu {

class MockNamedValueListNode
{
public:
  MockNamedValueListNode(MockNamedValue* new_value);

  cppmu::String get_name() const;
  cppmu::String get_type() const;

  MockNamedValueListNode* next();
  MockNamedValue* item();

  void destroy();
  void set_next(MockNamedValueListNode* node);

private:
  MockNamedValue* data_;
  MockNamedValueListNode* next_{ nullptr };
};

class MockNamedValueList
{
public:
  MockNamedValueList() = default;

  MockNamedValueListNode* begin();

  void add(MockNamedValue* new_value);
  void clear();

  MockNamedValue* get_value_by_name(const cppmu::String& name);

private:
  MockNamedValueListNode* head_{ nullptr };
};

} // namespace cppmu

#endif
