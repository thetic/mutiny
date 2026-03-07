add_library(CppUTestExt STATIC
    src/CodeMemoryReportFormatter.cpp
    src/IEEE754ExceptionsPlugin.cpp
    src/MemoryReporterPlugin.cpp
    src/MockFailure.cpp
    src/MockSupportPlugin.cpp
    src/MockActualCall.cpp
    src/MockSupport_c.cpp
    src/MemoryReportAllocator.cpp
    src/MockExpectedCall.cpp
    src/MockNamedValue.cpp
    src/OrderedTest.cpp
    src/MemoryReportFormatter.cpp
    src/MockExpectedCallsList.cpp
    src/MockSupport.cpp
)

target_sources(CppUTestExt
    PUBLIC FILE_SET HEADERS
        BASE_DIRS include
        FILES
            include/CppUTestExt/CodeMemoryReportFormatter.h
            include/CppUTestExt/IEEE754ExceptionsPlugin.h
            include/CppUTestExt/MemoryReportAllocator.h
            include/CppUTestExt/MemoryReportFormatter.h
            include/CppUTestExt/MemoryReporterPlugin.h
            include/CppUTestExt/MockActualCall.h
            include/CppUTestExt/MockCheckedActualCall.h
            include/CppUTestExt/MockCheckedExpectedCall.h
            include/CppUTestExt/MockExpectedCall.h
            include/CppUTestExt/MockExpectedCallsList.h
            include/CppUTestExt/MockFailure.h
            include/CppUTestExt/MockNamedValue.h
            include/CppUTestExt/MockSupport.h
            include/CppUTestExt/MockSupport_c.h
            include/CppUTestExt/MockSupportPlugin.h
            include/CppUTestExt/OrderedTest.h
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
