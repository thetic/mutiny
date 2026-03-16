#ifndef INCLUDED_CPPMU_TESTTERMINATOR_HPP
#define INCLUDED_CPPMU_TESTTERMINATOR_HPP

namespace cppmu {

class TestTerminator
{
public:
  virtual void exit_current_test() const = 0;
  virtual ~TestTerminator() = default;
};

class NormalTestTerminator : public TestTerminator
{
public:
  void exit_current_test() const override;
  ~NormalTestTerminator() override = default;
};

class TestTerminatorWithoutExceptions : public TestTerminator
{
public:
  void exit_current_test() const override;
  ~TestTerminatorWithoutExceptions() override = default;
};

class CrashingTestTerminator : public NormalTestTerminator
{
public:
  void exit_current_test() const override;
  ~CrashingTestTerminator() override = default;
};

class CrashingTestTerminatorWithoutExceptions
  : public TestTerminatorWithoutExceptions
{
public:
  void exit_current_test() const override;
  ~CrashingTestTerminatorWithoutExceptions() override = default;
};

} // namespace cppmu

#endif
