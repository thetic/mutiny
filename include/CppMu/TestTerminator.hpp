#ifndef INCLUDED_CPPMU_TESTTERMINATOR_HPP
#define INCLUDED_CPPMU_TESTTERMINATOR_HPP

namespace cppmu {

class TestTerminator
{
public:
  virtual void exit_current_test() const = 0;
  virtual ~TestTerminator();
};

class NormalTestTerminator : public TestTerminator
{
public:
  virtual void exit_current_test() const override;
  virtual ~NormalTestTerminator() override;
};

class TestTerminatorWithoutExceptions : public TestTerminator
{
public:
  virtual void exit_current_test() const override;
  virtual ~TestTerminatorWithoutExceptions() override;
};

class CrashingTestTerminator : public NormalTestTerminator
{
public:
  virtual void exit_current_test() const override;
  virtual ~CrashingTestTerminator() override;
};

class CrashingTestTerminatorWithoutExceptions
  : public TestTerminatorWithoutExceptions
{
public:
  virtual void exit_current_test() const override;
  virtual ~CrashingTestTerminatorWithoutExceptions() override;
};

} // namespace cppmu

#endif
