include(CheckSourceCompiles)
check_source_compiles(CXX
    "#include <fenv.h>
     int main() {
         feclearexcept(FE_ALL_EXCEPT);
         return fetestexcept(FE_OVERFLOW);
     }"
    CPPMU_HAVE_FENV
)

add_executable(ExampleTests
    main.cpp
    CheatSheet.test.cpp
    CircularBuffer.test.cpp
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

if(CPPMU_USE_STD_CPP_LIB)
    target_sources(ExampleTests PRIVATE EventDispatcher.test.cpp)
endif()

target_include_directories(ExampleTests
    PRIVATE
        .
)

target_compile_definitions(ExampleTests
    PRIVATE CPPMU_HAVE_FENV=$<BOOL:${CPPMU_HAVE_FENV}>
)

target_link_libraries(ExampleTests
    PRIVATE
        ExampleLib
        CppMu::CppMu
)

include(CppMu)
cppmu_discover_tests(ExampleTests)
