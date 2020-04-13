
add_subdirectory(environment)

set(INTERPRETER_SOURCE
        interpreter.cpp
        ../common/common.h context.h)


set(INTERPRETER_HEADERS_DIR
        ${CMAKE_CURRENT_SOURCE_DIR};
        PARENT_SCOPE
        )

add_library(tb_interpreter STATIC ${INTERPRETER_SOURCE})

target_include_directories(tb_interpreter PRIVATE ${PARSER_HEADERS_DIR})