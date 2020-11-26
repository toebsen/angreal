
SET(TEST_NAME test_at)

SET(TEST_FILES
        function_test.cpp
        example_test.cpp
        scoping_test.cpp
        errors_test.cpp
        while_test.cpp class_test.cpp)

add_executable(${TEST_NAME} ${TEST_FILES})
target_link_libraries(${TEST_NAME} angreal_lexer angreal_parser angreal_interpreter gtest gtest_main)
target_include_directories(${TEST_NAME} PRIVATE ${ANGREAL_SRC}/lexer ${ANGREAL_SRC}/parser ${ANGREAL_SRC}/interpreter ${ANGREAL_SRC}/common)
add_test(${TEST_NAME} ${TEST_NAME})