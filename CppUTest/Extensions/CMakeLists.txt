add_library(CppUTestExt)

target_sources(CppUTestExt
    PRIVATE
        src/OrderedTest.cpp
    PUBLIC FILE_SET HEADERS
        BASE_DIRS include
        FILES
            include/CppUTest/Extensions/OrderedTest.hpp
)

set_target_properties(CppUTestExt PROPERTIES EXPORT_NAME Extensions)

target_link_libraries(CppUTestExt PUBLIC CppUTest)

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
