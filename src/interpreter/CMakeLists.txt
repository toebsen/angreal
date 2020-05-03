
add_subdirectory(analysis)
add_subdirectory(environment)

set(INTERPRETER_SOURCE
        interpreter.cpp
        context.h executor.cpp executor.h)


set(INTERPRETER_HEADERS_DIR
        ${CMAKE_CURRENT_SOURCE_DIR};
        PARENT_SCOPE
        )

add_library(tb_interpreter STATIC ${INTERPRETER_SOURCE})
target_link_libraries(tb_interpreter tb_parser tb_lexer tb_env tb_sem_analyzer)
target_include_directories(tb_interpreter PRIVATE ${PARSER_HEADERS_DIR})