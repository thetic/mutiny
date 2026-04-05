add_subdirectory(util)

if(MUTINY_BUILD_UNIT_TESTING)
    add_subdirectory(unit)
endif()

add_subdirectory(integration)

get_cmake_property(_mutiny_is_multi_config GENERATOR_IS_MULTI_CONFIG)
add_test(
    NAME mutiny_discover_tests
    COMMAND ${CMAKE_COMMAND}
        -D "RunCMake_GENERATOR=${CMAKE_GENERATOR}"
        -D "RunCMake_GENERATOR_PLATFORM=${CMAKE_GENERATOR_PLATFORM}"
        -D "RunCMake_GENERATOR_TOOLSET=${CMAKE_GENERATOR_TOOLSET}"
        -D "RunCMake_GENERATOR_IS_MULTI_CONFIG=${_mutiny_is_multi_config}"
        -D "RunCMake_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/cmake/mutiny"
        -D "RunCMake_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}/cmake/mutiny"
        -D "MUTINY_MODULE_PATH=${PROJECT_SOURCE_DIR}/cmake/Modules"
        -D "CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/mutiny/RunCMakeTest.cmake"
)
