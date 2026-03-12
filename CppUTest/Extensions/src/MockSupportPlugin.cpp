#include "CppUTest/Extensions/MockSupportPlugin.hpp"

#include "CppUTest/Extensions/MockSupport.hpp"

#include "CppUTest/TestHarness.hpp"

namespace cpputest {
namespace extensions {

class MockSupportPluginReporter : public MockFailureReporter
{
  cpputest::TestShell& test_;
  TestResult& result_;

public:
  MockSupportPluginReporter(cpputest::TestShell& test, TestResult& result)
    : test_(test)
    , result_(result)
  {
  }

  virtual void failTest(MockFailure failure) override
  {
    result_.addFailure(failure);
  }

  virtual void reportFailure(const MockFailure& failure) override
  {
    result_.addFailure(failure);
  }

  virtual void exitTest() override {}

  virtual cpputest::TestShell* getTestToFail() override { return &test_; }
};

MockSupportPlugin::MockSupportPlugin(const String& name)
  : TestPlugin(name)
{
}

MockSupportPlugin::~MockSupportPlugin()
{
  clear();
}

void
MockSupportPlugin::clear()
{
  repository_.clear();
}

void
MockSupportPlugin::preTestAction(cpputest::TestShell&, TestResult&)
{
  mock().installComparatorsAndCopiers(repository_);
}

void
MockSupportPlugin::postTestAction(cpputest::TestShell& test, TestResult& result)
{
  MockSupportPluginReporter reporter(test, result);
  mock().setMockFailureStandardReporter(&reporter);
  if (!test.hasFailed())
    mock().checkExpectations();
  mock().clear();
  mock().setMockFailureStandardReporter(nullptr);
  mock().removeAllComparatorsAndCopiers();
}

void
MockSupportPlugin::installComparator(const String& name,
    MockNamedValueComparator& comparator)
{
  repository_.installComparator(name, comparator);
}

void
MockSupportPlugin::installCopier(const String& name,
    MockNamedValueCopier& copier)
{
  repository_.installCopier(name, copier);
}

} // namespace extensions
} // namespace cpputest
