cmake_minimum_required(VERSION 3.11)
project(angreal)

include(.cmake/common.cmake)
include(.cmake/clang-tidy.cmake)

########## Dependencies ##############

# see https://cmake.org/cmake/help/latest/module/FindBoost.html
#find_package(Boost REQUIRED)
#include_directories(${Boost_INCLUDE_DIR})
#add_definitions(-DBOOST_UUID_FORCE_AUTO_LINK)

include(.cmake/gtest.cmake)

########## Dependencies ##############

########## Project ##############


#set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party")
#include_directories("${THIRD_PARTY_DIR}/magic_enum")

add_subdirectory(src)


enable_testing()
add_subdirectory(tests)
