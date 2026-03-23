#ifndef INCLUDED_MUTINY_MOCKNAMEDVALUELIST_HPP
#define INCLUDED_MUTINY_MOCKNAMEDVALUELIST_HPP

#include "mutiny/mock/NamedValue.hpp"

namespace mu {
namespace tiny {
namespace mock {

class MockNamedValueListNode
{
public:
  MockNamedValueListNode(MockNamedValue* new_value);

  mu::tiny::test::String get_name() const;
  mu::tiny::test::String get_type() const;

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

  MockNamedValue* get_value_by_name(const mu::tiny::test::String& name);

private:
  MockNamedValueListNode* head_{ nullptr };
};

}
}
} // namespace mu::tiny::mock

#endif
