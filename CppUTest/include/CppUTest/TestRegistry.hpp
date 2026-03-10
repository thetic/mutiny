///////////////////////////////////////////////////////////////////////////////
//
// TestRegistry is a collection of tests that can be run
//

#ifndef D_TestRegistry_h
#define D_TestRegistry_h

#include "CppUTest/String.hpp"
#include "CppUTest/TestFilter.hpp"

#include <stddef.h>

namespace cpputest {

class TestShell;
class TestResult;
class TestPlugin;

class TestRegistry
{
public:
  TestRegistry();
  virtual ~TestRegistry();

  virtual void addTest(TestShell* test);
  virtual void unDoLastAddTest();
  virtual size_t countTests();
  virtual void runAllTests(TestResult& result);
  virtual void shuffleTests(size_t seed);
  virtual void reverseTests();
  virtual void listTestGroupNames(TestResult& result);
  virtual void listTestGroupAndCaseNames(TestResult& result);
  virtual void listTestLocations(TestResult& result);
  virtual void setNameFilters(const TestFilter* filters);
  virtual void setGroupFilters(const TestFilter* filters);
  virtual void installPlugin(TestPlugin* plugin);
  virtual void resetPlugins();
  virtual TestPlugin* getFirstPlugin();
  virtual TestPlugin* getPluginByName(const String& name);
  virtual void removePluginByName(const String& name);
  virtual int countPlugins();

  virtual TestShell* getFirstTest();
  virtual TestShell* getTestWithNext(TestShell* test);

  virtual TestShell* findTestWithName(const String& name);
  virtual TestShell* findTestWithGroup(const String& name);

  static TestRegistry* getCurrentRegistry();
  virtual void setCurrentRegistry(TestRegistry* registry);

  int getCurrentRepetition();
  void setRunIgnored();

private:
  bool testShouldRun(TestShell* test, TestResult& result);
  bool endOfGroup(TestShell* test);

  TestShell* tests_;
  const TestFilter* nameFilters_;
  const TestFilter* groupFilters_;
  TestPlugin* firstPlugin_;
  static TestRegistry* currentRegistry_;
  int currentRepetition_;
  bool runIgnored_;
};

} // namespace cpputest

#endif
