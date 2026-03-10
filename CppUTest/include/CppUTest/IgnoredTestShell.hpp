#ifndef D_IgnoredTestShell_h
#define D_IgnoredTestShell_h

#include "CppUTest/TestShell.hpp"

namespace cpputest {

class IgnoredTestShell : public TestShell
{
public:
  IgnoredTestShell();
  virtual ~IgnoredTestShell() override;
  explicit IgnoredTestShell(const char* groupName,
      const char* testName,
      const char* fileName,
      size_t lineNumber);
  virtual bool willRun() const override;
  virtual void setRunIgnored() override;

protected:
  virtual String getMacroName() const override;
  virtual void runOneTest(TestPlugin* plugin, TestResult& result) override;

private:
  IgnoredTestShell(const IgnoredTestShell&);
  IgnoredTestShell& operator=(const IgnoredTestShell&);

  bool runIgnored_;
};

} // namespace cpputest

#endif
