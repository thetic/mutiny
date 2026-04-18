// Demonstrates TEST_ORDERED: tests that run in level order across the suite.
//
// Use case: a stateful system (here, a simple in-memory event log) that must
// be initialized before use and flushed on shutdown. Ordered tests let each
// phase be a separate, named test rather than one monolithic test body.

#include "mu/tiny/test/Ordered.hpp"

#include "mu/tiny/test.hpp"

namespace {

struct EventLog
{
  const char* entries[8]{};
  int count{ 0 };
  bool initialized{ false };
  bool flushed{ false };

  void init()
  {
    initialized = true;
    count = 0;
  }

  void append(const char* msg)
  {
    if (count < 8)
      entries[count++] = msg;
  }

  void flush()
  {
    count = 0;
    flushed = true;
  }
};

EventLog g_log;

} // namespace

TEST_GROUP(OrderedDemo)
{};

// Regular test — unordered tests run before the ordered block.
TEST(OrderedDemo, StartsUninitialized)
{
  CHECK(!g_log.initialized);
}

// Level 10: initialize.
TEST_ORDERED(OrderedDemo, Init, 10)
{
  g_log.init();
  CHECK(g_log.initialized);
  CHECK_EQUAL(0, g_log.count);
}

// Level 20: record two events. Tests at the same level run in
// registration order (FirstEvent before SecondEvent).
TEST_ORDERED(OrderedDemo, FirstEvent, 20)
{
  g_log.append("startup");
  CHECK_EQUAL(1, g_log.count);
}

TEST_ORDERED(OrderedDemo, SecondEvent, 20)
{
  g_log.append("ready");
  CHECK_EQUAL(2, g_log.count);
}

// Level 30: verify accumulated state from both level-20 tests.
TEST_ORDERED(OrderedDemo, VerifyLog, 30)
{
  CHECK_EQUAL(2, g_log.count);
  STRCMP_EQUAL("startup", g_log.entries[0]);
  STRCMP_EQUAL("ready", g_log.entries[1]);
}

// Level 40: shutdown.
TEST_ORDERED(OrderedDemo, Shutdown, 40)
{
  g_log.flush();
  CHECK_EQUAL(0, g_log.count);
  CHECK(g_log.flushed);
}
