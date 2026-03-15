#include "CppUTest/TestPlugin.hpp"

#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestOutput.hpp"
#include "CppUTest/TestRegistry.hpp"
#include "CppUTest/TestTestingFixture.hpp"

#define GENERIC_PLUGIN "GenericPlugin"
#define GENERIC_PLUGIN2 "GenericPlugin2"
#define GENERIC_PLUGIN3 "GenericPlugin3"

namespace {
int sequenceNumber;

class DummyPlugin : public cpputest::TestPlugin
{
public:
  DummyPlugin(const cpputest::String& name)
    : TestPlugin(name)
    , preAction(0)
    , preActionSequence(0)
    , postAction(0)
    , postActionSequence(0)
  {
  }

  virtual void preTestAction(cpputest::TestShell&,
      cpputest::TestResult&) override
  {
    preAction++;
    preActionSequence = sequenceNumber++;
  }

  virtual void postTestAction(cpputest::TestShell&,
      cpputest::TestResult&) override
  {
    postAction++;
    postActionSequence = sequenceNumber++;
  }

  int preAction;
  int preActionSequence;
  int postAction;
  int postActionSequence;
};

class DummyPluginWhichAcceptsParameters : public DummyPlugin
{
public:
  DummyPluginWhichAcceptsParameters(const cpputest::String& name)
    : DummyPlugin(name)
  {
  }

  virtual bool parseArguments(int argc,
      const char* const* argv,
      int index) override
  {
    cpputest::String argument(argv[index]);
    if (argument == "-paccept")
      return true;
    return TestPlugin::parseArguments(argc, argv, index);
  }
};

} // namespace

TEST_GROUP(TestPlugin)
{
  DummyPlugin* firstPlugin;
  DummyPluginWhichAcceptsParameters* secondPlugin;
  DummyPlugin* thirdPlugin;
  cpputest::TestTestingFixture* genFixture;
  cpputest::TestRegistry* registry;

  void setup() override
  {
    firstPlugin = new DummyPlugin(GENERIC_PLUGIN);
    secondPlugin = new DummyPluginWhichAcceptsParameters(GENERIC_PLUGIN2);
    thirdPlugin = new DummyPlugin(GENERIC_PLUGIN3);
    genFixture = new cpputest::TestTestingFixture;
    registry = genFixture->getRegistry();
    registry->installPlugin(firstPlugin);
    sequenceNumber = 1;
  }

  void teardown() override
  {
    delete firstPlugin;
    delete secondPlugin;
    delete thirdPlugin;
    delete genFixture;
  }
};

#define GENERIC_PLUGIN "GenericPlugin"

TEST(TestPlugin, PluginHasName)
{
  STRCMP_EQUAL(GENERIC_PLUGIN, firstPlugin->getName().c_str());
}

TEST(TestPlugin, InstallPlugin)
{
  CHECK_EQUAL(firstPlugin, registry->getFirstPlugin());
  CHECK_EQUAL(firstPlugin, registry->getPluginByName(GENERIC_PLUGIN));
  LONGS_EQUAL(1, registry->countPlugins());
}

TEST(TestPlugin, InstallMultiplePlugins)
{
  registry->installPlugin(thirdPlugin);
  CHECK_EQUAL(firstPlugin, registry->getPluginByName(GENERIC_PLUGIN));
  CHECK_EQUAL(thirdPlugin, registry->getPluginByName(GENERIC_PLUGIN3));
  POINTERS_EQUAL(nullptr, registry->getPluginByName("I do not exist"));
}

TEST(TestPlugin, ActionsAllRun)
{
  genFixture->runAllTests();
  genFixture->runAllTests();
  CHECK_EQUAL(2, firstPlugin->preAction);
  CHECK_EQUAL(2, firstPlugin->postAction);
}

TEST(TestPlugin, Sequence)
{
  registry->installPlugin(thirdPlugin);
  genFixture->runAllTests();
  CHECK_EQUAL(1, thirdPlugin->preActionSequence);
  CHECK_EQUAL(2, firstPlugin->preActionSequence);
  CHECK_EQUAL(3, firstPlugin->postActionSequence);
  CHECK_EQUAL(4, thirdPlugin->postActionSequence);
  LONGS_EQUAL(2, registry->countPlugins());
}

TEST(TestPlugin, RemovePluginByName)
{
  registry->installPlugin(secondPlugin);
  registry->installPlugin(thirdPlugin);
  LONGS_EQUAL(3, registry->countPlugins());
  registry->removePluginByName(GENERIC_PLUGIN2);
  LONGS_EQUAL(2, registry->countPlugins());
}

struct DefaultPlugin : public cpputest::TestPlugin
{
  DefaultPlugin()
    : TestPlugin("default")
  {
  }
};

TEST(TestPlugin, DefaultPostTestActionDoesntDoAnything)
{
  DefaultPlugin defaultPlugin;
  registry->installPlugin(&defaultPlugin);
  genFixture->runAllTests();
}

TEST(TestPlugin, DisablesPluginsDontRun)
{
  registry->installPlugin(thirdPlugin);
  thirdPlugin->disable();
  genFixture->runAllTests();
  CHECK(!thirdPlugin->isEnabled());
  thirdPlugin->enable();
  genFixture->runAllTests();
  CHECK_EQUAL(2, firstPlugin->preAction);
  CHECK_EQUAL(1, thirdPlugin->preAction);
  CHECK(thirdPlugin->isEnabled());
}

TEST(TestPlugin, ParseArgumentsForUnknownArgumentsFails)
{
  registry->installPlugin(secondPlugin);
  const char* cmd_line[] = { "nonsense", "andmorenonsense" };
  CHECK(registry->getFirstPlugin()->parseAllArguments(2,
            const_cast<char**>(cmd_line),
            0) == false); /* cover non-const wrapper, too */
}

TEST(TestPlugin, ParseArgumentsContinuesAndSucceedsWhenAPluginCanParse)
{
  registry->installPlugin(secondPlugin);
  const char* cmd_line[] = { "-paccept", "andmorenonsense" };
  CHECK(registry->getFirstPlugin()->parseAllArguments(
      2, const_cast<char**>(cmd_line), 0)); /* cover non-const wrapper, too */
}
