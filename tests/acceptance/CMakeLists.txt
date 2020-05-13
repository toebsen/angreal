
SET(TEST_NAME test_at)

SET(TEST_FILES
        function_test.cpp
        example_test.cpp
        scoping_test.cpp
        errors_test.cpp
        while_test.cpp)

add_executable(${TEST_NAME} ${TEST_FILES})
target_link_libraries(${TEST_NAME} tb_lexer tb_parser tb_interpreter gtest gtest_main)
target_include_directories(${TEST_NAME} PRIVATE ${TB_LANG_SRC}/lexer ${TB_LANG_SRC}/parser ${TB_LANG_SRC}/interpreter ${TB_LANG_SRC}/common)
add_test(${TEST_NAME} ${TEST_NAME})