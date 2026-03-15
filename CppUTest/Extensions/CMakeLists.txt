include(CheckSourceCompiles)
check_source_compiles(CXX
    "#include <fenv.h>
     int main() {
         feclearexcept(FE_ALL_EXCEPT);
         return fetestexcept(FE_OVERFLOW);
     }"
    CPPUTEST_HAVE_FENV
)

add_library(CppUTestExt)

target_sources(CppUTestExt
    PRIVATE
        src/IEEE754ExceptionsPlugin.cpp
        src/OrderedTest.cpp
    PUBLIC FILE_SET HEADERS
        BASE_DIRS include
        FILES
            include/CppUTest/Extensions/IEEE754ExceptionsPlugin.hpp
            include/CppUTest/Extensions/OrderedTest.hpp
)

set_target_properties(CppUTestExt PROPERTIES EXPORT_NAME Extensions)

target_link_libraries(CppUTestExt PUBLIC CppUTest)

target_compile_definitions(CppUTestExt PUBLIC
    CPPUTEST_HAVE_FENV=$<BOOL:${CPPUTEST_HAVE_FENV}>
)

add_library(CppUTest::Extensions ALIAS CppUTestExt)

if(PROJECT_IS_TOP_LEVEL)
    install(
        TARGETS CppUTestExt
        EXPORT CppUTestTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()

if (CPPUTEST_BUILD_TESTING)
    add_subdirectory(tests)
endif ()
