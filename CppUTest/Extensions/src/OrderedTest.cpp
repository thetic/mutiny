#include "CppUTest/Extensions/OrderedTest.hpp"

#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestRegistry.hpp"

namespace cpputest {
namespace extensions {

OrderedTestShell* OrderedTestShell::_orderedTestsHead = nullptr;

OrderedTestShell::OrderedTestShell()
  : _nextOrderedTest(nullptr)
  , _level(0)
{
}

OrderedTestShell::~OrderedTestShell() {}

int
OrderedTestShell::getLevel()
{
  return _level;
}

void
OrderedTestShell::setLevel(int level)
{
  _level = level;
}

void
OrderedTestShell::setOrderedTestHead(OrderedTestShell* test)
{
  _orderedTestsHead = test;
}

OrderedTestShell*
OrderedTestShell::getOrderedTestHead()
{
  return _orderedTestsHead;
}

bool
OrderedTestShell::firstOrderedTest()
{
  return (getOrderedTestHead() == nullptr);
}

OrderedTestShell*
OrderedTestShell::addOrderedTest(OrderedTestShell* test)
{
  cpputest::TestShell::addTest(test);
  _nextOrderedTest = test;
  return this;
}

void
OrderedTestShell::addOrderedTestToHead(OrderedTestShell* test)
{
  TestRegistry* reg = TestRegistry::getCurrentRegistry();
  cpputest::TestShell* head = getOrderedTestHead();

  if (nullptr == reg->getFirstTest() || head == reg->getFirstTest()) {
    reg->addTest(test);
  } else {
    reg->getTestWithNext(head)->addTest(test);
    test->addTest(head);
  }

  test->_nextOrderedTest = getOrderedTestHead();
  setOrderedTestHead(test);
}

OrderedTestShell*
OrderedTestShell::getNextOrderedTest()
{
  return _nextOrderedTest;
}

OrderedTestInstaller::OrderedTestInstaller(OrderedTestShell& test,
    const char* groupName,
    const char* testName,
    const char* fileName,
    size_t lineNumber,
    int level)
{
  test.setTestName(testName);
  test.setGroupName(groupName);
  test.setFileName(fileName);
  test.setLineNumber(lineNumber);
  test.setLevel(level);

  if (OrderedTestShell::firstOrderedTest())
    OrderedTestShell::addOrderedTestToHead(&test);
  else
    addOrderedTestInOrder(&test);
}

void
OrderedTestInstaller::addOrderedTestInOrder(OrderedTestShell* test)
{
  if (test->getLevel() < OrderedTestShell::getOrderedTestHead()->getLevel())
    OrderedTestShell::addOrderedTestToHead(test);
  else
    addOrderedTestInOrderNotAtHeadPosition(test);
}

void
OrderedTestInstaller::addOrderedTestInOrderNotAtHeadPosition(
    OrderedTestShell* test)
{
  OrderedTestShell* current = OrderedTestShell::getOrderedTestHead();
  while (current->getNextOrderedTest()) {

    if (current->getNextOrderedTest()->getLevel() > test->getLevel()) {
      test->addOrderedTest(current->getNextOrderedTest());
      current->addOrderedTest(test);
      return;
    }
    current = current->getNextOrderedTest();
  }
  test->addOrderedTest(current->getNextOrderedTest());
  current->addOrderedTest(test);
}

OrderedTestInstaller::~OrderedTestInstaller() {}

} // namespace extensions
} // namespace cpputest
