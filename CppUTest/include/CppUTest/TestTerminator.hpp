#ifndef INCLUDED_CPPUTEST_TESTTERMINATOR_HPP
#define INCLUDED_CPPUTEST_TESTTERMINATOR_HPP

namespace cpputest {

class TestTerminator
{
public:
  virtual void exitCurrentTest() const = 0;
  virtual ~TestTerminator();
};

class NormalTestTerminator : public TestTerminator
{
public:
  virtual void exitCurrentTest() const override;
  virtual ~NormalTestTerminator() override;
};

class TestTerminatorWithoutExceptions : public TestTerminator
{
public:
  virtual void exitCurrentTest() const override;
  virtual ~TestTerminatorWithoutExceptions() override;
};

class CrashingTestTerminator : public NormalTestTerminator
{
public:
  virtual void exitCurrentTest() const override;
  virtual ~CrashingTestTerminator() override;
};

class CrashingTestTerminatorWithoutExceptions
  : public TestTerminatorWithoutExceptions
{
public:
  virtual void exitCurrentTest() const override;
  virtual ~CrashingTestTerminatorWithoutExceptions() override;
};

} // namespace cpputest

#endif
