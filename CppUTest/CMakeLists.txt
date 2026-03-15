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
            include/CppUTest/SetPointerPlugin.hpp
            include/CppUTest/String.hpp
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
    PRIVATE FILE_SET private_headers TYPE HEADERS
        BASE_DIRS include_private
        FILES
            include_private/CppUTest/CompositeTestOutput.hpp
            include_private/CppUTest/ConsoleTestOutput.hpp
            include_private/CppUTest/ExecFunctionTestShell.hpp
            include_private/CppUTest/JUnitTestOutput.hpp
            include_private/CppUTest/math.hpp
            include_private/CppUTest/MockActualCallTrace.hpp
            include_private/CppUTest/MockCheckedActualCall.hpp
            include_private/CppUTest/MockCheckedExpectedCall.hpp
            include_private/CppUTest/MockExpectedCallsList.hpp
            include_private/CppUTest/MockIgnoredActualCall.hpp
            include_private/CppUTest/MockIgnoredExpectedCall.hpp
            include_private/CppUTest/MockNamedValueList.hpp
            include_private/CppUTest/StringBufferTestOutput.hpp
            include_private/CppUTest/StringCollection.hpp
            include_private/CppUTest/TeamCityTestOutput.hpp
            include_private/CppUTest/TestJumpBuffer.hpp
            include_private/CppUTest/TestTestingFixture.hpp
            include_private/CppUTest/time.hpp
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

if (CPPUTEST_EXTENSIONS)
    add_subdirectory(Extensions)
endif ()

if (CPPUTEST_BUILD_TESTING)
    add_subdirectory(tests)
endif ()
