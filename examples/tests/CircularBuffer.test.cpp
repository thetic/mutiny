#include "CircularBuffer.hpp"

#include "MockPrinter.hpp"

#include "mu/tiny/test.hpp"

TEST_GROUP(CircularBuffer)
{
  CircularBuffer* buffer;

  void setup() override { buffer = new CircularBuffer(); }
  void teardown() override { delete buffer; }

  void fill_the_queue(int seed, int how_many)
  {
    for (int i = 0; i < how_many; i++)
      buffer->put(seed + i);
  }
  void remove_from_queue(int how_many)
  {
    for (int i = 0; i < how_many; i++)
      buffer->get();
  }
};

TEST(CircularBuffer, EmptyAfterCreation)
{
  CHECK(buffer->is_empty());
}

TEST(CircularBuffer, NotEmpty)
{
  buffer->put(10046);
  CHECK(!buffer->is_empty());
}

TEST(CircularBuffer, NotEmptyThenEmpty)
{
  buffer->put(4567);
  CHECK(!buffer->is_empty());
  buffer->get();
  CHECK(buffer->is_empty());
}

TEST(CircularBuffer, GetPutOneValue)
{
  buffer->put(4567);
  CHECK_EQUAL(4567, buffer->get());
}

TEST(CircularBuffer, GetPutAFew)
{
  buffer->put(1);
  buffer->put(2);
  buffer->put(3);
  CHECK_EQUAL(1, buffer->get());
  CHECK_EQUAL(2, buffer->get());
  CHECK_EQUAL(3, buffer->get());
}

TEST(CircularBuffer, Capacity)
{
  CircularBuffer b(2);
  CHECK_EQUAL(2, b.capacity());
}

TEST(CircularBuffer, IsFull)
{
  fill_the_queue(0, buffer->capacity());
  CHECK(buffer->is_full());
}

TEST(CircularBuffer, EmptyToFullToEmpty)
{
  fill_the_queue(100, buffer->capacity());

  CHECK(buffer->is_full());

  remove_from_queue(buffer->capacity());

  CHECK(buffer->is_empty());
}

TEST(CircularBuffer, WrapAround)
{
  fill_the_queue(100, buffer->capacity());

  CHECK(buffer->is_full());
  CHECK_EQUAL(100, buffer->get());
  CHECK(!buffer->is_full());
  buffer->put(1000);
  CHECK(buffer->is_full());

  remove_from_queue(buffer->capacity() - 1);

  CHECK_EQUAL(1000, buffer->get());
  CHECK(buffer->is_empty());
}

TEST(CircularBuffer, PutToFull)
{
  int capacity = buffer->capacity();
  fill_the_queue(900, capacity);
  buffer->put(9999);

  for (int i = 0; i < buffer->capacity() - 1; i++)
    CHECK_EQUAL(i + 900 + 1, buffer->get());

  CHECK_EQUAL(9999, buffer->get());
  CHECK(buffer->is_empty());
}

// Sometime people ask what tests the tests.
// Do you know the answer

TEST(CircularBuffer, GetFromEmpty)
{
  CHECK_EQUAL(-1, buffer->get());
  CHECK(buffer->is_empty());
}

/*
 * the next tests demonstrate using a mock object for
 * capturing output
 *
 */

TEST(CircularBuffer, PrintEmpty)
{
  MockPrinter mock;
  Printer* p = &mock;

  buffer->print(p);
  STRCMP_EQUAL("Circular buffer content:\n<>\n", mock.get_output().c_str());
}

TEST(CircularBuffer, PrintAfterOnePut)
{
  MockPrinter mock;

  buffer->put(1);
  buffer->print(&mock);
  STRCMP_EQUAL("Circular buffer content:\n<1>\n", mock.get_output().c_str());
}

TEST(CircularBuffer, PrintNotYetWrappedOrFull)
{
  MockPrinter mock;

  buffer->put(1);
  buffer->put(2);
  buffer->put(3);
  buffer->print(&mock);
  STRCMP_EQUAL(
      "Circular buffer content:\n<1, 2, 3>\n", mock.get_output().c_str()
  );
}

TEST(CircularBuffer, PrintNotYetWrappedAndIsFull)
{
  MockPrinter mock;

  fill_the_queue(200, buffer->capacity());

  buffer->print(&mock);
  const char* expected = "Circular buffer content:\n"
                         "<200, 201, 202, 203, 204>\n";

  STRCMP_EQUAL(expected, mock.get_output().c_str());
}

TEST(CircularBuffer, PrintWrappedAndIsFullOldestToNewest)
{
  MockPrinter mock;

  fill_the_queue(200, buffer->capacity());
  buffer->get();
  buffer->put(999);

  buffer->print(&mock);
  const char* expected = "Circular buffer content:\n"
                         "<201, 202, 203, 204, 999>\n";

  STRCMP_EQUAL(expected, mock.get_output().c_str());
}

TEST(CircularBuffer, PrintWrappedAndFullOverwriteOldest)
{
  MockPrinter mock;

  fill_the_queue(200, buffer->capacity());
  buffer->put(9999);

  buffer->print(&mock);
  const char* expected = "Circular buffer content:\n"
                         "<201, 202, 203, 204, 9999>\n";

  STRCMP_EQUAL(expected, mock.get_output().c_str());
}

TEST(CircularBuffer, PrintBoundary)
{
  MockPrinter mock;

  fill_the_queue(200, buffer->capacity());
  remove_from_queue(buffer->capacity() - 2);
  buffer->put(888);
  fill_the_queue(300, buffer->capacity() - 1);

  buffer->print(&mock);
  const char* expected = "Circular buffer content:\n"
                         "<888, 300, 301, 302, 303>\n";

  STRCMP_EQUAL(expected, mock.get_output().c_str());
}

TEST(CircularBuffer, FillEmptyThenPrint)
{
  MockPrinter mock;

  fill_the_queue(200, buffer->capacity());
  remove_from_queue(buffer->capacity());
  buffer->print(&mock);
  const char* expected = "Circular buffer content:\n"
                         "<>\n";

  STRCMP_EQUAL(expected, mock.get_output().c_str());
}
