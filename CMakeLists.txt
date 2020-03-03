cmake_minimum_required(VERSION 3.10)
project(tblang)

set(CMAKE_CXX_STANDARD 20)

########## BOOST ##############
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/.cmake)
set(BOOST_REQUESTED_VERSION 1.66)

########## GTEST ##############
include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.8.1
)

if (NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
endif()

if (MSVC)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif ()

if (CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "--coverage")
endif()

# Bring the populated content into the build
add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

########## Project ##############


set(TB_LANG_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party")
include_directories("${THIRD_PARTY_DIR}/magic_enum")

add_subdirectory(src)
set(TBLANG_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_subdirectory(tests)

enable_testing()
