#ifndef INCLUDED_MUTINY_TEST_EXPECTFAILSHELL_HPP
#define INCLUDED_MUTINY_TEST_EXPECTFAILSHELL_HPP

#include "mutiny/test/Shell.hpp"

namespace mu {
namespace tiny {
namespace test {

class ExpectFailShell : public Shell
{
public:
  ExpectFailShell() = default;
  explicit ExpectFailShell(
      const char* group_name,
      const char* test_name,
      const char* file_name,
      size_t line_number
  );
  ~ExpectFailShell() override = default;
  ExpectFailShell(const ExpectFailShell&) = delete;
  ExpectFailShell& operator=(const ExpectFailShell&) = delete;

protected:
  String get_macro_name() const override;
  void run_one_test(Plugin* plugin, Result& result) override;
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
