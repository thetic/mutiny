#include "mutiny/test/Test.hpp"

#include "mutiny/test/Failure.hpp"
#include "mutiny/test/Shell.hpp"
#include "mutiny/test/jump_buffer.h"

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
    jump_result = mutiny_set_jmp(helper_do_test_setup, this);
    current->print_very_verbose("\n-------- after  setup: ");

    if (jump_result) {
      current->print_very_verbose("\n----------  before body: ");
      mutiny_set_jmp(helper_do_test_body, this);
      current->print_very_verbose("\n----------  after body: ");
    }
  } catch (FailedException&) {
    mutiny_restore_jump_buffer();
  }
#if MUTINY_USE_STD_CPP_LIB
  catch (const std::exception& e) {
    current->add_failure(UnexpectedExceptionFailure(current, e));
    mutiny_restore_jump_buffer();
    if (current->is_rethrowing_exceptions()) {
      throw;
    }
  }
#endif
  catch (...) {
    current->add_failure(UnexpectedExceptionFailure(current));
    mutiny_restore_jump_buffer();
    if (current->is_rethrowing_exceptions()) {
      throw;
    }
  }

  try {
    current->print_very_verbose("\n--------  before teardown: ");
    mutiny_set_jmp(helper_do_test_teardown, this);
    current->print_very_verbose("\n--------  after teardown: ");
  } catch (FailedException&) {
    mutiny_restore_jump_buffer();
  }
#if MUTINY_USE_STD_CPP_LIB
  catch (const std::exception& e) {
    current->add_failure(UnexpectedExceptionFailure(current, e));
    mutiny_restore_jump_buffer();
    if (current->is_rethrowing_exceptions()) {
      throw;
    }
  }
#endif
  catch (...) {
    current->add_failure(UnexpectedExceptionFailure(current));
    mutiny_restore_jump_buffer();
    if (current->is_rethrowing_exceptions()) {
      throw;
    }
  }
}
#else

void Test::run()
{
  if (mutiny_set_jmp(helper_do_test_setup, this)) {
    mutiny_set_jmp(helper_do_test_body, this);
  }
  mutiny_set_jmp(helper_do_test_teardown, this);
}

#endif

void Test::setup() {}

void Test::test_body() {}

void Test::teardown() {}

} // namespace test
} // namespace tiny
} // namespace mu
