#include "CppUTest/Test.hpp"

#include "TestJumpBuffer.hpp"

#include "CppUTest/TestFailure.hpp"
#include "CppUTest/TestShell.hpp"

namespace cpputest {

namespace {
void
helperDoTestSetup(void* data)
{
  static_cast<Test*>(data)->setup();
}

void
helperDoTestBody(void* data)
{
  static_cast<Test*>(data)->testBody();
}

void
helperDoTestTeardown(void* data)
{
  static_cast<Test*>(data)->teardown();
}
} // namespace

Test::Test() {}

Test::~Test() {}

#if CPPUTEST_HAVE_EXCEPTIONS

void
Test::run()
{
  TestShell* current = TestShell::getCurrent();
  int jumpResult = 0;
  try {
    current->printVeryVerbose("\n-------- before setup: ");
    jumpResult = TestSetJmp(helperDoTestSetup, this);
    current->printVeryVerbose("\n-------- after  setup: ");

    if (jumpResult) {
      current->printVeryVerbose("\n----------  before body: ");
      TestSetJmp(helperDoTestBody, this);
      current->printVeryVerbose("\n----------  after body: ");
    }
  } catch (FailedException&) {
    TestRestoreJumpBuffer();
  }
#if CPPUTEST_USE_STD_CPP_LIB
  catch (const std::exception& e) {
    current->addFailure(UnexpectedExceptionFailure(current, e));
    TestRestoreJumpBuffer();
    if (current->isRethrowingExceptions()) {
      throw;
    }
  }
#endif
  catch (...) {
    current->addFailure(UnexpectedExceptionFailure(current));
    TestRestoreJumpBuffer();
    if (current->isRethrowingExceptions()) {
      throw;
    }
  }

  try {
    current->printVeryVerbose("\n--------  before teardown: ");
    TestSetJmp(helperDoTestTeardown, this);
    current->printVeryVerbose("\n--------  after teardown: ");
  } catch (FailedException&) {
    TestRestoreJumpBuffer();
  }
#if CPPUTEST_USE_STD_CPP_LIB
  catch (const std::exception& e) {
    current->addFailure(UnexpectedExceptionFailure(current, e));
    TestRestoreJumpBuffer();
    if (current->isRethrowingExceptions()) {
      throw;
    }
  }
#endif
  catch (...) {
    current->addFailure(UnexpectedExceptionFailure(current));
    TestRestoreJumpBuffer();
    if (current->isRethrowingExceptions()) {
      throw;
    }
  }
}
#else

void
Test::run()
{
  if (TestSetJmp(helperDoTestSetup, this)) {
    TestSetJmp(helperDoTestBody, this);
  }
  TestSetJmp(helperDoTestTeardown, this);
}

#endif

void
Test::setup()
{
}

void
Test::testBody()
{
}

void
Test::teardown()
{
}

} // namespace cpputest
