add_library(CppUTestExtTests_main OBJECT
    AllTests.cpp
)

target_link_libraries(CppUTestExtTests_main
    PUBLIC
        CppUTest
        CppUTestExt
)

include(CppUTest)

add_executable(CppUTestExtTests
    MockFailureReporterForTest.cpp
    ExpectedFunctionsListTest.cpp
    MemoryReportAllocatorTest.cpp
    MemoryReportFormatterTest.cpp
    MemoryReporterPluginTest.cpp
    MockActualCallTest.cpp
    MockCheatSheetTest.cpp
    MockComparatorCopierTest.cpp
    MockExpectedCallTest.cpp
    MockHierarchyTest.cpp
    CodeMemoryReporterTest.cpp
    OrderedTestTest.cpp
    OrderedTestTest_c.c
    MockReturnValueTest.cpp
    MockNamedValueTest.cpp
    MockPluginTest.cpp
    MockSupport_cTest.cpp
    MockSupport_cTestCFile.c
    MockCallTest.cpp
    MockParameterTest.cpp
    IEEE754PluginTest.cpp
    IEEE754PluginTest_c.c
    MockFailureTest.cpp
    MockStrictOrderTest.cpp
    MockSupportTest.cpp
)

add_mapfile(CppUTestExtTests)

cpputest_discover_tests(CppUTestExtTests
    DETAILED FALSE
)

target_link_libraries(CppUTestExtTests
    PRIVATE CppUTestExtTests_main
)
