cmake_minimum_required(VERSION 3.29)

project(mutiny
    VERSION 0.1
)

include(CTest)

include(CMakeDependentOption)
option(MUTINY_USE_STD_CPP_LIB "Use the standard C++ library" ON)
cmake_dependent_option(MUTINY_USE_STD_STRING "Use std::string"
    ON "MUTINY_USE_STD_CPP_LIB" OFF)
cmake_dependent_option(MUTINY_BUILD_TESTING "Compile and make tests for mutiny"
    ${PROJECT_IS_TOP_LEVEL} "BUILD_TESTING" OFF)
cmake_dependent_option(MUTINY_EXAMPLES "Compile and make examples?"
    ON "PROJECT_IS_TOP_LEVEL" OFF)


set(MUTINY_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules"
    CACHE PATH "Path to the mutiny CMake modules directory" FORCE
)
list(APPEND CMAKE_MODULE_PATH "${MUTINY_MODULE_PATH}")
if(NOT PROJECT_IS_TOP_LEVEL)
    set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" PARENT_SCOPE)
endif()

include(GNUInstallDirs)

add_library(mutiny)

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
        src/test/String.cpp
        src/test/StringCollection.cpp
        src/test/Terminator.cpp
        src/test/Test.cpp
        src/test/TestingFixture.cpp
        src/test/jump_buffer.c
        src/test/math.cpp
        src/test/test.c.cpp
        src/test/time.cpp
    PUBLIC FILE_SET HEADERS
        BASE_DIRS include
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
            include/mutiny/test.h
            include/mutiny/test.hpp
            include/mutiny/test/CommandLineArguments.hpp
            include/mutiny/test/CommandLineRunner.hpp
            include/mutiny/test/ConsoleOutput.hpp
            include/mutiny/test/ExpectFailShell.hpp
            include/mutiny/test/Failure.hpp
            include/mutiny/test/Filter.hpp
            include/mutiny/test/IgnoredShell.hpp
            include/mutiny/test/Installer.hpp
            include/mutiny/test/Ordered.hpp
            include/mutiny/test/Output.hpp
            include/mutiny/test/Plugin.hpp
            include/mutiny/test/Registry.hpp
            include/mutiny/test/Result.hpp
            include/mutiny/test/SetPointerPlugin.hpp
            include/mutiny/test/Shell.hpp
            include/mutiny/test/ShellPointerArray.hpp
            include/mutiny/test/String.hpp
            include/mutiny/test/StringBufferOutput.hpp
            include/mutiny/test/Terminator.hpp
            include/mutiny/test/Test.hpp
            include/mutiny/test/TestingFixture.hpp
            include/mutiny/test/features.hpp
            include/mutiny/test/time.hpp
    PRIVATE FILE_SET private_headers TYPE HEADERS
        BASE_DIRS src/include
        FILES
            src/include/mutiny/mock/ActualCallTrace.hpp
            src/include/mutiny/mock/CheckedActualCall.hpp
            src/include/mutiny/mock/CheckedExpectedCall.hpp
            src/include/mutiny/mock/ExpectedCallsList.hpp
            src/include/mutiny/mock/IgnoredActualCall.hpp
            src/include/mutiny/mock/IgnoredExpectedCall.hpp
            src/include/mutiny/mock/NamedValueList.hpp
            src/include/mutiny/test/CompositeOutput.hpp
            src/include/mutiny/test/ExecFunctionShell.hpp
            src/include/mutiny/test/JUnitOutput.hpp
            src/include/mutiny/test/JUnitOutputPlugin.hpp
            src/include/mutiny/test/NullPlugin.hpp
            src/include/mutiny/test/StringCollection.hpp
            src/include/mutiny/test/jump_buffer.h
            src/include/mutiny/test/math.hpp
)

include(CheckCXXSymbolExists)
set(CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_FLAGS})
check_cxx_symbol_exists(gettimeofday "sys/time.h" MUTINY_HAVE_GETTIMEOFDAY)

# jump_buffer.c is compiled as C (to avoid MSVC C4611) but C++ exceptions
# must be able to propagate through mutiny_set_jmp() at runtime.  On ARM EHABI
# the unwinder requires every function in the unwind chain to have an
# .ARM.exidx entry; C files get none unless -funwind-tables is passed.
include(CheckCCompilerFlag)
check_c_compiler_flag(-funwind-tables MUTINY_C_HAS_FUNWIND_TABLES)
if(MUTINY_C_HAS_FUNWIND_TABLES)
    set_source_files_properties(
        src/test/jump_buffer.c
        PROPERTIES COMPILE_OPTIONS "-funwind-tables"
    )
endif()

target_compile_features(mutiny
    PUBLIC
        cxx_std_11
        c_std_99
)

target_compile_definitions(mutiny
    PUBLIC
        MUTINY_USE_STD_CPP_LIB=$<BOOL:${MUTINY_USE_STD_CPP_LIB}>
        MUTINY_USE_STD_STRING=$<BOOL:${MUTINY_USE_STD_STRING}>
    PRIVATE
        $<$<BOOL:${MUTINY_HAVE_GETTIMEOFDAY}>:MUTINY_HAVE_GETTIMEOFDAY>
)

add_library(mutiny::mutiny ALIAS mutiny)

# Apply C-specific naming rules (mutiny_ prefix) only to the library target.
# Tests and examples are not subject to this convention.
if(CMAKE_C_CLANG_TIDY)
    set_target_properties(mutiny PROPERTIES
        C_CLANG_TIDY "${CMAKE_C_CLANG_TIDY};--config-file=${CMAKE_CURRENT_SOURCE_DIR}/cmake/c.clang-tidy"
    )
endif()

if (MUTINY_BUILD_TESTING)
    add_subdirectory(tests)
endif ()

if (MUTINY_EXAMPLES)
    add_subdirectory(examples)
endif()

if(PROJECT_IS_TOP_LEVEL)
    install(
        TARGETS mutiny
        EXPORT mutinyTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    include(CMakePackageConfigHelpers)

    set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
    set(LIB_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
    set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
    set(MODULES_DIR "${PROJECT_SOURCE_DIR}/cmake/Modules")

    set(MUTINY_CONFIG_DEST "${LIB_INSTALL_DIR}/mutiny/cmake")

    # CMake package config files (install tree).
    configure_package_config_file(cmake/mutiny-config.cmake.install.in
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
        NAMESPACE mutiny::
        DESTINATION "${MUTINY_CONFIG_DEST}"
    )
    install(
        FILES
            ${PROJECT_SOURCE_DIR}/cmake/Modules/mutiny.cmake
            ${PROJECT_SOURCE_DIR}/cmake/Modules/_mutiny_discovery.cmake
        DESTINATION "${MUTINY_CONFIG_DEST}/Modules"
    )

    # CMake package config files (build tree).
    configure_package_config_file(cmake/mutiny-config.cmake.build.in
        ${PROJECT_BINARY_DIR}/mutiny-config.cmake
        INSTALL_DESTINATION ${PROJECT_BINARY_DIR}
        PATH_VARS INCLUDE_DIR PROJECT_BINARY_DIR MODULES_DIR
    )
    write_basic_package_version_file(
        ${PROJECT_BINARY_DIR}/mutiny-configVersion.cmake
        COMPATIBILITY SameMajorVersion
    )
endif()
