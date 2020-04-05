
SET(TEST_NAME test_at)

add_executable(${TEST_NAME} at.cpp)
target_link_libraries(${TEST_NAME} tb_lexer tb_parser gtest gtest_main)
target_include_directories(${TEST_NAME} PRIVATE ${TB_LANG_SRC}/lexer ${TB_LANG_SRC}/parser ${TB_LANG_SRC}/common)