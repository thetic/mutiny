#include "mu/tiny/test/ShellPointerArray.hpp"

#include "mu/tiny/test/Shell.hpp"

#include <stdlib.h>

namespace mu {
namespace tiny {
namespace test {

int (*rand)() = ::rand;

ShellPointerArray::ShellPointerArray(Shell* first_test)

{
  count_ = (first_test != nullptr) ? first_test->count_tests() : 0;
  if (count_ == 0) {
    return;
  }

  array_of_tests_ = new Shell*[count_];

  Shell* current_test = first_test;
  for (size_t i = 0; i < count_; i++) {
    array_of_tests_[i] = current_test;
    current_test = current_test->get_next();
  }
}

ShellPointerArray::~ShellPointerArray()
{
  delete[] array_of_tests_;
}

void ShellPointerArray::swap(size_t index1, size_t index2)
{
  Shell* e2 = array_of_tests_[index2];
  Shell* e1 = array_of_tests_[index1];
  array_of_tests_[index1] = e2;
  array_of_tests_[index2] = e1;
}

void ShellPointerArray::shuffle(size_t seed)
{
  if (count_ == 0) {
    return;
  }

  srand(static_cast<unsigned int>(seed));

  for (size_t i = count_ - 1; i >= 1; --i) {
    const size_t j =
        static_cast<size_t>(rand()) %
        (i + 1); // distribution biased by modulo, but good enough for shuffling
    swap(i, j);
  }
  relink_tests_in_order();
}

void ShellPointerArray::reverse()
{
  if (count_ == 0) {
    return;
  }

  size_t half_count = count_ / 2;
  for (size_t i = 0; i < half_count; i++) {
    size_t j = count_ - i - 1;
    swap(i, j);
  }
  relink_tests_in_order();
}

void ShellPointerArray::relink_tests_in_order()
{
  Shell* tests = nullptr;
  for (size_t i = 0; i < count_; i++) {
    tests = array_of_tests_[count_ - i - 1]->add_test(tests);
  }
}

Shell* ShellPointerArray::get_first_test() const
{
  return get(0);
}

Shell* ShellPointerArray::get(size_t index) const
{
  if (index >= count_) {
    return nullptr;
  }
  return array_of_tests_[index];
}

} // namespace test
} // namespace tiny
} // namespace mu
