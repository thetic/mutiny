#include "mutiny/test/Registry.hpp"

#include "mutiny/test.hpp"

class TestInstallerTestShell : public mu::tiny::test::Shell
{};

// this is file scope because the test is installed
// with all other tests, which also happen to be
// created as static instances at file scope

TEST_GROUP(Installer)
{
  mu::tiny::test::Installer* test_installer;
  mu::tiny::test::Registry* my_registry;
  TestInstallerTestShell shell;
  void setup() override
  {
    my_registry = new mu::tiny::test::Registry();
    my_registry->set_current_registry(my_registry);
    test_installer = new mu::tiny::test::Installer(
        shell, "Installer", "test", __FILE__, __LINE__
    );
  }
  void teardown() override
  {
    my_registry->set_current_registry(nullptr);
    test_installer->un_do();
    delete test_installer;
    delete my_registry;
  }
};

TEST(Installer, Create) {}
