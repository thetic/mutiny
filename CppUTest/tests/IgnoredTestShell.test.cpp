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

#include "CppUTest/IgnoredTestShell.hpp"

#include "CppUTest/ExecFunctionTestShell.hpp"
#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestTestingFixture.hpp"

TEST_GROUP(IgnoredUtestShell)
{
  cpputest::TestTestingFixture fixture;
  cpputest::IgnoredTestShell ignoredTest;
  cpputest::ExecFunctionTestShell normalUtestShell;

  void setup() override
  {
    fixture.addTest(&ignoredTest);
    fixture.addTest(&normalUtestShell);
  }
};

TEST(IgnoredUtestShell, doesIgnoreCount)
{
  fixture.runAllTests();
  LONGS_EQUAL(1, fixture.getIgnoreCount());
}

TEST(IgnoredUtestShell, printsIGNORE_TESTwhenVerbose)
{
  fixture.setOutputVerbose();
  fixture.runAllTests();
  fixture.assertPrintContains("IGNORE_TEST");
}

TEST(IgnoredUtestShell, runIgnoredOptionSpecifiedThenIncreaseRunCount)
{
  ignoredTest.setRunIgnored();
  fixture.runAllTests();
  LONGS_EQUAL(3, fixture.getRunCount());
  LONGS_EQUAL(0, fixture.getIgnoreCount());
}

TEST(IgnoredUtestShell, runIgnoredOptionNotSpecifiedThenIncreaseIgnoredCount)
{
  fixture.runAllTests();
  LONGS_EQUAL(2, fixture.getRunCount());
  LONGS_EQUAL(1, fixture.getIgnoreCount());
}

TEST(IgnoredUtestShell,
    runIgnoredOptionSpecifiedWillNotInfluenceNormalTestCount)
{
  normalUtestShell.setRunIgnored();
  fixture.runAllTests();
  LONGS_EQUAL(2, fixture.getRunCount());
  LONGS_EQUAL(1, fixture.getIgnoreCount());
}

TEST(IgnoredUtestShell, runIgnoredOptionSpecifiedThenReturnTESTInFormattedName)
{
  ignoredTest.setGroupName("TestGroup");
  ignoredTest.setTestName("TestName");
  ignoredTest.setRunIgnored();
  fixture.runAllTests();
  STRCMP_EQUAL(
      "TEST(TestGroup, TestName)", ignoredTest.getFormattedName().c_str());
}

TEST(IgnoredUtestShell,
    runIgnoredOptionNotSpecifiedThenReturnIGNORETESTInFormattedName)
{
  ignoredTest.setGroupName("TestGroup");
  ignoredTest.setTestName("TestName");
  fixture.runAllTests();
  STRCMP_EQUAL("IGNORE_TEST(TestGroup, TestName)",
      ignoredTest.getFormattedName().c_str());
}

TEST(IgnoredUtestShell, runIgnoredOptionNotSpecifiedThenWillRunReturnFalse)
{
  CHECK_FALSE(ignoredTest.willRun());
}

TEST(IgnoredUtestShell, runIgnoredOptionSpecifiedThenWillRunReturnTrue)
{
  ignoredTest.setRunIgnored();
  CHECK_TRUE(ignoredTest.willRun());
}
