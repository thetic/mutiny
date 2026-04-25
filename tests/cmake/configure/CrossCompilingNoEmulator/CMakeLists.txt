cmake_minimum_required(VERSION 3.10)
project(test LANGUAGES NONE)
list(APPEND CMAKE_MODULE_PATH "${MUTINY_MODULE_PATH}")
include(mu.tiny)
# With LANGUAGES NONE cmake skips toolchain detection, so CMAKE_CROSSCOMPILING
# is never derived from CMAKE_SYSTEM_NAME.  Set it explicitly after project().
set(CMAKE_CROSSCOMPILING TRUE)
add_executable(MyTests IMPORTED GLOBAL)
set_target_properties(MyTests PROPERTIES IMPORTED_LOCATION "${CMAKE_COMMAND}")
mutiny_discover_tests(MyTests)
