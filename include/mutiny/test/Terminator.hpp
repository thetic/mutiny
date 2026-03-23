#ifndef INCLUDED_MUTINY_TESTTERMINATOR_HPP
#define INCLUDED_MUTINY_TESTTERMINATOR_HPP

namespace mu {
namespace tiny {
namespace test {

class TestTerminator
{
public:
  [[noreturn]] virtual void exit_current_test() const = 0;
  virtual ~TestTerminator() = default;
};

class NormalTestTerminator : public TestTerminator
{
public:
  [[noreturn]] void exit_current_test() const override;
  ~NormalTestTerminator() override = default;
};

class TestTerminatorWithoutExceptions : public TestTerminator
{
public:
  [[noreturn]] void exit_current_test() const override;
  ~TestTerminatorWithoutExceptions() override = default;
};

class CrashingTestTerminator : public NormalTestTerminator
{
public:
  [[noreturn]] void exit_current_test() const override;
  ~CrashingTestTerminator() override = default;
};

class CrashingTestTerminatorWithoutExceptions
  : public TestTerminatorWithoutExceptions
{
public:
  [[noreturn]] void exit_current_test() const override;
  ~CrashingTestTerminatorWithoutExceptions() override = default;
};

}
}
} // namespace mu::tiny::test

#endif
