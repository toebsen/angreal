
SET(TEST_NAME test_at)
MESSAGE(STATUS "Creating ${TEST_NAME}")

SET(TEST_FILES
    AT_test.cpp)

add_executable(${TEST_NAME} ${TEST_FILES})
target_link_libraries(${TEST_NAME} tb_lexer tb_parser tb_interpreter tb_env gtest gtest_main)
target_include_directories(${TEST_NAME} PRIVATE ${TB_LANG_SRC}/lexer ${TB_LANG_SRC}/parser ${TB_LANG_SRC}/interpreter ${TB_LANG_SRC}/common)
add_test(${TEST_NAME} ${TEST_NAME})