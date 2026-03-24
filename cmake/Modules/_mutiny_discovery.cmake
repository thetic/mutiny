cmake_minimum_required(VERSION 3.3...3.29)

if(POLICY CMP0057)
    cmake_policy(SET CMP0057 NEW)
endif()

set(script)

function(add_command NAME)
    set(_args "")
    # use ARGV* instead of ARGN, because ARGN splits arrays into multiple arguments
    math(EXPR _last_arg ${ARGC}-1)
    foreach(_n RANGE 1 ${_last_arg})
        set(_arg "${ARGV${_n}}")
        if(_arg MATCHES "[^-./:a-zA-Z0-9_]")
            set(_args "${_args} [==[${_arg}]==]") # form a bracket_argument
        else()
            set(_args "${_args} ${_arg}")
        endif()
    endforeach()
    set(script "${script}${NAME}(${_args})\n" PARENT_SCOPE)
endfunction()

if(NOT EXISTS "${EXECUTABLE}")
    message(FATAL_ERROR
        "Executable \"${EXECUTABLE}\" does not exist"
    )
endif()

macro(add_test_to_script TEST_NAME TEST_LOCATION SELECT_ARG)
    add_command(
        add_test
        "${TARGET_NAME}.${TEST_NAME}"
        ${EMULATOR}
        "${EXECUTABLE}"
        ${ARGS}
        ${SELECT_ARG}
        ${TEST_NAME}
    )
    add_command(
        set_tests_properties
        "${TARGET_NAME}.${TEST_NAME}"
        PROPERTIES
        DEF_SOURCE_LINE
        "${TEST_LOCATION}"
    )
endmacro()

execute_process(
    COMMAND ${EMULATOR} "${EXECUTABLE}" -ll
    OUTPUT_VARIABLE discovered_tests
    RESULT_VARIABLE result
    ERROR_VARIABLE error
)
if(NOT ${result} EQUAL 0)
    message(FATAL_ERROR
        "Error executing ${EXECUTABLE}:\n"
        "${error}"
    )
endif()

string(CONCAT LL_LINE_REGEX
    "^([^.]*)" # test group
    "\\."
    "([^.]*)"  # test name
    "\\."
    "(.*)"     # file name (only this field is allowed to contain dots)
    "\\."
    "([^.]*)"  # line number
    "\n"
)
string(REGEX MATCHALL "[^\n]+\n" discovered_test_lines "${discovered_tests}")
if(TESTS_DETAILED)
    # Identify groups containing ordered tests.
    # -lo is supported since mutiny gained TEST_ORDERED awareness in discovery;
    # old binaries exit non-zero and we fall back to treating all tests as individual.
    execute_process(
        COMMAND ${EMULATOR} "${EXECUTABLE}" -lo
        OUTPUT_VARIABLE ordered_locations_output
        RESULT_VARIABLE lo_result
        ERROR_VARIABLE _lo_error
    )
    set(ordered_groups)
    if(lo_result EQUAL 0)
        string(REGEX MATCHALL "[^\n]+\n" ordered_lines "${ordered_locations_output}")
        foreach(line IN LISTS ordered_lines)
            string(REGEX MATCH "${LL_LINE_REGEX}" __unused "${line}")
            if(CMAKE_MATCH_1)
                list(APPEND ordered_groups "${CMAKE_MATCH_1}")
            endif()
        endforeach()
        list(REMOVE_DUPLICATES ordered_groups)
    endif()

    set(registered_ordered_groups)
    foreach(line IN LISTS discovered_test_lines)
        string(REGEX MATCH "${LL_LINE_REGEX}" __unused "${line}")
        set(group_name "${CMAKE_MATCH_1}")
        file(TO_CMAKE_PATH "${CMAKE_MATCH_3}" test_file)
        set(test_location "${test_file}:${CMAKE_MATCH_4}")

        if(group_name IN_LIST ordered_groups)
            # Register the whole ordered group as one CTest test (preserves level ordering).
            if(NOT group_name IN_LIST registered_ordered_groups)
                list(APPEND registered_ordered_groups "${group_name}")
                add_test_to_script("${group_name}" "${test_location}" -sg)
            endif()
        else()
            set(test_name "${group_name}.${CMAKE_MATCH_2}")
            add_test_to_script("${test_name}" "${test_location}" -st)
        endif()
    endforeach()
else()
    foreach(line IN LISTS discovered_test_lines)
        string(REGEX MATCH "${LL_LINE_REGEX}" __unused "${line}")
        set(test_name "${CMAKE_MATCH_1}")
        file(TO_CMAKE_PATH "${CMAKE_MATCH_3}" test_file)
        set(test_line "${CMAKE_MATCH_4}")
        if (NOT _${test_name}_file)
            # if the group spans two files, arbitrarily choose the first one encountered
            set(_${test_name}_file "${test_file}")
            set(_${test_name}_line "${test_line}")
        elseif(test_file STREQUAL _${test_name}_file AND test_line LESS _${test_name}_line)
            # line number will eventually be the first line of the first test in the group's file
            set(_${test_name}_line ${test_line})
        endif()
        list(APPEND groups_seen ${test_name})
    endforeach()
    list(REMOVE_DUPLICATES groups_seen)
    foreach(test_name IN LISTS groups_seen)
        set(test_location "${_${test_name}_file}:${_${test_name}_line}")
        add_test_to_script("${test_name}" "${test_location}" -sg)
    endforeach()
endif()

file(WRITE "${CTEST_FILE}" "${script}")
