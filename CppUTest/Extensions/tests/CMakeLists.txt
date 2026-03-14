add_executable(CppUTestExtTests
    main.cpp
    IEEE754ExceptionsPlugin.c
    IEEE754ExceptionsPlugin.test.cpp
    MockCall.test.cpp
    MockCheckedActualCall.test.cpp
    MockComparatorCopier.test.cpp
    MockExpectedCall.test.cpp
    MockExpectedCallsList.test.cpp
    MockFailure.test.cpp
    MockFailureReporterForTest.cpp
    MockHierarchy.test.cpp
    MockNamedValue.test.cpp
    MockParameter.test.cpp
    MockReturnValue.test.cpp
    MockStrictOrder.test.cpp
    MockSupport.c
    MockSupport.test.cpp
    MockSupportPlugin.test.cpp
    MockSupport_c.test.cpp
    OrderedTest.test.c
    OrderedTest.test.cpp
)

target_link_libraries(CppUTestExtTests
    PRIVATE
        CppUTest
        CppUTestExt
)

include(CppUTest)

cpputest_discover_tests(CppUTestExtTests)
