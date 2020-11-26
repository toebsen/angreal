set(LIBANGREAL_SOURCE
        chunk.cpp
        debug.cpp
        value.cpp
        )


add_library(libangreal STATIC ${LIBANGREAL_SOURCE} ${ANGREAL_INCLUDES})
target_include_directories(libangreal PRIVATE ${ANGREAL_INCLUDE_DIR})
