#ifndef INCLUDED_MUTINY_TEST_SHELLPOINTERARRAY_HPP
#define INCLUDED_MUTINY_TEST_SHELLPOINTERARRAY_HPP

#include <stddef.h>

namespace mu {
namespace tiny {
namespace test {

extern int (*rand)();

class Shell;

class ShellPointerArray
{
public:
  ShellPointerArray(Shell* first_test);
  ~ShellPointerArray();

  void shuffle(size_t seed);
  void reverse();
  void relink_tests_in_order();
  Shell* get_first_test() const;
  Shell* get(size_t index) const;

private:
  void swap(size_t index1, size_t index2);

  Shell** array_of_tests_{ nullptr };
  size_t count_{ 0 };
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
