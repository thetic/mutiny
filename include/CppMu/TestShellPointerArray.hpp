#ifndef INCLUDED_CPPMU_TESTSHELLPOINTERARRAY_HPP
#define INCLUDED_CPPMU_TESTSHELLPOINTERARRAY_HPP

#include <stddef.h>

namespace cppmu {

extern int (*rand)();

class TestShell;

class TestShellPointerArray
{
public:
  TestShellPointerArray(TestShell* first_test);
  ~TestShellPointerArray();

  void shuffle(size_t seed);
  void reverse();
  void relink_tests_in_order();
  TestShell* get_first_test() const;
  TestShell* get(size_t index) const;

private:
  void swap(size_t index1, size_t index2);

  TestShell** array_of_tests_{ nullptr };
  size_t count_{ 0 };
};

} // namespace cppmu

#endif
