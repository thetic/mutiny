add_library(mutiny_test_util STATIC
    MockFailureReporter.cpp
)

target_include_directories(mutiny_test_util PUBLIC .)

target_link_libraries(mutiny_test_util
    PUBLIC mu::tiny
)
