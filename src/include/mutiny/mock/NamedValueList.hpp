#ifndef INCLUDED_MUTINY_MOCK_NAMEDVALUELIST_HPP
#define INCLUDED_MUTINY_MOCK_NAMEDVALUELIST_HPP

#include "mutiny/mock/NamedValue.hpp"

namespace mu {
namespace tiny {
namespace mock {

class NamedValueListNode
{
public:
  NamedValueListNode(NamedValue* new_value);

  mu::tiny::test::String get_name() const;
  mu::tiny::test::String get_type() const;

  NamedValueListNode* next();
  NamedValue* item();

  void destroy();
  void set_next(NamedValueListNode* node);

private:
  NamedValue* data_;
  NamedValueListNode* next_{ nullptr };
};

class NamedValueList
{
public:
  NamedValueList() = default;

  NamedValueListNode* begin();

  void add(NamedValue* new_value);
  void clear();

  NamedValue* get_value_by_name(const mu::tiny::test::String& name);

private:
  NamedValueListNode* head_{ nullptr };
};

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
