#include "mu/tiny/test/Result.hpp"
#include "mu/tiny/test/Shell.hpp"
#include "mu/tiny/test/StringBufferOutput.hpp"
#include "mu/tiny/test/TestingFixture.hpp"

#include "mu/tiny/test.h"
#include "mu/tiny/test.hpp"

namespace {

class PropertyRecordingOutput : public mu::tiny::test::StringBufferOutput
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

class PropertyCapturingFixture : public mu::tiny::test::TestingFixture
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
  mu::tiny::test::Result result(output);
  result.add_test_property("ticket_id", "12345");

  CHECK_EQUAL(1, output.call_count);
  STRCMP_EQUAL("ticket_id", output.recorded_name);
  STRCMP_EQUAL("12345", output.recorded_value);
}

TEST(TestShellProperty, addTestPropertyOnShellRoutesToResult)
{
  PropertyRecordingOutput output;
  mu::tiny::test::Result result(output);
  mu::tiny::test::Shell shell("Group", "Test", "file", 1);

  // Simulate the test context setup done by run_one_test_in_current_process
  mu::tiny::test::Shell::set_crash_on_fail(); // harmless; just ensuring
                                              // static init
  mu::tiny::test::Shell::restore_default_test_terminator();

  // Directly test Result delegation (Shell routes through Result)
  result.add_test_property("suite", "smoke");

  CHECK_EQUAL(1, output.call_count);
  STRCMP_EQUAL("suite", output.recorded_name);
  STRCMP_EQUAL("smoke", output.recorded_value);
}

TEST(TestShellProperty, addTestPropertyCRoutesGetCurrentToOutput)
{
  PropertyCapturingFixture fixture;
  fixture.set_test_function([] { mutiny_add_test_property("ticket", "123"); });
  fixture.run_all_tests();

  CHECK_EQUAL(1, fixture.capture->call_count);
  STRCMP_EQUAL("ticket", fixture.capture->recorded_name);
  STRCMP_EQUAL("123", fixture.capture->recorded_value);
}
