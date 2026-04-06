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

# Error cases
run_cmake(NotATarget)
run_cmake(NotAnExecutable)

# Happy path — single-config generator
run_cmake(ValidTarget)

# Multi-configuration generator (CMAKE_CONFIGURATION_TYPES is non-empty)
list(APPEND RunCMake_TEST_OPTIONS "-DCMAKE_CONFIGURATION_TYPES=Debug")
run_cmake(MultiConfig)
list(REMOVE_ITEM RunCMake_TEST_OPTIONS "-DCMAKE_CONFIGURATION_TYPES=Debug")

# Per-call function arguments
run_cmake(DetailedArgOn)
run_cmake(ExtraArgsArg)

# Cache-variable defaults
list(APPEND RunCMake_TEST_OPTIONS "-DMUTINY_TESTS_DETAILED:BOOL=ON")
run_cmake(CacheDetailedVar)
# Interaction: DETAILED OFF arg must take precedence over MUTINY_TESTS_DETAILED=ON
run_cmake(DetailedArgOffOverridesCache)
list(REMOVE_ITEM RunCMake_TEST_OPTIONS "-DMUTINY_TESTS_DETAILED:BOOL=ON")

list(APPEND RunCMake_TEST_OPTIONS "-DMUTINY_JUNIT_REPORT:BOOL=ON")
run_cmake(CacheJUnitVar)
list(REMOVE_ITEM RunCMake_TEST_OPTIONS "-DMUTINY_JUNIT_REPORT:BOOL=ON")

list(APPEND RunCMake_TEST_OPTIONS "-DMUTINY_EXTRA_ARGS=--custom-arg")
run_cmake(CacheExtraArgs)
# Interaction: EXTRA_ARGS function argument must take precedence over MUTINY_EXTRA_ARGS
run_cmake(ExtraArgsArgOverridesCache)
list(REMOVE_ITEM RunCMake_TEST_OPTIONS "-DMUTINY_EXTRA_ARGS=--custom-arg")

# No emulator: LANGUAGES NONE skips toolchain detection, so CMAKE_CROSSCOMPILING is set
# explicitly inside CMakeLists.txt after project() — no extra -D option needed here.
run_cmake(CrossCompilingNoEmulator)

# With emulator: enable_language(CXX) runs full detection; CMAKE_SYSTEM_NAME=Generic
# causes CMake to derive CMAKE_CROSSCOMPILING=TRUE automatically.
list(APPEND RunCMake_TEST_OPTIONS "-DCMAKE_SYSTEM_NAME=Generic")
run_cmake(CrossCompilingWithEmulator)
list(REMOVE_ITEM RunCMake_TEST_OPTIONS "-DCMAKE_SYSTEM_NAME=Generic")

# Legacy cmake <3.10 code path (simulated via override variable)
list(APPEND RunCMake_TEST_OPTIONS "-D_MUTINY_CMAKE_VERSION_OVERRIDE=3.9.0")
run_cmake(LegacySingleInclude)
run_cmake(LegacyDuplicateError)
list(REMOVE_ITEM RunCMake_TEST_OPTIONS "-D_MUTINY_CMAKE_VERSION_OVERRIDE=3.9.0")

# ── _mutiny_discovery.cmake script tests ───────────────────────────────────────
#
# The "executable" is a cmake -P script (mock_test_exe.cmake) whose stdout
# output matches the -ll / -lo format consumed by the discovery script.
# EMULATOR_COMMAND + EMULATOR_ARG are reassembled into a list inside the
# wrapper scripts to avoid semicolon-in-D-value quoting issues.

set(discovery_common
    "-DMUTINY_MODULE_PATH=${MUTINY_MODULE_PATH}"
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
