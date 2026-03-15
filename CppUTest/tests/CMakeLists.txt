add_executable(CppUTestTests
    main.cpp
    CommandLineArguments.test.cpp
    CommandLineTestRunner.test.cpp
    Compatibility.test.cpp
    CppUTest.test.c
    CppUTest.test.cpp
    ExpectFailTestShell.test.cpp
    IgnoredTestShell.test.cpp
    JUnitTestOutput.test.cpp
    Preprocessor.test.cpp
    SetPointerPlugin.test.cpp
    String.test.cpp
    StringCollection.test.cpp
    TeamCityTestOutput.test.cpp
    TestFailure.test.cpp
    TestFailureNaN.test.cpp
    TestFilter.test.cpp
    TestInstaller.test.cpp
    TestOutput.test.cpp
    TestPlugin.test.cpp
    TestRegistry.test.cpp
    TestResult.test.cpp
    TestShell.test.cpp
    TestShellPointerArray.test.cpp
)

target_link_libraries(CppUTestTests
    PRIVATE CppUTest
)

target_include_directories(CppUTestTests PRIVATE
    $<TARGET_PROPERTY:CppUTest,INCLUDE_DIRECTORIES>
)

include(CppUTest)

cpputest_discover_tests(CppUTestTests)
