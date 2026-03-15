#ifndef INCLUDED_CPPUTEST_SETPOINTERPLUGIN_HPP
#define INCLUDED_CPPUTEST_SETPOINTERPLUGIN_HPP

#include "CppUTest/TestPlugin.hpp"

namespace cpputest {

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
  virtual void postTestAction(TestShell&, TestResult&) override;

  static constexpr int MAX_SET = 32;

  template<typename T>
  static void set_pointer(T*& a, T* b)
  {
    store(reinterpret_cast<void**>(&(a)));
    a = b;
  }

private:
  static void store(void** location);
};

} // namespace cpputest

#define UT_PTR_SET(a, b) cpputest::SetPointerPlugin::set_pointer((a), b)

#endif
