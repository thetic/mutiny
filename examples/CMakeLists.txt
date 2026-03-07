cmake_minimum_required(VERSION 4.0)

project(CppUTestExample)

if(PROJECT_IS_TOP_LEVEL)
  find_package(CppUTest 4.0 REQUIRED)
endif()

include(CTest)

add_subdirectory(AllTests)
add_subdirectory(ApplicationLib)
