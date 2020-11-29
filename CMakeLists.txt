cmake_minimum_required(VERSION 3.11)
project(angreal)

include(.cmake/common.cmake)
include(.cmake/clang-tidy.cmake)

########## Dependencies ##############

include(.cmake/gtest.cmake)

########## Dependencies ##############
set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party")
include_directories("${THIRD_PARTY_DIR}/magic_enum")

########## Project ##############

set(ANGREAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/include)

add_subdirectory(src)

if (ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()