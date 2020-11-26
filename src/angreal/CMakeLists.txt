set(ANGREAL_SOURCE
        main.cpp
        )

add_executable(angreal ${ANGREAL_SOURCE} ${ANGREAL_INCLUDE_DIR})
target_link_libraries(angreal LINK_PUBLIC libangreal)
target_include_directories(angreal PRIVATE ${ANGREAL_INCLUDE_DIR})