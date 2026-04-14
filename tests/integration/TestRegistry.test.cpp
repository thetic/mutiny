#include "mutiny/test/GroupLocation.hpp"
#include "mutiny/test/Ordered.hpp"
#include "mutiny/test/Output.hpp"
#include "mutiny/test/Registry.hpp"
#include "mutiny/test/StringBufferOutput.hpp"

#include "mutiny/test.hpp"

namespace {
const int test_line_number = 1;

int get_zero()
{
  return 0;
}

class MockTest : public mu::tiny::test::Shell
{
public:
  MockTest(const char* group = "Group")
    : Shell(group, "Name", "File", test_line_number)

  {
  }
  void run_one_test(mu::tiny::test::Plugin*, mu::tiny::test::Result&) override
  {
    has_run = true;
  }

  bool has_run{ false };
};

class MockOrderedTest : public mu::tiny::test::OrderedShell
{
public:
  void run_one_test(mu::tiny::test::Plugin*, mu::tiny::test::Result&) override
  {
  }
};

class MockTestResult : public mu::tiny::test::Result
{
public:
  int count_tests_started;
  int count_tests_ended;
  int count_current_test_started;
  int count_current_test_ended;
  int count_current_group_started;
  int count_current_group_ended;

  MockTestResult(mu::tiny::test::Output& p)
    : Result(p)
  {
    reset_count();
  }

  ~MockTestResult() override = default;

  void reset_count()
  {
    count_tests_started = 0;
    count_tests_ended = 0;
    count_current_test_started = 0;
    count_current_test_ended = 0;
    count_current_group_started = 0;
    count_current_group_ended = 0;
  }

  void tests_started() override { count_tests_started++; }
  void tests_ended() override { count_tests_ended++; }
  void current_test_started(mu::tiny::test::Shell* /*test*/) override
  {
    count_current_test_started++;
  }
  void current_test_ended(mu::tiny::test::Shell* /*test*/) override
  {
    count_current_test_ended++;
  }
  void current_group_started(mu::tiny::test::Shell* /*test*/) override
  {
    count_current_group_started++;
  }
  void current_group_ended(mu::tiny::test::Shell* /*test*/) override
  {
    count_current_group_ended++;
  }
};

class MyTestPluginDummy : public mu::tiny::test::Plugin
{
public:
  MyTestPluginDummy(const mu::tiny::String& name)
    : Plugin(name)
  {
  }
  ~MyTestPluginDummy() override = default;
  void run_all_pre_test_action(
      mu::tiny::test::Shell&,
      mu::tiny::test::Result&
  ) override
  {
  }
  void run_all_post_test_action(
      mu::tiny::test::Shell&,
      mu::tiny::test::Result&
  ) override
  {
  }
};

} // namespace

TEST_GROUP(Registry)
{
  mu::tiny::test::Registry* my_registry;
  mu::tiny::test::StringBufferOutput* output;
  MockTest* test1;
  MockTest* test2;
  MockTest* test3;
  MockTest* test4;
  MockOrderedTest* ordered_test;
  mu::tiny::test::Result* result;
  MockTestResult* mock_result;
  void setup() override
  {
    output = new mu::tiny::test::StringBufferOutput();
    mock_result = new MockTestResult(*output);
    result = mock_result;
    test1 = new MockTest();
    test2 = new MockTest();
    test3 = new MockTest("group2");
    test4 = new MockTest();
    ordered_test = new MockOrderedTest();
    my_registry = new mu::tiny::test::Registry();
    my_registry->set_current_registry(my_registry);
  }

  void teardown() override
  {
    my_registry->set_current_registry(nullptr);
    delete my_registry;
    delete test1;
    delete test2;
    delete test3;
    delete test4;
    delete ordered_test;
    delete result;
    delete output;
  }

  void add_and_run_all_tests()
  {
    my_registry->add_test(test1);
    my_registry->add_test(test2);
    my_registry->add_test(test3);
    my_registry->run_all_tests(*result);
  }
};

TEST(Registry, registryMyRegistryAndReset)
{
  CHECK(my_registry->get_current_registry() == my_registry);
}

TEST(Registry, emptyRegistryIsEmpty)
{
  CHECK(my_registry->count_tests() == 0);
}

TEST(Registry, addOneTestIsNotEmpty)
{
  my_registry->add_test(test1);
  CHECK(my_registry->count_tests() == 1);
}

TEST(Registry, addOneTwoTests)
{
  my_registry->add_test(test1);
  my_registry->add_test(test2);
  CHECK(my_registry->count_tests() == 2);
}

TEST(Registry, runTwoTests)
{
  my_registry->add_test(test1);
  my_registry->add_test(test2);
  CHECK(!test1->has_run);
  CHECK(!test2->has_run);
  my_registry->run_all_tests(*result);
  CHECK(test1->has_run);
  CHECK(test2->has_run);
}

TEST(Registry, runTwoTestsCheckResultFunctionsCalled)
{
  my_registry->add_test(test1);
  my_registry->add_test(test2);
  my_registry->run_all_tests(*result);
  CHECK_EQUAL(1, mock_result->count_tests_started);
  CHECK_EQUAL(1, mock_result->count_tests_ended);
  CHECK_EQUAL(1, mock_result->count_current_group_started);
  CHECK_EQUAL(1, mock_result->count_current_group_ended);
  CHECK_EQUAL(2, mock_result->count_current_test_started);
  CHECK_EQUAL(2, mock_result->count_current_test_ended);
}

TEST(Registry, runThreeTestsandTwoGroupsCheckResultFunctionsCalled)
{
  add_and_run_all_tests();
  CHECK_EQUAL(2, mock_result->count_current_group_started);
  CHECK_EQUAL(2, mock_result->count_current_group_ended);
  CHECK_EQUAL(3, mock_result->count_current_test_started);
  CHECK_EQUAL(3, mock_result->count_current_test_ended);
}

TEST(Registry, unDoTest)
{
  my_registry->add_test(test1);
  CHECK(my_registry->count_tests() == 1);
  my_registry->un_do_last_add_test();
  CHECK(my_registry->count_tests() == 0);
}

TEST(Registry, unDoButNoTest)
{
  CHECK(my_registry->count_tests() == 0);
  my_registry->un_do_last_add_test();
  CHECK(my_registry->count_tests() == 0);
}

TEST(Registry, reallyUndoLastTest)
{
  my_registry->add_test(test1);
  my_registry->add_test(test2);
  CHECK(my_registry->count_tests() == 2);
  my_registry->un_do_last_add_test();
  CHECK(my_registry->count_tests() == 1);
  my_registry->run_all_tests(*result);
  CHECK(test1->has_run);
  CHECK(!test2->has_run);
}

TEST(Registry, findTestWithNameDoesntExist)
{
  CHECK(my_registry->find_test_with_name("ThisTestDoesntExists") == nullptr);
}

TEST(Registry, findTestWithName)
{
  test1->set_test_name("NameOfATestThatDoesExist");
  test2->set_test_name("SomeOtherTest");
  my_registry->add_test(test1);
  my_registry->add_test(test2);
  CHECK(
      my_registry->find_test_with_name("NameOfATestThatDoesExist") != nullptr
  );
}

TEST(Registry, findTestWithGroupDoesntExist)
{
  CHECK(
      my_registry->find_test_with_group("ThisTestGroupDoesntExists") == nullptr
  );
}

TEST(Registry, findTestWithGroup)
{
  test1->set_group_name("GroupOfATestThatDoesExist");
  test2->set_group_name("SomeOtherGroup");
  my_registry->add_test(test1);
  my_registry->add_test(test2);
  CHECK(
      my_registry->find_test_with_group("GroupOfATestThatDoesExist") != nullptr
  );
}

TEST(Registry, nameFilterWorks)
{
  test1->set_test_name("testname");
  test2->set_test_name("noname");
  mu::tiny::test::Filter name_filter("testname");
  my_registry->set_name_filters(&name_filter);
  add_and_run_all_tests();
  CHECK(test1->has_run);
  CHECK(!test2->has_run);
}

TEST(Registry, groupFilterWorks)
{
  test1->set_group_name("groupname");
  test2->set_group_name("noname");
  mu::tiny::test::Filter group_filter("groupname");
  my_registry->set_group_filters(&group_filter);
  add_and_run_all_tests();
  CHECK(test1->has_run);
  CHECK(!test2->has_run);
}

TEST(Registry, CurrentRepetitionIsCorrectNone)
{
  CHECK(0 == my_registry->get_current_repetition());
  my_registry->run_all_tests(*result);
  CHECK_EQUAL(1, my_registry->get_current_repetition());
}

TEST(Registry, CurrentRepetitionIsCorrectTwo)
{
  CHECK(0 == my_registry->get_current_repetition());
  my_registry->run_all_tests(*result);
  my_registry->run_all_tests(*result);
  CHECK_EQUAL(2, my_registry->get_current_repetition());
}

TEST(Registry, ResetPluginsWorks)
{
  MyTestPluginDummy plugin1("Plugin-1");
  MyTestPluginDummy plugin2("Plugin-2");
  my_registry->install_plugin(&plugin1);
  my_registry->install_plugin(&plugin2);
  CHECK_EQUAL(2, my_registry->count_plugins());
  my_registry->reset_plugins();
  CHECK_EQUAL(0, my_registry->count_plugins());
}

TEST(Registry, listTestGroupNames_shouldListBackwardsGroup1AfterGroup11AndGroup2OnlyOnce)
{
  test1->set_group_name("GROUP_1");
  my_registry->add_test(test1);
  test2->set_group_name("GROUP_2");
  my_registry->add_test(test2);
  test3->set_group_name("GROUP_11");
  my_registry->add_test(test3);
  test4->set_group_name("GROUP_2");
  my_registry->add_test(test4);

  my_registry->list_test_group_names(*result);
  mu::tiny::String s = output->get_output();
  STRCMP_EQUAL("GROUP_2 GROUP_11 GROUP_1", s.c_str());
}

TEST(Registry, listTestGroupAndCaseNames_shouldListBackwardsGroupATestaAfterGroupAtestaa)
{
  test1->set_group_name("GROUP_A");
  test1->set_test_name("test_a");
  my_registry->add_test(test1);
  test2->set_group_name("GROUP_B");
  test2->set_test_name("test_b");
  my_registry->add_test(test2);
  test3->set_group_name("GROUP_A");
  test3->set_test_name("test_aa");
  my_registry->add_test(test3);

  my_registry->list_test_group_and_case_names(*result);
  mu::tiny::String s = output->get_output();
  STRCMP_EQUAL("GROUP_A.test_aa GROUP_B.test_b GROUP_A.test_a", s.c_str());
}

TEST(Registry, listTestLocations_shouldListBackwardsGroupATestaAfterGroupAtestaa)
{
  test1->set_group_name("GROUP_A");
  test1->set_test_name("test_a");
  test1->set_file_name("cpptest_simple/my_tests/testa.cpp");
  test1->set_line_number(100);
  my_registry->add_test(test1);
  test2->set_group_name("GROUP_B");
  test2->set_test_name("test_b");
  test2->set_file_name("cpptest_simple/my tests/testb.cpp");
  test2->set_line_number(200);
  my_registry->add_test(test2);
  test3->set_group_name("GROUP_A");
  test3->set_test_name("test_aa");
  test3->set_file_name("cpptest_simple/my_tests/testaa.cpp");
  test3->set_line_number(300);
  my_registry->add_test(test3);

  my_registry->list_test_locations(*result);
  mu::tiny::String s = output->get_output();
  STRCMP_EQUAL(
      "GROUP_A.test_aa.cpptest_simple/my_tests/"
      "testaa.cpp.300\nGROUP_B.test_b.cpptest_simple/my "
      "tests/testb.cpp.200\nGROUP_A.test_a.cpptest_simple/my_tests/"
      "testa.cpp.100\n",
      s.c_str()
  );
}

TEST(Registry, listOrderedTestLocations_onlyIncludesOrderedTests)
{
  ordered_test->set_group_name("GROUP_A");
  ordered_test->set_test_name("test_a");
  ordered_test->set_file_name("my_tests/testa.cpp");
  ordered_test->set_line_number(100);
  my_registry->add_test(ordered_test);

  test1->set_group_name("GROUP_B");
  test1->set_test_name("test_b");
  my_registry->add_test(test1);

  my_registry->list_ordered_test_locations(*result);
  STRCMP_EQUAL(
      "GROUP_A.test_a.my_tests/testa.cpp.100\n", output->get_output().c_str()
  );
}

TEST(Registry, listTestGroupLocations_outputsGroupFileLine)
{
  mu::tiny::test::GroupLocation* saved_head =
      mu::tiny::test::GroupLocation::get_head();
  mu::tiny::test::GroupLocation::set_head(nullptr);

  mu::tiny::test::GroupLocation loc_b("GROUP_B", "tests/group_b.cpp", 5);
  mu::tiny::test::GroupLocation loc_a("GROUP_A", "tests/group a.cpp", 12);

  my_registry->list_test_group_locations(*result);
  STRCMP_EQUAL(
      "GROUP_A.tests/group a.cpp.12\nGROUP_B.tests/group_b.cpp.5\n",
      output->get_output().c_str()
  );

  mu::tiny::test::GroupLocation::set_head(saved_head);
}

TEST(Registry, shuffleEmptyListIsNoOp)
{
  CHECK(my_registry->get_first_test() == nullptr);
  my_registry->shuffle_tests(0);
  CHECK(my_registry->get_first_test() == nullptr);
}

TEST(Registry, shuffleSingleTestIsNoOp)
{
  my_registry->add_test(test1);
  my_registry->shuffle_tests(0);
  CHECK(my_registry->get_first_test() == test1);
}

IGNORE_TEST(Registry, shuffleTestList)
{
  MUTINY_PTR_SET(mu::tiny::test::rand, get_zero);
  my_registry->add_test(test3);
  my_registry->add_test(test2);
  my_registry->add_test(test1);

  mu::tiny::test::Shell* first_before = my_registry->get_first_test();
  mu::tiny::test::Shell* second_before = first_before->get_next();
  mu::tiny::test::Shell* third_before = second_before->get_next();

  CHECK(first_before == test1);
  CHECK(second_before == test2);
  CHECK(third_before == test3);
  CHECK(third_before->get_next() == nullptr);

  // shuffle always with element at index 0: [1] 2 [3] --> [3] [2] 1 --> 2 3 1
  my_registry->shuffle_tests(0);

  mu::tiny::test::Shell* first_after = my_registry->get_first_test();
  mu::tiny::test::Shell* second_after = first_after->get_next();
  mu::tiny::test::Shell* third_after = second_after->get_next();

  CHECK(first_after == test2);
  CHECK(second_after == test3);
  CHECK(third_after == test1);
  CHECK(third_after->get_next() == nullptr);
}

TEST(Registry, reverseTests)
{
  my_registry->add_test(test1);
  my_registry->add_test(test2);

  my_registry->reverse_tests();

  CHECK(test1 == my_registry->get_first_test());
}

TEST(Registry, reverseZeroTests)
{
  my_registry->reverse_tests();

  CHECK(nullptr == my_registry->get_first_test());
}
