#include "mu/tiny/test/TestingFixture.hpp"

#include "mu/tiny/test/ExecFunctionShell.hpp"
#include "mu/tiny/test/Result.hpp"
#include "mu/tiny/test/Shell.hpp"

namespace mu {
namespace tiny {
namespace test {

bool TestingFixture::line_of_code_executed_after_check_ = false;

void TestingFixture::replace_output(StringBufferOutput* new_output)
{
  delete result_;
  delete output_;
  output_ = new_output;
  result_ = new Result(*output_);
}

TestingFixture::TestingFixture()
{
  output_ = new StringBufferOutput();
  result_ = new Result(*output_);
  gen_test_ = new ExecFunctionShell();
  registry_ = new Registry();
  owns_exec_function_ = false;

  registry_->set_current_registry(registry_);
  registry_->add_test(gen_test_);

  line_of_code_executed_after_check_ = false;
}

void TestingFixture::flush_output_and_reset_result()
{
  output_->flush();
  delete result_;
  result_ = new Result(*output_);
}

TestingFixture::~TestingFixture()
{
  registry_->set_current_registry(nullptr);
  clear_exec_function();
  delete registry_;
  delete result_;
  delete output_;
  delete gen_test_;
}

void TestingFixture::clear_exec_function()
{
  if (gen_test_->test_function && owns_exec_function_) {
    delete gen_test_->test_function;
  }
}

void TestingFixture::add_test(Shell* test)
{
  registry_->add_test(test);
}

void TestingFixture::set_test_function(void (*test_function)())
{
  clear_exec_function();

  gen_test_->test_function = new ExecFunctionWithoutParameters(test_function);
  owns_exec_function_ = true;
}

void TestingFixture::set_test_function(ExecFunction* test_function)
{
  clear_exec_function();

  gen_test_->test_function = test_function;

  owns_exec_function_ = false;
}

void TestingFixture::set_setup(void (*setup_function)())
{
  gen_test_->setup = setup_function;
}

void TestingFixture::set_teardown(void (*teardown_function)())
{
  gen_test_->teardown = teardown_function;
}

void TestingFixture::install_plugin(Plugin* plugin)
{
  registry_->install_plugin(plugin);
}

void TestingFixture::set_output_verbose()
{
  output_->verbose(Output::MutinyVerbosityLevel::verbose);
}

void TestingFixture::run_test_with_method(void (*method)())
{
  set_test_function(method);
  run_all_tests();
}

void TestingFixture::run_all_tests()
{
  registry_->run_all_tests(*result_);
}

size_t TestingFixture::get_failure_count()
{
  return result_->get_failure_count();
}

size_t TestingFixture::get_check_count()
{
  return result_->get_check_count();
}

size_t TestingFixture::get_test_count()
{
  return result_->get_test_count();
}

size_t TestingFixture::get_ignore_count()
{
  return result_->get_ignored_count();
}

Registry* TestingFixture::get_registry()
{
  return registry_;
}

bool TestingFixture::has_test_failed()
{
  return gen_test_->has_failed();
}

void TestingFixture::assert_print_contains(const String& contains)
{
  STRCMP_CONTAINS(contains.c_str(), get_output().c_str());
}

void TestingFixture::assert_print_contains_not(const String& contains)
{
  CHECK(!string_contains(get_output(), contains));
}

const String& TestingFixture::get_output()
{
  return output_->get_output();
}

size_t TestingFixture::get_run_count()
{
  return result_->get_run_count();
}

void TestingFixture::line_executed_after_check()
{
  line_of_code_executed_after_check_ = true;
}

void TestingFixture::check_test_fails_with_proper_test_location(
    const char* text,
    const char* file,
    size_t line
)
{
  if (get_failure_count() != 1) {
    FAIL_TEST_LOCATION(
        string_from_format(
            "Expected one test failure, but got %d amount of test failures",
            static_cast<int>(get_failure_count())
        )
            .c_str(),
        file,
        line
    );
  }

  STRCMP_CONTAINS_LOCATION(text, output_->get_output().c_str(), "", file, line);

  if (line_of_code_executed_after_check_) {
    FAIL_TEST_LOCATION(
        "The test should jump/throw on failure and not execute the "
        "next line. However, the next line was executed.",
        file,
        line
    );
  }
}

} // namespace test
} // namespace tiny
} // namespace mu
