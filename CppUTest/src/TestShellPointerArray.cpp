#include "CppUTest/TestShellPointerArray.hpp"

#include "CppUTest/PlatformSpecificFunctions.h"
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
