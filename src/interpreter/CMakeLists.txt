
add_subdirectory(analysis)
add_subdirectory(environment)

set(INTERPRETER_SOURCE
        interpreter.cpp
        context.h executor.cpp executor.h)


set(INTERPRETER_HEADERS_DIR
        ${CMAKE_CURRENT_SOURCE_DIR};
        PARENT_SCOPE
        )

add_library(angreal_interpreter STATIC ${INTERPRETER_SOURCE})
target_link_libraries(angreal_interpreter angreal_parser angreal_lexer angreal_env angreal_sem_analyzer)
target_include_directories(angreal_interpreter PRIVATE ${PARSER_HEADERS_DIR})