#include "CppUTest/TestHarness.h"

#include "TestHarness_cTestCFile.h"

#include "CppUTest/PlatformSpecificFunctions.hpp"
#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestOutput.hpp"
#include "CppUTest/TestRegistry.hpp"
#include "CppUTest/TestTestingFixture.hpp"
int setup_teardown_was_called_in_test_group_in_C = 0;
int test_was_called_in_test_group_in_C = 0;

TEST_GROUP_C_WRAPPER(TestHarnessC)
{
  TEST_GROUP_C_SETUP_WRAPPER(TestHarnessC)
  TEST_GROUP_C_TEARDOWN_WRAPPER(TestHarnessC)
};

TEST_C_WRAPPER(TestHarnessC, checkThatTheTestHasRun)
IGNORE_TEST_C_WRAPPER(TestHarnessC, ignoreMacroForCFile)

/*
 * This test is a bit strange. They use the fact that you can do -r2 for
 * repeating the same run. When you do so, the same statics will be shared and
 * therefore we can test whether the setup/teardown is run correctly.
 */

TEST(TestHarnessC, setupHasBeenCalled)
{
  test_was_called_in_test_group_in_C++;
  /* Increased in setup, decreased in teardown. So at this point it must be 1
   * also on a multiple run */
  LONGS_EQUAL(1, setup_teardown_was_called_in_test_group_in_C);
}

static bool hasDestructorOfTheDestructorCheckedBeenCalled;

class HasTheDestructorBeenCalledChecker
{
public:
  HasTheDestructorBeenCalledChecker() {}
  ~HasTheDestructorBeenCalledChecker()
  {
    hasDestructorOfTheDestructorCheckedBeenCalled = true;
  }
};

TEST_GROUP(TestHarness)
{
  cpputest::TestTestingFixture* fixture;
  TEST_SETUP()
  {
    hasDestructorOfTheDestructorCheckedBeenCalled = false;
    fixture = new cpputest::TestTestingFixture();
  }
  TEST_TEARDOWN() { delete fixture; }
};

static void
failBoolMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_BOOL(1, 0);
}

TEST(TestHarness, checkBool)
{
  CHECK_EQUAL_C_BOOL(1, 1);
  CHECK_EQUAL_C_BOOL(1, 2);
  fixture->setTestFunction(failBoolMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <true>\n\tbut was  <false>");
  fixture->assertPrintContains("TestHarness");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failBoolTextMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_BOOL_TEXT(1, 0, "BoolTestText");
}

TEST(TestHarness, checkBoolText)
{
  CHECK_EQUAL_C_BOOL_TEXT(1, 1, "Text");
  CHECK_EQUAL_C_BOOL_TEXT(1, 2, "Text");
  fixture->setTestFunction(failBoolTextMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <true>\n\tbut was  <false>");
  fixture->assertPrintContains("TestHarness");
  fixture->assertPrintContains("Message: BoolTestText");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failIntMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_INT(1, 2);
}

TEST(TestHarness, checkInt)
{
  CHECK_EQUAL_C_INT(2, 2);
  fixture->setTestFunction(failIntMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assertPrintContains("TestHarness");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failIntTextMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_INT_TEXT(1, 2, "IntTestText");
}

TEST(TestHarness, checkIntText)
{
  CHECK_EQUAL_C_INT_TEXT(2, 2, "Text");
  fixture->setTestFunction(failIntTextMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assertPrintContains("TestHarness");
  fixture->assertPrintContains("Message: IntTestText");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failUnsignedIntMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_UINT(1, 2);
}

TEST(TestHarness, checkUnsignedInt)
{
  CHECK_EQUAL_C_UINT(2, 2);
  fixture->setTestFunction(failUnsignedIntMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assertPrintContains("TestHarness");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failUnsignedIntTextMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_UINT_TEXT(1, 2, "UnsignedIntTestText");
}

TEST(TestHarness, checkUnsignedIntText)
{
  CHECK_EQUAL_C_UINT_TEXT(2, 2, "Text");
  fixture->setTestFunction(failUnsignedIntTextMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assertPrintContains("TestHarness");
  fixture->assertPrintContains("Message: UnsignedIntTestText");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failLongIntMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_LONG(1, 2);
}

TEST(TestHarness, checkLongInt)
{
  CHECK_EQUAL_C_LONG(2, 2);
  fixture->setTestFunction(failLongIntMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assertPrintContains("TestHarness");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failLongIntTextMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_LONG_TEXT(1, 2, "LongIntTestText");
}

TEST(TestHarness, checkLongIntText)
{
  CHECK_EQUAL_C_LONG_TEXT(2, 2, "Text");
  fixture->setTestFunction(failLongIntTextMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assertPrintContains("TestHarness");
  fixture->assertPrintContains("Message: LongIntTestText");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failUnsignedLongIntMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_ULONG(1, 2);
}

TEST(TestHarness, checkUnsignedLongInt)
{
  CHECK_EQUAL_C_ULONG(2, 2);
  fixture->setTestFunction(failUnsignedLongIntMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assertPrintContains("TestHarness");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failUnsignedLongIntTextMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_ULONG_TEXT(1, 2, "UnsignedLongIntTestText");
}

TEST(TestHarness, checkUnsignedLongIntText)
{
  CHECK_EQUAL_C_ULONG_TEXT(2, 2, "Text");
  fixture->setTestFunction(failUnsignedLongIntTextMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assertPrintContains("TestHarness");
  fixture->assertPrintContains("Message: UnsignedLongIntTestText");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failLongLongIntMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_LONGLONG(1, 2);
}

TEST(TestHarness, checkLongLongInt)
{
  CHECK_EQUAL_C_LONGLONG(2, 2);
  fixture->setTestFunction(failLongLongIntMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assertPrintContains("TestHarness");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failLongLongIntTextMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_LONGLONG_TEXT(1, 2, "LongLongTestText");
}

TEST(TestHarness, checkLongLongIntText)
{
  CHECK_EQUAL_C_LONGLONG_TEXT(2, 2, "Text");
  fixture->setTestFunction(failLongLongIntTextMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assertPrintContains("TestHarness");
  fixture->assertPrintContains("Message: LongLongTestText");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failUnsignedLongLongIntMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_ULONGLONG(1, 2);
}

TEST(TestHarness, checkUnsignedLongLongInt)
{
  CHECK_EQUAL_C_ULONGLONG(2, 2);
  fixture->setTestFunction(failUnsignedLongLongIntMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assertPrintContains("TestHarness");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failUnsignedLongLongIntTextMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_ULONGLONG_TEXT(1, 2, "UnsignedLongLongTestText");
}

TEST(TestHarness, checkUnsignedLongLongIntText)
{
  CHECK_EQUAL_C_ULONGLONG_TEXT(2, 2, "Text");
  fixture->setTestFunction(failUnsignedLongLongIntTextMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <1 (0x1)>\n\tbut was  <2 (0x2)>");
  fixture->assertPrintContains("TestHarness");
  fixture->assertPrintContains("Message: UnsignedLongLongTestText");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failRealMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_REAL(1.0, 2.0, 0.5);
}

TEST(TestHarness, checkReal)
{
  CHECK_EQUAL_C_REAL(1.0, 1.1, 0.5);
  fixture->setTestFunction(failRealMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <1>\n\tbut was  <2>");
  fixture->assertPrintContains("TestHarness");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failRealTextMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_REAL_TEXT(1.0, 2.0, 0.5, "RealTestText");
}

TEST(TestHarness, checkRealText)
{
  CHECK_EQUAL_C_REAL_TEXT(1.0, 1.1, 0.5, "Text");
  fixture->setTestFunction(failRealTextMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <1>\n\tbut was  <2>");
  fixture->assertPrintContains("TestHarness");
  fixture->assertPrintContains("Message: RealTestText");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failCharMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_CHAR('a', 'c');
}

TEST(TestHarness, checkChar)
{
  CHECK_EQUAL_C_CHAR('a', 'a');
  fixture->setTestFunction(failCharMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <a>\n\tbut was  <c>");
  fixture->assertPrintContains("TestHarness");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failCharTextMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_CHAR_TEXT('a', 'c', "CharTestText");
}

TEST(TestHarness, checkCharText)
{
  CHECK_EQUAL_C_CHAR_TEXT('a', 'a', "Text");
  fixture->setTestFunction(failCharTextMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <a>\n\tbut was  <c>");
  fixture->assertPrintContains("TestHarness");
  fixture->assertPrintContains("Message: CharTestText");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failUnsignedByteMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_UBYTE(254, 253);
}

TEST(TestHarness, checkUnsignedByte)
{
  CHECK_EQUAL_C_UBYTE(254, 254);
  fixture->setTestFunction(failUnsignedByteMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <254>\n\tbut was  <253>");
  fixture->assertPrintContains("TestHarness");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failUnsignedByteTextMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_UBYTE_TEXT(254, 253, "UnsignedByteTestText");
}

TEST(TestHarness, checkUnsignedByteText)
{
  CHECK_EQUAL_C_UBYTE_TEXT(254, 254, "Text");
  fixture->setTestFunction(failUnsignedByteTextMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <254>\n\tbut was  <253>");
  fixture->assertPrintContains("TestHarness");
  fixture->assertPrintContains("Message: UnsignedByteTestText");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failSignedByteMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_SBYTE(-3, -5);
}

TEST(TestHarness, checkSignedByte)
{
  CHECK_EQUAL_C_SBYTE(-3, -3);
  fixture->setTestFunction(failSignedByteMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <-3>\n\tbut was  <-5>");
  fixture->assertPrintContains("TestHarness");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failSignedByteTextMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_SBYTE_TEXT(-3, -5, "SignedByteTestText");
}

TEST(TestHarness, checkSignedByteText)
{
  CHECK_EQUAL_C_SBYTE_TEXT(-3, -3, "Text");
  fixture->setTestFunction(failSignedByteTextMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <-3>\n\tbut was  <-5>");
  fixture->assertPrintContains("TestHarness");
  fixture->assertPrintContains("Message: SignedByteTestText");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failStringMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_STRING("Hello", "Hello World");
}

TEST(TestHarness, checkString)
{
  CHECK_EQUAL_C_STRING("Hello", "Hello");
  fixture->setTestFunction(failStringMethod_);
  fixture->runAllTests();

  cpputest::StringEqualFailure failure(
      cpputest::TestShell::getCurrent(), "file", 1, "Hello", "Hello World", "");
  fixture->assertPrintContains(failure.getMessage());
  fixture->assertPrintContains("TestHarness");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failStringTextMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_STRING_TEXT("Hello", "Hello World", "StringTestText");
}

TEST(TestHarness, checkStringText)
{
  CHECK_EQUAL_C_STRING_TEXT("Hello", "Hello", "Text");
  fixture->setTestFunction(failStringTextMethod_);
  fixture->runAllTests();

  cpputest::StringEqualFailure failure(
      cpputest::TestShell::getCurrent(), "file", 1, "Hello", "Hello World", "");
  fixture->assertPrintContains(failure.getMessage());
  fixture->assertPrintContains("TestHarness");
  fixture->assertPrintContains("Message: StringTestText");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failPointerMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_POINTER(nullptr, reinterpret_cast<void*>(0x1));
}

TEST(TestHarness, checkPointer)
{
  CHECK_EQUAL_C_POINTER(nullptr, nullptr);
  fixture->setTestFunction(failPointerMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <0x0>\n\tbut was  <0x1>");
  fixture->assertPrintContains("TestHarness");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failPointerTextMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_EQUAL_C_POINTER_TEXT(
      nullptr, reinterpret_cast<void*>(0x1), "PointerTestText");
}

TEST(TestHarness, checkPointerText)
{
  CHECK_EQUAL_C_POINTER_TEXT(nullptr, nullptr, "Text");
  fixture->setTestFunction(failPointerTextMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("expected <0x0>\n\tbut was  <0x1>");
  fixture->assertPrintContains("TestHarness");
  fixture->assertPrintContains("Message: PointerTestText");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failMemcmpMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  unsigned char expectedData[] = { 0x00, 0x01, 0x02, 0x03 };
  unsigned char actualData[] = { 0x00, 0x01, 0x03, 0x03 };

  CHECK_EQUAL_C_MEMCMP(expectedData, actualData, sizeof(expectedData));
}

TEST(TestHarness, checkMemcmp)
{
  CHECK_EQUAL_C_MEMCMP("TEST", "TEST", 5);
  fixture->setTestFunction(failMemcmpMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains(
      "expected <00 01 02 03>\n\tbut was  <00 01 03 03>");
  fixture->assertPrintContains("TestHarness");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failMemcmpTextMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  unsigned char expectedData[] = { 0x00, 0x01, 0x02, 0x03 };
  unsigned char actualData[] = { 0x00, 0x01, 0x03, 0x03 };

  CHECK_EQUAL_C_MEMCMP_TEXT(
      expectedData, actualData, sizeof(expectedData), "MemcmpTestText");
}

TEST(TestHarness, checkMemcmpText)
{
  CHECK_EQUAL_C_MEMCMP_TEXT("TEST", "TEST", 5, "Text");
  fixture->setTestFunction(failMemcmpTextMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains(
      "expected <00 01 02 03>\n\tbut was  <00 01 03 03>");
  fixture->assertPrintContains("TestHarness");
  fixture->assertPrintContains("Message: MemcmpTestText");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failTextMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  FAIL_TEXT_C("Booo");
}

TEST(TestHarness, checkFailText)
{
  fixture->setTestFunction(failTextMethod_);
  fixture->runAllTests();
  fixture->assertPrintContains("Booo");
  fixture->assertPrintContains("TestHarness");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
failMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  FAIL_C();
}

TEST(TestHarness, checkFail)
{
  fixture->setTestFunction(failMethod_);
  fixture->runAllTests();
  LONGS_EQUAL(1, fixture->getFailureCount());
  fixture->assertPrintContains("TestHarness");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static bool cpputestHasCrashed;

static void
crashMethod()
{
  cpputestHasCrashed = true;
}

TEST(TestHarness, doesNotCrashIfNotSetToCrash)
{
  cpputestHasCrashed = false;
  cpputest::TestShell::setCrashMethod(crashMethod);
  fixture->setTestFunction(failMethod_);

  fixture->runAllTests();

  CHECK_FALSE(cpputestHasCrashed);
  LONGS_EQUAL(1, fixture->getFailureCount());
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);

  cpputest::TestShell::resetCrashMethod();
}

TEST(TestHarness, doesCrashIfSetToCrash)
{
  cpputestHasCrashed = false;
  cpputest::TestShell::setCrashOnFail();
  cpputest::TestShell::setCrashMethod(crashMethod);
  fixture->setTestFunction(failMethod_);

  fixture->runAllTests();

  CHECK(cpputestHasCrashed);
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);

  cpputest::TestShell::restoreDefaultTestTerminator();
  cpputest::TestShell::resetCrashMethod();
}

static void
CheckMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_C(false);
}

TEST(TestHarness, checkCheck)
{
  CHECK_C(true);
  fixture->setTestFunction(CheckMethod_);
  fixture->runAllTests();
  LONGS_EQUAL(1, fixture->getFailureCount());
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}

static void
CheckTextMethod_()
{
  HasTheDestructorBeenCalledChecker checker;
  CHECK_C_TEXT(false, "CheckTestText");
}

TEST(TestHarness, checkCheckText)
{
  CHECK_C_TEXT(true, "Text");
  fixture->setTestFunction(CheckTextMethod_);
  fixture->runAllTests();
  LONGS_EQUAL(1, fixture->getFailureCount());
  fixture->assertPrintContains("Message: CheckTestText");
  CHECK(!hasDestructorOfTheDestructorCheckedBeenCalled);
}
