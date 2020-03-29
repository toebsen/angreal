cmake_minimum_required(VERSION 3.10)
project(tblang)

set(CMAKE_CXX_STANDARD 20)

########## Dependencies ##############
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/.cmake)
set(BOOST_REQUESTED_VERSION 1.66)

include(.cmake/gtest.cmake)

########## Project ##############
set(TB_LANG_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party")
include_directories("${THIRD_PARTY_DIR}/magic_enum")

add_subdirectory(src)
set(TBLANG_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_subdirectory(tests)
enable_testing()
