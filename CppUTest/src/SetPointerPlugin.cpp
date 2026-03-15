#include "CppUTest/SetPointerPlugin.hpp"

#include "CppUTest/TestShell.hpp"
#include "CppUTest/UtestMacros.hpp"

namespace cpputest {

namespace {
struct cpputest_pair
{
  void** orig;
  void* orig_value;
};

int pointerTableIndex;
cpputest_pair setlist[SetPointerPlugin::MAX_SET];
} // namespace

SetPointerPlugin::SetPointerPlugin(const String& name)
  : TestPlugin(name)
{
  pointerTableIndex = 0;
}

void
SetPointerPlugin::store(void** function)
{
  if (pointerTableIndex >= SetPointerPlugin::MAX_SET) {
    FAIL("Maximum number of function pointers installed!");
  }
  setlist[pointerTableIndex].orig_value = *function;
  setlist[pointerTableIndex].orig = function;
  pointerTableIndex++;
}

void
SetPointerPlugin::postTestAction(TestShell& /*test*/, TestResult& /*result*/)
{
  for (int i = pointerTableIndex - 1; i >= 0; i--)
    *reinterpret_cast<void**>(setlist[i].orig) = setlist[i].orig_value;
  pointerTableIndex = 0;
}

}
