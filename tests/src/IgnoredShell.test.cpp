#include "mutiny/test/IgnoredShell.hpp"

#include "mutiny/test.hpp"

TEST_GROUP(IgnoredShell){};

TEST(IgnoredShell, getFormattedName_whenRunIgnored_showsTEST)
{
  mu::tiny::test::IgnoredShell shell;
  shell.set_group_name("TestGroup");
  shell.set_test_name("TestName");
  shell.set_run_ignored();
  STRCMP_EQUAL("TEST(TestGroup, TestName)", shell.get_formatted_name().c_str());
}
