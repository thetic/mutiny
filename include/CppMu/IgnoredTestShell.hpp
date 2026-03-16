#ifndef INCLUDED_CPPMU_IGNOREDTESTSHELL_HPP
#define INCLUDED_CPPMU_IGNOREDTESTSHELL_HPP

#include "CppMu/TestShell.hpp"

namespace cppmu {

class IgnoredTestShell : public TestShell
{
public:
  IgnoredTestShell();
  ~IgnoredTestShell() override = default;
  explicit IgnoredTestShell(const char* group_name,
      const char* test_name,
      const char* file_name,
      size_t line_number);
  IgnoredTestShell(const IgnoredTestShell&) = delete;
  IgnoredTestShell& operator=(const IgnoredTestShell&) = delete;
  bool will_run() const override;
  void set_run_ignored() override;

protected:
  String get_macro_name() const override;
  void run_one_test(TestPlugin* plugin, TestResult& result) override;

private:
  bool run_ignored_;
};

} // namespace cppmu

#endif
