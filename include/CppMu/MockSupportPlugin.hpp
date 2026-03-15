#ifndef INCLUDED_CPPMU_MOCKSUPPORTPLUGIN_HPP
#define INCLUDED_CPPMU_MOCKSUPPORTPLUGIN_HPP

#include "CppMu/MockNamedValueComparator.hpp"
#include "CppMu/MockNamedValueComparatorsAndCopiersRepository.hpp"
#include "CppMu/MockNamedValueCopier.hpp"
#include "CppMu/TestPlugin.hpp"

namespace cppmu {

class MockSupportPlugin : public cppmu::TestPlugin
{
public:
  MockSupportPlugin(const cppmu::String& name = "MockSupportPLugin");
  virtual ~MockSupportPlugin() override;

  virtual void pre_test_action(cppmu::TestShell&, cppmu::TestResult&) override;
  virtual void post_test_action(cppmu::TestShell&, cppmu::TestResult&) override;

  virtual void install_comparator(const cppmu::String& name,
      MockNamedValueComparator& comparator);
  virtual void install_copier(const cppmu::String& name,
      MockNamedValueCopier& copier);

  void clear();

private:
  MockNamedValueComparatorsAndCopiersRepository repository_;
};

} // namespace cppmu

#endif
