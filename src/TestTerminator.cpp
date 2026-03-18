#include "CppMu/TestTerminator.hpp"

#include "CppMu/TestShell.hpp"
#include "CppMu/jump_buffer.h"

namespace cppmu {

void NormalTestTerminator::exit_current_test() const
{
#if CPPMU_HAVE_EXCEPTIONS
  throw FailedException();
#else
  TestTerminatorWithoutExceptions().exit_current_test();
#endif
}

void TestTerminatorWithoutExceptions::exit_current_test() const
{
  cppmu_long_jmp();
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

} // namespace cppmu
