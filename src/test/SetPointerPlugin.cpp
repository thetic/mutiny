#include "mutiny/test/SetPointerPlugin.hpp"

#include "mutiny/test/Shell.hpp"

namespace mu {
namespace tiny {
namespace test {

namespace {
struct MutinyPair
{
  void** orig;
  void* orig_value;
};

size_t pointer_table_index;
MutinyPair setlist[SetPointerPlugin::max_set];
} // namespace

SetPointerPlugin::SetPointerPlugin()
  : Plugin(name)
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

void SetPointerPlugin::post_test_action(Shell& /*test*/, Result& /*result*/)
{
  for (size_t i{ pointer_table_index }; i > 0; i--)
    *reinterpret_cast<void**>(setlist[i - 1].orig) = setlist[i - 1].orig_value;
  pointer_table_index = 0;
}

} // namespace test
} // namespace tiny
} // namespace mu
