cmake_minimum_required(VERSION 3.29)

project(mutinyExample)

if(PROJECT_IS_TOP_LEVEL)
  find_package(mutiny 0.1 REQUIRED)
endif()

include(CTest)

if(NOT DEFINED MUTINY_USE_STD_CPP_LIB)
    set(MUTINY_USE_STD_CPP_LIB ON)
endif()

add_library(ExampleLib)

target_sources(ExampleLib
    PRIVATE
        src/CircularBuffer.cpp
        src/Printer.cpp
        src/hello.c
    PUBLIC FILE_SET HEADERS
        BASE_DIRS include
        FILES
            include/CircularBuffer.hpp
            include/Printer.hpp
            include/hello.h
)

if(MUTINY_USE_STD_CPP_LIB)
    target_sources(ExampleLib
        PRIVATE src/EventDispatcher.cpp
        PUBLIC FILE_SET HEADERS BASE_DIRS include
            FILES include/EventDispatcher.hpp
    )
endif()

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
