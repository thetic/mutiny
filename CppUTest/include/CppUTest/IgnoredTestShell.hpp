#ifndef INCLUDED_CPPUTEST_IGNOREDTESTSHELL_HPP
#define INCLUDED_CPPUTEST_IGNOREDTESTSHELL_HPP

#include "CppUTest/TestShell.hpp"

namespace cpputest {

class IgnoredTestShell : public TestShell
{
public:
  IgnoredTestShell();
  virtual ~IgnoredTestShell() override;
  explicit IgnoredTestShell(const char* group_name,
      const char* test_name,
      const char* file_name,
      size_t line_number);
  virtual bool will_run() const override;
  virtual void set_run_ignored() override;

protected:
  virtual String get_macro_name() const override;
  virtual void run_one_test(TestPlugin* plugin, TestResult& result) override;

private:
  IgnoredTestShell(const IgnoredTestShell&);
  IgnoredTestShell& operator=(const IgnoredTestShell&);

  bool run_ignored_;
};

} // namespace cpputest

#endif
