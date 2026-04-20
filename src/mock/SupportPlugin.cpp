#include "mu/tiny/mock/SupportPlugin.hpp"

#include "mu/tiny/mock/Failure.hpp"

#include "mu/tiny/test/Result.hpp"
#include "mu/tiny/test/Shell.hpp"

#include "mu/tiny/mock.hpp"

namespace mu {
namespace tiny {
namespace mock {

namespace {
class MockSupportPluginReporter : public FailureReporter
{
  test::Shell& test_;
  test::Result& result_;

public:
  MockSupportPluginReporter(test::Shell& test, test::Result& result)
    : test_(test)
    , result_(result)
  {
  }

  void fail_test(Failure failure) override { result_.add_failure(failure); }

  test::Shell* get_test_to_fail() override { return &test_; }
};
} // namespace

SupportPlugin::SupportPlugin(const String& name)
  : Plugin(name)
{
}

SupportPlugin::~SupportPlugin()
{
  clear();
}

void SupportPlugin::clear()
{
  repository_.clear();
}

void SupportPlugin::
    pre_test_action(test::Shell& /*test*/, test::Result& /*result*/)
{
  mock().install_comparators_and_copiers(repository_);
}

void SupportPlugin::post_test_action(test::Shell& test, test::Result& result)
{
  MockSupportPluginReporter reporter(test, result);
  mock().set_mock_failure_standard_reporter(&reporter);
  if (!test.has_failed()) {
    mock().check_expectations();
  }
  mock().clear();
  mock().set_mock_failure_standard_reporter(nullptr);
  mock().remove_all_comparators_and_copiers();
}

void SupportPlugin::install_comparator(
    StringView name,
    NamedValueComparator& comparator
)
{
  repository_.install_comparator(name, comparator);
}

void SupportPlugin::install_copier(StringView name, NamedValueCopier& copier)
{
  repository_.install_copier(name, copier);
}

} // namespace mock
} // namespace tiny
} // namespace mu
