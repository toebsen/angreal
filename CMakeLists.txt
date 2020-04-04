cmake_minimum_required(VERSION 3.10)
project(tblang)

set(CMAKE_CXX_STANDARD 20)

########## Dependencies ##############list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/.cmake)
set(BOOST_REQUESTED_VERSION 1.66)
# see https://cmake.org/cmake/help/latest/module/FindBoost.html
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
add_definitions(-DBOOST_UUID_FORCE_AUTO_LINK)

include(.cmake/gtest.cmake)
########## Project ##############

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

set(TB_LANG_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party")
include_directories("${THIRD_PARTY_DIR}/magic_enum")

add_subdirectory(src)
set(TBLANG_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_subdirectory(tests)
enable_testing()
