#ifndef D_SetPointerPlugin_h
#define D_SetPointerPlugin_h

#include "CppUTest/TestPlugin.hpp"

namespace cpputest {

///////////////////////////////////////////////////////////////////////////////
//
// SetPointerPlugin
//
// This is a very small plugin_ that resets pointers to their original value.
//
///////////////////////////////////////////////////////////////////////////////

void
CppUTestStore(void** location);

class SetPointerPlugin : public TestPlugin
{
public:
  SetPointerPlugin(const String& name);
  virtual void postTestAction(TestShell&, TestResult&) override;

  static constexpr int MAX_SET = 32;
};

} // namespace cpputest

#define UT_PTR_SET(a, b)                                                       \
  do {                                                                         \
    cpputest::CppUTestStore(reinterpret_cast<void**>(&(a)));                   \
    (a) = b;                                                                   \
  } while (0)

#endif
