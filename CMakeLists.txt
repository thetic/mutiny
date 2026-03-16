cmake_minimum_required(VERSION 3.29)

project(CppMu
    VERSION 0.1
)

include(CTest)

include(CMakeDependentOption)
option(CPPMU_USE_STD_CPP_LIB "Use the standard C++ library" ON)
cmake_dependent_option(CPPMU_BUILD_TESTING "Compile and make tests for CppMu"
    ${PROJECT_IS_TOP_LEVEL} "BUILD_TESTING" OFF)
cmake_dependent_option(CPPMU_EXAMPLES "Compile and make examples?"
    ON "PROJECT_IS_TOP_LEVEL" OFF)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
if(NOT PROJECT_IS_TOP_LEVEL)
    set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" PARENT_SCOPE)
endif()

include(GNUInstallDirs)

add_library(CppMu)

target_sources(CppMu
    PRIVATE
        src/CommandLineArguments.cpp
        src/CommandLineTestRunner.cpp
        src/CompositeTestOutput.cpp
        src/ConsoleTestOutput.cpp
        src/CppMu.cpp
        src/ExecFunctionTestShell.cpp
        src/ExpectFailTestShell.cpp
        src/IgnoredTestShell.cpp
        src/JUnitTestOutput.cpp
        src/math.cpp
        src/MockActualCall.cpp
        src/MockActualCallTrace.cpp
        src/MockCheckedActualCall.cpp
        src/MockCheckedExpectedCall.cpp
        src/MockExpectedCall.cpp
        src/MockExpectedCallsList.cpp
        src/MockFailure.cpp
        src/MockIgnoredExpectedCall.cpp
        src/MockNamedValue.cpp
        src/MockNamedValueComparatorsAndCopiersRepository.cpp
        src/MockNamedValueList.cpp
        src/MockSupport.cpp
        src/MockSupportPlugin.cpp
        src/SetPointerPlugin.cpp
        src/String.cpp
        src/StringBufferTestOutput.cpp
        src/StringCollection.cpp
        src/TeamCityTestOutput.cpp
        src/Test.cpp
        src/TestFailure.cpp
        src/TestFilter.cpp
        src/OrderedTest.cpp
        src/TestInstaller.cpp
        src/test_jump_buffer.c
        src/TestOutput.cpp
        src/TestPlugin.cpp
        src/TestRegistry.cpp
        src/TestResult.cpp
        src/TestShell.cpp
        src/TestShellPointerArray.cpp
        src/TestTerminator.cpp
        src/TestTestingFixture.cpp
        src/time.cpp
    PUBLIC FILE_SET HEADERS
        BASE_DIRS include
        FILES
            include/CppMu/CommandLineArguments.hpp
            include/CppMu/CommandLineTestRunner.hpp
            include/CppMu/CppMu.h
            include/CppMu/CppMu.hpp
            include/CppMu/ExpectFailTestShell.hpp
            include/CppMu/features.hpp
            include/CppMu/IgnoredTestShell.hpp
            include/CppMu/MockActualCall.hpp
            include/CppMu/MockExpectedCall.hpp
            include/CppMu/MockFailure.hpp
            include/CppMu/MockNamedValue.hpp
            include/CppMu/MockNamedValueComparator.hpp
            include/CppMu/MockNamedValueComparatorsAndCopiersRepository.hpp
            include/CppMu/MockNamedValueCopier.hpp
            include/CppMu/MockSupport.h
            include/CppMu/MockSupport.hpp
            include/CppMu/MockSupportPlugin.hpp
            include/CppMu/OrderedTest.hpp
            include/CppMu/SetPointerPlugin.hpp
            include/CppMu/String.hpp
            include/CppMu/StringBufferTestOutput.hpp
            include/CppMu/Test.hpp
            include/CppMu/TestFailure.hpp
            include/CppMu/TestFilter.hpp
            include/CppMu/TestInstaller.hpp
            include/CppMu/TestOutput.hpp
            include/CppMu/TestPlugin.hpp
            include/CppMu/TestRegistry.hpp
            include/CppMu/TestResult.hpp
            include/CppMu/TestShell.hpp
            include/CppMu/TestShellPointerArray.hpp
            include/CppMu/TestTerminator.hpp
            include/CppMu/TestTestingFixture.hpp
    PRIVATE FILE_SET private_headers TYPE HEADERS
        BASE_DIRS src/include
        FILES
            src/include/CppMu/CompositeTestOutput.hpp
            src/include/CppMu/ConsoleTestOutput.hpp
            src/include/CppMu/ExecFunctionTestShell.hpp
            src/include/CppMu/JUnitTestOutput.hpp
            src/include/CppMu/math.hpp
            src/include/CppMu/MockActualCallTrace.hpp
            src/include/CppMu/MockCheckedActualCall.hpp
            src/include/CppMu/MockCheckedExpectedCall.hpp
            src/include/CppMu/MockExpectedCallsList.hpp
            src/include/CppMu/MockIgnoredActualCall.hpp
            src/include/CppMu/MockIgnoredExpectedCall.hpp
            src/include/CppMu/MockNamedValueList.hpp
            src/include/CppMu/StringCollection.hpp
            src/include/CppMu/TeamCityTestOutput.hpp
            src/include/CppMu/test_jump_buffer.h
            src/include/CppMu/time.hpp
)

include(CheckCXXSymbolExists)
set(CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_FLAGS})
check_cxx_symbol_exists(gettimeofday "sys/time.h" CPPMU_HAVE_GETTIMEOFDAY)

target_compile_features(CppMu
    INTERFACE c_std_11
    PUBLIC cxx_std_11
)

target_compile_definitions(CppMu
    PUBLIC
        CPPMU_USE_STD_CPP_LIB=$<BOOL:${CPPMU_USE_STD_CPP_LIB}>
    PRIVATE
        $<$<BOOL:${CPPMU_HAVE_GETTIMEOFDAY}>:CPPMU_HAVE_GETTIMEOFDAY>
)

add_library(CppMu::CppMu ALIAS CppMu)

if (CPPMU_BUILD_TESTING)
    add_subdirectory(tests)
endif ()

if (CPPMU_EXAMPLES)
    add_subdirectory(examples)
endif()

if(PROJECT_IS_TOP_LEVEL)
    install(
        TARGETS CppMu
        EXPORT CppMuTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    include(CMakePackageConfigHelpers)

    set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
    set(LIB_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
    set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
    set(MODULES_DIR "${PROJECT_SOURCE_DIR}/cmake/Modules")

    set(CPPMU_CONFIG_DEST "${LIB_INSTALL_DIR}/CppMu/cmake")

    # CMake package config files (install tree).
    configure_package_config_file(cmake/CppMuConfig.cmake.install.in
        ${CMAKE_CURRENT_BINARY_DIR}/install/CppMuConfig.cmake
        INSTALL_DESTINATION "${CPPMU_CONFIG_DEST}"
        PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR
    )
    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/install/CppMuConfigVersion.cmake
        COMPATIBILITY SameMajorVersion
    )
    install(
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/install/CppMuConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/install/CppMuConfigVersion.cmake
        DESTINATION "${CPPMU_CONFIG_DEST}"
    )
    install(
        EXPORT CppMuTargets
        NAMESPACE CppMu::
        DESTINATION "${CPPMU_CONFIG_DEST}"
    )
    install(
        FILES
            ${PROJECT_SOURCE_DIR}/cmake/Modules/CppMu.cmake
            ${PROJECT_SOURCE_DIR}/cmake/Modules/_CppMuDiscovery.cmake
        DESTINATION "${CPPMU_CONFIG_DEST}/Modules"
    )

    # CMake package config files (build tree).
    configure_package_config_file(cmake/CppMuConfig.cmake.build.in
        ${PROJECT_BINARY_DIR}/CppMuConfig.cmake
        INSTALL_DESTINATION ${PROJECT_BINARY_DIR}
        PATH_VARS INCLUDE_DIR PROJECT_BINARY_DIR MODULES_DIR
    )
    write_basic_package_version_file(
        ${PROJECT_BINARY_DIR}/CppMuConfigVersion.cmake
        COMPATIBILITY SameMajorVersion
    )
endif()
