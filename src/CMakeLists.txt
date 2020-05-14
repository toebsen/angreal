
set(ANGREAL_COMMON_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/common")
include_directories(${ANGREAL_COMMON_INCLUDES})

add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(interpreter)