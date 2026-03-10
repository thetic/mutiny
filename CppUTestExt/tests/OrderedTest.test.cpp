/*
 * Copyright (c) 2007, Michael Feathers, James Grenning and Bas Vodde
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "CppUTestExt/OrderedTest.hpp"

#include "OrderedTestTest.h"

#include "CppUTest/ExecFunctionTestShell.hpp"
#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestOutput.hpp"
#include "CppUTest/TestRegistry.hpp"
#include "CppUTest/TestTestingFixture.hpp"

TEST_GROUP(TestOrderedTest)
{
  cpputest::TestTestingFixture* fixture;

  cpputest::extensions::OrderedTestShell orderedTest;
  cpputest::extensions::OrderedTestShell orderedTest2;
  cpputest::extensions::OrderedTestShell orderedTest3;
  cpputest::ExecFunctionTestShell normalTest;
  cpputest::ExecFunctionTestShell normalTest2;
  cpputest::ExecFunctionTestShell normalTest3;

  cpputest::extensions::OrderedTestShell* orderedTestCache;
  void setup() override
  {
    orderedTestCache =
        cpputest::extensions::OrderedTestShell::getOrderedTestHead();
    cpputest::extensions::OrderedTestShell::setOrderedTestHead(nullptr);

    fixture = new cpputest::TestTestingFixture();
    fixture->getRegistry()->unDoLastAddTest();
  }

  void teardown() override
  {
    delete fixture;
    cpputest::extensions::OrderedTestShell::setOrderedTestHead(
        orderedTestCache);
  }

  void InstallOrderedTest(cpputest::extensions::OrderedTestShell& test,
      int level)
  {
    cpputest::extensions::OrderedTestInstaller(
        test, "testgroup", "testname", __FILE__, __LINE__, level);
  }

  void InstallNormalTest(cpputest::TestShell& test)
  {
    cpputest::TestInstaller(test, "testgroup", "testname", __FILE__, __LINE__);
  }

  cpputest::TestShell* firstTest()
  {
    return fixture->getRegistry()->getFirstTest();
  }

  cpputest::TestShell* secondTest() { return firstTest()->getNext(); }
};

TEST(TestOrderedTest, TestInstallerSetsFields)
{
  cpputest::extensions::OrderedTestInstaller installer(
      orderedTest, "testgroup", "testname", "this.cpp", 10, 5);
  STRCMP_EQUAL("testgroup", orderedTest.getGroup().c_str());
  STRCMP_EQUAL("testname", orderedTest.getName().c_str());
  STRCMP_EQUAL("this.cpp", orderedTest.getFile().c_str());
  LONGS_EQUAL(10, orderedTest.getLineNumber());
  LONGS_EQUAL(5, orderedTest.getLevel());
}

TEST(TestOrderedTest, InstallOneText)
{
  InstallOrderedTest(orderedTest, 5);
  CHECK(firstTest() == &orderedTest);
}

TEST(TestOrderedTest, OrderedTestsAreLast)
{
  InstallNormalTest(normalTest);
  InstallOrderedTest(orderedTest, 5);
  CHECK(firstTest() == &normalTest);
  CHECK(secondTest() == &orderedTest);
}

TEST(TestOrderedTest, TwoTestsAddedInReverseOrder)
{
  InstallOrderedTest(orderedTest, 5);
  InstallOrderedTest(orderedTest2, 3);
  CHECK(firstTest() == &orderedTest2);
  CHECK(secondTest() == &orderedTest);
}

TEST(TestOrderedTest, TwoTestsAddedInOrder)
{
  InstallOrderedTest(orderedTest2, 3);
  InstallOrderedTest(orderedTest, 5);
  CHECK(firstTest() == &orderedTest2);
  CHECK(secondTest() == &orderedTest);
}

TEST(TestOrderedTest, MultipleOrderedTests)
{
  InstallNormalTest(normalTest);
  InstallOrderedTest(orderedTest2, 3);
  InstallNormalTest(normalTest2);
  InstallOrderedTest(orderedTest, 5);
  InstallNormalTest(normalTest3);
  InstallOrderedTest(orderedTest3, 7);

  cpputest::TestShell* firstOrderedTest =
      firstTest()->getNext()->getNext()->getNext();
  CHECK(firstOrderedTest == &orderedTest2);
  CHECK(firstOrderedTest->getNext() == &orderedTest);
  CHECK(firstOrderedTest->getNext()->getNext() == &orderedTest3);
}

TEST(TestOrderedTest, MultipleOrderedTests2)
{
  InstallOrderedTest(orderedTest, 3);
  InstallOrderedTest(orderedTest2, 1);
  InstallOrderedTest(orderedTest3, 2);

  CHECK(firstTest() == &orderedTest2);
  CHECK(secondTest() == &orderedTest3);
  CHECK(secondTest()->getNext() == &orderedTest);
}

class OrderedTestTestingFixture
{
public:
  static void checkRun(int run)
  {
    if (run != run_) {
      run_ = run;
      count_ = 0;
    }
  }
  static int count(void) { return count_++; }

private:
  static int run_;
  static int count_;
};

int OrderedTestTestingFixture::run_ = 0;
int OrderedTestTestingFixture::count_ = 0;

TEST_GROUP(TestOrderedTestMacros)
{
  void setup() override
  {
    OrderedTestTestingFixture::checkRun(
        cpputest::TestRegistry::getCurrentRegistry()->getCurrentRepetition());
  }
};

TEST(TestOrderedTestMacros, NormalTest)
{
  CHECK(OrderedTestTestingFixture::count() == 0);
}

TEST_ORDERED(TestOrderedTestMacros, Test2, 2)
{
  CHECK(OrderedTestTestingFixture::count() == 2);
}

TEST_ORDERED(TestOrderedTestMacros, Test1, 1)
{
  CHECK(OrderedTestTestingFixture::count() == 1);
}

TEST_ORDERED(TestOrderedTestMacros, Test4, 4)
{
  CHECK(OrderedTestTestingFixture::count() == 4);
}

TEST_ORDERED(TestOrderedTestMacros, Test3, 3)
{
  CHECK(OrderedTestTestingFixture::count() == 3);
}

// Test with same level
TEST_ORDERED(TestOrderedTestMacros, Test5_1, 5)
{
  CHECK(OrderedTestTestingFixture::count() == 5);
}

TEST_ORDERED(TestOrderedTestMacros, Test6_1, 6)
{
  CHECK(OrderedTestTestingFixture::count() == 7);
}

TEST_ORDERED(TestOrderedTestMacros, Test5_2, 5)
{
  CHECK(OrderedTestTestingFixture::count() == 6);
}

TEST_ORDERED(TestOrderedTestMacros, Test6_2, 6)
{
  CHECK(OrderedTestTestingFixture::count() == 8);
}

// Test C-Interface
TEST_ORDERED(TestOrderedTestMacros, Test10, 10)
{
  CHECK(OrderedTestTestingFixture::count() == 12);
}

TEST_ORDERED(TestOrderedTestMacros, Test8, 8)
{
  CHECK(OrderedTestTestingFixture::count() == 10);
}

int
orderedTestFixtureCWrapper(void)
{
  return OrderedTestTestingFixture::count();
}

TEST_ORDERED_C_WRAPPER(TestOrderedTestMacros, Test11, 11)

TEST_ORDERED_C_WRAPPER(TestOrderedTestMacros, Test7, 7)

TEST_ORDERED_C_WRAPPER(TestOrderedTestMacros, Test9, 9)
