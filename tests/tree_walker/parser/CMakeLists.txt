
SET(TEST_NAME test_parser)

add_executable(${TEST_NAME} parser_test.cpp test_fixtures.h)
target_link_libraries(${TEST_NAME} angreal_lexer angreal_parser gtest gtest_main)
target_include_directories(${TEST_NAME} PRIVATE ${ANGREAL_SRC}/lexer ${ANGREAL_SRC}/parser ${ANGREAL_SRC}/common)
add_test(${TEST_NAME} ${TEST_NAME})