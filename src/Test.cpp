#include "CppMu/Test.hpp"

#include "CppMu/TestFailure.hpp"
#include "CppMu/TestShell.hpp"
#include "CppMu/jump_buffer.h"

namespace cppmu {

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

#if CPPMU_HAVE_EXCEPTIONS

void Test::run()
{
  TestShell* current = TestShell::get_current();
  int jump_result = 0;
  try {
    current->print_very_verbose("\n-------- before setup: ");
    jump_result = cppmu_set_jmp(helper_do_test_setup, this);
    current->print_very_verbose("\n-------- after  setup: ");

    if (jump_result) {
      current->print_very_verbose("\n----------  before body: ");
      cppmu_set_jmp(helper_do_test_body, this);
      current->print_very_verbose("\n----------  after body: ");
    }
  } catch (FailedException&) {
    cppmu_restore_jump_buffer();
  }
#if CPPMU_USE_STD_CPP_LIB
  catch (const std::exception& e) {
    current->add_failure(UnexpectedExceptionFailure(current, e));
    cppmu_restore_jump_buffer();
    if (current->is_rethrowing_exceptions()) {
      throw;
    }
  }
#endif
  catch (...) {
    current->add_failure(UnexpectedExceptionFailure(current));
    cppmu_restore_jump_buffer();
    if (current->is_rethrowing_exceptions()) {
      throw;
    }
  }

  try {
    current->print_very_verbose("\n--------  before teardown: ");
    cppmu_set_jmp(helper_do_test_teardown, this);
    current->print_very_verbose("\n--------  after teardown: ");
  } catch (FailedException&) {
    cppmu_restore_jump_buffer();
  }
#if CPPMU_USE_STD_CPP_LIB
  catch (const std::exception& e) {
    current->add_failure(UnexpectedExceptionFailure(current, e));
    cppmu_restore_jump_buffer();
    if (current->is_rethrowing_exceptions()) {
      throw;
    }
  }
#endif
  catch (...) {
    current->add_failure(UnexpectedExceptionFailure(current));
    cppmu_restore_jump_buffer();
    if (current->is_rethrowing_exceptions()) {
      throw;
    }
  }
}
#else

void Test::run()
{
  if (cppmu_set_jmp(helper_do_test_setup, this)) {
    cppmu_set_jmp(helper_do_test_body, this);
  }
  cppmu_set_jmp(helper_do_test_teardown, this);
}

#endif

void Test::setup() {}

void Test::test_body() {}

void Test::teardown() {}

} // namespace cppmu
