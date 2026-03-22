# Verify the _include.cmake file uses a static path (single-config generator).
set(include_file "${RunCMake_BINARY_DIR}/ValidTarget/MyTests_include.cmake")
if(NOT EXISTS "${include_file}")
    set(RunCMake_TEST_FAILED "Include file not found: ${include_file}")
    return()
endif()

file(READ "${include_file}" content)

if(content MATCHES "CTEST_CONFIGURATION_TYPE")
    set(RunCMake_TEST_FAILED
        "Single-config include file should not contain CTEST_CONFIGURATION_TYPE\nActual:\n${content}"
    )
elseif(NOT content MATCHES "MyTests\\.cmake")
    set(RunCMake_TEST_FAILED
        "Single-config include file missing static path to MyTests.cmake\nActual:\n${content}"
    )
endif()
