SET(TEST_NAME unit_tests)

add_executable(${TEST_NAME}

        test_chunk.cpp
        test_value.cpp
        test_vm.cpp

        ut_test_vm.h
        ut_test_vm.cpp

        ${ANGREAL_INCLUDES})

target_link_libraries(${TEST_NAME} libangreal angreal_mocks gtest gmock gtest_main)
target_include_directories(${TEST_NAME} PRIVATE ${ANGREAL_INCLUDE_DIR} ${ANGREAL_MOCKS_INCLUDE_DIR} ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})
set_mutable_for_testing(${TEST_NAME})

add_test(${TEST_NAME} ${TEST_NAME})
