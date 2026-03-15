#ifndef INCLUDED_CPPUTEST_EXTENSIONS_MOCKSUPPORTPLUGIN_HPP
#define INCLUDED_CPPUTEST_EXTENSIONS_MOCKSUPPORTPLUGIN_HPP

#include "CppUTest/Extensions/MockNamedValueComparator.hpp"
#include "CppUTest/Extensions/MockNamedValueComparatorsAndCopiersRepository.hpp"
#include "CppUTest/Extensions/MockNamedValueCopier.hpp"

#include "CppUTest/TestPlugin.hpp"

namespace cpputest {
namespace extensions {

class MockSupportPlugin : public cpputest::TestPlugin
{
public:
  MockSupportPlugin(const cpputest::String& name = "MockSupportPLugin");
  virtual ~MockSupportPlugin() override;

  virtual void pre_test_action(cpputest::TestShell&,
      cpputest::TestResult&) override;
  virtual void post_test_action(cpputest::TestShell&,
      cpputest::TestResult&) override;

  virtual void install_comparator(const cpputest::String& name,
      MockNamedValueComparator& comparator);
  virtual void install_copier(const cpputest::String& name,
      MockNamedValueCopier& copier);

  void clear();

private:
  MockNamedValueComparatorsAndCopiersRepository repository_;
};

} // namespace extensions
} // namespace cpputest

#endif
