#ifndef INCLUDED_CPPMU_TEST_HPP
#define INCLUDED_CPPMU_TEST_HPP

namespace cppmu {

class Test
{
public:
  Test() = default;
  virtual ~Test() = default;
  virtual void run();

  virtual void setup();
  virtual void teardown();
  virtual void test_body();
};

} // namespace cppmu

#endif
