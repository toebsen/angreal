SET(TEST_NAME unit_tests)

add_executable(${TEST_NAME}

        test_chunk.cpp
        test_value.cpp
        test_vm.cpp

        ${ANGREAL_INCLUDES})

target_link_libraries(${TEST_NAME} libangreal gtest gmock gtest_main)
target_include_directories(${TEST_NAME} PRIVATE ${ANGREAL_INCLUDE_DIR})
set_mutable_for_testing(${TEST_NAME})

add_test(${TEST_NAME} ${TEST_NAME})
