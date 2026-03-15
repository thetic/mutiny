#ifndef INCLUDED_CPPUTEST_TEST_HPP
#define INCLUDED_CPPUTEST_TEST_HPP

namespace cpputest {

class Test
{
public:
  Test();
  virtual ~Test();
  virtual void run();

  virtual void setup();
  virtual void teardown();
  virtual void test_body();
};

} // namespace cpputest

#endif
