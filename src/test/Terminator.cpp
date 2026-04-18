#include "mu/tiny/test/Terminator.hpp"

#include "mu/tiny/test/Shell.hpp"
#include "mu/tiny/test/jump_buffer.hpp"

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
  long_jump();
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

} // namespace test
} // namespace tiny
} // namespace mu
