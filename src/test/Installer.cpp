#include "mutiny/test/Installer.hpp"

#include "mutiny/test/Registry.hpp"
#include "mutiny/test/Shell.hpp"

namespace mu {
namespace tiny {
namespace test {

Installer::Installer(
    Shell& shell,
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
  Registry::get_current_registry()->add_test(&shell);
}

void Installer::un_do()
{
  Registry::get_current_registry()->un_do_last_add_test();
}

} // namespace test
} // namespace tiny
} // namespace mu
