# Verify that -pjunit is present in each add_test command.
# This exercises the MUTINY_JUNIT_REPORT path in mutiny_discover_tests.
set(generated_file "${RunCMake_BINARY_DIR}/DiscoveryJUnit-generated.cmake")
if(NOT EXISTS "${generated_file}")
    set(RunCMake_TEST_FAILED "Generated file not found: ${generated_file}")
    return()
endif()

file(READ "${generated_file}" content)

set(patterns
    "add_test\\( MyTests\\.Group1 .* -pjunit -sg Group1\\)"
    "add_test\\( MyTests\\.Group2 .* -pjunit -sg Group2\\)"
    "add_test\\( MyTests\\.OrderedGroup .* -pjunit -sg OrderedGroup\\)"
)

foreach(p IN LISTS patterns)
    if(NOT content MATCHES "${p}")
        string(APPEND RunCMake_TEST_FAILED "Pattern not matched: ${p}\n")
    endif()
endforeach()

if(RunCMake_TEST_FAILED)
    string(APPEND RunCMake_TEST_FAILED "Actual content:\n${content}")
endif()
