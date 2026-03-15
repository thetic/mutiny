#include "CppMu/TestPlugin.hpp"

#include "CppMu/CppMu.hpp"
#include "CppMu/TestOutput.hpp"
#include "CppMu/TestRegistry.hpp"
#include "CppMu/TestTestingFixture.hpp"

#define GENERIC_PLUGIN "GenericPlugin"
#define GENERIC_PLUGIN2 "GenericPlugin2"
#define GENERIC_PLUGIN3 "GenericPlugin3"

namespace {
int sequence_number;

class DummyPlugin : public cppmu::TestPlugin
{
public:
  DummyPlugin(const cppmu::String& name)
    : TestPlugin(name)
    , pre_action(0)
    , pre_action_sequence(0)
    , post_action(0)
    , post_action_sequence(0)
  {
  }

  virtual void pre_test_action(cppmu::TestShell&, cppmu::TestResult&) override
  {
    pre_action++;
    pre_action_sequence = sequence_number++;
  }

  virtual void post_test_action(cppmu::TestShell&, cppmu::TestResult&) override
  {
    post_action++;
    post_action_sequence = sequence_number++;
  }

  int pre_action;
  int pre_action_sequence;
  int post_action;
  int post_action_sequence;
};

class DummyPluginWhichAcceptsParameters : public DummyPlugin
{
public:
  DummyPluginWhichAcceptsParameters(const cppmu::String& name)
    : DummyPlugin(name)
  {
  }

  virtual bool parse_arguments(int argc,
      const char* const* argv,
      int index) override
  {
    cppmu::String argument(argv[index]);
    if (argument == "-paccept")
      return true;
    return TestPlugin::parse_arguments(argc, argv, index);
  }
};

} // namespace

TEST_GROUP(TestPlugin)
{
  DummyPlugin* first_plugin;
  DummyPluginWhichAcceptsParameters* second_plugin;
  DummyPlugin* third_plugin;
  cppmu::TestTestingFixture* gen_fixture;
  cppmu::TestRegistry* registry;

  void setup() override
  {
    first_plugin = new DummyPlugin(GENERIC_PLUGIN);
    second_plugin = new DummyPluginWhichAcceptsParameters(GENERIC_PLUGIN2);
    third_plugin = new DummyPlugin(GENERIC_PLUGIN3);
    gen_fixture = new cppmu::TestTestingFixture;
    registry = gen_fixture->get_registry();
    registry->install_plugin(first_plugin);
    sequence_number = 1;
  }

  void teardown() override
  {
    delete first_plugin;
    delete second_plugin;
    delete third_plugin;
    delete gen_fixture;
  }
};

#define GENERIC_PLUGIN "GenericPlugin"

TEST(TestPlugin, PluginHasName)
{
  STRCMP_EQUAL(GENERIC_PLUGIN, first_plugin->get_name().c_str());
}

TEST(TestPlugin, InstallPlugin)
{
  CHECK_EQUAL(first_plugin, registry->get_first_plugin());
  CHECK_EQUAL(first_plugin, registry->get_plugin_by_name(GENERIC_PLUGIN));
  LONGS_EQUAL(1, registry->count_plugins());
}

TEST(TestPlugin, InstallMultiplePlugins)
{
  registry->install_plugin(third_plugin);
  CHECK_EQUAL(first_plugin, registry->get_plugin_by_name(GENERIC_PLUGIN));
  CHECK_EQUAL(third_plugin, registry->get_plugin_by_name(GENERIC_PLUGIN3));
  POINTERS_EQUAL(nullptr, registry->get_plugin_by_name("I do not exist"));
}

TEST(TestPlugin, ActionsAllRun)
{
  gen_fixture->run_all_tests();
  gen_fixture->run_all_tests();
  CHECK_EQUAL(2, first_plugin->pre_action);
  CHECK_EQUAL(2, first_plugin->post_action);
}

TEST(TestPlugin, Sequence)
{
  registry->install_plugin(third_plugin);
  gen_fixture->run_all_tests();
  CHECK_EQUAL(1, third_plugin->pre_action_sequence);
  CHECK_EQUAL(2, first_plugin->pre_action_sequence);
  CHECK_EQUAL(3, first_plugin->post_action_sequence);
  CHECK_EQUAL(4, third_plugin->post_action_sequence);
  LONGS_EQUAL(2, registry->count_plugins());
}

TEST(TestPlugin, RemovePluginByName)
{
  registry->install_plugin(second_plugin);
  registry->install_plugin(third_plugin);
  LONGS_EQUAL(3, registry->count_plugins());
  registry->remove_plugin_by_name(GENERIC_PLUGIN2);
  LONGS_EQUAL(2, registry->count_plugins());
}

struct DefaultPlugin : public cppmu::TestPlugin
{
  DefaultPlugin()
    : TestPlugin("default")
  {
  }
};

TEST(TestPlugin, DefaultPostTestActionDoesntDoAnything)
{
  DefaultPlugin default_plugin;
  registry->install_plugin(&default_plugin);
  gen_fixture->run_all_tests();
}

TEST(TestPlugin, DisablesPluginsDontRun)
{
  registry->install_plugin(third_plugin);
  third_plugin->disable();
  gen_fixture->run_all_tests();
  CHECK(!third_plugin->is_enabled());
  third_plugin->enable();
  gen_fixture->run_all_tests();
  CHECK_EQUAL(2, first_plugin->pre_action);
  CHECK_EQUAL(1, third_plugin->pre_action);
  CHECK(third_plugin->is_enabled());
}

TEST(TestPlugin, ParseArgumentsForUnknownArgumentsFails)
{
  registry->install_plugin(second_plugin);
  const char* cmd_line[] = { "nonsense", "andmorenonsense" };
  CHECK(registry->get_first_plugin()->parse_all_arguments(2,
            const_cast<char**>(cmd_line),
            0) == false); /* cover non-const wrapper, too */
}

TEST(TestPlugin, ParseArgumentsContinuesAndSucceedsWhenAPluginCanParse)
{
  registry->install_plugin(second_plugin);
  const char* cmd_line[] = { "-paccept", "andmorenonsense" };
  CHECK(registry->get_first_plugin()->parse_all_arguments(
      2, const_cast<char**>(cmd_line), 0)); /* cover non-const wrapper, too */
}
