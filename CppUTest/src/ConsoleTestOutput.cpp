#include "CppUTest/ConsoleTestOutput.hpp"

#include "CppUTest/PlatformSpecificFunctions.hpp"

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
  PlatformSpecificFlush();
}

} // namespace cpputest
