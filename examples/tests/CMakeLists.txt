add_executable(ExampleTests
    AllTests.cpp
    CircularBuffer.test.cpp
    EventDispatcher.test.cpp
    FEDemo.test.cpp
    hello.test.cpp
    hello.test.c
    MockDocumentation.test.cpp
    Printer.test.cpp
)

target_include_directories(ExampleTests
    PRIVATE
        .
)

target_compile_options(ExampleTests
    PRIVATE $<$<BOOL:${MSVC}>:/wd4723>
)

target_link_libraries(ExampleTests
    PRIVATE
        ExampleLib
        CppUTest::CppUTestExt
)

include(CppUTest)
cpputest_discover_tests(ExampleTests)
