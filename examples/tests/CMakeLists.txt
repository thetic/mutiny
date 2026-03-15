include(CheckSourceCompiles)
check_source_compiles(CXX
    "#include <fenv.h>
     int main() {
         feclearexcept(FE_ALL_EXCEPT);
         return fetestexcept(FE_OVERFLOW);
     }"
    CPPUTEST_HAVE_FENV
)

add_executable(ExampleTests
    main.cpp
    CheatSheet.test.cpp
    CircularBuffer.test.cpp
    EventDispatcher.test.cpp
    FEDemo.test.cpp
    hello.test.cpp
    hello.test.c
    IEEE754ExceptionsPlugin.c
    IEEE754ExceptionsPlugin.cpp
    IEEE754ExceptionsPlugin.test.cpp
    MockCheatSheet.test.cpp
    MockDocumentation.test.c
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

target_compile_definitions(ExampleTests
    PRIVATE CPPUTEST_HAVE_FENV=$<BOOL:${CPPUTEST_HAVE_FENV}>
)

target_link_libraries(ExampleTests
    PRIVATE
        ExampleLib
        CppUTest::CppUTest
)

include(CppUTest)
cpputest_discover_tests(ExampleTests)
