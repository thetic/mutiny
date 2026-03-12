add_library(CppUTest)

target_sources(CppUTest
    PRIVATE
        src/CommandLineArguments.cpp
        src/CommandLineTestRunner.cpp
        src/CompositeTestOutput.cpp
        src/ConsoleTestOutput.cpp
        src/ExecFunctionTestShell.cpp
        src/IgnoredTestShell.cpp
        src/JUnitTestOutput.cpp
        src/SetPointerPlugin.cpp
        src/String.cpp
        src/StringBufferTestOutput.cpp
        src/TeamCityTestOutput.cpp
        src/Test.cpp
        src/TestFailure.cpp
        src/TestFilter.cpp
        src/TestHarness.cpp
        src/TestInstaller.cpp
        src/TestMemoryAllocator.cpp
        src/TestOutput.cpp
        src/TestPlugin.cpp
        src/TestRegistry.cpp
        src/TestResult.cpp
        src/TestShell.cpp
        src/TestShellPointerArray.cpp
        src/TestTerminator.cpp
        src/TestTestingFixture.cpp
    PUBLIC FILE_SET HEADERS
        BASE_DIRS include
        FILES
            include/CppUTest/CommandLineArguments.hpp
            include/CppUTest/CommandLineTestRunner.hpp
            include/CppUTest/CompositeTestOutput.hpp
            include/CppUTest/ConsoleTestOutput.hpp
            include/CppUTest/ExecFunctionTestShell.hpp
            include/CppUTest/IgnoredTestShell.hpp
            include/CppUTest/JUnitTestOutput.hpp
            include/CppUTest/PlatformSpecificFunctions.h
            include/CppUTest/SetPointerPlugin.hpp
            include/CppUTest/String.hpp
            include/CppUTest/StringBufferTestOutput.hpp
            include/CppUTest/TeamCityTestOutput.hpp
            include/CppUTest/Test.hpp
            include/CppUTest/TestFailure.hpp
            include/CppUTest/TestFilter.hpp
            include/CppUTest/TestHarness.h
            include/CppUTest/TestHarness.hpp
            include/CppUTest/TestInstaller.hpp
            include/CppUTest/TestMemoryAllocator.hpp
            include/CppUTest/TestOutput.hpp
            include/CppUTest/TestPlugin.hpp
            include/CppUTest/TestRegistry.hpp
            include/CppUTest/TestResult.hpp
            include/CppUTest/TestShell.hpp
            include/CppUTest/TestShellPointerArray.hpp
            include/CppUTest/TestTerminator.hpp
            include/CppUTest/TestTestingFixture.hpp
            include/CppUTest/UtestMacros.hpp
)

#[[Set CPPUTEST_PLATFORM in a parent CMakeLists.txt if reusing one of the provided platforms, else supply the missing definitions]]
if(CPPUTEST_PLATFORM)
    target_sources(CppUTest
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/platforms/${CPPUTEST_PLATFORM}/PlatformSpecificFunctions.cpp
    )
endif()

include(CheckCXXSymbolExists)
set(CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_FLAGS})
check_cxx_symbol_exists(gettimeofday "sys/time.h" CPPUTEST_HAVE_GETTIMEOFDAY)

target_compile_features(CppUTest
    INTERFACE c_std_11
    PUBLIC cxx_std_11
)

target_compile_definitions(CppUTest
    PUBLIC
        CPPUTEST_USE_STD_CPP_LIB=$<BOOL:${CPPUTEST_USE_STD_CPP_LIB}>
    PRIVATE
        $<$<BOOL:${CPPUTEST_HAVE_GETTIMEOFDAY}>:CPPUTEST_HAVE_GETTIMEOFDAY>
)

add_library(CppUTest::CppUTest ALIAS CppUTest)

if(PROJECT_IS_TOP_LEVEL)
    install(
        TARGETS CppUTest
        EXPORT CppUTestTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()

if (CPPUTEST_EXTENSIONS)
    add_subdirectory(Extensions)
endif ()

if (CPPUTEST_BUILD_TESTING)
    add_subdirectory(tests)
endif ()
