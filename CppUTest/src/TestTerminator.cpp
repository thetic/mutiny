#include "CppUTest/TestTerminator.hpp"

#include "CppUTest/PlatformSpecificFunctions.hpp"
#include "CppUTest/TestShell.hpp"

namespace cpputest {

TestTerminator::~TestTerminator() {}

void
NormalTestTerminator::exitCurrentTest() const
{
#if CPPUTEST_HAVE_EXCEPTIONS
  throw FailedException();
#else
  TestTerminatorWithoutExceptions().exitCurrentTest();
#endif
}

NormalTestTerminator::~NormalTestTerminator() {}

void
TestTerminatorWithoutExceptions::exitCurrentTest() const
{
  PlatformSpecificLongJmp();
}

TestTerminatorWithoutExceptions::~TestTerminatorWithoutExceptions() {}

void
CrashingTestTerminator::exitCurrentTest() const
{
  TestShell::crash();
  NormalTestTerminator::exitCurrentTest();
}

CrashingTestTerminator::~CrashingTestTerminator() {}

void
CrashingTestTerminatorWithoutExceptions::exitCurrentTest() const
{
  TestShell::crash();
  TestTerminatorWithoutExceptions::exitCurrentTest();
}

CrashingTestTerminatorWithoutExceptions::
    ~CrashingTestTerminatorWithoutExceptions()
{
}

} // namespace cpputest
