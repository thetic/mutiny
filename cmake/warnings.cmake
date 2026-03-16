unset(WARNING_COMMON_FLAGS)
unset(WARNING_C_FLAGS)
unset(WARNING_CXX_FLAGS)

if(
    (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") OR
    (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
)
    # MSVC / ClangCL: use MSVC-style flags. -Wall maps to /Wall (== -Weverything); use /W4 instead.
    set(WARNING_COMMON_FLAGS
        /W4
    )

    set(WARNING_C_FLAGS ${WARNING_COMMON_FLAGS})
    set(WARNING_CXX_FLAGS ${WARNING_COMMON_FLAGS})
elseif(
    (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") OR
    (CMAKE_CXX_COMPILER_ID STREQUAL "Clang") OR
    (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
)
    set(WARNING_COMMON_FLAGS
        -Wall
        -Wextra
        -Wshadow
        -Wswitch-default
        -Wswitch-enum
        -Wconversion
        -Wsign-conversion
        -Wmissing-include-dirs
    )

    set(WARNING_C_FLAGS
        ${WARNING_COMMON_FLAGS}
        -Wstrict-prototypes
    )

    set(WARNING_CXX_FLAGS
        ${WARNING_COMMON_FLAGS}
        -Woverloaded-virtual
        -Wsuggest-override
        -Wold-style-cast
    )
endif()

message(CHECK_START "Checking supported C warning options")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
unset(warningFlags)
include(CheckCCompilerFlag)
foreach(flag IN LISTS WARNING_C_FLAGS)
    check_c_compiler_flag("${flag}" WARNING_C_FLAG_${flag})
    if(WARNING_C_FLAG_${flag})
        list(APPEND warningFlags "${flag}")
    endif()
endforeach()
list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "${warningFlags}")
add_compile_options("$<$<COMPILE_LANGUAGE:C>:${warningFlags}>")

message(CHECK_START "Checking supported C++ warning options")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
unset(warningFlags)
include(CheckCXXCompilerFlag)
foreach(flag IN LISTS WARNING_CXX_FLAGS)
    string(REPLACE "++" "xx" WARNING_CXX_FLAG_VAR "WARNING_CXX_FLAG_${flag}")
    check_cxx_compiler_flag("${flag}" ${WARNING_CXX_FLAG_VAR})
    if(${WARNING_CXX_FLAG_VAR})
        list(APPEND warningFlags "${flag}")
    endif()
endforeach()
list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "${warningFlags}")
add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:${warningFlags}>")
