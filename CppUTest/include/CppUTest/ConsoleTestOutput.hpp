#ifndef D_ConsoleTestOutput_h
#define D_ConsoleTestOutput_h

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

  virtual void printBuffer(const char* s) override;
  virtual void flush() override;

private:
  ConsoleTestOutput(const ConsoleTestOutput&);
  ConsoleTestOutput& operator=(const ConsoleTestOutput&);
};

} // namespace cpputest

#endif
