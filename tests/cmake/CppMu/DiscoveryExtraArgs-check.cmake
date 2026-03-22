# Verify that EXTRA_ARGS replaces (not appends to) the default -v flag.
set(generated_file "${RunCMake_BINARY_DIR}/DiscoveryExtraArgs-generated.cmake")
if(NOT EXISTS "${generated_file}")
    set(RunCMake_TEST_FAILED "Generated file not found: ${generated_file}")
    return()
endif()

file(READ "${generated_file}" content)

# Custom arg must be present.
if(NOT content MATCHES "--custom-arg")
    string(APPEND RunCMake_TEST_FAILED "Expected --custom-arg not found\n")
endif()

# Default -v must be absent (EXTRA_ARGS replaces, does not append).
if(content MATCHES "add_test\\([^\n]* -v ")
    string(APPEND RunCMake_TEST_FAILED
        "Unexpected -v flag found; EXTRA_ARGS should replace the default\n"
    )
endif()

if(RunCMake_TEST_FAILED)
    string(APPEND RunCMake_TEST_FAILED "Actual content:\n${content}")
endif()
