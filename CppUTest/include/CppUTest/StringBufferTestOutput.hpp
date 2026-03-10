#ifndef D_StringBufferTestOutput_h
#define D_StringBufferTestOutput_h

#include "CppUTest/TestOutput.hpp"

namespace cpputest {

///////////////////////////////////////////////////////////////////////////////
//
//  TestOutput for test purposes
//
///////////////////////////////////////////////////////////////////////////////

class StringBufferTestOutput : public TestOutput
{
public:
  explicit StringBufferTestOutput() {}

  virtual ~StringBufferTestOutput() override;

  void printBuffer(const char* s) override { output += s; }

  void flush() override { output = ""; }

  const String& getOutput() { return output; }

protected:
  String output;

private:
  StringBufferTestOutput(const StringBufferTestOutput&);
  StringBufferTestOutput& operator=(const StringBufferTestOutput&);
};

} // namespace cpputest

#endif
