add_executable(mutiny_unit
    main.cpp
    CompositeTestOutput.test.cpp
    IgnoredTestShell.test.cpp
    JUnitOutputPlugin.test.cpp
    JUnitTestOutput.test.cpp
    MockCall.test.cpp
    MockCheckedActualCall.test.cpp
    MockExpectedCall.test.cpp
    MockExpectedCallsList.test.cpp
    MockIgnoredActualCall.test.cpp
    MockIgnoredExpectedCall.test.cpp
    MockFailure.test.cpp
    MockFailureReporterForTest.cpp
    StringCollection.test.cpp
)

target_link_libraries(mutiny_unit
    PRIVATE
        mu::tiny
        mutiny_test_util
)

target_include_directories(mutiny_unit
    PRIVATE $<TARGET_PROPERTY:mutiny,INCLUDE_DIRECTORIES>
)

include(Mutiny)
mutiny_discover_tests(mutiny_unit)
