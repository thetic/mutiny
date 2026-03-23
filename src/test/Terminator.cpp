#include "mutiny/test/Terminator.hpp"

#include "mutiny/test/Shell.hpp"
#include "mutiny/test/jump_buffer.h"

namespace mu {
namespace tiny {
namespace test {

void NormalTerminator::exit_current_test() const
{
#if MUTINY_HAVE_EXCEPTIONS
  throw FailedException();
#else
  TerminatorWithoutExceptions().exit_current_test();
#endif
}

void TerminatorWithoutExceptions::exit_current_test() const
{
  mutiny_long_jmp();
}

void CrashingTerminator::exit_current_test() const
{
  Shell::crash();
  NormalTerminator::exit_current_test();
}

void CrashingTerminatorWithoutExceptions::exit_current_test() const
{
  Shell::crash();
  TerminatorWithoutExceptions::exit_current_test();
}

}
}
} // namespace mu::tiny::test
