#ifndef INCLUDED_MUTINY_TEST_TERMINATOR_HPP
#define INCLUDED_MUTINY_TEST_TERMINATOR_HPP

namespace mu {
namespace tiny {
namespace test {

class Terminator
{
public:
  [[noreturn]] virtual void exit_current_test() const = 0;
  virtual ~Terminator() = default;
};

class NormalTerminator : public Terminator
{
public:
  [[noreturn]] void exit_current_test() const override;
  ~NormalTerminator() override = default;
};

class TerminatorWithoutExceptions : public Terminator
{
public:
  [[noreturn]] void exit_current_test() const override;
  ~TerminatorWithoutExceptions() override = default;
};

class CrashingTerminator : public NormalTerminator
{
public:
  [[noreturn]] void exit_current_test() const override;
  ~CrashingTerminator() override = default;
};

class CrashingTerminatorWithoutExceptions : public TerminatorWithoutExceptions
{
public:
  [[noreturn]] void exit_current_test() const override;
  ~CrashingTerminatorWithoutExceptions() override = default;
};

}
}
} // namespace mu::tiny::test

#endif
