#ifndef INCLUDED_MUTINY_CONSOLETESTOUTPUT_HPP
#define INCLUDED_MUTINY_CONSOLETESTOUTPUT_HPP

#include "mutiny/test/Output.hpp"

namespace mu {
namespace tiny {
namespace test {

///////////////////////////////////////////////////////////////////////////////
//
//  Printf Based Solution
//
///////////////////////////////////////////////////////////////////////////////

class ConsoleTestOutput : public TestOutput
{
public:
  explicit ConsoleTestOutput() = default;
  ~ConsoleTestOutput() override = default;

  void print_buffer(const char* s) override;
  void flush() override;

  ConsoleTestOutput(const ConsoleTestOutput&) = delete;
  ConsoleTestOutput& operator=(const ConsoleTestOutput&) = delete;
};

}
}
} // namespace mu::tiny::test

#endif
