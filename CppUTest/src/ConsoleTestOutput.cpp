#include "CppUTest/ConsoleTestOutput.hpp"

#include <stdio.h>

namespace cpputest {

void
ConsoleTestOutput::print_buffer(const char* s)
{
  cpputest::f_puts(s, cpputest::std_out);
  flush();
}

void
ConsoleTestOutput::flush()
{
  fflush(stdout);
}

} // namespace cpputest
