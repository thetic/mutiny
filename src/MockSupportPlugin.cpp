#include "CppMu/MockSupportPlugin.hpp"

#include "CppMu/CppMu.hpp"
#include "CppMu/MockFailure.hpp"
#include "CppMu/MockSupport.hpp"

namespace cppmu {

class MockSupportPluginReporter : public MockFailureReporter
{
  cppmu::TestShell& test_;
  TestResult& result_;

public:
  MockSupportPluginReporter(cppmu::TestShell& test, TestResult& result)
    : test_(test)
    , result_(result)
  {
  }

  virtual void fail_test(MockFailure failure) override
  {
    result_.add_failure(failure);
  }

  virtual void report_failure(const MockFailure& failure) override
  {
    result_.add_failure(failure);
  }

  virtual void exit_test() override {}

  virtual cppmu::TestShell* get_test_to_fail() override { return &test_; }
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
MockSupportPlugin::pre_test_action(cppmu::TestShell&, TestResult&)
{
  mock().install_comparators_and_copiers(repository_);
}

void
MockSupportPlugin::post_test_action(cppmu::TestShell& test, TestResult& result)
{
  MockSupportPluginReporter reporter(test, result);
  mock().set_mock_failure_standard_reporter(&reporter);
  if (!test.has_failed())
    mock().check_expectations();
  mock().clear();
  mock().set_mock_failure_standard_reporter(nullptr);
  mock().remove_all_comparators_and_copiers();
}

void
MockSupportPlugin::install_comparator(const String& name,
    MockNamedValueComparator& comparator)
{
  repository_.install_comparator(name, comparator);
}

void
MockSupportPlugin::install_copier(const String& name,
    MockNamedValueCopier& copier)
{
  repository_.install_copier(name, copier);
}

} // namespace cppmu
