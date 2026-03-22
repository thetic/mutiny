add_executable(CppMuTests
    main.cpp
    CommandLineArguments.test.cpp
    CommandLineTestRunner.test.cpp
    Compatibility.test.cpp
    CompositeTestOutput.test.cpp
    CppMuC.test.c
    CppMuC.test.cpp
    CppMuCFixture.test.cpp
    fail_method_c_body.c
    ExpectFailTestShell.test.cpp
    IgnoredTestShell.test.cpp
    JUnitTestOutput.test.cpp
    MockCall.test.cpp
    MockCheckedActualCall.test.cpp
    MockComparatorCopier.test.cpp
    MockExpectedCall.test.cpp
    MockExpectedCallsList.test.cpp
    MockIgnoredExpectedCall.test.cpp
    MockNamedValueComparatorsAndCopiersRepository.test.cpp
    MockFailure.test.cpp
    MockFailureReporterForTest.cpp
    MockHierarchy.test.cpp
    MockNamedValue.test.cpp
    MockParameter.test.cpp
    MockReturnValue.test.cpp
    MockStrictOrder.test.cpp
    MockSupport.c
    MockSupport.test.cpp
    MockSupportWithFixture.test.cpp
    MockSupport_c.test.cpp
    MockSupportPlugin.test.cpp
    OrderedTestC.test.c
    OrderedTest.test.cpp
    OrderedTestC.test.cpp
    Preprocessor.test.cpp
    SetPointerPlugin.test.cpp
    String.test.cpp
    StringCollection.test.cpp
    TestFailure.test.cpp
    TestFailureNaN.test.cpp
    TestFilter.test.cpp
    TestInstaller.test.cpp
    TestOutput.test.cpp
    TestPlugin.test.cpp
    TestRegistry.test.cpp
    TestResult.test.cpp
    TestShell.test.cpp
    TestShellCustomBase.test.cpp
    TestShellMacros.test.cpp
    TestShellPointerArray.test.cpp
    TestShellProperty.test.cpp
    TestShellStringMacros.test.cpp
)

target_link_libraries(CppMuTests
    PRIVATE CppMu
)

target_include_directories(CppMuTests PRIVATE
    $<TARGET_PROPERTY:CppMu,INCLUDE_DIRECTORIES>
)

include(CppMu)

cppmu_discover_tests(CppMuTests)

add_test(
    NAME cppmu_discover_tests
    COMMAND ${CMAKE_COMMAND}
        -D "RunCMake_GENERATOR=${CMAKE_GENERATOR}"
        -D "RunCMake_GENERATOR_PLATFORM=${CMAKE_GENERATOR_PLATFORM}"
        -D "RunCMake_GENERATOR_TOOLSET=${CMAKE_GENERATOR_TOOLSET}"
        -D "RunCMake_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/cmake/CppMu"
        -D "RunCMake_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}/cmake/CppMu"
        -D "CPPMU_MODULE_PATH=${PROJECT_SOURCE_DIR}/cmake/Modules"
        -D "CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CppMu/RunCMakeTest.cmake"
)
