#include "CppUTest/MockNamedValueList.hpp"

namespace cpputest {

void
MockNamedValueListNode::set_next(MockNamedValueListNode* node)
{
  next_ = node;
}

MockNamedValueListNode*
MockNamedValueListNode::next()
{
  return next_;
}

MockNamedValue*
MockNamedValueListNode::item()
{
  return data_;
}

void
MockNamedValueListNode::destroy()
{
  delete data_;
}

MockNamedValueListNode::MockNamedValueListNode(MockNamedValue* new_value)
  : data_(new_value)
  , next_(nullptr)
{
}

String
MockNamedValueListNode::get_name() const
{
  return data_->get_name();
}

String
MockNamedValueListNode::get_type() const
{
  return data_->get_type();
}

MockNamedValueList::MockNamedValueList()
  : head_(nullptr)
{
}

void
MockNamedValueList::clear()
{
  while (head_) {
    MockNamedValueListNode* n = head_->next();
    head_->destroy();
    delete head_;
    head_ = n;
  }
}

void
MockNamedValueList::add(MockNamedValue* new_value)
{
  auto* new_node = new MockNamedValueListNode(new_value);
  if (head_ == nullptr)
    head_ = new_node;
  else {
    MockNamedValueListNode* last_node = head_;
    while (last_node->next())
      last_node = last_node->next();
    last_node->set_next(new_node);
  }
}

MockNamedValue*
MockNamedValueList::get_value_by_name(const String& name)
{
  for (MockNamedValueListNode* p = head_; p; p = p->next())
    if (p->get_name() == name)
      return p->item();
  return nullptr;
}

MockNamedValueListNode*
MockNamedValueList::begin()
{
  return head_;
}

} // namespace cpputest
