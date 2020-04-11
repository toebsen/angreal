
SET(TEST_NAME test_interpreter)

MESSAGE(STATUS "Creating ${TEST_NAME}")

SET(TEST_FILES
        test_fixtures.h test_fixtures.cpp
        declaration_test.cpp identifier_test.cpp assignment_test.cpp binary_op_test.cpp unary_op_test.cpp callable_test.cpp ../acceptance/test_fixtures.h)


add_executable(${TEST_NAME} ${TEST_FILES})
target_link_libraries(${TEST_NAME} tb_lexer tb_parser tb_interpreter tb_env gtest gtest_main)
target_include_directories(${TEST_NAME} PRIVATE ${TB_LANG_SRC}/lexer ${TB_LANG_SRC}/parser ${TB_LANG_SRC}/interpreter ${TB_LANG_SRC}/common)
add_test(${TEST_NAME} ${TEST_NAME})