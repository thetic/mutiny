include("${CMAKE_CURRENT_LIST_DIR}/../RunCMake.cmake")

# Pass the C++ compiler so each sub-configure doesn't re-detect it.
set(RunCMake_TEST_OPTIONS
    "-DMUTINY_MODULE_PATH=${MUTINY_MODULE_PATH}"
    "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
)
if(DEFINED CMAKE_TRY_COMPILE_TARGET_TYPE)
    list(APPEND RunCMake_TEST_OPTIONS
        "-DCMAKE_TRY_COMPILE_TARGET_TYPE=${CMAKE_TRY_COMPILE_TARGET_TYPE}"
    )
endif()

# ── Mutiny.cmake configure-time tests ─────────────────────────────────────────

set(RunCMake_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/configure")

# Error cases
run_cmake(NotATarget)
run_cmake(NotAnExecutable)

# Happy path — single-config and multi-config generators
run_cmake(ValidTarget)
run_cmake(MultiConfig -DCMAKE_CONFIGURATION_TYPES=Debug)

# Per-call function arguments
run_cmake(DetailedArgOn)
run_cmake(ExtraArgsArg)

# Cache-variable defaults and override interactions
run_cmake(CacheDetailedVar          -DMUTINY_TESTS_DETAILED:BOOL=ON)
run_cmake(DetailedArgOffOverridesCache -DMUTINY_TESTS_DETAILED:BOOL=ON)
run_cmake(CacheJUnitVar             -DMUTINY_JUNIT_REPORT:BOOL=ON)
run_cmake(CacheExtraArgs            -DMUTINY_EXTRA_ARGS=--custom-arg)
run_cmake(ExtraArgsArgOverridesCache -DMUTINY_EXTRA_ARGS=--custom-arg)

# Cross-compilation
run_cmake(CrossCompilingNoEmulator)
run_cmake(CrossCompilingWithEmulator -DCMAKE_SYSTEM_NAME=Generic)

# Legacy cmake <3.10 code path (simulated via override variable)
run_cmake(LegacySingleInclude  -D_MUTINY_CMAKE_VERSION_OVERRIDE=3.9.0)
run_cmake(LegacyDuplicateError -D_MUTINY_CMAKE_VERSION_OVERRIDE=3.9.0)

# ── _mutiny_discovery.cmake script tests ───────────────────────────────────────
#
# The "executable" is a cmake -P script (mock_test_exe.cmake) whose stdout
# matches the -ll / -lo format consumed by the discovery script.
# EMULATOR_COMMAND + EMULATOR_ARG are reassembled into a list inside the
# Discovery*.cmake wrappers to avoid semicolon-in-D-value quoting issues.

set(RunCMake_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/discovery")

set(discovery_common
    "-D_MUTINY_DISCOVERY_SCRIPT=${_MUTINY_DISCOVERY_SCRIPT}"
    "-DEXECUTABLE=${RunCMake_SOURCE_DIR}/mock_test_exe.cmake"
    "-DEMULATOR_COMMAND=${CMAKE_COMMAND}"
    "-DEMULATOR_ARG=-P"
    "-DTARGET_NAME=MyTests"
)

# Group mode (default)
run_cmake_script(Discovery
    ${discovery_common}
    "-DTESTS_DETAILED=OFF"
    "-DARGS=-v"
    "-DCTEST_FILE=${RunCMake_BINARY_DIR}/Discovery-generated.cmake"
)

# Detailed mode with ordered-test group preserved as single entry
run_cmake_script(DiscoveryDetailed
    ${discovery_common}
    "-DTESTS_DETAILED=ON"
    "-DARGS=-v"
    "-DCTEST_FILE=${RunCMake_BINARY_DIR}/DiscoveryDetailed-generated.cmake"
)

# MUTINY_JUNIT_REPORT: -pjunit appended to ARGS
run_cmake_script(DiscoveryJUnit
    ${discovery_common}
    "-DTESTS_DETAILED=OFF"
    "-DARGS=-v\;-pjunit"
    "-DCTEST_FILE=${RunCMake_BINARY_DIR}/DiscoveryJUnit-generated.cmake"
)

# EXTRA_ARGS: replaces the default -v entirely
run_cmake_script(DiscoveryExtraArgs
    ${discovery_common}
    "-DTESTS_DETAILED=OFF"
    "-DARGS=--custom-arg"
    "-DCTEST_FILE=${RunCMake_BINARY_DIR}/DiscoveryExtraArgs-generated.cmake"
)
