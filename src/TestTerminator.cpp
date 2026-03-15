#include "CppMu/TestTerminator.hpp"

#include "CppMu/TestJumpBuffer.hpp"
#include "CppMu/TestShell.hpp"

namespace cppmu {

TestTerminator::~TestTerminator() {}

void
NormalTestTerminator::exit_current_test() const
{
#if CPPMU_HAVE_EXCEPTIONS
  throw FailedException();
#else
  TestTerminatorWithoutExceptions().exit_current_test();
#endif
}

NormalTestTerminator::~NormalTestTerminator() {}

void
TestTerminatorWithoutExceptions::exit_current_test() const
{
  test_long_jmp();
}

TestTerminatorWithoutExceptions::~TestTerminatorWithoutExceptions() {}

void
CrashingTestTerminator::exit_current_test() const
{
  TestShell::crash();
  NormalTestTerminator::exit_current_test();
}

CrashingTestTerminator::~CrashingTestTerminator() {}

void
CrashingTestTerminatorWithoutExceptions::exit_current_test() const
{
  TestShell::crash();
  TestTerminatorWithoutExceptions::exit_current_test();
}

CrashingTestTerminatorWithoutExceptions::
    ~CrashingTestTerminatorWithoutExceptions()
{
}

} // namespace cppmu
