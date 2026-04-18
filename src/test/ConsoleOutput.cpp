#include "mu/tiny/test/ConsoleOutput.hpp"

#include <stdio.h>

namespace mu {
namespace tiny {
namespace test {

void ConsoleOutput::print_buffer(const char* s)
{
  Output::fputs_(s, stdout_);
  flush();
}

void ConsoleOutput::flush()
{
  fflush(stdout);
}

} // namespace test
} // namespace tiny
} // namespace mu
