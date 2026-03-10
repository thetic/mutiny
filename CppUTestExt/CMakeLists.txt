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
            include/CppUTestExt/IEEE754ExceptionsPlugin.hpp
            include/CppUTestExt/MockActualCall.hpp
            include/CppUTestExt/MockActualCallTrace.hpp
            include/CppUTestExt/MockCheckedActualCall.hpp
            include/CppUTestExt/MockIgnoredActualCall.hpp
            include/CppUTestExt/MockCheckedExpectedCall.hpp
            include/CppUTestExt/MockExpectedCall.hpp
            include/CppUTestExt/MockExpectedCallsList.hpp
            include/CppUTestExt/MockFailure.hpp
            include/CppUTestExt/MockIgnoredExpectedCall.hpp
            include/CppUTestExt/MockNamedValue.hpp
            include/CppUTestExt/MockNamedValueComparator.hpp
            include/CppUTestExt/MockNamedValueComparatorsAndCopiersRepository.hpp
            include/CppUTestExt/MockNamedValueCopier.hpp
            include/CppUTestExt/MockNamedValueList.hpp
            include/CppUTestExt/MockSupport.h
            include/CppUTestExt/MockSupport.hpp
            include/CppUTestExt/MockSupportPlugin.hpp
            include/CppUTestExt/OrderedTest.hpp
)

target_link_libraries(CppUTestExt PUBLIC CppUTest)

add_library(CppUTest::CppUTestExt ALIAS CppUTestExt)

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
