cmake_minimum_required(VERSION 3.29)

file(READ "cfg/VERSION" MUTINY_VERSION_RAW)
string(STRIP "${MUTINY_VERSION_RAW}" MUTINY_VERSION)

project(mutiny
    VERSION ${MUTINY_VERSION}
    LANGUAGES CXX
)

include(CTest)

include(CMakeDependentOption)
option(MUTINY_USE_STD_CPP_LIB "Use the standard C++ library" ON)
cmake_dependent_option(MUTINY_USE_STD_STRING "Use std::string"
    ON "MUTINY_USE_STD_CPP_LIB" OFF)
cmake_dependent_option(MUTINY_BUILD_TESTING "Build tests for mutiny"
    ${PROJECT_IS_TOP_LEVEL}
    "BUILD_TESTING;EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests"
    OFF)
cmake_dependent_option(MUTINY_BUILD_UNIT_TESTING "Build unit tests for mutiny"
    ${PROJECT_IS_TOP_LEVEL}
    "MUTINY_BUILD_TESTING;NOT BUILD_SHARED_LIBS"
    OFF)
cmake_dependent_option(MUTINY_EXAMPLES "Compile and make examples?"
    ON
    "PROJECT_IS_TOP_LEVEL;EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/examples"
    OFF)


set(MUTINY_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    CACHE PATH "Path to the mutiny CMake modules directory" FORCE
)
list(APPEND CMAKE_MODULE_PATH "${MUTINY_MODULE_PATH}")
if(NOT PROJECT_IS_TOP_LEVEL)
    set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" PARENT_SCOPE)
endif()

add_library(mutiny)

configure_file(
    include/mutiny/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/mutiny/version.h
)

include(GenerateExportHeader)
generate_export_header(mutiny
    EXPORT_FILE_NAME include/mutiny/export.h
    INCLUDE_GUARD_NAME INCLUDED_MUTINY_EXPORT_H
)

target_sources(mutiny
    PRIVATE
        src/mock/ActualCallTrace.cpp
        src/mock/CheckedActualCall.cpp
        src/mock/CheckedExpectedCall.cpp
        src/mock/ExpectedCallsList.cpp
        src/mock/Failure.cpp
        src/mock/IgnoredExpectedCall.cpp
        src/mock/NamedValue.cpp
        src/mock/NamedValueComparatorsAndCopiersRepository.cpp
        src/mock/NamedValueList.cpp
        src/mock/SupportPlugin.cpp
        src/mock/mock.c.cpp
        src/mock/mock.cpp
        src/String.cpp
        src/test/CommandLineArguments.cpp
        src/test/CommandLineRunner.cpp
        src/test/CompositeOutput.cpp
        src/test/ConsoleOutput.cpp
        src/test/ExecFunctionShell.cpp
        src/test/ExpectFailShell.cpp
        src/test/Failure.cpp
        src/test/Filter.cpp
        src/test/IgnoredShell.cpp
        src/test/Installer.cpp
        src/test/JUnitOutput.cpp
        src/test/JUnitOutputPlugin.cpp
        src/test/NullPlugin.cpp
        src/test/Ordered.cpp
        src/test/Output.cpp
        src/test/Plugin.cpp
        src/test/Registry.cpp
        src/test/Result.cpp
        src/test/SetPointerPlugin.cpp
        src/test/Shell.cpp
        src/test/ShellPointerArray.cpp
        src/StringCollection.cpp
        src/test/Terminator.cpp
        src/test/Test.cpp
        src/test/TestingFixture.cpp
        src/test/jump_buffer.cpp
        src/math.cpp
        src/test/test.c.cpp
        src/time.cpp
    PUBLIC FILE_SET HEADERS
        BASE_DIRS
            include
            "${CMAKE_CURRENT_BINARY_DIR}/include"
        FILES
            include/mutiny/mock.h
            include/mutiny/mock.hpp
            include/mutiny/mock/ActualCall.hpp
            include/mutiny/mock/ExpectedCall.hpp
            include/mutiny/mock/Failure.hpp
            include/mutiny/mock/NamedValue.hpp
            include/mutiny/mock/NamedValueComparator.hpp
            include/mutiny/mock/NamedValueComparatorsAndCopiersRepository.hpp
            include/mutiny/mock/NamedValueCopier.hpp
            include/mutiny/mock/SupportPlugin.hpp
            include/mutiny/String.hpp
            include/mutiny/test.h
            include/mutiny/test.hpp
            include/mutiny/test/CommandLineArguments.hpp
            include/mutiny/test/CommandLineRunner.hpp
            include/mutiny/test/ConsoleOutput.hpp
            include/mutiny/test/ExecFunction.hpp
            include/mutiny/test/ExpectFailShell.hpp
            include/mutiny/test/Failure.hpp
            include/mutiny/test/Filter.hpp
            include/mutiny/test/IgnoredShell.hpp
            include/mutiny/test/Installer.hpp
            include/mutiny/test/JUnitOutputPlugin.hpp
            include/mutiny/test/Ordered.hpp
            include/mutiny/test/Output.hpp
            include/mutiny/test/Plugin.hpp
            include/mutiny/test/Registry.hpp
            include/mutiny/test/Result.hpp
            include/mutiny/test/SetPointerPlugin.hpp
            include/mutiny/test/Shell.hpp
            include/mutiny/test/ShellPointerArray.hpp
            include/mutiny/test/StringBufferOutput.hpp
            include/mutiny/test/Terminator.hpp
            include/mutiny/test/Test.hpp
            include/mutiny/test/TestingFixture.hpp
            include/mutiny/features.hpp
            include/mutiny/time.hpp
            ${CMAKE_CURRENT_BINARY_DIR}/include/mutiny/version.h
            ${CMAKE_CURRENT_BINARY_DIR}/include/mutiny/export.h
    PRIVATE FILE_SET private_headers TYPE HEADERS
        BASE_DIRS src
        FILES
            src/mutiny/mock/ActualCallTrace.hpp
            src/mutiny/mock/CheckedActualCall.hpp
            src/mutiny/mock/CheckedExpectedCall.hpp
            src/mutiny/mock/ExpectedCallsList.hpp
            src/mutiny/mock/IgnoredActualCall.hpp
            src/mutiny/mock/IgnoredExpectedCall.hpp
            src/mutiny/mock/NamedValueList.hpp
            src/mutiny/test/CompositeOutput.hpp
            src/mutiny/test/ExecFunctionShell.hpp
            src/mutiny/test/JUnitOutput.hpp
            src/mutiny/test/NullPlugin.hpp
            src/mutiny/StringCollection.hpp
            src/mutiny/test/jump_buffer.hpp
            src/mutiny/math.hpp
)

include(CheckCXXSymbolExists)
check_cxx_symbol_exists(gettimeofday "sys/time.h" MUTINY_HAVE_GETTIMEOFDAY)

target_compile_features(mutiny
    PUBLIC cxx_std_11
)

target_compile_definitions(mutiny
    PUBLIC
        MUTINY_USE_STD_CPP_LIB=$<BOOL:${MUTINY_USE_STD_CPP_LIB}>
        MUTINY_USE_STD_STRING=$<BOOL:${MUTINY_USE_STD_STRING}>
    PRIVATE
        $<$<BOOL:${MUTINY_HAVE_GETTIMEOFDAY}>:MUTINY_HAVE_GETTIMEOFDAY>
)

set_target_properties(mutiny PROPERTIES
    EXPORT_NAME tiny
    CXX_VISIBILITY_PRESET hidden
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION}
)

add_library(mu::tiny ALIAS mutiny)

if (MUTINY_BUILD_TESTING)
    add_subdirectory(tests)
endif ()

if (MUTINY_EXAMPLES)
    add_subdirectory(examples)
endif()

if(PROJECT_IS_TOP_LEVEL)
    include(GNUInstallDirs)

    install(
        TARGETS mutiny
        EXPORT mutinyTargets
        RUNTIME
            DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT mutiny_Runtime
        LIBRARY
            DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT mutiny_Runtime
            NAMELINK_COMPONENT mutiny_Development
        ARCHIVE
            DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT mutiny_Development
        FILE_SET HEADERS
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            COMPONENT mutiny_Development
    )

    include(CMakePackageConfigHelpers)

    set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
    set(LIB_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
    set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
    set(MODULES_DIR "${MUTINY_MODULE_PATH}")

    set(MUTINY_CONFIG_DEST "${LIB_INSTALL_DIR}/mutiny/cmake")

    # CMake package config files (install tree).
    configure_package_config_file(cfg/mutiny-config.cmake.install.in
        ${CMAKE_CURRENT_BINARY_DIR}/install/mutiny-config.cmake
        INSTALL_DESTINATION "${MUTINY_CONFIG_DEST}"
        PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR
    )
    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/install/mutiny-configVersion.cmake
        COMPATIBILITY SameMajorVersion
    )
    install(
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/install/mutiny-config.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/install/mutiny-configVersion.cmake
        DESTINATION "${MUTINY_CONFIG_DEST}"
    )
    install(
        EXPORT mutinyTargets
        NAMESPACE mu::
        DESTINATION "${MUTINY_CONFIG_DEST}"
    )
    install(
        FILES
            ${MUTINY_MODULE_PATH}/Mutiny.cmake
            ${MUTINY_MODULE_PATH}/_mutiny_discovery.cmake
        DESTINATION "${MUTINY_CONFIG_DEST}/Modules"
    )

    # CMake package config files (build tree).
    configure_package_config_file(cfg/mutiny-config.cmake.build.in
        ${PROJECT_BINARY_DIR}/mutiny-config.cmake
        INSTALL_DESTINATION ${PROJECT_BINARY_DIR}
        PATH_VARS INCLUDE_DIR PROJECT_BINARY_DIR MODULES_DIR
    )
    write_basic_package_version_file(
        ${PROJECT_BINARY_DIR}/mutiny-configVersion.cmake
        COMPATIBILITY SameMajorVersion
    )

    execute_process(
        COMMAND git rev-parse HEAD
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_REVISION
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    file(WRITE "${CMAKE_BINARY_DIR}/source-extras/GIT_REVISION" "${GIT_REVISION}\n")

    set(CPACK_VERBATIM_VARIABLES YES)
    set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
    set(CPACK_SOURCE_INSTALLED_DIRECTORIES
        "${CMAKE_SOURCE_DIR}" "/"
        "${CMAKE_BINARY_DIR}/source-extras" "/"
    )
    set(CPACK_SOURCE_IGNORE_FILES
        "/\\.git/"
        "/\\.github/"
        "/\\.cache/"
        "/\\.claude/"
        "/\\.direnv/"
        "/build/"
        "/docs/"
        "/examples/"
        "/tests/"
        "/cfg/CTestCustom\\.cmake\\.in$"
        "/cfg/lcov\\.cmake$"
        "/cfg/warnings\\.cmake$"
        "/\\.clang-format$"
        "/\\.clang-tidy$"
        "/CMakePresets\\.json$"
        "/CONTRIBUTING\\.md$"
        "/compile_commands\\.json$"
        "/\\.envrc$"
        "/\\.gitignore$"
        "/\\.git-blame-ignore-revs$"
    )
    include(CPack)
endif()
