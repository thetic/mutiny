# Verify the generated CTest file for detailed-mode discovery (TESTS_DETAILED=ON).
# Non-ordered tests are registered individually with -st; ordered groups stay
# as a single -sg group entry.

set(generated_file "${RunCMake_BINARY_DIR}/DiscoveryDetailed-generated.cmake")
if(NOT EXISTS "${generated_file}")
    set(RunCMake_TEST_FAILED "Generated file not found: ${generated_file}")
    return()
endif()

file(READ "${generated_file}" content)

set(patterns
    "add_test\\( MyTests\\.Group1\\.Test1 .* -st Group1\\.Test1\\)"
    "set_tests_properties\\( MyTests\\.Group1\\.Test1 PROPERTIES DEF_SOURCE_LINE file1\\.cpp:10\\)"
    "add_test\\( MyTests\\.Group1\\.Test2 .* -st Group1\\.Test2\\)"
    "set_tests_properties\\( MyTests\\.Group1\\.Test2 PROPERTIES DEF_SOURCE_LINE file1\\.cpp:20\\)"
    "add_test\\( MyTests\\.Group2\\.Test3 .* -st Group2\\.Test3\\)"
    "set_tests_properties\\( MyTests\\.Group2\\.Test3 PROPERTIES DEF_SOURCE_LINE file2\\.cpp:30\\)"
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
