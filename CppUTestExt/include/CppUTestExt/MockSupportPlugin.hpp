#ifndef D_MockSupportPlugin_h
#define D_MockSupportPlugin_h

#include "CppUTestExt/MockNamedValueComparator.hpp"
#include "CppUTestExt/MockNamedValueComparatorsAndCopiersRepository.hpp"
#include "CppUTestExt/MockNamedValueCopier.hpp"

#include "CppUTest/TestPlugin.hpp"

namespace cpputest {
namespace extensions {

class MockSupportPlugin : public cpputest::TestPlugin
{
public:
  MockSupportPlugin(const cpputest::String& name = "MockSupportPLugin");
  virtual ~MockSupportPlugin() override;

  virtual void preTestAction(cpputest::TestShell&,
      cpputest::TestResult&) override;
  virtual void postTestAction(cpputest::TestShell&,
      cpputest::TestResult&) override;

  virtual void installComparator(const cpputest::String& name,
      MockNamedValueComparator& comparator);
  virtual void installCopier(const cpputest::String& name,
      MockNamedValueCopier& copier);

  void clear();

private:
  MockNamedValueComparatorsAndCopiersRepository repository_;
};

} // namespace extensions
} // namespace cpputest

#endif
