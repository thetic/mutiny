#ifndef INCLUDED_CPPUTEST_CONSOLETESTOUTPUT_HPP
#define INCLUDED_CPPUTEST_CONSOLETESTOUTPUT_HPP

#include "CppUTest/TestOutput.hpp"

namespace cpputest {

///////////////////////////////////////////////////////////////////////////////
//
//  Printf Based Solution
//
///////////////////////////////////////////////////////////////////////////////

class ConsoleTestOutput : public TestOutput
{
public:
  explicit ConsoleTestOutput() {}
  virtual ~ConsoleTestOutput() override {}

  virtual void print_buffer(const char* s) override;
  virtual void flush() override;

private:
  ConsoleTestOutput(const ConsoleTestOutput&);
  ConsoleTestOutput& operator=(const ConsoleTestOutput&);
};

} // namespace cpputest

#endif
