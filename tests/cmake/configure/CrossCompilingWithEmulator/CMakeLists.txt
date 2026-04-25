cmake_minimum_required(VERSION 3.10)
project(test LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH "${MUTINY_MODULE_PATH}")
include(mu.tiny)
add_executable(MyTests "${CMAKE_CURRENT_LIST_DIR}/../empty.cpp")
# CMAKE_SYSTEM_NAME=Generic is passed on the command line; CMake derives
# CMAKE_CROSSCOMPILING=TRUE from it automatically during project() detection.
set_target_properties(MyTests PROPERTIES CROSSCOMPILING_EMULATOR "${CMAKE_COMMAND}")
mutiny_discover_tests(MyTests)
get_property(p DIRECTORY PROPERTY TEST_INCLUDE_FILES)
message(STATUS "TEST_INCLUDE_FILES: ${p}")
