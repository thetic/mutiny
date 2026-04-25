# Minimal RunCMake-style test runner.
# Inspired by CMake's own test infrastructure (BSD-3-Clause).

foreach(var RunCMake_GENERATOR RunCMake_SOURCE_DIR RunCMake_BINARY_DIR)
    if(NOT DEFINED ${var})
        message(FATAL_ERROR "${var} not given")
    endif()
endforeach()

file(MAKE_DIRECTORY "${RunCMake_BINARY_DIR}")

function(_run_cmake_check test result stdout stderr)
    set(src "${RunCMake_SOURCE_DIR}")
    set(failed "")

    set(expected_result 0)
    if(EXISTS "${src}/${test}/result.txt")
        file(READ "${src}/${test}/result.txt" expected_result)
        string(STRIP "${expected_result}" expected_result)
    endif()
    if(NOT "${result}" EQUAL "${expected_result}")
        string(APPEND failed
            "  Result: expected [${expected_result}], got [${result}]\n"
        )
    endif()

    foreach(stream IN ITEMS stdout stderr)
        if(EXISTS "${src}/${test}/${stream}.txt")
            file(READ "${src}/${test}/${stream}.txt" expected)
            string(STRIP "${expected}" expected)
            # Normalize whitespace so patterns are not sensitive to cmake's
            # line-wrapping (which occurs at a fixed column, not terminal width).
            string(REGEX REPLACE "[ \t\r\n]+" " " stream_normalized "${${stream}}")
            if(NOT "${stream_normalized}" MATCHES "${expected}")
                string(APPEND failed
                    "  ${stream} pattern not matched:\n    ${expected}\n"
                    "  Actual ${stream}:\n    ${${stream}}\n"
                )
            endif()
        endif()
    endforeach()

    set(RunCMake_TEST_FAILED "")
    if(EXISTS "${src}/${test}/check.cmake")
        include("${src}/${test}/check.cmake")
        if(RunCMake_TEST_FAILED)
            string(APPEND failed "  ${RunCMake_TEST_FAILED}\n")
        endif()
    endif()

    if(failed)
        message(SEND_ERROR "${test}: FAILED\n${failed}")
    else()
        message(STATUS "${test}: passed")
    endif()
endfunction()

function(run_cmake test)
    set(gen_opts -G "${RunCMake_GENERATOR}")
    if(RunCMake_GENERATOR_PLATFORM)
        list(APPEND gen_opts -A "${RunCMake_GENERATOR_PLATFORM}")
    endif()
    if(RunCMake_GENERATOR_TOOLSET)
        list(APPEND gen_opts -T "${RunCMake_GENERATOR_TOOLSET}")
    endif()
    execute_process(
        COMMAND ${CMAKE_COMMAND}
            ${gen_opts}
            -S "${RunCMake_SOURCE_DIR}/${test}"
            -B "${RunCMake_BINARY_DIR}/${test}"
            ${RunCMake_TEST_OPTIONS}
            ${ARGN}
        RESULT_VARIABLE result
        OUTPUT_VARIABLE stdout
        ERROR_VARIABLE stderr
    )
    _run_cmake_check("${test}" "${result}" "${stdout}" "${stderr}")
endfunction()

function(run_cmake_script test)
    # Remaining arguments are -D flags; they must come before -P.
    execute_process(
        COMMAND ${CMAKE_COMMAND} ${ARGN} -P "${RunCMake_SOURCE_DIR}/${test}.cmake"
        RESULT_VARIABLE result
        OUTPUT_VARIABLE stdout
        ERROR_VARIABLE stderr
    )
    _run_cmake_check("${test}" "${result}" "${stdout}" "${stderr}")
endfunction()
