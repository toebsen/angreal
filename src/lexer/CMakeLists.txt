
set(LEX_SOURCE
        lexer.cpp
        tb_dfa.cpp
        )

set(LEX_HEADERS_DIR
        ${CMAKE_CURRENT_SOURCE_DIR};
        PARENT_SCOPE
        )

add_library(tb_lexer STATIC ${LEX_SOURCE})
target_include_directories(tb_lexer PRIVATE ${LEX_HEADERS_DIR} {$TB_LANG_COMMON_INCLUDES})