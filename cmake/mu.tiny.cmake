#[=======================================================================[.rst:
Provides CTest integration for the mutiny unit-testing framework.

.. code-block:: cmake

   include(mu.tiny)

After including this module the :command:`mutiny_discover_tests` command is
available.  It attaches a post-build step to a test executable that enumerates
its tests and registers each one as an individual CTest test, giving
fine-grained pass/fail reporting without requiring a CMake re-run when tests
are added or removed.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables control default behaviour globally.  Each may
be overridden per target through the matching keyword argument of
:command:`mutiny_discover_tests`.

.. variable:: MUTINY_TESTS_DETAILED

  Boolean option.  When ``ON``, each discovered test case is run as its own
  CTest invocation instead of sharing a single test-executable run.
  Default: ``OFF``.

.. variable:: MUTINY_JUNIT_REPORT

  Boolean option.  When ``ON``, each discovered test invocation receives a
  unique :option:`-pjunit=\<target\>.\<group\>` argument so the executable
  emits a JUnit-format XML report named ``<target>.<group>.xml`` in the
  test's working directory.  One file is produced per CTest test, avoiding
  collisions when tests run in parallel.  Default: ``OFF``.

.. variable:: MUTINY_EXTRA_ARGS

  Semicolon-separated list of extra command-line arguments forwarded to each
  discovered test invocation.  Default: :option:`-v`.

Commands
^^^^^^^^

.. command:: mutiny_discover_tests

  Automatically register CTest tests by querying the compiled test executable:

  .. code-block:: cmake

     mutiny_discover_tests(target
                           [DETAILED <bool>]
                           [EXTRA_ARGS <arg>...])

  ``mutiny_discover_tests()`` adds a post-build command on *target* that runs
  the executable to list its test groups and names, then writes a generated
  ``.cmake`` file.  CTest includes that file at test time to register each
  case individually.  The function is a no-op when
  :variable:`BUILD_TESTING <cmake:variable:BUILD_TESTING>` is ``OFF``.

  ``target``
    Name of an executable target built by the current project.  A
    ``FATAL_ERROR`` is raised if the target does not exist or is not an
    executable.

  ``DETAILED <bool>``
    Override :variable:`MUTINY_TESTS_DETAILED` for this target.

  ``EXTRA_ARGS <arg>...``
    Override :variable:`MUTINY_EXTRA_ARGS` for this target.

  **Cross-compilation:** when
  :variable:`CMAKE_CROSSCOMPILING <cmake:variable:CMAKE_CROSSCOMPILING>` is
  set, the target must have a
  :prop_tgt:`CROSSCOMPILING_EMULATOR <cmake:prop_tgt:CROSSCOMPILING_EMULATOR>`
  defined; if absent, discovery is skipped with a warning.

  **CMake version compatibility:** CMake older than 3.10 supports only one
  ``mutiny_discover_tests()`` call per directory because
  :prop_dir:`TEST_INCLUDE_FILE <cmake:prop_dir:TEST_INCLUDE_FILE>` accepts a
  single value.  CMake 3.10 and later use the list-capable
  :prop_dir:`TEST_INCLUDE_FILES <cmake:prop_dir:TEST_INCLUDE_FILES>` property
  and support multiple calls per directory.

#]=======================================================================]

set(_MUTINY_DISCOVERY_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/_mutiny_discovery.cmake
    CACHE INTERNAL "mutiny discovery scripts"
)

option(MUTINY_TESTS_DETAILED "Run discovered tests individually")
option(MUTINY_JUNIT_REPORT "Output JUnit test reports")
set(MUTINY_EXTRA_ARGS "-v" CACHE STRING
    "Default extra arguments passed to each discovered test run"
)
# _MUTINY_CMAKE_VERSION_OVERRIDE lets tests exercise the legacy code path.
set(_MUTINY_CMAKE_VERSION_OVERRIDE "" CACHE STRING
    "Override the cmake version used by mutiny_discover_tests (for testing only)"
)
mark_as_advanced(_MUTINY_CMAKE_VERSION_OVERRIDE)

function(mutiny_discover_tests target)
    if(DEFINED BUILD_TESTING AND NOT BUILD_TESTING)
        return()
    endif()

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
        set(_EXTRA_ARGS ${MUTINY_EXTRA_ARGS})
    endif()

    if(NOT DEFINED _DETAILED)
        set(_DETAILED ${MUTINY_TESTS_DETAILED})
    endif()

    set_target_properties(${target} PROPERTIES
        MUTINY_DISCOVER_ARGS    "${_EXTRA_ARGS}"
        MUTINY_DISCOVER_DETAILED "${_DETAILED}"
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
            -D "JUNIT_REPORT:BOOL=${MUTINY_JUNIT_REPORT}"
            -D "CTEST_FILE=${CTEST_GENERATED_FILE}"
            -P "${_MUTINY_DISCOVERY_SCRIPT}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        VERBATIM
    )

    if(_MUTINY_CMAKE_VERSION_OVERRIDE)
        set(_mutiny_cmake_version "${_MUTINY_CMAKE_VERSION_OVERRIDE}")
    else()
        set(_mutiny_cmake_version "${CMAKE_VERSION}")
    endif()

    if(_mutiny_cmake_version VERSION_LESS "3.10")
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
