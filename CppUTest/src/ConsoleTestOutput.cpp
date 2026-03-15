#include "CppUTest/ConsoleTestOutput.hpp"

#include <stdio.h>

namespace cpputest {

void
ConsoleTestOutput::printBuffer(const char* s)
{
  cpputest::FPuts(s, cpputest::StdOut);
  flush();
}

void
ConsoleTestOutput::flush()
{
  fflush(stdout);
}

} // namespace cpputest
