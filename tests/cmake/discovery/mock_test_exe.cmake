# Fake mutiny test executable for use in discovery script tests.
#
# When invoked as "cmake -P mock_test_exe.cmake --list-locations" it prints
# mock test location lines to stdout in the format expected by
# _mutiny_discovery.cmake:
#   GROUP.NAME.FILE.LINE
#
# When invoked with --list-ordered-locations it prints only the ordered-test
# group line.

set(mode "")
math(EXPR last "${CMAKE_ARGC} - 1")
foreach(i RANGE 0 "${last}")
    if("${CMAKE_ARGV${i}}" STREQUAL "--list-locations")
        set(mode ll)
    elseif("${CMAKE_ARGV${i}}" STREQUAL "--list-ordered-locations")
        set(mode lo)
    elseif("${CMAKE_ARGV${i}}" STREQUAL "--list-group-locations")
        set(mode lgl)
    endif()
endforeach()

if(mode STREQUAL "ll")
    execute_process(COMMAND "${CMAKE_COMMAND}" -E echo "Group1.Test1.file1.cpp.10")
    execute_process(COMMAND "${CMAKE_COMMAND}" -E echo "Group1.Test2.file1.cpp.20")
    execute_process(COMMAND "${CMAKE_COMMAND}" -E echo "Group2.Test3.file2.cpp.30")
    execute_process(COMMAND "${CMAKE_COMMAND}" -E echo "OrderedGroup.Test4.file3.cpp.40")
elseif(mode STREQUAL "lo")
    execute_process(COMMAND "${CMAKE_COMMAND}" -E echo "OrderedGroup.Test4.file3.cpp.40")
elseif(mode STREQUAL "lgl")
    execute_process(COMMAND "${CMAKE_COMMAND}" -E echo "Group1.file1.cpp.5")
    execute_process(COMMAND "${CMAKE_COMMAND}" -E echo "Group2.file2.cpp.25")
    execute_process(COMMAND "${CMAKE_COMMAND}" -E echo "OrderedGroup.file3.cpp.35")
else()
    message(FATAL_ERROR "Expected --list-locations, --list-ordered-locations, or --list-group-locations flag")
endif()
