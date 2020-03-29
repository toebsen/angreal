
set(Interpreter_SOURCE
        interpreter.cpp
        Scope.cpp
        )


set(INTERPRETER_HEADERS_DIR
        ${CMAKE_CURRENT_SOURCE_DIR};
        PARENT_SCOPE
        )

add_library(tb_interpreter STATIC ${Interpreter_SOURCE})

target_include_directories(tb_interpreter PRIVATE ${PARSER_HEADERS_DIR})