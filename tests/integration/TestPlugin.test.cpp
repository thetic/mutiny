#include "mutiny/test/Output.hpp"
#include "mutiny/test/Plugin.hpp"
#include "mutiny/test/Registry.hpp"
#include "mutiny/test/TestingFixture.hpp"

#include "mutiny/test.hpp"

#define GENERIC_PLUGIN "GenericPlugin"
#define GENERIC_PLUGIN2 "GenericPlugin2"
#define GENERIC_PLUGIN3 "GenericPlugin3"

namespace {
int sequence_number;

class DummyPlugin : public mu::tiny::test::Plugin
{
public:
  DummyPlugin(const mu::tiny::String& name)
    : Plugin(name)

  {
  }

  void pre_test_action(mu::tiny::test::Shell&, mu::tiny::test::Result&) override
  {
    pre_action++;
    pre_action_sequence = sequence_number++;
  }

  void post_test_action(
      mu::tiny::test::Shell&,
      mu::tiny::test::Result&
  ) override
  {
    post_action++;
    post_action_sequence = sequence_number++;
  }

  int pre_action{ 0 };
  int pre_action_sequence{ 0 };
  int post_action{ 0 };
  int post_action_sequence{ 0 };
};

class DummyPluginWhichAcceptsParameters : public DummyPlugin
{
public:
  DummyPluginWhichAcceptsParameters(const mu::tiny::String& name)
    : DummyPlugin(name)
  {
  }

  bool parse_arguments(int argc, const char* const* argv, int index) override
  {
    mu::tiny::String argument(argv[index]);
    if (argument == "-paccept")
      return true;
    return Plugin::parse_arguments(argc, argv, index);
  }
};

} // namespace

TEST_GROUP(Plugin)
{
  DummyPlugin* first_plugin;
  DummyPluginWhichAcceptsParameters* second_plugin;
  DummyPlugin* third_plugin;
  mu::tiny::test::TestingFixture* gen_fixture;
  mu::tiny::test::Registry* registry;

  void setup() override
  {
    first_plugin = new DummyPlugin(GENERIC_PLUGIN);
    second_plugin = new DummyPluginWhichAcceptsParameters(GENERIC_PLUGIN2);
    third_plugin = new DummyPlugin(GENERIC_PLUGIN3);
    gen_fixture = new mu::tiny::test::TestingFixture;
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

TEST(Plugin, PluginHasName)
{
  STRCMP_EQUAL(GENERIC_PLUGIN, first_plugin->get_name().c_str());
}

TEST(Plugin, InstallPlugin)
{
  CHECK_EQUAL(first_plugin, registry->get_first_plugin());
  CHECK_EQUAL(first_plugin, registry->get_plugin_by_name(GENERIC_PLUGIN));
  CHECK_EQUAL(1, registry->count_plugins());
}

TEST(Plugin, InstallMultiplePlugins)
{
  registry->install_plugin(third_plugin);
  CHECK_EQUAL(first_plugin, registry->get_plugin_by_name(GENERIC_PLUGIN));
  CHECK_EQUAL(third_plugin, registry->get_plugin_by_name(GENERIC_PLUGIN3));
  POINTERS_EQUAL(nullptr, registry->get_plugin_by_name("I do not exist"));
}

TEST(Plugin, ActionsAllRun)
{
  gen_fixture->run_all_tests();
  gen_fixture->run_all_tests();
  CHECK_EQUAL(2, first_plugin->pre_action);
  CHECK_EQUAL(2, first_plugin->post_action);
}

TEST(Plugin, Sequence)
{
  registry->install_plugin(third_plugin);
  gen_fixture->run_all_tests();
  CHECK_EQUAL(1, third_plugin->pre_action_sequence);
  CHECK_EQUAL(2, first_plugin->pre_action_sequence);
  CHECK_EQUAL(3, first_plugin->post_action_sequence);
  CHECK_EQUAL(4, third_plugin->post_action_sequence);
  CHECK_EQUAL(2, registry->count_plugins());
}

TEST(Plugin, RemovePluginByName)
{
  registry->install_plugin(second_plugin);
  registry->install_plugin(third_plugin);
  CHECK_EQUAL(3, registry->count_plugins());
  registry->remove_plugin_by_name(GENERIC_PLUGIN2);
  CHECK_EQUAL(2, registry->count_plugins());
}

struct DefaultPlugin : public mu::tiny::test::Plugin
{
  DefaultPlugin()
    : Plugin("default")
  {
  }
};

TEST(Plugin, DefaultPostTestActionDoesntDoAnything)
{
  DefaultPlugin default_plugin;
  registry->install_plugin(&default_plugin);
  gen_fixture->run_all_tests();
}

TEST(Plugin, DisablesPluginsDontRun)
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

TEST(Plugin, ParseArgumentsForUnknownArgumentsFails)
{
  registry->install_plugin(second_plugin);
  const char* cmd_line[] = { "nonsense", "andmorenonsense" };
  CHECK(
      registry->get_first_plugin()->parse_all_arguments(
          2, const_cast<char**>(cmd_line), 0
      ) == false
  ); /* cover non-const wrapper, too */
}

TEST(Plugin, ParseArgumentsContinuesAndSucceedsWhenAPluginCanParse)
{
  registry->install_plugin(second_plugin);
  const char* cmd_line[] = { "-paccept", "andmorenonsense" };
  CHECK(registry->get_first_plugin()->parse_all_arguments(
      2, const_cast<char**>(cmd_line), 0
  )); /* cover non-const wrapper, too */
}
