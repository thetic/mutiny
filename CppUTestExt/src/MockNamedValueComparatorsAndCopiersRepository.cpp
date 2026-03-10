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

#include "CppUTestExt/MockNamedValueComparatorsAndCopiersRepository.hpp"

namespace cpputest {
namespace extensions {

struct MockNamedValueComparatorsAndCopiersRepositoryNode
{
  MockNamedValueComparatorsAndCopiersRepositoryNode(const String& name,
      MockNamedValueComparator* comparator,
      MockNamedValueComparatorsAndCopiersRepositoryNode* next)
    : name_(name)
    , comparator_(comparator)
    , copier_(nullptr)
    , next_(next)
  {
  }
  MockNamedValueComparatorsAndCopiersRepositoryNode(const String& name,
      MockNamedValueCopier* copier,
      MockNamedValueComparatorsAndCopiersRepositoryNode* next)
    : name_(name)
    , comparator_(nullptr)
    , copier_(copier)
    , next_(next)
  {
  }
  MockNamedValueComparatorsAndCopiersRepositoryNode(const String& name,
      MockNamedValueComparator* comparator,
      MockNamedValueCopier* copier,
      MockNamedValueComparatorsAndCopiersRepositoryNode* next)
    : name_(name)
    , comparator_(comparator)
    , copier_(copier)
    , next_(next)
  {
  }
  String name_;
  MockNamedValueComparator* comparator_;
  MockNamedValueCopier* copier_;
  MockNamedValueComparatorsAndCopiersRepositoryNode* next_;
};

MockNamedValueComparatorsAndCopiersRepository::
    MockNamedValueComparatorsAndCopiersRepository()
  : head_(nullptr)
{
}

MockNamedValueComparatorsAndCopiersRepository::
    ~MockNamedValueComparatorsAndCopiersRepository()
{
  clear();
}

void
MockNamedValueComparatorsAndCopiersRepository::clear()
{
  while (head_) {
    MockNamedValueComparatorsAndCopiersRepositoryNode* next = head_->next_;
    delete head_;
    head_ = next;
  }
}

void
MockNamedValueComparatorsAndCopiersRepository::installComparator(
    const String& name,
    MockNamedValueComparator& comparator)
{
  head_ = new MockNamedValueComparatorsAndCopiersRepositoryNode(
      name, &comparator, head_);
}

void
MockNamedValueComparatorsAndCopiersRepository::installCopier(const String& name,
    MockNamedValueCopier& copier)
{
  head_ = new MockNamedValueComparatorsAndCopiersRepositoryNode(
      name, &copier, head_);
}

MockNamedValueComparator*
MockNamedValueComparatorsAndCopiersRepository::getComparatorForType(
    const String& name)
{
  for (MockNamedValueComparatorsAndCopiersRepositoryNode* p = head_; p;
      p = p->next_)
    if (p->name_ == name && p->comparator_)
      return p->comparator_;
  return nullptr;
}

MockNamedValueCopier*
MockNamedValueComparatorsAndCopiersRepository::getCopierForType(
    const String& name)
{
  for (MockNamedValueComparatorsAndCopiersRepositoryNode* p = head_; p;
      p = p->next_)
    if (p->name_ == name && p->copier_)
      return p->copier_;
  return nullptr;
}

void
MockNamedValueComparatorsAndCopiersRepository::installComparatorsAndCopiers(
    const MockNamedValueComparatorsAndCopiersRepository& repository)
{
  for (MockNamedValueComparatorsAndCopiersRepositoryNode* p = repository.head_;
      p;
      p = p->next_)
    head_ = new MockNamedValueComparatorsAndCopiersRepositoryNode(
        p->name_, p->comparator_, p->copier_, head_);
}

} // namespace extensions
} // namespace cpputest
