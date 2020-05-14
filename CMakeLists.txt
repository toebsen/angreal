cmake_minimum_required(VERSION 3.10)
project(angreal)

include(.cmake/common.cmake)

########## Dependencies ##############
# see https://cmake.org/cmake/help/latest/module/FindBoost.html
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
add_definitions(-DBOOST_UUID_FORCE_AUTO_LINK)

include(.cmake/gtest.cmake)

########## Project ##############

set(ANGREAL_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party")
include_directories("${THIRD_PARTY_DIR}/magic_enum")

add_subdirectory(src)
set(ANGREAL_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/src")

enable_testing()
add_subdirectory(tests)
