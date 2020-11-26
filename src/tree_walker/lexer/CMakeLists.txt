
set(LEX_SOURCE
        lexer.cpp
        angreal_dfa.cpp)

set(LEX_HEADERS_DIR
        ${CMAKE_CURRENT_SOURCE_DIR};
        PARENT_SCOPE
        )

add_library(angreal_lexer STATIC ${LEX_SOURCE})
set_property(TARGET angreal_lexer PROPERTY CXX_STANDARD 20)
target_include_directories(angreal_lexer PRIVATE ${LEX_HEADERS_DIR})