#include "CppUTest/CppUTest.hpp"
#include "CppUTest/TestRegistry.hpp"

class TestInstallerTestUtestShell : public cpputest::TestShell
{};

// this is file scope because the test is installed
// with all other tests, which also happen to be
// created as static instances at file scope

TEST_GROUP(TestInstaller)
{
  cpputest::TestInstaller* test_installer;
  cpputest::TestRegistry* my_registry;
  TestInstallerTestUtestShell shell;
  void setup() override
  {
    my_registry = new cpputest::TestRegistry();
    my_registry->set_current_registry(my_registry);
    test_installer = new cpputest::TestInstaller(
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
