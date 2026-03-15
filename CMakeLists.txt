cmake_minimum_required(VERSION 4.0)

project(CppUTest
    VERSION 4.0
)

###############
# Conan support
###############
if (EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    message(STATUS "conan_basic_setup()")
    conan_basic_setup()
endif()

include(CTest)

include(CMakeDependentOption)
option(CPPUTEST_USE_STD_CPP_LIB "Use the standard C++ library" ON)
cmake_dependent_option(CPPUTEST_BUILD_TESTING "Compile and make tests for CppUTest"
    ${PROJECT_IS_TOP_LEVEL} "BUILD_TESTING" OFF)
cmake_dependent_option(CPPUTEST_EXAMPLES "Compile and make examples?"
    ${PROJECT_IS_TOP_LEVEL} "CPPUTEST_USE_STD_CPP_LIB" OFF)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
if(NOT PROJECT_IS_TOP_LEVEL)
    set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" PARENT_SCOPE)
endif()

include(GNUInstallDirs)

add_library(CppUTest)

target_sources(CppUTest
    PRIVATE
        src/CommandLineArguments.cpp
        src/CommandLineTestRunner.cpp
        src/CompositeTestOutput.cpp
        src/ConsoleTestOutput.cpp
        src/CppUTest.cpp
        src/ExecFunctionTestShell.cpp
        src/ExpectFailTestShell.cpp
        src/IgnoredTestShell.cpp
        src/JUnitTestOutput.cpp
        src/math.cpp
        src/MockActualCall.cpp
        src/MockActualCallTrace.cpp
        src/MockCheckedActualCall.cpp
        src/MockCheckedExpectedCall.cpp
        src/MockExpectedCall.cpp
        src/MockExpectedCallsList.cpp
        src/MockFailure.cpp
        src/MockIgnoredExpectedCall.cpp
        src/MockNamedValue.cpp
        src/MockNamedValueComparatorsAndCopiersRepository.cpp
        src/MockNamedValueList.cpp
        src/MockSupport.cpp
        src/MockSupportPlugin.cpp
        src/SetPointerPlugin.cpp
        src/String.cpp
        src/StringBufferTestOutput.cpp
        src/StringCollection.cpp
        src/TeamCityTestOutput.cpp
        src/Test.cpp
        src/TestFailure.cpp
        src/TestFilter.cpp
        src/OrderedTest.cpp
        src/TestInstaller.cpp
        src/TestJumpBuffer.cpp
        src/TestOutput.cpp
        src/TestPlugin.cpp
        src/TestRegistry.cpp
        src/TestResult.cpp
        src/TestShell.cpp
        src/TestShellPointerArray.cpp
        src/TestTerminator.cpp
        src/TestTestingFixture.cpp
        src/time.cpp
    PUBLIC FILE_SET HEADERS
        BASE_DIRS include
        FILES
            include/CppUTest/CommandLineArguments.hpp
            include/CppUTest/CommandLineTestRunner.hpp
            include/CppUTest/CppUTest.h
            include/CppUTest/CppUTest.hpp
            include/CppUTest/ExpectFailTestShell.hpp
            include/CppUTest/IgnoredTestShell.hpp
            include/CppUTest/MockActualCall.hpp
            include/CppUTest/MockExpectedCall.hpp
            include/CppUTest/MockFailure.hpp
            include/CppUTest/MockNamedValue.hpp
            include/CppUTest/MockNamedValueComparator.hpp
            include/CppUTest/MockNamedValueComparatorsAndCopiersRepository.hpp
            include/CppUTest/MockNamedValueCopier.hpp
            include/CppUTest/MockSupport.h
            include/CppUTest/MockSupport.hpp
            include/CppUTest/MockSupportPlugin.hpp
            include/CppUTest/OrderedTest.hpp
            include/CppUTest/SetPointerPlugin.hpp
            include/CppUTest/String.hpp
            include/CppUTest/StringBufferTestOutput.hpp
            include/CppUTest/Test.hpp
            include/CppUTest/TestFailure.hpp
            include/CppUTest/TestFilter.hpp
            include/CppUTest/TestInstaller.hpp
            include/CppUTest/TestOutput.hpp
            include/CppUTest/TestPlugin.hpp
            include/CppUTest/TestRegistry.hpp
            include/CppUTest/TestResult.hpp
            include/CppUTest/TestShell.hpp
            include/CppUTest/TestShellPointerArray.hpp
            include/CppUTest/TestTerminator.hpp
            include/CppUTest/TestTestingFixture.hpp
    PRIVATE FILE_SET private_headers TYPE HEADERS
        BASE_DIRS src/include
        FILES
            src/include/CppUTest/CompositeTestOutput.hpp
            src/include/CppUTest/ConsoleTestOutput.hpp
            src/include/CppUTest/ExecFunctionTestShell.hpp
            src/include/CppUTest/JUnitTestOutput.hpp
            src/include/CppUTest/math.hpp
            src/include/CppUTest/MockActualCallTrace.hpp
            src/include/CppUTest/MockCheckedActualCall.hpp
            src/include/CppUTest/MockCheckedExpectedCall.hpp
            src/include/CppUTest/MockExpectedCallsList.hpp
            src/include/CppUTest/MockIgnoredActualCall.hpp
            src/include/CppUTest/MockIgnoredExpectedCall.hpp
            src/include/CppUTest/MockNamedValueList.hpp
            src/include/CppUTest/StringCollection.hpp
            src/include/CppUTest/TeamCityTestOutput.hpp
            src/include/CppUTest/TestJumpBuffer.hpp
            src/include/CppUTest/time.hpp
)

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

if (CPPUTEST_BUILD_TESTING)
    add_subdirectory(tests)
endif ()

if (CPPUTEST_EXAMPLES)
    add_subdirectory(examples)
endif()

if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory(package)
endif()
