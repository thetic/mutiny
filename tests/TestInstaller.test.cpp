#include "CppMu/CppMu.hpp"
#include "CppMu/TestRegistry.hpp"

class TestInstallerTestShell : public cppmu::TestShell
{};

// this is file scope because the test is installed
// with all other tests, which also happen to be
// created as static instances at file scope

TEST_GROUP(TestInstaller)
{
  cppmu::TestInstaller* test_installer;
  cppmu::TestRegistry* my_registry;
  TestInstallerTestShell shell;
  void setup() override
  {
    my_registry = new cppmu::TestRegistry();
    my_registry->set_current_registry(my_registry);
    test_installer = new cppmu::TestInstaller(
        shell, "TestInstaller", "test", __FILE__, __LINE__);
  }
  void teardown() override
  {
    my_registry->set_current_registry(nullptr);
    test_installer->un_do();
    delete test_installer;
    delete my_registry;
  }
};

TEST(TestInstaller, Create)
{
}
