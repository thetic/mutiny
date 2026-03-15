#include "CppMu/TestShellPointerArray.hpp"

#include "CppMu/TestShell.hpp"

#include <stdlib.h>

namespace cppmu {

int (*rand)() = ::rand;

TestShellPointerArray::TestShellPointerArray(TestShell* first_test)
  : array_of_tests_(nullptr)
  , count_(0)
{
  count_ = (first_test) ? first_test->count_tests() : 0;
  if (count_ == 0)
    return;

  array_of_tests_ = new TestShell*[count_];

  TestShell* current_test = first_test;
  for (size_t i = 0; i < count_; i++) {
    array_of_tests_[i] = current_test;
    current_test = current_test->get_next();
  }
}

TestShellPointerArray::~TestShellPointerArray()
{
  delete[] array_of_tests_;
}

void
TestShellPointerArray::swap(size_t index1, size_t index2)
{
  TestShell* e2 = array_of_tests_[index2];
  TestShell* e1 = array_of_tests_[index1];
  array_of_tests_[index1] = e2;
  array_of_tests_[index2] = e1;
}

void
TestShellPointerArray::shuffle(size_t seed)
{
  if (count_ == 0)
    return;

  srand(static_cast<unsigned int>(seed));

  for (size_t i = count_ - 1; i >= 1; --i) {
    if (count_ == 0)
      return;

    const size_t j =
        static_cast<size_t>(rand()) %
        (i + 1); // distribution biased by modulo, but good enough for shuffling
    swap(i, j);
  }
  relink_tests_in_order();
}

void
TestShellPointerArray::reverse()
{
  if (count_ == 0)
    return;

  size_t half_count = count_ / 2;
  for (size_t i = 0; i < half_count; i++) {
    size_t j = count_ - i - 1;
    swap(i, j);
  }
  relink_tests_in_order();
}

void
TestShellPointerArray::relink_tests_in_order()
{
  TestShell* tests = nullptr;
  for (size_t i = 0; i < count_; i++)
    tests = array_of_tests_[count_ - i - 1]->add_test(tests);
}

TestShell*
TestShellPointerArray::get_first_test() const
{
  return get(0);
}

TestShell*
TestShellPointerArray::get(size_t index) const
{
  if (index >= count_)
    return nullptr;
  return array_of_tests_[index];
}

} // namespace cppmu
