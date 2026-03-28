add_executable(mutiny_unit
    main.cpp
    CompositeTestOutput.test.cpp
    IgnoredTestShell.test.cpp
    JUnitTestOutput.test.cpp
    MockCall.test.cpp
    MockCheckedActualCall.test.cpp
    MockComparatorCopier.test.cpp
    MockExpectedCall.test.cpp
    MockExpectedCallsList.test.cpp
    MockIgnoredActualCall.test.cpp
    MockIgnoredExpectedCall.test.cpp
    MockNamedValueComparatorsAndCopiersRepository.test.cpp
    MockFailure.test.cpp
    MockFailureReporterForTest.cpp
    MockHierarchy.test.cpp
    MockParameter.test.cpp
    MockReturnValue.test.cpp
    MockStrictOrder.test.cpp
    MockSupport.test.cpp
    MockSupportWithFixture.test.cpp
    MockSupportPlugin.test.cpp
    OrderedTest.test.cpp
    StringCollection.test.cpp
    TestShell.test.cpp
)

target_link_libraries(mutiny_unit
    PRIVATE mutiny
)
 target_include_directories(mutiny_unit
    PRIVATE $<TARGET_PROPERTY:mutiny,INCLUDE_DIRECTORIES>
 )

include(mutiny)
mutiny_discover_tests(mutiny_unit)
