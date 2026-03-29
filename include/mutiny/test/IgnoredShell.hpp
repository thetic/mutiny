#ifndef INCLUDED_MUTINY_TEST_IGNOREDSHELL_HPP
#define INCLUDED_MUTINY_TEST_IGNOREDSHELL_HPP

#include "mutiny/test/Shell.hpp"

#include "mutiny/export.h"

namespace mu {
namespace tiny {
namespace test {

class MUTINY_EXPORT IgnoredShell : public Shell
{
public:
  IgnoredShell();
  ~IgnoredShell() override = default;
  explicit IgnoredShell(
      const char* group_name,
      const char* test_name,
      const char* file_name,
      size_t line_number
  );
  IgnoredShell(const IgnoredShell&) = delete;
  IgnoredShell& operator=(const IgnoredShell&) = delete;
  bool will_run() const override;
  void set_run_ignored() override;

protected:
  String get_macro_name() const override;
  void run_one_test(Plugin* plugin, Result& result) override;

private:
  bool run_ignored_;
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
