
SET(TEST_NAME test_parser)

add_executable(${TEST_NAME} parser_test.cpp test_fixtures.h)
target_link_libraries(${TEST_NAME} tb_lexer tb_parser gtest gtest_main)
target_include_directories(${TEST_NAME} PRIVATE ${TB_LANG_SRC}/lexer ${TB_LANG_SRC}/parser ${TB_LANG_SRC}/common)
add_test(${TEST_NAME} ${TEST_NAME})