add_executable(CppUTestExtTests
    main.cpp
    OrderedTest.test.c
    OrderedTest.test.cpp
)

target_link_libraries(CppUTestExtTests
    PRIVATE
        CppUTest
        CppUTestExt
)

target_include_directories(CppUTestExtTests PRIVATE
    $<TARGET_PROPERTY:CppUTest,INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:CppUTestExt,INCLUDE_DIRECTORIES>
)

include(CppUTest)

cpputest_discover_tests(CppUTestExtTests)
