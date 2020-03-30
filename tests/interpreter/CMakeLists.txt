
SET(TEST_NAME test_interpreter)

SET(TEST_FILES
        test_fixtures.h test_fixtures.cpp
        declaration_test.cpp identifier_test.cpp assignment_test.cpp)


add_executable(${TEST_NAME} main.cpp ${TEST_FILES})
target_link_libraries(${TEST_NAME} tb_lexer tb_parser tb_interpreter gtest gtest_main)
target_include_directories(${TEST_NAME} PRIVATE ${TB_LANG_SRC}/lexer ${TB_LANG_SRC}/parser ${TB_LANG_SRC}/interpreter ${TB_LANG_SRC}/common)