#include "CppMu/TestTestingFixture.hpp"

#include "CppMu/CppMu.hpp"
#include "CppMu/ExecFunctionTestShell.hpp"

namespace cppmu {

bool TestTestingFixture::line_of_code_executed_after_check_ = false;

TestTestingFixture::TestTestingFixture()
{
  output_ = new StringBufferTestOutput();
  result_ = new TestResult(*output_);
  gen_test_ = new ExecFunctionTestShell();
  registry_ = new TestRegistry();
  owns_exec_function_ = false;

  registry_->set_current_registry(registry_);
  registry_->add_test(gen_test_);

  line_of_code_executed_after_check_ = false;
}

void TestTestingFixture::flush_output_and_reset_result()
{
  output_->flush();
  delete result_;
  result_ = new TestResult(*output_);
}

TestTestingFixture::~TestTestingFixture()
{
  registry_->set_current_registry(nullptr);
  clear_exec_function();
  delete registry_;
  delete result_;
  delete output_;
  delete gen_test_;
}

void TestTestingFixture::clear_exec_function()
{
  if (gen_test_->test_function && owns_exec_function_)
    delete gen_test_->test_function;
}

void TestTestingFixture::add_test(TestShell* test)
{
  registry_->add_test(test);
}

void TestTestingFixture::set_test_function(void (*test_function)())
{
  clear_exec_function();

  gen_test_->test_function = new ExecFunctionWithoutParameters(test_function);
  owns_exec_function_ = true;
}

void TestTestingFixture::set_test_function(ExecFunction* test_function)
{
  clear_exec_function();

  gen_test_->test_function = test_function;

  owns_exec_function_ = false;
}

void TestTestingFixture::set_setup(void (*setup_function)())
{
  gen_test_->setup = setup_function;
}

void TestTestingFixture::set_teardown(void (*teardown_function)())
{
  gen_test_->teardown = teardown_function;
}

void TestTestingFixture::install_plugin(TestPlugin* plugin)
{
  registry_->install_plugin(plugin);
}

void TestTestingFixture::set_output_verbose()
{
  output_->verbose(TestOutput::VerbosityLevel::verbose);
}

void TestTestingFixture::run_test_with_method(void (*method)())
{
  set_test_function(method);
  run_all_tests();
}

void TestTestingFixture::run_all_tests()
{
  registry_->run_all_tests(*result_);
}

size_t TestTestingFixture::get_failure_count()
{
  return result_->get_failure_count();
}

size_t TestTestingFixture::get_check_count()
{
  return result_->get_check_count();
}

size_t TestTestingFixture::get_test_count()
{
  return result_->get_test_count();
}

size_t TestTestingFixture::get_ignore_count()
{
  return result_->get_ignored_count();
}

TestRegistry* TestTestingFixture::get_registry()
{
  return registry_;
}

bool TestTestingFixture::has_test_failed()
{
  return gen_test_->has_failed();
}

void TestTestingFixture::assert_print_contains(const String& contains)
{
  STRCMP_CONTAINS(contains.c_str(), get_output().c_str());
}

void TestTestingFixture::assert_print_contains_not(const String& contains)
{
  CHECK(!string_contains(get_output(), contains));
}

const String& TestTestingFixture::get_output()
{
  return output_->get_output();
}

size_t TestTestingFixture::get_run_count()
{
  return result_->get_run_count();
}

void TestTestingFixture::line_executed_after_check()
{
  line_of_code_executed_after_check_ = true;
}

void TestTestingFixture::check_test_fails_with_proper_test_location(
    const char* text,
    const char* file,
    size_t line
)
{
  if (get_failure_count() != 1)
    FAIL_LOCATION(
        string_from_format(
            "Expected one test failure, but got %d amount of test failures",
            static_cast<int>(get_failure_count())
        )
            .c_str(),
        file,
        line
    );

  STRCMP_CONTAINS_LOCATION(text, output_->get_output().c_str(), "", file, line);

  if (line_of_code_executed_after_check_)
    FAIL_LOCATION(
        "The test should jump/throw on failure and not execute the "
        "next line. However, the next line was executed.",
        file,
        line
    );
}

} // namespace cppmu
