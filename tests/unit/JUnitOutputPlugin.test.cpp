#include "mutiny/test/JUnitOutputPlugin.hpp"

#include "mutiny/test/JUnitOutput.hpp"

#include "mutiny/test.hpp"

TEST_GROUP(JUnitOutputPlugin) {};

TEST(JUnitOutputPlugin, doesNotActivateOnUnrelatedArg)
{
  mu::tiny::test::JUnitOutputPlugin plugin;
  const char* argv[] = { "tests", "-v" };
  CHECK(!plugin.parse_arguments(2, argv, 1));
  CHECK(plugin.create_output() == nullptr);
}

TEST(JUnitOutputPlugin, activatesOnBarePjunit)
{
  mu::tiny::test::JUnitOutputPlugin plugin;
  const char* argv[] = { "tests", "-pjunit" };
  CHECK(plugin.parse_arguments(2, argv, 1));
  auto* out = static_cast<mu::tiny::test::JUnitOutput*>(plugin.create_output());
  STRCMP_EQUAL("tests.xml", out->create_file_name().c_str());
  delete out;
}

TEST(JUnitOutputPlugin, stripsDirectoryFromExecutableName)
{
  mu::tiny::test::JUnitOutputPlugin plugin;
  const char* argv[] = { "/path/to/tests", "-pjunit" };
  CHECK(plugin.parse_arguments(2, argv, 1));
  auto* out = static_cast<mu::tiny::test::JUnitOutput*>(plugin.create_output());
  STRCMP_EQUAL("tests.xml", out->create_file_name().c_str());
  delete out;
}

TEST(JUnitOutputPlugin, emptyExecutableNameFallsBackToDefaultFileName)
{
  mu::tiny::test::JUnitOutputPlugin plugin;
  const char* argv[] = { "", "-pjunit" };
  CHECK(plugin.parse_arguments(2, argv, 1));
  auto* out = static_cast<mu::tiny::test::JUnitOutput*>(plugin.create_output());
  STRCMP_EQUAL("mutiny.xml", out->create_file_name().c_str());
  delete out;
}

TEST(JUnitOutputPlugin, rejectsPjunitWithNonEqualsSuffix)
{
  mu::tiny::test::JUnitOutputPlugin plugin;
  const char* argv[] = { "tests", "-pjunitX" };
  CHECK(!plugin.parse_arguments(2, argv, 1));
  CHECK(plugin.create_output() == nullptr);
}

TEST(JUnitOutputPlugin, usesExplicitPackageName)
{
  mu::tiny::test::JUnitOutputPlugin plugin;
  const char* argv[] = { "tests", "-pjunit=mypackage" };
  CHECK(plugin.parse_arguments(2, argv, 1));
  auto* out = static_cast<mu::tiny::test::JUnitOutput*>(plugin.create_output());
  STRCMP_EQUAL("mypackage.xml", out->create_file_name().c_str());
  delete out;
}
