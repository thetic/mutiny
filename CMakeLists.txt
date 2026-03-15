cmake_minimum_required(VERSION 4.0)

project(CppUTest
    VERSION 4.0
)

###############
# Conan support
###############
if (EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    message(STATUS "conan_basic_setup()")
    conan_basic_setup()
endif()

include(CTest)

include(CMakeDependentOption)
option(CPPUTEST_USE_STD_CPP_LIB "Use the standard C++ library" ON)
option(CPPUTEST_EXTENSIONS "Use the CppUTest extension library" ON)
cmake_dependent_option(CPPUTEST_BUILD_TESTING "Compile and make tests for CppUTest"
    ${PROJECT_IS_TOP_LEVEL} "BUILD_TESTING" OFF)
cmake_dependent_option(CPPUTEST_EXAMPLES "Compile and make examples?"
    ${PROJECT_IS_TOP_LEVEL} "CPPUTEST_EXTENSIONS;CPPUTEST_USE_STD_CPP_LIB" OFF)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
if(NOT PROJECT_IS_TOP_LEVEL)
    set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" PARENT_SCOPE)
endif()

include(GNUInstallDirs)

add_subdirectory(CppUTest)

if (CPPUTEST_EXAMPLES)
    add_subdirectory(examples)
endif()

if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory(package)
endif()
