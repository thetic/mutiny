#ifndef INCLUDED_CPPUTEST_TESTSHELLPOINTERARRAY_HPP
#define INCLUDED_CPPUTEST_TESTSHELLPOINTERARRAY_HPP

#include <stddef.h>

namespace cpputest {

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

  TestShell** array_of_tests_;
  size_t count_;
};

} // namespace cpputest

#endif
