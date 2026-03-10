#ifndef D_Test_h
#define D_Test_h

namespace cpputest {

class Test
{
public:
  Test();
  virtual ~Test();
  virtual void run();

  virtual void setup();
  virtual void teardown();
  virtual void testBody();
};

} // namespace cpputest

#endif
