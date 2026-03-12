add_executable(CppUTestTests
    main.cpp
    CheatSheet.test.cpp
    CommandLineArguments.test.cpp
    CommandLineTestRunner.test.cpp
    Compatibility.test.cpp
    IgnoredTestShell.test.cpp
    JUnitTestOutput.test.cpp
    Preprocessor.test.cpp
    SetPointerPlugin.test.cpp
    String.test.cpp
    TeamCityTestOutput.test.cpp
    TestFailure.test.cpp
    TestFailureNaN.test.cpp
    TestFilter.test.cpp
    TestHarness.test.c
    TestHarness.test.cpp
    TestInstaller.test.cpp
    TestMemoryAllocator.test.cpp
    TestOutput.test.cpp
    TestPlugin.test.cpp
    TestRegistry.test.cpp
    TestResult.test.cpp
    TestShell.test.cpp
    TestShellPointerArray.test.cpp
    UtestMacros.test.cpp
    UtestStringMacros.test.cpp
)

target_link_libraries(CppUTestTests
    PRIVATE CppUTest
)

include(CppUTest)

cpputest_discover_tests(CppUTestTests)
