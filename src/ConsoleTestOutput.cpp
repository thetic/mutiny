#include "CppMu/ConsoleTestOutput.hpp"

#include <stdio.h>

namespace cppmu {

void
ConsoleTestOutput::print_buffer(const char* s)
{
  cppmu::f_puts(s, cppmu::std_out);
  flush();
}

void
ConsoleTestOutput::flush()
{
  fflush(stdout);
}

} // namespace cppmu
