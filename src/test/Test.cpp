#include "mu/tiny/test/Test.hpp"

#include "mu/tiny/test/Failure.hpp"
#include "mu/tiny/test/Shell.hpp"
#include "mu/tiny/test/jump_buffer.hpp"

namespace mu {
namespace tiny {
namespace test {

namespace {
void helper_do_test_setup(void* data)
{
  static_cast<Test*>(data)->setup();
}

void helper_do_test_body(void* data)
{
  static_cast<Test*>(data)->test_body();
}

void helper_do_test_teardown(void* data)
{
  static_cast<Test*>(data)->teardown();
}
} // namespace

#if MUTINY_HAVE_EXCEPTIONS

void Test::run()
{
  Shell* current = Shell::get_current();
  int jump_result = 0;
  try {
    current->print_very_verbose("\n-------- before setup: ");
    jump_result = set_jump(helper_do_test_setup, this);
    current->print_very_verbose("\n-------- after  setup: ");

    if (jump_result) {
      current->print_very_verbose("\n----------  before body: ");
      set_jump(helper_do_test_body, this);
      current->print_very_verbose("\n----------  after body: ");
    }
  } catch (FailedException&) {
    restore_jump_buffer();
  }
#if MUTINY_USE_STD_CPP_LIB
  catch (const std::exception& e) {
    current->add_failure(UnexpectedExceptionFailure(current, e));
    restore_jump_buffer();
    if (current->is_rethrowing_exceptions()) {
      throw;
    }
  }
#endif
  catch (...) {
    current->add_failure(UnexpectedExceptionFailure(current));
    restore_jump_buffer();
    if (current->is_rethrowing_exceptions()) {
      throw;
    }
  }

  try {
    current->print_very_verbose("\n--------  before teardown: ");
    set_jump(helper_do_test_teardown, this);
    current->print_very_verbose("\n--------  after teardown: ");
  } catch (FailedException&) {
    restore_jump_buffer();
  }
#if MUTINY_USE_STD_CPP_LIB
  catch (const std::exception& e) {
    current->add_failure(UnexpectedExceptionFailure(current, e));
    restore_jump_buffer();
    if (current->is_rethrowing_exceptions()) {
      throw;
    }
  }
#endif
  catch (...) {
    current->add_failure(UnexpectedExceptionFailure(current));
    restore_jump_buffer();
    if (current->is_rethrowing_exceptions()) {
      throw;
    }
  }
}
#else

void Test::run()
{
  if (set_jump(helper_do_test_setup, this)) {
    set_jump(helper_do_test_body, this);
  }
  set_jump(helper_do_test_teardown, this);
}

#endif

void Test::setup() {}

void Test::test_body() {}

void Test::teardown() {}

} // namespace test
} // namespace tiny
} // namespace mu
