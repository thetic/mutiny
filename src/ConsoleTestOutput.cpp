#include "mutiny/test/ConsoleOutput.hpp"

#include <stdio.h>

namespace mu {
namespace tiny {
namespace test {

void ConsoleTestOutput::print_buffer(const char* s)
{
  TestOutput::fputs_(s, stdout_);
  flush();
}

void ConsoleTestOutput::flush()
{
  fflush(stdout);
}

}
}
} // namespace mu::tiny::test
