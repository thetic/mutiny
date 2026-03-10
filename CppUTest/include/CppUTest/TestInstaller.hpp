#ifndef D_TestInstaller_h
#define D_TestInstaller_h

#include <stddef.h>

namespace cpputest {

class TestShell;

class TestInstaller
{
public:
  explicit TestInstaller(TestShell& shell,
      const char* groupName,
      const char* testName,
      const char* fileName,
      size_t lineNumber);
  virtual ~TestInstaller();

  void unDo();

private:
  TestInstaller(const TestInstaller&);
  TestInstaller& operator=(const TestInstaller&);
};

} // namespace cpputest

#endif
