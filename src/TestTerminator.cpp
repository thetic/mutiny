#include "mutiny/test/Shell.hpp"
#include "mutiny/test/Terminator.hpp"
#include "mutiny/test/jump_buffer.h"

namespace mu {
namespace tiny {
namespace test {

void NormalTestTerminator::exit_current_test() const
{
#if MUTINY_HAVE_EXCEPTIONS
  throw FailedException();
#else
  TestTerminatorWithoutExceptions().exit_current_test();
#endif
}

void TestTerminatorWithoutExceptions::exit_current_test() const
{
  mutiny_long_jmp();
}

void CrashingTestTerminator::exit_current_test() const
{
  TestShell::crash();
  NormalTestTerminator::exit_current_test();
}

void CrashingTestTerminatorWithoutExceptions::exit_current_test() const
{
  TestShell::crash();
  TestTerminatorWithoutExceptions::exit_current_test();
}

}
}
} // namespace mu::tiny::test
