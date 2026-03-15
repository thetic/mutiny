#ifndef INCLUDED_CPPUTEST_EXPECTFAILTESTSHELL_HPP
#define INCLUDED_CPPUTEST_EXPECTFAILTESTSHELL_HPP

#include "CppUTest/TestShell.hpp"

namespace cpputest {

class ExpectFailTestShell : public TestShell
{
public:
  ExpectFailTestShell();
  explicit ExpectFailTestShell(const char* groupName,
      const char* testName,
      const char* fileName,
      size_t lineNumber);
  virtual ~ExpectFailTestShell() override;

protected:
  virtual String getMacroName() const override;
  virtual void runOneTest(TestPlugin* plugin, TestResult& result) override;

private:
  ExpectFailTestShell(const ExpectFailTestShell&);
  ExpectFailTestShell& operator=(const ExpectFailTestShell&);
};

} // namespace cpputest

#endif
