########## GTEST ##############
include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.8.1
)

if (NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
endif ()

if (MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif ()

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "--coverage")
endif ()

# Bring the populated content into the build
add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
