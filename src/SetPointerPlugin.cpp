#include "CppMu/SetPointerPlugin.hpp"

#include "CppMu/TestShell.hpp"

namespace cppmu {

namespace {
struct CppMuPair
{
  void** orig;
  void* orig_value;
};

int pointer_table_index;
CppMuPair setlist[SetPointerPlugin::max_set];
} // namespace

SetPointerPlugin::SetPointerPlugin(const String& name)
  : TestPlugin(name)
{
  pointer_table_index = 0;
}

void SetPointerPlugin::store(void** function)
{
  if (pointer_table_index >= SetPointerPlugin::max_set) {
    FAIL("Maximum number of function pointers installed!");
  }
  setlist[pointer_table_index].orig_value = *function;
  setlist[pointer_table_index].orig = function;
  pointer_table_index++;
}

void SetPointerPlugin::
    post_test_action(TestShell& /*test*/, TestResult& /*result*/)
{
  for (int i = pointer_table_index - 1; i >= 0; i--)
    *reinterpret_cast<void**>(setlist[i].orig) = setlist[i].orig_value;
  pointer_table_index = 0;
}

}
