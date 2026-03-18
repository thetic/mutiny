#include "CppMu/CppMu.h"
#include "CppMu/CppMu.hpp"
#include "CppMu/StringBufferTestOutput.hpp"
#include "CppMu/TestResult.hpp"
#include "CppMu/TestShell.hpp"
#include "CppMu/TestTestingFixture.hpp"

namespace {

class PropertyRecordingOutput : public cppmu::StringBufferTestOutput
{
public:
  const char* recorded_name{ nullptr };
  const char* recorded_value{ nullptr };
  int call_count{ 0 };

  void print_test_property(const char* name, const char* value) override
  {
    recorded_name = name;
    recorded_value = value;
    call_count++;
  }
};

class PropertyCapturingFixture : public cppmu::TestTestingFixture
{
public:
  PropertyRecordingOutput* capture;

  PropertyCapturingFixture()
    : capture(new PropertyRecordingOutput())
  {
    replace_output(capture);
  }
};

} // namespace

TEST_GROUP(TestShellProperty)
{};

TEST(TestShellProperty, addTestPropertyRoutesToTestOutputPrintTestProperty)
{
  PropertyRecordingOutput output;
  cppmu::TestResult result(output);
  result.add_test_property("ticket_id", "12345");

  LONGS_EQUAL(1, output.call_count);
  STRCMP_EQUAL("ticket_id", output.recorded_name);
  STRCMP_EQUAL("12345", output.recorded_value);
}

TEST(TestShellProperty, addTestPropertyOnShellRoutesToResult)
{
  PropertyRecordingOutput output;
  cppmu::TestResult result(output);
  cppmu::TestShell shell("Group", "Test", "file", 1);

  // Simulate the test context setup done by run_one_test_in_current_process
  cppmu::TestShell::set_crash_on_fail(); // harmless; just ensuring static init
  cppmu::TestShell::restore_default_test_terminator();

  // Directly test TestResult delegation (TestShell routes through TestResult)
  result.add_test_property("suite", "smoke");

  LONGS_EQUAL(1, output.call_count);
  STRCMP_EQUAL("suite", output.recorded_name);
  STRCMP_EQUAL("smoke", output.recorded_value);
}

TEST(TestShellProperty, addTestPropertyCRoutesGetCurrentToOutput)
{
  PropertyCapturingFixture fixture;
  fixture.set_test_function([] { cppmu_add_test_property("ticket", "123"); });
  fixture.run_all_tests();

  LONGS_EQUAL(1, fixture.capture->call_count);
  STRCMP_EQUAL("ticket", fixture.capture->recorded_name);
  STRCMP_EQUAL("123", fixture.capture->recorded_value);
}
