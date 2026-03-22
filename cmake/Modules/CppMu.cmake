set(_CPPMU_DISCOVERY_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/_CppMuDiscovery.cmake
    CACHE INTERNAL "CppMu discovery scripts"
)

option(CPPMU_TESTS_DETAILED "Run discovered tests individually")
option(CPPMU_JUNIT_REPORT "Output JUnit test reports")
set(CPPMU_EXTRA_ARGS "-v" CACHE STRING
    "Default extra arguments passed to each discovered test run"
)
# _CPPMU_CMAKE_VERSION_OVERRIDE lets tests exercise the legacy code path.
set(_CPPMU_CMAKE_VERSION_OVERRIDE "" CACHE STRING
    "Override the cmake version used by cppmu_discover_tests (for testing only)"
)
mark_as_advanced(_CPPMU_CMAKE_VERSION_OVERRIDE)

function(cppmu_discover_tests target)
    set(oneValueArgs DETAILED)
    set(multiValueArgs EXTRA_ARGS)
    cmake_parse_arguments(
        ""
        ""
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    # Validate arguments.
    if(NOT TARGET ${target})
        message(FATAL_ERROR
            "Cannot discover tests for target \"${target}\" "
            "which is not built by this project."
        )
    endif()

    get_property(target_type
        TARGET ${target}
        PROPERTY TYPE
    )
    if(NOT target_type STREQUAL "EXECUTABLE")
        message(FATAL_ERROR
        "Cannot discover tests for target \"${target}\" "
        "which is not an executable."
        )
    endif()
    
    get_property(emulator
        TARGET ${target}
        PROPERTY CROSSCOMPILING_EMULATOR
    )
    if(CMAKE_CROSSCOMPILING)
        if(NOT emulator)
            message(WARNING "Cannot discover cross compiled tests without an emulator")
            return()
        endif()
    endif()

    if(NOT DEFINED _EXTRA_ARGS)
        set(_EXTRA_ARGS ${CPPMU_EXTRA_ARGS})
    endif()

    if(NOT DEFINED _DETAILED)
        set(_DETAILED ${CPPMU_TESTS_DETAILED})
    endif()

    if(CPPMU_JUNIT_REPORT)
        list(APPEND _EXTRA_ARGS -pjunit)
    endif()

    set_target_properties(${target} PROPERTIES
        CPPMU_DISCOVER_ARGS    "${_EXTRA_ARGS}"
        CPPMU_DISCOVER_DETAILED "${_DETAILED}"
    )

    set(CTEST_INCLUDE_FILE "${CMAKE_CURRENT_BINARY_DIR}/${target}_include.cmake")
    if(CMAKE_CONFIGURATION_TYPES)
        set(CTEST_GENERATED_FILE "${CMAKE_CURRENT_BINARY_DIR}/${target}.$<CONFIG>.cmake")
        file(WRITE "${CTEST_INCLUDE_FILE}"
            "if(EXISTS \"${CMAKE_CURRENT_BINARY_DIR}/${target}.\${CTEST_CONFIGURATION_TYPE}.cmake\")\n"
            "  include(\"${CMAKE_CURRENT_BINARY_DIR}/${target}.\${CTEST_CONFIGURATION_TYPE}.cmake\")\n"
            "endif()\n"
        )
    else()
        set(CTEST_GENERATED_FILE "${CMAKE_CURRENT_BINARY_DIR}/${target}.cmake")
        file(WRITE "${CTEST_INCLUDE_FILE}"
            "if(EXISTS \"${CTEST_GENERATED_FILE}\")\n"
            "  include(\"${CTEST_GENERATED_FILE}\")\n"
            "endif()\n"
        )
    endif()

    add_custom_command(
        TARGET ${target} POST_BUILD
        BYPRODUCTS "${CTEST_GENERATED_FILE}"
        COMMAND
            "${CMAKE_COMMAND}"
            -D "TESTS_DETAILED:BOOL=${_DETAILED}"
            -D "EXECUTABLE=$<TARGET_FILE:${target}>"
            -D "TARGET_NAME=${target}"
            -D "EMULATOR=${emulator}"
            -D "ARGS=${_EXTRA_ARGS}"
            -D "CTEST_FILE=${CTEST_GENERATED_FILE}"
            -P "${_CPPMU_DISCOVERY_SCRIPT}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        VERBATIM
    )

    if(_CPPMU_CMAKE_VERSION_OVERRIDE)
        set(_cppmu_cmake_version "${_CPPMU_CMAKE_VERSION_OVERRIDE}")
    else()
        set(_cppmu_cmake_version "${CMAKE_VERSION}")
    endif()

    if(_cppmu_cmake_version VERSION_LESS "3.10")
        # We can only set one.
        get_property(already_set
            DIRECTORY
            PROPERTY TEST_INCLUDE_FILE
            SET
        )
        if(${already_set})
            message(FATAL_ERROR
                "Cannot discover multiple tests from the same file"
            )
        endif()
        set_property(
            DIRECTORY
            PROPERTY TEST_INCLUDE_FILE "${CTEST_INCLUDE_FILE}"
        )
    else()
        set_property(
            DIRECTORY APPEND
            PROPERTY TEST_INCLUDE_FILES "${CTEST_INCLUDE_FILE}"
        )
    endif()

endfunction()
