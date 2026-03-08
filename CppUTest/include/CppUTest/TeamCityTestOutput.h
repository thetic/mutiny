#ifndef D_TeamCityTestOutput_h
#define D_TeamCityTestOutput_h

#include "CppUTest/TestOutput.h"
#include "CppUTest/SimpleString.h"

namespace cpputest {

class TeamCityTestOutput : public ConsoleTestOutput
{
public:
    TeamCityTestOutput(void);
    virtual ~TeamCityTestOutput(void) override;

    virtual void printCurrentTestStarted(const TestShell& test) override;
    virtual void printCurrentTestEnded(const TestResult& res) override;
    virtual void printCurrentGroupStarted(const TestShell& test) override;
    virtual void printCurrentGroupEnded(const TestResult& res) override;

    virtual void printFailure(const TestFailure& failure) override;

protected:

private:
    void printEscaped(const char* s);
    const TestShell *currtest_;
    SimpleString currGroup_;
};

} // namespace cpputest

#endif
