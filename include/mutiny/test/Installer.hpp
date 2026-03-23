#ifndef INCLUDED_MUTINY_TEST_INSTALLER_HPP
#define INCLUDED_MUTINY_TEST_INSTALLER_HPP

#include <stddef.h>

namespace mu {
namespace tiny {
namespace test {

class Shell;

class Installer
{
public:
  explicit Installer(
      Shell& shell,
      const char* group_name,
      const char* test_name,
      const char* file_name,
      size_t line_number
  );
  virtual ~Installer() = default;
  Installer(const Installer&) = delete;
  Installer& operator=(const Installer&) = delete;

  void un_do();
};

}
}
} // namespace mu::tiny::test

#endif
