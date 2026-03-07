add_library(CppUTest
        src/CommandLineArguments.cpp
        src/TestHarness_c.cpp
        src/TestRegistry.cpp
        src/CommandLineTestRunner.cpp
        src/SimpleString.cpp
        src/SimpleStringInternalCache.cpp
        src/TestMemoryAllocator.cpp
        src/TestResult.cpp
        src/JUnitTestOutput.cpp
        src/TeamCityTestOutput.cpp
        src/TestFailure.cpp
        src/TestOutput.cpp
        src/TestFilter.cpp
        src/TestPlugin.cpp
        src/TestTestingFixture.cpp
        src/SimpleMutex.cpp
        src/Utest.cpp
)

target_sources(CppUTest
    PUBLIC FILE_SET HEADERS
        BASE_DIRS include
        FILES
            include/CppUTest/CommandLineArguments.h
            include/CppUTest/CommandLineTestRunner.h
            include/CppUTest/CppUTestConfig.h
            include/CppUTest/JUnitTestOutput.h
            include/CppUTest/PlatformSpecificFunctions.h
            include/CppUTest/PlatformSpecificFunctions_c.h
            include/CppUTest/SimpleMutex.h
            include/CppUTest/SimpleString.h
            include/CppUTest/SimpleStringInternalCache.h
            include/CppUTest/TeamCityTestOutput.h
            include/CppUTest/TestFailure.h
            include/CppUTest/TestFilter.h
            include/CppUTest/TestHarness.h
            include/CppUTest/TestHarness_c.h
            include/CppUTest/TestMemoryAllocator.h
            include/CppUTest/TestOutput.h
            include/CppUTest/TestPlugin.h
            include/CppUTest/TestRegistry.h
            include/CppUTest/TestResult.h
            include/CppUTest/TestTestingFixture.h
            include/CppUTest/Utest.h
            include/CppUTest/UtestMacros.h
)

#[[Set CPPUTEST_PLATFORM in a parent CMakeLists.txt if reusing one of the provided platforms, else supply the missing definitions]]
if(CPPUTEST_PLATFORM)
    target_sources(CppUTest
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/platforms/${CPPUTEST_PLATFORM}/UtestPlatform.cpp
    )
endif()

if(CPPUTEST_PLATFORM STREQUAL "Iar")
    target_compile_options(CppUTest
        PRIVATE
            --dlib_config full
    )
endif()

include(CheckCXXSymbolExists)
set(CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_FLAGS})
check_cxx_symbol_exists(gettimeofday "sys/time.h" CPPUTEST_HAVE_GETTIMEOFDAY)
check_cxx_symbol_exists(pthread_mutex_lock "pthread.h" CPPUTEST_HAVE_PTHREAD_MUTEX_LOCK)

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "IAR")
    check_cxx_symbol_exists(strdup "string.h" CPPUTEST_HAVE_STRDUP)
endif()

if (MINGW)
    include(CheckStructHasMember)
    check_struct_has_member("struct timespec" tv_sec time.h HAVE_STRUCT_TIMESPEC LANGUAGE CXX)
endif()

target_compile_definitions(CppUTest
    PUBLIC
        CPPUTEST_USE_STD_CPP_LIB=$<NOT:$<BOOL:${CPPUTEST_STD_CPP_LIB_DISABLED}>>
        $<$<BOOL:${CPPUTEST_HAVE_STRDUP}>:CPPUTEST_HAVE_STRDUP>
        $<$<BOOL:${CPPUTEST_HAVE_PTHREAD_MUTEX_LOCK}>:CPPUTEST_HAVE_PTHREAD_MUTEX_LOCK>
    PRIVATE
        $<$<BOOL:${CPPUTEST_HAVE_GETTIMEOFDAY}>:CPPUTEST_HAVE_GETTIMEOFDAY>
        # Apply workaround for MinGW timespec redefinition (pthread.h / time.h).
        $<$<BOOL:${HAVE_STRUCT_TIMESPEC}>:_TIMESPEC_DEFINED>
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)

target_link_libraries(CppUTest
    PRIVATE
        $<$<BOOL:${WIN32}>:winmm>
        $<$<BOOL:${CMAKE_USE_PTHREADS_INIT}>:${CMAKE_THREAD_LIBS_INIT}>
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
