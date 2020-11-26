
SET(TEST_NAME test_lexer)

add_executable(${TEST_NAME} dfa_test.cpp lexer_test.cpp)
target_link_libraries(${TEST_NAME} angreal_lexer gtest gtest_main)
add_test(${TEST_NAME} ${TEST_NAME})