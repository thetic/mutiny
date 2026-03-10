#ifndef D_TestShellPointerArray_h
#define D_TestShellPointerArray_h

#include <stddef.h>

namespace cpputest {

class TestShell;

class TestShellPointerArray
{
public:
  TestShellPointerArray(TestShell* firstTest);
  ~TestShellPointerArray();

  void shuffle(size_t seed);
  void reverse();
  void relinkTestsInOrder();
  TestShell* getFirstTest() const;
  TestShell* get(size_t index) const;

private:
  void swap(size_t index1, size_t index2);

  TestShell** arrayOfTests_;
  size_t count_;
};

} // namespace cpputest

#endif
