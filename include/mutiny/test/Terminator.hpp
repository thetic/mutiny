#ifndef INCLUDED_MUTINY_TEST_TERMINATOR_HPP
#define INCLUDED_MUTINY_TEST_TERMINATOR_HPP

#include "mutiny/export.h"

namespace mu {
namespace tiny {
namespace test {

class MUTINY_EXPORT Terminator
{
public:
  [[noreturn]] virtual void exit_current_test() const = 0;
  virtual ~Terminator() = default;
};

class MUTINY_EXPORT NormalTerminator : public Terminator
{
public:
  [[noreturn]] void exit_current_test() const override;
  ~NormalTerminator() override = default;
};

class MUTINY_EXPORT TerminatorWithoutExceptions : public Terminator
{
public:
  [[noreturn]] void exit_current_test() const override;
  ~TerminatorWithoutExceptions() override = default;
};

class MUTINY_EXPORT CrashingTerminator : public NormalTerminator
{
public:
  [[noreturn]] void exit_current_test() const override;
  ~CrashingTerminator() override = default;
};

class MUTINY_EXPORT CrashingTerminatorWithoutExceptions
  : public TerminatorWithoutExceptions
{
public:
  [[noreturn]] void exit_current_test() const override;
  ~CrashingTerminatorWithoutExceptions() override = default;
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
