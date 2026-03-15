#ifndef INCLUDED_CPPUTEST_TESTINSTALLER_HPP
#define INCLUDED_CPPUTEST_TESTINSTALLER_HPP

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
