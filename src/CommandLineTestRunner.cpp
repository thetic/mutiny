#include "CppMu/CommandLineTestRunner.hpp"

#include "CppMu/CompositeTestOutput.hpp"
#include "CppMu/ConsoleTestOutput.hpp"
#include "CppMu/JUnitTestOutput.hpp"
#include "CppMu/SetPointerPlugin.hpp"
#include "CppMu/TestOutput.hpp"
#include "CppMu/TestRegistry.hpp"

namespace cppmu {

int CommandLineTestRunner::run_all_tests(int argc, char** argv)
{
  return run_all_tests(argc, reinterpret_cast<const char* const*>(argv));
}

int CommandLineTestRunner::run_all_tests(int argc, const char* const* argv)
{
  CommandLineTestRunner runner(
      argc, argv, TestRegistry::get_current_registry()
  );
  return runner.run_all_tests_main();
}

CommandLineTestRunner::CommandLineTestRunner(
    int argc,
    const char* const* argv,
    TestRegistry* registry
)
  : registry_(registry)
{
  arguments_ = new CommandLineArguments(argc, argv);
}

CommandLineTestRunner::~CommandLineTestRunner()
{
  delete arguments_;
  delete output_;
}

int CommandLineTestRunner::run_all_tests_main()
{
  int test_result = 1;

  SetPointerPlugin plugin;
  registry_->install_plugin(&plugin);

  if (parse_arguments(registry_->get_first_plugin()))
    test_result = run_all_tests();

  registry_->remove_plugin_by_name(plugin.name);
  return test_result;
}

void CommandLineTestRunner::initialize_test_run()
{
  registry_->set_group_filters(arguments_->get_group_filters());
  registry_->set_name_filters(arguments_->get_name_filters());

  if (arguments_->is_verbose())
    output_->verbose(TestOutput::VerbosityLevel::verbose);
  if (arguments_->is_very_verbose())
    output_->verbose(TestOutput::VerbosityLevel::very_verbose);
  if (arguments_->is_color())
    output_->color();
  if (arguments_->is_run_ignored())
    registry_->set_run_ignored();
  if (arguments_->is_crashing_on_fail())
    TestShell::set_crash_on_fail();

  TestShell::set_rethrow_exceptions(arguments_->is_rethrowing_exceptions());
}

int CommandLineTestRunner::run_all_tests()
{
  initialize_test_run();
  size_t loop_count = 0;
  size_t failed_test_count = 0;
  size_t failed_execution_count = 0;
  size_t repeat_count = arguments_->get_repeat_count();

  if (arguments_->is_listing_test_group_names()) {
    TestResult tr(*output_);
    registry_->list_test_group_names(tr);
    return 0;
  }

  if (arguments_->is_listing_test_group_and_case_names()) {
    TestResult tr(*output_);
    registry_->list_test_group_and_case_names(tr);
    return 0;
  }

  if (arguments_->is_listing_test_locations()) {
    TestResult tr(*output_);
    registry_->list_test_locations(tr);
    return 0;
  }

  if (arguments_->is_listing_ordered_test_locations()) {
    TestResult tr(*output_);
    registry_->list_ordered_test_locations(tr);
    return 0;
  }

  if (arguments_->is_reversing())
    registry_->reverse_tests();

  if (arguments_->is_shuffling()) {
    output_->print("Test order shuffling enabled with seed: ");
    output_->print(arguments_->get_shuffle_seed());
    output_->print("\n");
  }
  while (loop_count++ < repeat_count) {

    if (arguments_->is_shuffling())
      registry_->shuffle_tests(arguments_->get_shuffle_seed());

    output_->print_test_run(loop_count, repeat_count);
    TestResult tr(*output_);
    registry_->run_all_tests(tr);
    failed_test_count += tr.get_failure_count();
    if (tr.is_failure()) {
      failed_execution_count++;
    }
  }
  return static_cast<int>(
      failed_test_count != 0 ? failed_test_count : failed_execution_count
  );
}

TestOutput* CommandLineTestRunner::create_j_unit_output(
    const String& package_name
)
{
  auto* junit_output = new JUnitTestOutput;
  if (junit_output != nullptr) {
    junit_output->set_package_name(package_name);
  }
  return junit_output;
}

TestOutput* CommandLineTestRunner::create_console_output()
{
  return new ConsoleTestOutput;
}

TestOutput* CommandLineTestRunner::create_composite_output(
    TestOutput* output_one,
    TestOutput* output_two
)
{
  auto* composite = new CompositeTestOutput;
  composite->set_output_one(output_one);
  composite->set_output_two(output_two);
  return composite;
}

bool CommandLineTestRunner::parse_arguments(TestPlugin* plugin)
{
  if (!arguments_->parse(plugin)) {
    output_ = create_console_output();
    output_->print(
        (arguments_->need_help()) ? arguments_->help() : arguments_->usage()
    );
    return false;
  }

  if (arguments_->is_j_unit_output()) {
    output_ = create_j_unit_output(arguments_->get_package_name());
    if (arguments_->is_verbose() || arguments_->is_very_verbose())
      output_ = create_composite_output(output_, create_console_output());
  } else {
    TestOutput* plugin_output =
        registry_->get_first_plugin()->create_all_outputs();
    output_ = plugin_output ? plugin_output : create_console_output();
  }
  return true;
}

} // namespace cppmu
