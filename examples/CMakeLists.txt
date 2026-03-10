cmake_minimum_required(VERSION 4.0)

project(CppUTestExample)

if(PROJECT_IS_TOP_LEVEL)
  find_package(CppUTest 4.0 REQUIRED)
endif()

include(CTest)

add_library(ExampleLib
    src/CircularBuffer.cpp
    src/EventDispatcher.cpp
    src/hello.c
    src/Printer.cpp
)

target_sources(ExampleLib
    PUBLIC
    FILE_SET HEADERS
        BASE_DIRS include
        FILES
            include/CircularBuffer.h
            include/EventDispatcher.h
            include/hello.h
            include/Printer.h
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
