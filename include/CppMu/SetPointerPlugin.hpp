#ifndef INCLUDED_CPPMU_SETPOINTERPLUGIN_HPP
#define INCLUDED_CPPMU_SETPOINTERPLUGIN_HPP

#include "CppMu/TestPlugin.hpp"

namespace cppmu {

///////////////////////////////////////////////////////////////////////////////
//
// SetPointerPlugin
//
// This is a very small plugin_ that resets pointers to their original value.
//
///////////////////////////////////////////////////////////////////////////////

class SetPointerPlugin : public TestPlugin
{
public:
  SetPointerPlugin(const String& name);
  virtual void post_test_action(TestShell&, TestResult&) override;

  static constexpr int max_set = 32;

  template<typename T>
  static void set_pointer(T*& a, T* b)
  {
    store(reinterpret_cast<void**>(&(a)));
    a = b;
  }

private:
  static void store(void** location);
};

} // namespace cppmu

#define CPPMU_PTR_SET(a, b) cppmu::SetPointerPlugin::set_pointer((a), b)

#endif
