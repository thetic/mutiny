#ifndef D_MockPrinter_H
#define D_MockPrinter_H

///////////////////////////////////////////////////////////////////////////////
//
//  MockPrinter.h
//
//  MockPrinter is responsible for providing a test stub for Printer
//
///////////////////////////////////////////////////////////////////////////////
#include "Printer.h"

#include "CppUTest/String.hpp"

#include <string>

#include <stdlib.h>

class MockPrinter : public Printer
{
public:
  explicit MockPrinter() {}
  virtual ~MockPrinter() override {}

  virtual void Print(const char* s) override { savedOutput.append(s); }

  virtual void Print(long int value) override
  {
    cpputest::String buffer;
    buffer = cpputest::StringFromFormat("%ld", value);
    savedOutput.append(buffer.c_str());
  }

  std::string getOutput() const { return savedOutput; }

private:
  std::string savedOutput;

  MockPrinter(const MockPrinter&);
  MockPrinter& operator=(const MockPrinter&);
};

#endif // D_MockPrinter_H
