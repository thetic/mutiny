cmake_minimum_required(VERSION 3.10)
project(mutinyConfigTests LANGUAGES NONE)

list(APPEND CMAKE_MODULE_PATH "${MUTINY_MODULE_PATH}")
include(Mutiny)

# ── Error cases (no compiler needed) ─────────────────────────────────────────

if(RunCMake_TEST STREQUAL "NotATarget")
    mutiny_discover_tests(NotATarget)

elseif(RunCMake_TEST STREQUAL "NotAnExecutable")
    add_library(MyLib INTERFACE)
    mutiny_discover_tests(MyLib)

# ── Cross-compile: no emulator (uses IMPORTED target, returns before add_custom_command) ──

elseif(RunCMake_TEST STREQUAL "CrossCompilingNoEmulator")
    # With LANGUAGES NONE cmake skips toolchain detection, so CMAKE_CROSSCOMPILING
    # is never derived from CMAKE_SYSTEM_NAME.  Set it explicitly after project().
    set(CMAKE_CROSSCOMPILING TRUE)
    add_executable(MyTests IMPORTED GLOBAL)
    set_target_properties(MyTests PROPERTIES IMPORTED_LOCATION "${CMAKE_COMMAND}")
    mutiny_discover_tests(MyTests)

# ── Tests requiring a real compiled executable ────────────────────────────────

else()
    enable_language(CXX)
    add_executable(MyTests empty.cpp)

    if(RunCMake_TEST STREQUAL "CrossCompilingWithEmulator")
        set_target_properties(MyTests PROPERTIES
            CROSSCOMPILING_EMULATOR "${CMAKE_COMMAND}"
        )
        mutiny_discover_tests(MyTests)

    elseif(RunCMake_TEST STREQUAL "DetailedArgOn")
        mutiny_discover_tests(MyTests DETAILED ON)

    elseif(RunCMake_TEST STREQUAL "ExtraArgsArg")
        mutiny_discover_tests(MyTests EXTRA_ARGS --custom-arg)

    elseif(RunCMake_TEST STREQUAL "CacheExtraArgs")
        mutiny_discover_tests(MyTests)
        get_target_property(p MyTests MUTINY_DISCOVER_ARGS)
        message(STATUS "DISCOVER_ARGS: ${p}")

    elseif(RunCMake_TEST STREQUAL "ExtraArgsArgOverridesCache")
        mutiny_discover_tests(MyTests EXTRA_ARGS --function-arg)
        get_target_property(p MyTests MUTINY_DISCOVER_ARGS)
        message(STATUS "DISCOVER_ARGS: ${p}")

    elseif(RunCMake_TEST STREQUAL "DetailedArgOffOverridesCache")
        mutiny_discover_tests(MyTests DETAILED OFF)
        get_target_property(p MyTests MUTINY_DISCOVER_DETAILED)
        message(STATUS "DISCOVER_DETAILED: ${p}")

    elseif(RunCMake_TEST STREQUAL "LegacySingleInclude")
        mutiny_discover_tests(MyTests)

    elseif(RunCMake_TEST STREQUAL "LegacyDuplicateError")
        mutiny_discover_tests(MyTests)
        add_executable(MyTests2 empty.cpp)
        mutiny_discover_tests(MyTests2)   # should FATAL_ERROR

    else()
        # ValidTarget, MultiConfig, CacheDetailedVar, CacheJUnitVar
        mutiny_discover_tests(MyTests)
    endif()

    # Print the property set by mutiny_discover_tests for stdout verification.
    # The legacy path uses the singular TEST_INCLUDE_FILE property.
    if(RunCMake_TEST STREQUAL "LegacySingleInclude")
        get_property(p DIRECTORY PROPERTY TEST_INCLUDE_FILE)
        message(STATUS "TEST_INCLUDE_FILE: ${p}")
    elseif(NOT RunCMake_TEST STREQUAL "LegacyDuplicateError")
        get_property(p DIRECTORY PROPERTY TEST_INCLUDE_FILES)
        message(STATUS "TEST_INCLUDE_FILES: ${p}")
    endif()
endif()
