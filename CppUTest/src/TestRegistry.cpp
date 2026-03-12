#include "CppUTest/TestRegistry.hpp"

#include "CppUTest/TestHarness.hpp"

namespace cpputest {

TestRegistry::TestRegistry()
  : tests_(nullptr)
  , nameFilters_(nullptr)
  , groupFilters_(nullptr)
  , firstPlugin_(NullTestPlugin::instance())
  , currentRepetition_(0)
  , runIgnored_(false)
{
}

TestRegistry::~TestRegistry() {}

void
TestRegistry::addTest(TestShell* test)
{
  tests_ = test->addTest(tests_);
}

void
TestRegistry::runAllTests(TestResult& result)
{
  bool groupStart = true;

  result.testsStarted();
  for (TestShell* test = tests_; test != nullptr; test = test->getNext()) {
    if (runIgnored_)
      test->setRunIgnored();

    if (groupStart) {
      result.currentGroupStarted(test);
      groupStart = false;
    }

    result.countTest();
    if (testShouldRun(test, result)) {
      result.currentTestStarted(test);
      test->runOneTest(firstPlugin_, result);
      result.currentTestEnded(test);
    }

    if (endOfGroup(test)) {
      groupStart = true;
      result.currentGroupEnded(test);
    }
  }
  result.testsEnded();
  currentRepetition_++;
}

void
TestRegistry::listTestGroupNames(TestResult& result)
{
  String groupList;

  for (TestShell* test = tests_; test != nullptr; test = test->getNext()) {
    String gname;
    gname += "#";
    gname += test->getGroup();
    gname += "#";

    if (!stringContains(groupList, gname)) {
      groupList += gname;
      groupList += " ";
    }
  }

  groupList.replace("#", "");

  if (stringEndsWith(groupList, " "))
    groupList = groupList.substr(0, groupList.size() - 1);
  result.print(groupList.c_str());
}

void
TestRegistry::listTestGroupAndCaseNames(TestResult& result)
{
  String groupAndNameList;

  for (TestShell* test = tests_; test != nullptr; test = test->getNext()) {
    if (testShouldRun(test, result)) {
      String groupAndName;
      groupAndName += "#";
      groupAndName += test->getGroup();
      groupAndName += ".";
      groupAndName += test->getName();
      groupAndName += "#";

      if (!stringContains(groupAndNameList, groupAndName)) {
        groupAndNameList += groupAndName;
        groupAndNameList += " ";
      }
    }
  }

  groupAndNameList.replace("#", "");

  if (stringEndsWith(groupAndNameList, " "))
    groupAndNameList = groupAndNameList.substr(0, groupAndNameList.size() - 1);
  result.print(groupAndNameList.c_str());
}

void
TestRegistry::listTestLocations(TestResult& result)
{
  String testLocations;

  for (TestShell* test = tests_; test != nullptr; test = test->getNext()) {
    String testLocation;
    testLocation += test->getGroup();
    testLocation += ".";
    testLocation += test->getName();
    testLocation += ".";
    testLocation += test->getFile();
    testLocation += ".";
    testLocation +=
        StringFromFormat("%d\n", static_cast<int>(test->getLineNumber()));

    testLocations += testLocation;
  }

  result.print(testLocations.c_str());
}

bool
TestRegistry::endOfGroup(TestShell* test)
{
  return (!test || !test->getNext() ||
          test->getGroup() != test->getNext()->getGroup());
}

size_t
TestRegistry::countTests()
{
  return tests_ ? tests_->countTests() : 0;
}

TestRegistry* TestRegistry::currentRegistry_ = nullptr;

TestRegistry*
TestRegistry::getCurrentRegistry()
{
  static TestRegistry registry;
  return (currentRegistry_ == nullptr) ? &registry : currentRegistry_;
}

void
TestRegistry::setCurrentRegistry(TestRegistry* registry)
{
  currentRegistry_ = registry;
}

void
TestRegistry::unDoLastAddTest()
{
  tests_ = tests_ ? tests_->getNext() : nullptr;
}

void
TestRegistry::setNameFilters(const TestFilter* filters)
{
  nameFilters_ = filters;
}

void
TestRegistry::setGroupFilters(const TestFilter* filters)
{
  groupFilters_ = filters;
}

void
TestRegistry::setRunIgnored()
{
  runIgnored_ = true;
}

int
TestRegistry::getCurrentRepetition()
{
  return currentRepetition_;
}

bool
TestRegistry::testShouldRun(TestShell* test, TestResult& result)
{
  if (test->shouldRun(groupFilters_, nameFilters_))
    return true;
  else {
    result.countFilteredOut();
    return false;
  }
}

void
TestRegistry::resetPlugins()
{
  firstPlugin_ = NullTestPlugin::instance();
}

void
TestRegistry::installPlugin(TestPlugin* plugin)
{
  firstPlugin_ = plugin->addPlugin(firstPlugin_);
}

TestPlugin*
TestRegistry::getFirstPlugin()
{
  return firstPlugin_;
}

TestPlugin*
TestRegistry::getPluginByName(const String& name)
{
  return firstPlugin_->getPluginByName(name);
}

void
TestRegistry::removePluginByName(const String& name)
{
  if (firstPlugin_->removePluginByName(name) == firstPlugin_)
    firstPlugin_ = firstPlugin_->getNext();
  if (firstPlugin_->getName() == name)
    firstPlugin_ = firstPlugin_->getNext();
  firstPlugin_->removePluginByName(name);
}

int
TestRegistry::countPlugins()
{
  int count = 0;
  for (TestPlugin* plugin = firstPlugin_; plugin != NullTestPlugin::instance();
      plugin = plugin->getNext())
    count++;
  return count;
}

TestShell*
TestRegistry::getFirstTest()
{
  return tests_;
}

void
TestRegistry::shuffleTests(size_t seed)
{
  TestShellPointerArray array(getFirstTest());
  array.shuffle(seed);
  tests_ = array.getFirstTest();
}

void
TestRegistry::reverseTests()
{
  TestShellPointerArray array(getFirstTest());
  array.reverse();
  tests_ = array.getFirstTest();
}

TestShell*
TestRegistry::getTestWithNext(TestShell* test)
{
  TestShell* current = tests_;
  while (current && current->getNext() != test)
    current = current->getNext();
  return current;
}

TestShell*
TestRegistry::findTestWithName(const String& name)
{
  TestShell* current = tests_;
  while (current) {
    if (current->getName() == name)
      return current;
    current = current->getNext();
  }
  return nullptr;
}

TestShell*
TestRegistry::findTestWithGroup(const String& group)
{
  TestShell* current = tests_;
  while (current) {
    if (current->getGroup() == group)
      return current;
    current = current->getNext();
  }
  return nullptr;
}

} // namespace cpputest
