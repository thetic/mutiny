include(CheckCompilerFlag)

# MSVC and ClangCL require MSVC-style flag syntax.
# All other compilers are assumed to accept GCC-style -W flags;
# cmake_check_compiler_flag() silently drops any flag not supported.
if(
    (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") OR
    (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
)
    # /Wall on MSVC is equivalent to -Weverything; use /W4 instead.
    set(_warning_c_flags   /W4)
    set(_warning_cxx_flags /W4)
else()
    set(_common_flags
        -Wall
        -Wextra
        -Wshadow
        -Wswitch-default
        -Wswitch-enum
        -Wconversion
        -Wsign-conversion
        -Wmissing-include-dirs
        -Wdocumentation
    )
    set(_warning_c_flags   ${_common_flags} -Wstrict-prototypes)
    set(_warning_cxx_flags ${_common_flags} -Woverloaded-virtual -Wold-style-cast)
endif()

function(_add_checked_warning_flags lang flags)
    get_property(languages GLOBAL PROPERTY ENABLED_LANGUAGES)
    if (NOT "${lang}" IN_LIST languages)
        return()
    endif()
    set(CMAKE_REQUIRED_QUIET TRUE)
    message(CHECK_START "Checking supported ${lang} warning options")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    set(_supported)
    foreach(flag IN LISTS flags)
        string(MAKE_C_IDENTIFIER "WARN_${lang}_${flag}" _var)
        cmake_check_compiler_flag(${lang} "${flag}" ${_var})
        if(${_var})
            list(APPEND _supported "${flag}")
        endif()
    endforeach()

    list(POP_BACK CMAKE_MESSAGE_INDENT)
    message(CHECK_PASS "${_supported}")
    add_compile_options("$<$<COMPILE_LANGUAGE:${lang}>:${_supported}>")
endfunction()

_add_checked_warning_flags(C   "${_warning_c_flags}")
_add_checked_warning_flags(CXX "${_warning_cxx_flags}")
