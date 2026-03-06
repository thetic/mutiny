add_library(CppUTestTests_main OBJECT
    AllTests.cpp
)

if(CPPUTEST_STD_C_LIB_DISABLED)
    target_sources(CppUTestTests_main
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/../DummyUTestPlatform/DummyUTestPlatform.cpp
    )
endif()

target_link_libraries(CppUTestTests_main
    PUBLIC CppUTest
)

include(CppUTest)

add_executable(CppUTestTests
    AllocLetTestFree.c
    AllocLetTestFreeTest.cpp
    CheatSheetTest.cpp
    CompatabilityTests.cpp
    CommandLineArgumentsTest.cpp
    CommandLineTestRunnerTest.cpp
    JUnitOutputTest.cpp
    DummyMemoryLeakDetector.cpp
    MemoryLeakWarningTest.cpp
    PluginTest.cpp
    PreprocessorTest.cpp
    SetPluginTest.cpp
    SimpleMutexTest.cpp
    TeamCityOutputTest.cpp
    TestFailureNaNTest.cpp
    TestFailureTest.cpp
    TestResultTest.cpp
    MemoryLeakDetectorTest.cpp
    SimpleStringTest.cpp
    SimpleStringCacheTest.cpp
    TestOutputTest.cpp
    TestRegistryTest.cpp
    AllocationInCFile.c
    AllocationInCppFile.cpp
    MemoryOperatorOverloadTest.cpp
    TestFilterTest.cpp
    TestHarness_cTest.cpp
    TestHarness_cTestCFile.c
    TestInstallerTest.cpp
    TestMemoryAllocatorTest.cpp
    TestUTestMacro.cpp
    UtestPlatformTest.cpp
    UtestTest.cpp
    TestUTestStringMacro.cpp
)

add_mapfile(CppUTestTests)

cpputest_discover_tests(CppUTestTests)

target_link_libraries(CppUTestTests
    PRIVATE CppUTestTests_main
)
