#include "CppMu/ConsoleTestOutput.hpp"

#include <stdio.h>

namespace cppmu {

void ConsoleTestOutput::print_buffer(const char* s)
{
  TestOutput::fputs_(s, stdout_);
  flush();
}

void ConsoleTestOutput::flush()
{
  fflush(stdout);
}

} // namespace cppmu
