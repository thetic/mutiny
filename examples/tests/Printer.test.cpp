#include "Printer.hpp"

#include "MockPrinter.hpp"

#include "CppUTest/TestHarness.hpp"

TEST_GROUP(Printer)
{
  Printer* printer;
  MockPrinter* mockPrinter;

  void setup() override
  {
    mockPrinter = new MockPrinter();
    printer = mockPrinter;
  }
  void teardown() override { delete printer; }
};

TEST(Printer, PrintConstCharStar)
{
  printer->Print("hello");
  printer->Print("hello\n");
  const char* expected = "hellohello\n";
  CHECK_EQUAL(expected, mockPrinter->getOutput());
}

TEST(Printer, PrintLong)
{
  printer->Print(1234);
  const char* expected = "1234";
  CHECK_EQUAL(expected, mockPrinter->getOutput());
}

TEST(Printer, StreamOperators)
{
  *printer << "n=" << 1234;
  const char* expected = "n=1234";
  CHECK_EQUAL(expected, mockPrinter->getOutput());
}
