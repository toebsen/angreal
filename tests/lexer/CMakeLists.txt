
SET(TEST_NAME test_lexer)
MESSAGE(STATUS "Creating ${TEST_NAME}")
add_executable(${TEST_NAME} dfa_test.cpp lexer_test.cpp)
target_link_libraries(${TEST_NAME} tb_lexer gtest gtest_main)
target_include_directories(${TEST_NAME} PRIVATE ${TB_LANG_SRC}/common)
add_test(${TEST_NAME} ${TEST_NAME})