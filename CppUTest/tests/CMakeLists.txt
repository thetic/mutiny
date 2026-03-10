add_library(CppUTestTests_main OBJECT
    AllTests.cpp
)

target_link_libraries(CppUTestTests_main
    PUBLIC CppUTest
)

include(CppUTest)

add_executable(CppUTestTests
    CheatSheet.test.cpp
    Compatibility.test.cpp
    CommandLineArguments.test.cpp
    CommandLineTestRunner.test.cpp
    JUnitTestOutput.test.cpp
    TestPlugin.test.cpp
    Preprocessor.test.cpp
    TeamCityTestOutput.test.cpp
    TestFailureNaN.test.cpp
    TestFailure.test.cpp
    TestResult.test.cpp
    String.test.cpp
    TestOutput.test.cpp
    TestRegistry.test.cpp
    TestFilter.test.cpp
    TestHarness.test.cpp
    TestHarness.test.c
    TestInstaller.test.cpp
    TestMemoryAllocator.test.cpp
    UtestMacros.test.cpp
    Utest.test.cpp
    UtestStringMacros.test.cpp
)

add_mapfile(CppUTestTests)

cpputest_discover_tests(CppUTestTests)

target_link_libraries(CppUTestTests
    PRIVATE CppUTestTests_main
)
