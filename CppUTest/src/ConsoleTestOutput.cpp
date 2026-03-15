#include "CppUTest/ConsoleTestOutput.hpp"

#include "CppUTest/PlatformSpecificFunctions.hpp"

#include <stdio.h>

namespace cpputest {

void
ConsoleTestOutput::printBuffer(const char* s)
{
  PlatformSpecificFPuts(s, PlatformSpecificStdOut);
  flush();
}

void
ConsoleTestOutput::flush()
{
  fflush(stdout);
}

} // namespace cpputest
