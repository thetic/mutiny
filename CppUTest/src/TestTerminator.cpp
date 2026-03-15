#include "CppUTest/TestTerminator.hpp"

#include "CppUTest/TestJumpBuffer.hpp"
#include "CppUTest/TestShell.hpp"

namespace cpputest {

TestTerminator::~TestTerminator() {}

void
NormalTestTerminator::exit_current_test() const
{
#if CPPUTEST_HAVE_EXCEPTIONS
  throw FailedException();
#else
  TestTerminatorWithoutExceptions().exitCurrentTest();
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

} // namespace cpputest
