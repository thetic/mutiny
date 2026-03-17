include(CheckSourceRuns)
# Verify that FP exception flags are actually raised at runtime, not just that
# fenv.h compiles.  Soft-float targets (e.g. ARM without -mfpu/-mfloat-abi)
# compile fenv.h cleanly but never write FPSCR, so fetestexcept() returns 0.
check_source_runs(CXX
    "#include <fenv.h>
     int main(void) {
         volatile float z = 0.0f;
         volatile float r = 1.0f / z;
         (void)r;
         return fetestexcept(FE_DIVBYZERO) ? 0 : 1;
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
    IEEE754ExceptionsPlugin.cpp
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
