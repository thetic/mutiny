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

if(NOT DEFINED CPPUTEST_PLATFORM)
  if(DEFINED CPP_PLATFORM)
    message(DEPRECATION
      "The CPP_PLATFORM variable has been deprecated. "
      "Use CPPUTEST_PLATFORM instead."
    )
    set(CPPUTEST_PLATFORM ${CPP_PLATFORM})
  else()
    if(MSVC)
      set(CPPUTEST_PLATFORM VisualCpp)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "IAR")
      set(CPPUTEST_PLATFORM Iar)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "ARMCC")
      set(CPPUTEST_PLATFORM armcc)
    elseif(DOS)
      set(CPPUTEST_PLATFORM Dos)
    elseif(BORLAND)
      set(CPPUTEST_PLATFORM Borland)
    else()
      set(CPPUTEST_PLATFORM Gcc)
    endif()
  endif()
  message(STATUS "Setting CPPUTEST_PLATFORM: ${CPPUTEST_PLATFORM}")
endif()
set(CPPUTEST_PLATFORM "${CPPUTEST_PLATFORM}" CACHE STRING "Platform implementation")
set_property(
  CACHE CPPUTEST_PLATFORM
  PROPERTY STRINGS
    armcc Borland C2000 Dos Gcc Iar Keil Symbian VisualCpp OFF
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
if(NOT PROJECT_IS_TOP_LEVEL)
  set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" PARENT_SCOPE)
endif()

include(GNUInstallDirs)

# Helper to handle generating a map file, which is annoyingly tricky.
function(add_mapfile target)
    set(mapfile "$<TARGET_FILE:${target}>.map")
    if(CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
        # ClangCL (lld-link) can't generate map files
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "IAR")
        target_link_options(${target} PRIVATE "SHELL:--map ${mapfile}.map")
    elseif(MSVC)
        target_link_options(${target} PRIVATE "/MAP:${mapfile}")
    elseif(
        (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") OR
        (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    )
        target_link_options(${target} PRIVATE "LINKER:-Map=${mapfile}")
    endif()
endfunction()

add_subdirectory(CppUTest)

if (CPPUTEST_EXTENSIONS)
    add_subdirectory(CppUTestExt)
endif ()

if (CPPUTEST_EXAMPLES)
  add_subdirectory(examples)
endif()

if(PROJECT_IS_TOP_LEVEL)
  add_subdirectory(package)
endif()
