# Verify the generated CTest file for group-mode discovery (TESTS_DETAILED=OFF).
# Each group becomes a single add_test(-sg <group>) entry.

set(generated_file "${RunCMake_BINARY_DIR}/Discovery-generated.cmake")
if(NOT EXISTS "${generated_file}")
    set(RunCMake_TEST_FAILED "Generated file not found: ${generated_file}")
    return()
endif()

file(READ "${generated_file}" content)

set(patterns
    "add_test\\( MyTests\\.Group1 .* -sg Group1\\)"
    "set_tests_properties\\( MyTests\\.Group1 PROPERTIES DEF_SOURCE_LINE file1\\.cpp:5\\)"
    "add_test\\( MyTests\\.Group2 .* -sg Group2\\)"
    "set_tests_properties\\( MyTests\\.Group2 PROPERTIES DEF_SOURCE_LINE file2\\.cpp:25\\)"
    "add_test\\( MyTests\\.OrderedGroup .* -sg OrderedGroup\\)"
    "set_tests_properties\\( MyTests\\.OrderedGroup PROPERTIES DEF_SOURCE_LINE file3\\.cpp:35\\)"
)

foreach(p IN LISTS patterns)
    if(NOT content MATCHES "${p}")
        string(APPEND RunCMake_TEST_FAILED "Pattern not matched: ${p}\n")
    endif()
endforeach()

if(RunCMake_TEST_FAILED)
    string(APPEND RunCMake_TEST_FAILED "Actual content:\n${content}")
endif()
