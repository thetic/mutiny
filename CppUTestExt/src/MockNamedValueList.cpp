/*
 * Copyright (c) 2007, Michael Feathers, James Grenning and Bas Vodde
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
