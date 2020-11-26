
set(ERROR_REPORTER_SOURCE
        error_reporter.cpp
        error_reporter.h

        )

set(ERROR_REPORTER_HEADERS_DIR
        ${CMAKE_CURRENT_SOURCE_DIR};
        PARENT_SCOPE
        )

add_library(angreal_error_reporter STATIC ${ERROR_REPORTER_SOURCE})
set_property(TARGET angreal_error_reporter PROPERTY CXX_STANDARD 20)
target_include_directories(angreal_error_reporter PRIVATE ${ERROR_REPORTER_HEADERS_DIR})