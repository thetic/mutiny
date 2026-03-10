#include "CppUTestExt/MockNamedValueList.hpp"

namespace cpputest {
namespace extensions {

void
MockNamedValueListNode::setNext(MockNamedValueListNode* node)
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

MockNamedValueListNode::MockNamedValueListNode(MockNamedValue* newValue)
  : data_(newValue)
  , next_(nullptr)
{
}

String
MockNamedValueListNode::getName() const
{
  return data_->getName();
}

String
MockNamedValueListNode::getType() const
{
  return data_->getType();
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
MockNamedValueList::add(MockNamedValue* newValue)
{
  MockNamedValueListNode* newNode = new MockNamedValueListNode(newValue);
  if (head_ == nullptr)
    head_ = newNode;
  else {
    MockNamedValueListNode* lastNode = head_;
    while (lastNode->next())
      lastNode = lastNode->next();
    lastNode->setNext(newNode);
  }
}

MockNamedValue*
MockNamedValueList::getValueByName(const String& name)
{
  for (MockNamedValueListNode* p = head_; p; p = p->next())
    if (p->getName() == name)
      return p->item();
  return nullptr;
}

MockNamedValueListNode*
MockNamedValueList::begin()
{
  return head_;
}

} // namespace extensions
} // namespace cpputest
