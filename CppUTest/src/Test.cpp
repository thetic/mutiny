#include "CppUTest/Test.hpp"

#include "CppUTest/PlatformSpecificFunctions.h"
#include "CppUTest/TestFailure.hpp"
#include "CppUTest/TestShell.hpp"

namespace cpputest {

static void
helperDoTestSetup(void* data)
{
  static_cast<Test*>(data)->setup();
}

static void
helperDoTestBody(void* data)
{
  static_cast<Test*>(data)->testBody();
}

static void
helperDoTestTeardown(void* data)
{
  static_cast<Test*>(data)->teardown();
}

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
    jumpResult = PlatformSpecificSetJmp(helperDoTestSetup, this);
    current->printVeryVerbose("\n-------- after  setup: ");

    if (jumpResult) {
      current->printVeryVerbose("\n----------  before body: ");
      PlatformSpecificSetJmp(helperDoTestBody, this);
      current->printVeryVerbose("\n----------  after body: ");
    }
  } catch (FailedException&) {
    PlatformSpecificRestoreJumpBuffer();
  }
#if CPPUTEST_USE_STD_CPP_LIB
  catch (const std::exception& e) {
    current->addFailure(UnexpectedExceptionFailure(current, e));
    PlatformSpecificRestoreJumpBuffer();
    if (current->isRethrowingExceptions()) {
      throw;
    }
  }
#endif
  catch (...) {
    current->addFailure(UnexpectedExceptionFailure(current));
    PlatformSpecificRestoreJumpBuffer();
    if (current->isRethrowingExceptions()) {
      throw;
    }
  }

  try {
    current->printVeryVerbose("\n--------  before teardown: ");
    PlatformSpecificSetJmp(helperDoTestTeardown, this);
    current->printVeryVerbose("\n--------  after teardown: ");
  } catch (FailedException&) {
    PlatformSpecificRestoreJumpBuffer();
  }
#if CPPUTEST_USE_STD_CPP_LIB
  catch (const std::exception& e) {
    current->addFailure(UnexpectedExceptionFailure(current, e));
    PlatformSpecificRestoreJumpBuffer();
    if (current->isRethrowingExceptions()) {
      throw;
    }
  }
#endif
  catch (...) {
    current->addFailure(UnexpectedExceptionFailure(current));
    PlatformSpecificRestoreJumpBuffer();
    if (current->isRethrowingExceptions()) {
      throw;
    }
  }
}
#else

void
Test::run()
{
  if (PlatformSpecificSetJmp(helperDoTestSetup, this)) {
    PlatformSpecificSetJmp(helperDoTestBody, this);
  }
  PlatformSpecificSetJmp(helperDoTestTeardown, this);
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
