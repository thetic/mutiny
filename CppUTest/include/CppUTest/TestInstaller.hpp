#ifndef INCLUDED_CPPUTEST_TESTINSTALLER_HPP
#define INCLUDED_CPPUTEST_TESTINSTALLER_HPP

#include <stddef.h>

namespace cpputest {

class TestShell;

class TestInstaller
{
public:
  explicit TestInstaller(TestShell& shell,
      const char* group_name,
      const char* test_name,
      const char* file_name,
      size_t line_number);
  virtual ~TestInstaller();

  void un_do();

private:
  TestInstaller(const TestInstaller&);
  TestInstaller& operator=(const TestInstaller&);
};

} // namespace cpputest

#endif
