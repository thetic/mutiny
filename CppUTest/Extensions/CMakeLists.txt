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
        src/MockActualCall.cpp
        src/MockActualCallTrace.cpp
        src/MockCheckedActualCall.cpp
        src/MockCheckedExpectedCall.cpp
        src/MockExpectedCall.cpp
        src/MockExpectedCallsList.cpp
        src/MockIgnoredExpectedCall.cpp
        src/MockFailure.cpp
        src/MockNamedValue.cpp
        src/MockNamedValueComparatorsAndCopiersRepository.cpp
        src/MockNamedValueList.cpp
        src/MockSupport.cpp
        src/MockSupportPlugin.cpp
        src/OrderedTest.cpp
    PUBLIC FILE_SET HEADERS
        BASE_DIRS include
        FILES
            include/CppUTest/Extensions/IEEE754ExceptionsPlugin.hpp
            include/CppUTest/Extensions/MockActualCall.hpp
            include/CppUTest/Extensions/MockCheckedActualCall.hpp
            include/CppUTest/Extensions/MockCheckedExpectedCall.hpp
            include/CppUTest/Extensions/MockExpectedCall.hpp
            include/CppUTest/Extensions/MockExpectedCallsList.hpp
            include/CppUTest/Extensions/MockFailure.hpp
            include/CppUTest/Extensions/MockNamedValue.hpp
            include/CppUTest/Extensions/MockNamedValueComparator.hpp
            include/CppUTest/Extensions/MockNamedValueComparatorsAndCopiersRepository.hpp
            include/CppUTest/Extensions/MockNamedValueCopier.hpp
            include/CppUTest/Extensions/MockNamedValueList.hpp
            include/CppUTest/Extensions/MockSupport.h
            include/CppUTest/Extensions/MockSupport.hpp
            include/CppUTest/Extensions/MockSupportPlugin.hpp
            include/CppUTest/Extensions/OrderedTest.hpp
    PRIVATE FILE_SET private_headers TYPE HEADERS
        BASE_DIRS include_private
        FILES
            include_private/CppUTest/Extensions/MockActualCallTrace.hpp
            include_private/CppUTest/Extensions/MockIgnoredActualCall.hpp
            include_private/CppUTest/Extensions/MockIgnoredExpectedCall.hpp
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
