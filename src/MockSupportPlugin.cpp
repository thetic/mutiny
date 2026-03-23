#include "mutiny/mock.hpp"
#include "mutiny/mock/Failure.hpp"
#include "mutiny/mock/SupportPlugin.hpp"
#include "mutiny/test/Result.hpp"
#include "mutiny/test/Shell.hpp"

namespace mu {
namespace tiny {
namespace mock {
using namespace mu::tiny::test;

namespace {
class MockSupportPluginReporter : public MockFailureReporter
{
  mu::tiny::test::TestShell& test_;
  TestResult& result_;

public:
  MockSupportPluginReporter(mu::tiny::test::TestShell& test, TestResult& result)
    : test_(test)
    , result_(result)
  {
  }

  void fail_test(MockFailure failure) override { result_.add_failure(failure); }

  mu::tiny::test::TestShell* get_test_to_fail() override { return &test_; }
};
}

MockSupportPlugin::MockSupportPlugin(const String& name)
  : TestPlugin(name)
{
}

MockSupportPlugin::~MockSupportPlugin()
{
  clear();
}

void MockSupportPlugin::clear()
{
  repository_.clear();
}

void MockSupportPlugin::pre_test_action(mu::tiny::test::TestShell&, TestResult&)
{
  mock().install_comparators_and_copiers(repository_);
}

void MockSupportPlugin::post_test_action(
    mu::tiny::test::TestShell& test,
    TestResult& result
)
{
  MockSupportPluginReporter reporter(test, result);
  mock().set_mock_failure_standard_reporter(&reporter);
  if (!test.has_failed())
    mock().check_expectations();
  mock().clear();
  mock().set_mock_failure_standard_reporter(nullptr);
  mock().remove_all_comparators_and_copiers();
}

void MockSupportPlugin::install_comparator(
    const String& name,
    MockNamedValueComparator& comparator
)
{
  repository_.install_comparator(name, comparator);
}

void MockSupportPlugin::install_copier(
    const String& name,
    MockNamedValueCopier& copier
)
{
  repository_.install_copier(name, copier);
}

}
}
} // namespace mu::tiny::mock
