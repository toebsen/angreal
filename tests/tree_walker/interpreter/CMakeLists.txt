
SET(TEST_NAME test_interpreter)

SET(TEST_FILES
        test_fixtures.h test_fixtures.cpp
        declaration_test.cpp identifier_test.cpp assignment_test.cpp binary_op_test.cpp unary_op_test.cpp callable_test.cpp scope_test.cpp class_test.cpp)


add_executable(${TEST_NAME} ${TEST_FILES})
target_link_libraries(${TEST_NAME} angreal_lexer angreal_parser angreal_interpreter gtest gtest_main)
target_include_directories(${TEST_NAME} PRIVATE ${ANGREAL_SRC}/lexer ${ANGREAL_SRC}/parser ${ANGREAL_SRC}/interpreter ${ANGREAL_SRC}/common)
add_test(${TEST_NAME} ${TEST_NAME})

