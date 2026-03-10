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

#include "CppUTest/TestShellPointerArray.hpp"

#include "CppUTest/PlatformSpecificFunctions.hpp"
#include "CppUTest/TestShell.hpp"

namespace cpputest {

TestShellPointerArray::TestShellPointerArray(TestShell* firstTest)
  : arrayOfTests_(nullptr)
  , count_(0)
{
  count_ = (firstTest) ? firstTest->countTests() : 0;
  if (count_ == 0)
    return;

  arrayOfTests_ = new TestShell*[count_];

  TestShell* currentTest = firstTest;
  for (size_t i = 0; i < count_; i++) {
    arrayOfTests_[i] = currentTest;
    currentTest = currentTest->getNext();
  }
}

TestShellPointerArray::~TestShellPointerArray()
{
  delete[] arrayOfTests_;
}

void
TestShellPointerArray::swap(size_t index1, size_t index2)
{
  TestShell* e2 = arrayOfTests_[index2];
  TestShell* e1 = arrayOfTests_[index1];
  arrayOfTests_[index1] = e2;
  arrayOfTests_[index2] = e1;
}

void
TestShellPointerArray::shuffle(size_t seed)
{
  if (count_ == 0)
    return;

  PlatformSpecificSrand(static_cast<unsigned int>(seed));

  for (size_t i = count_ - 1; i >= 1; --i) {
    if (count_ == 0)
      return;

    const size_t j =
        static_cast<size_t>(PlatformSpecificRand()) %
        (i + 1); // distribution biased by modulo, but good enough for shuffling
    swap(i, j);
  }
  relinkTestsInOrder();
}

void
TestShellPointerArray::reverse()
{
  if (count_ == 0)
    return;

  size_t halfCount = count_ / 2;
  for (size_t i = 0; i < halfCount; i++) {
    size_t j = count_ - i - 1;
    swap(i, j);
  }
  relinkTestsInOrder();
}

void
TestShellPointerArray::relinkTestsInOrder()
{
  TestShell* tests = nullptr;
  for (size_t i = 0; i < count_; i++)
    tests = arrayOfTests_[count_ - i - 1]->addTest(tests);
}

TestShell*
TestShellPointerArray::getFirstTest() const
{
  return get(0);
}

TestShell*
TestShellPointerArray::get(size_t index) const
{
  if (index >= count_)
    return nullptr;
  return arrayOfTests_[index];
}

} // namespace cpputest
