#ifndef INCLUDED_CPPMU_EXPECTFAILTESTSHELL_HPP
#define INCLUDED_CPPMU_EXPECTFAILTESTSHELL_HPP

#include "CppMu/TestShell.hpp"

namespace cppmu {

class ExpectFailTestShell : public TestShell
{
public:
  ExpectFailTestShell() = default;
  explicit ExpectFailTestShell(const char* group_name,
      const char* test_name,
      const char* file_name,
      size_t line_number);
  ~ExpectFailTestShell() override = default;

protected:
  String get_macro_name() const override;
  void run_one_test(TestPlugin* plugin, TestResult& result) override;

private:
  ExpectFailTestShell(const ExpectFailTestShell&);
  ExpectFailTestShell& operator=(const ExpectFailTestShell&);
};

} // namespace cppmu

#endif
