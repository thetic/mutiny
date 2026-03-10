add_library(CppUTestTests_main OBJECT
    AllTests.cpp
)

target_link_libraries(CppUTestTests_main
    PUBLIC CppUTest
)

include(CppUTest)

add_executable(CppUTestTests
    CheatSheetTest.cpp
    CompatabilityTests.cpp
    CommandLineArgumentsTest.cpp
    CommandLineTestRunnerTest.cpp
    JUnitOutputTest.cpp
    PluginTest.cpp
    PreprocessorTest.cpp
    TeamCityOutputTest.cpp
    TestFailureNaNTest.cpp
    TestFailureTest.cpp
    TestResultTest.cpp
    StringTest.cpp
    TestOutputTest.cpp
    TestRegistryTest.cpp
    TestFilterTest.cpp
    TestHarness_cTest.cpp
    TestHarness_cTestCFile.c
    TestInstallerTest.cpp
    TestMemoryAllocatorTest.cpp
    TestUTestMacro.cpp
    UtestTest.cpp
    TestUTestStringMacro.cpp
)

add_mapfile(CppUTestTests)

cpputest_discover_tests(CppUTestTests)

target_link_libraries(CppUTestTests
    PRIVATE CppUTestTests_main
)
