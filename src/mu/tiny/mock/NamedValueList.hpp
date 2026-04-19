#ifndef INCLUDED_MU_TINY_MOCK_NAMEDVALUELIST_HPP
#define INCLUDED_MU_TINY_MOCK_NAMEDVALUELIST_HPP

#include "mu/tiny/mock/NamedValue.hpp"

#include "mu/tiny/StringView.hpp"

namespace mu {
namespace tiny {
namespace mock {

class NamedValueListNode
{
public:
  NamedValueListNode(NamedValue* new_value);

  const String& get_name() const;
  const String& get_type() const;

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

  NamedValue* get_value_by_name(StringView name);

private:
  NamedValueListNode* head_{ nullptr };
};

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
