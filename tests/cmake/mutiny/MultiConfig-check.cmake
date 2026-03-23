# Verify the _include.cmake file uses CTEST_CONFIGURATION_TYPE (multi-config generator).
set(include_file "${RunCMake_BINARY_DIR}/MultiConfig/MyTests_include.cmake")
if(NOT EXISTS "${include_file}")
    set(RunCMake_TEST_FAILED "Include file not found: ${include_file}")
    return()
endif()

file(READ "${include_file}" content)

if(NOT content MATCHES "CTEST_CONFIGURATION_TYPE")
    set(RunCMake_TEST_FAILED
        "Multi-config include file missing CTEST_CONFIGURATION_TYPE variable reference\nActual:\n${content}"
    )
endif()
