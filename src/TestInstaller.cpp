#include "CppMu/TestInstaller.hpp"

#include "CppMu/TestRegistry.hpp"
#include "CppMu/TestShell.hpp"

namespace cppmu {

TestInstaller::TestInstaller(
    TestShell& shell,
    const char* group_name,
    const char* test_name,
    const char* file_name,
    size_t line_number
)
{
  shell.set_group_name(group_name);
  shell.set_test_name(test_name);
  shell.set_file_name(file_name);
  shell.set_line_number(line_number);
  TestRegistry::get_current_registry()->add_test(&shell);
}

void TestInstaller::un_do()
{
  TestRegistry::get_current_registry()->un_do_last_add_test();
}

} // namespace cppmu
