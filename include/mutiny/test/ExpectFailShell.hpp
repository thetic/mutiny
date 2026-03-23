#ifndef INCLUDED_MUTINY_EXPECTFAILTESTSHELL_HPP
#define INCLUDED_MUTINY_EXPECTFAILTESTSHELL_HPP

#include "mutiny/test/Shell.hpp"

namespace mu {
namespace tiny {
namespace test {

class ExpectFailTestShell : public TestShell
{
public:
  ExpectFailTestShell() = default;
  explicit ExpectFailTestShell(
      const char* group_name,
      const char* test_name,
      const char* file_name,
      size_t line_number
  );
  ~ExpectFailTestShell() override = default;
  ExpectFailTestShell(const ExpectFailTestShell&) = delete;
  ExpectFailTestShell& operator=(const ExpectFailTestShell&) = delete;

protected:
  String get_macro_name() const override;
  void run_one_test(TestPlugin* plugin, TestResult& result) override;
};

}
}
} // namespace mu::tiny::test

#endif
