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
    MockExpectedCallsList.test.cpp
    MockCheckedActualCall.test.cpp
    MockCheatSheet.test.cpp
    MockComparatorCopier.test.cpp
    MockExpectedCall.test.cpp
    MockHierarchy.test.cpp
    OrderedTest.test.cpp
    OrderedTest.test.c
    MockReturnValue.test.cpp
    MockNamedValue.test.cpp
    MockSupportPlugin.test.cpp
    MockSupport_c.test.cpp
    MockSupport.test.c
    MockCall.test.cpp
    MockParameter.test.cpp
    IEEE754ExceptionsPlugin.test.cpp
    IEEE754ExceptionsPlugin.test.c
    MockFailure.test.cpp
    MockStrictOrder.test.cpp
    MockSupport.test.cpp
)

cpputest_discover_tests(CppUTestExtTests
    DETAILED FALSE
)

target_link_libraries(CppUTestExtTests
    PRIVATE CppUTestExtTests_main
)
