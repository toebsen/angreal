
set(TB_LANG_COMMON_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/common")
include_directories(${TB_LANG_COMMON_INCLUDES})

add_subdirectory(lexer)
add_subdirectory(parser)