#ifndef INCLUDED_CPPUTEST_EXPECTFAILTESTSHELL_HPP
#define INCLUDED_CPPUTEST_EXPECTFAILTESTSHELL_HPP

#include "CppUTest/TestShell.hpp"

namespace cpputest {

class ExpectFailTestShell : public TestShell
{
public:
  ExpectFailTestShell();
  explicit ExpectFailTestShell(const char* group_name,
      const char* test_name,
      const char* file_name,
      size_t line_number);
  virtual ~ExpectFailTestShell() override;

protected:
  virtual String get_macro_name() const override;
  virtual void run_one_test(TestPlugin* plugin, TestResult& result) override;

private:
  ExpectFailTestShell(const ExpectFailTestShell&);
  ExpectFailTestShell& operator=(const ExpectFailTestShell&);
};

} // namespace cpputest

#endif
