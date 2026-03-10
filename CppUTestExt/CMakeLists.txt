add_library(CppUTestExt STATIC
    src/IEEE754ExceptionsPlugin.cpp
    src/MockFailure.cpp
    src/MockSupportPlugin.cpp
    src/MockActualCall.cpp
    src/MockSupport_c.cpp
    src/MockExpectedCall.cpp
    src/MockNamedValue.cpp
    src/OrderedTest.cpp
    src/MockExpectedCallsList.cpp
    src/MockSupport.cpp
)

target_sources(CppUTestExt
    PUBLIC FILE_SET HEADERS
        BASE_DIRS include
        FILES
            include/CppUTestExt/IEEE754ExceptionsPlugin.hpp
            include/CppUTestExt/MockActualCall.hpp
            include/CppUTestExt/MockCheckedActualCall.hpp
            include/CppUTestExt/MockCheckedExpectedCall.hpp
            include/CppUTestExt/MockExpectedCall.hpp
            include/CppUTestExt/MockExpectedCallsList.hpp
            include/CppUTestExt/MockFailure.hpp
            include/CppUTestExt/MockNamedValue.hpp
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
