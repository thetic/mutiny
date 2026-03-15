#ifndef INCLUDED_CPPUTEST_STRINGBUFFERTESTOUTPUT_HPP
#define INCLUDED_CPPUTEST_STRINGBUFFERTESTOUTPUT_HPP

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

  void print_buffer(const char* s) override { output_ += s; }

  void flush() override { output_ = ""; }

  const String& get_output() { return output_; }

protected:
  String output_;

private:
  StringBufferTestOutput(const StringBufferTestOutput&);
  StringBufferTestOutput& operator=(const StringBufferTestOutput&);
};

} // namespace cpputest

#endif
