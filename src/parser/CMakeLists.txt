
set(Parser_SOURCE
        parser.cpp
        ast.cpp
        type_system.cpp
        )


set(PARSER_HEADERS_DIR
        ${CMAKE_CURRENT_SOURCE_DIR};
        PARENT_SCOPE
        )

add_library(tb_parser STATIC ${Parser_SOURCE})

target_include_directories(tb_parser PRIVATE ${LEX_HEADERS_DIR} ${PARSER_HEADERS_DIR})