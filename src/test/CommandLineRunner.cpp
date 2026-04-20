#include "mu/tiny/test/CommandLineRunner.hpp"

#include "mu/tiny/test/CompositeOutput.hpp"
#include "mu/tiny/test/ConsoleOutput.hpp"
#include "mu/tiny/test/JUnitOutputPlugin.hpp"
#include "mu/tiny/test/Output.hpp"
#include "mu/tiny/test/Registry.hpp"
#include "mu/tiny/test/Result.hpp"
#include "mu/tiny/test/SetPointerPlugin.hpp"
#include "mu/tiny/test/Shell.hpp"
#include "mu/tiny/test/TapOutputPlugin.hpp"

namespace mu {
namespace tiny {
namespace test {

int CommandLineRunner::run_all_tests(int argc, char** argv)
{
  return run_all_tests(argc, reinterpret_cast<const char* const*>(argv));
}

int CommandLineRunner::run_all_tests(int argc, const char* const* argv)
{
  CommandLineRunner runner(argc, argv, Registry::get_current_registry());
  return runner.run_all_tests_main();
}

CommandLineRunner::CommandLineRunner(
    int argc,
    const char* const* argv,
    Registry* registry
)
  : registry_(registry)
{
  arguments_ = new CommandLineArguments(argc, argv);
}

CommandLineRunner::~CommandLineRunner()
{
  delete arguments_;
  delete output_;
}

int CommandLineRunner::run_all_tests_main()
{
  int test_result = 1;

  SetPointerPlugin set_pointer_plugin;
  JUnitOutputPlugin junit_plugin;
  TapOutputPlugin tap_plugin;
  registry_->install_plugin(&set_pointer_plugin);
  registry_->install_plugin(&junit_plugin);
  registry_->install_plugin(&tap_plugin);

  if (parse_arguments(registry_->get_first_plugin())) {
    if (arguments_->need_help()) {
      output_->print(arguments_->help().c_str());
      test_result = 0;
    } else {
      test_result = run_all_tests();
    }
  }

  registry_->remove_plugin_by_name(tap_plugin.name);
  registry_->remove_plugin_by_name(junit_plugin.name);
  registry_->remove_plugin_by_name(set_pointer_plugin.name);
  return test_result;
}

void CommandLineRunner::initialize_test_run()
{
  registry_->set_group_filters(arguments_->get_group_filters());
  registry_->set_name_filters(arguments_->get_name_filters());

  if (arguments_->is_verbose()) {
    output_->verbose(Output::MutinyVerbosityLevel::verbose);
  }
  if (arguments_->is_very_verbose()) {
    output_->verbose(Output::MutinyVerbosityLevel::very_verbose);
  }
  if (arguments_->is_color()) {
    output_->color();
  }
  if (arguments_->is_run_ignored()) {
    registry_->set_run_ignored();
  }
  if (arguments_->is_crashing_on_fail()) {
    Shell::set_crash_on_fail();
  }

  Shell::set_rethrow_exceptions(arguments_->is_rethrowing_exceptions());
}

int CommandLineRunner::run_all_tests()
{
  initialize_test_run();
  size_t loop_count = 0;
  size_t failed_test_count = 0;
  size_t failed_execution_count = 0;
  size_t repeat_count = arguments_->get_repeat_count();

  if (arguments_->is_listing_test_group_names()) {
    Result tr(*output_);
    registry_->list_test_group_names(tr);
    return 0;
  }

  if (arguments_->is_listing_test_group_and_case_names()) {
    Result tr(*output_);
    registry_->list_test_group_and_case_names(tr);
    return 0;
  }

  if (arguments_->is_listing_test_locations()) {
    Result tr(*output_);
    registry_->list_test_locations(tr);
    return 0;
  }

  if (arguments_->is_listing_ordered_test_locations()) {
    Result tr(*output_);
    registry_->list_ordered_test_locations(tr);
    return 0;
  }

  if (arguments_->is_listing_test_group_locations()) {
    Result tr(*output_);
    registry_->list_test_group_locations(tr);
    return 0;
  }

  if (arguments_->is_reversing()) {
    registry_->reverse_tests();
  }

  if (arguments_->is_shuffling()) {
    output_->print("Test order shuffling enabled with seed: ");
    output_->print(arguments_->get_shuffle_seed());
    output_->print("\n");
  }
  while (loop_count++ < repeat_count) {

    if (arguments_->is_shuffling()) {
      registry_->shuffle_tests(arguments_->get_shuffle_seed());
    }

    output_->print_test_run(loop_count, repeat_count);
    Result tr(*output_);
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

Output* CommandLineRunner::create_console_output()
{
  return new ConsoleOutput;
}

Output* CommandLineRunner::create_composite_output(
    Output* output_one,
    Output* output_two
)
{
  auto* composite = new CompositeOutput;
  composite->set_output_one(output_one);
  composite->set_output_two(output_two);
  return composite;
}

bool CommandLineRunner::parse_arguments(Plugin* plugin)
{
  if (!arguments_->parse(plugin)) {
    output_ = create_console_output();
    output_->print(arguments_->help().c_str());
    return false;
  }

  Output* plugin_output = registry_->get_first_plugin()->create_all_outputs();
  if (plugin_output != nullptr) {
    output_ = plugin_output;
    if ((arguments_->is_verbose() || arguments_->is_very_verbose()) &&
        plugin_output->needs_console_companion()) {
      output_ = create_composite_output(output_, create_console_output());
    }
  } else {
    output_ = create_console_output();
  }
  return true;
}

} // namespace test
} // namespace tiny
} // namespace mu
