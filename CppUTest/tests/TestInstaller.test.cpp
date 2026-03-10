#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestRegistry.hpp"

class TestInstallerTestUtestShell : public cpputest::TestShell
{};

// this is file scope because the test is installed
// with all other tests, which also happen to be
// created as static instances at file scope

TEST_GROUP(TestInstaller)
{
  cpputest::TestInstaller* testInstaller;
  cpputest::TestRegistry* myRegistry;
  TestInstallerTestUtestShell shell;
  void setup() override
  {
    myRegistry = new cpputest::TestRegistry();
    myRegistry->setCurrentRegistry(myRegistry);
    testInstaller = new cpputest::TestInstaller(
        shell, "TestInstaller", "test", __FILE__, __LINE__);
  }
  void teardown() override
  {
    myRegistry->setCurrentRegistry(nullptr);
    testInstaller->unDo();
    delete testInstaller;
    delete myRegistry;
  }
};

TEST(TestInstaller, Create)
{
}
