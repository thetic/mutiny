#ifndef INCLUDED_CPPMU_TESTINSTALLER_HPP
#define INCLUDED_CPPMU_TESTINSTALLER_HPP

#include <stddef.h>

namespace cppmu {

class TestShell;

class TestInstaller
{
public:
  explicit TestInstaller(
      TestShell& shell,
      const char* group_name,
      const char* test_name,
      const char* file_name,
      size_t line_number
  );
  virtual ~TestInstaller() = default;
  TestInstaller(const TestInstaller&) = delete;
  TestInstaller& operator=(const TestInstaller&) = delete;

  void un_do();
};

} // namespace cppmu

#endif
