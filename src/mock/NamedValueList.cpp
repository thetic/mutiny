#include "mu/tiny/mock/NamedValueList.hpp"

#include "mu/tiny/StringView.hpp"

namespace mu {
namespace tiny {
namespace mock {

void NamedValueListNode::set_next(NamedValueListNode* node)
{
  next_ = node;
}

NamedValueListNode* NamedValueListNode::next()
{
  return next_;
}

NamedValue* NamedValueListNode::item()
{
  return data_;
}

void NamedValueListNode::destroy()
{
  delete data_;
}

NamedValueListNode::NamedValueListNode(NamedValue* new_value)
  : data_(new_value)

{
}

const String& NamedValueListNode::get_name() const
{
  return data_->get_name();
}

StringView NamedValueListNode::get_type() const
{
  return data_->get_type();
}

void NamedValueList::clear()
{
  while (head_ != nullptr) {
    NamedValueListNode* n = head_->next();
    head_->destroy();
    delete head_;
    head_ = n;
  }
}

void NamedValueList::add(NamedValue* new_value)
{
  auto* new_node = new NamedValueListNode(new_value);
  if (head_ == nullptr) {
    head_ = new_node;
  } else {
    NamedValueListNode* last_node = head_;
    while (last_node->next() != nullptr) {
      last_node = last_node->next();
    }
    last_node->set_next(new_node);
  }
}

NamedValue* NamedValueList::get_value_by_name(StringView name)
{
  for (NamedValueListNode* p = head_; p != nullptr; p = p->next()) {
    if (p->get_name() == name) {
      return p->item();
    }
  }
  return nullptr;
}

NamedValueListNode* NamedValueList::begin()
{
  return head_;
}

} // namespace mock
} // namespace tiny
} // namespace mu
