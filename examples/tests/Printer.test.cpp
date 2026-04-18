#include "Printer.hpp"

#include "MockPrinter.hpp"

#include "mu/tiny/test.hpp"

TEST_GROUP(Printer)
{
  Printer* printer;
  MockPrinter* mock_printer;

  void setup() override
  {
    mock_printer = new MockPrinter();
    printer = mock_printer;
  }
  void teardown() override { delete printer; }
};

TEST(Printer, PrintConstCharStar)
{
  printer->print("hello");
  printer->print("hello\n");
  const char* expected = "hellohello\n";
  CHECK_EQUAL(expected, mock_printer->get_output());
}

TEST(Printer, PrintLong)
{
  printer->print(1234);
  const char* expected = "1234";
  CHECK_EQUAL(expected, mock_printer->get_output());
}

TEST(Printer, StreamOperators)
{
  *printer << "n=" << 1234;
  const char* expected = "n=1234";
  CHECK_EQUAL(expected, mock_printer->get_output());
}
