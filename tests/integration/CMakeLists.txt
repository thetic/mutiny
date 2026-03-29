add_executable(mutiny_integration
    main.cpp
    CommandLineArguments.test.cpp
    CommandLineTestRunner.test.cpp
    Compatibility.test.cpp
    # CompositeTestOutput.test.cpp
    mutiny_c.test.c
    mutiny_c.test.cpp
    mutiny_c_fixture.test.cpp
    fail_method_c_body.c
    ExpectFailTestShell.test.cpp
    # IgnoredTestShell.test.cpp
    # JUnitTestOutput.test.cpp
    # MockCall.test.cpp
    # MockCheckedActualCall.test.cpp
    # MockComparatorCopier.test.cpp
    # MockExpectedCall.test.cpp
    # MockExpectedCallsList.test.cpp
    # MockIgnoredActualCall.test.cpp
    # MockIgnoredExpectedCall.test.cpp
    # MockNamedValueComparatorsAndCopiersRepository.test.cpp
    # MockFailure.test.cpp
    # MockFailureReporterForTest.cpp
    # MockHierarchy.test.cpp
    MockNamedValue.test.cpp
    # MockParameter.test.cpp
    # MockReturnValue.test.cpp
    # MockStrictOrder.test.cpp
    MockSupport.c
    # MockSupport.test.cpp
    # MockSupportWithFixture.test.cpp
    MockSupport_c.test.cpp
    # MockSupportPlugin.test.cpp
    OrderedTestC.test.c
    Ordered.test.cpp
    OrderedTestC.test.cpp
    Preprocessor.test.cpp
    SetPointerPlugin.test.cpp
    String.test.cpp
    TestFailure.test.cpp
    TestFailureNaN.test.cpp
    TestFilter.test.cpp
    TestInstaller.test.cpp
    TestOutput.test.cpp
    TestPlugin.test.cpp
    TestRegistry.test.cpp
    TestResult.test.cpp
    Shell.test.cpp
    TestShellCustomBase.test.cpp
    TestShellMacros.test.cpp
    TestShellPointerArray.test.cpp
    TestShellProperty.test.cpp
    TestShellStringMacros.test.cpp
)

target_link_libraries(mutiny_integration
    PRIVATE mutiny
)

include(mutiny)
mutiny_discover_tests(mutiny_integration)
