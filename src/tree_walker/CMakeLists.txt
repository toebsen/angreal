
set(ANGREAL_TW_COMMON_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/common")
include_directories(${ANGREAL_TW_COMMON_INCLUDES})

set(ANGREAL_SRC "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(error_handler)
add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(interpreter)

set(ANGREAL_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}")