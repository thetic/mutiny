cmake_minimum_required(VERSION 4.0)

project(CppMuExample)

if(PROJECT_IS_TOP_LEVEL)
  find_package(CppMu 0.1 REQUIRED)
endif()

include(CTest)

add_library(ExampleLib)

target_sources(ExampleLib
    PRIVATE
        src/CircularBuffer.cpp
        src/EventDispatcher.cpp
        src/Printer.cpp
        src/hello.c
    PUBLIC FILE_SET HEADERS
        BASE_DIRS include
        FILES
            include/CircularBuffer.hpp
            include/EventDispatcher.hpp
            include/Printer.hpp
            include/hello.h
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
