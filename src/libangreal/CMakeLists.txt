set(LIBANGREAL_SOURCE
        chunk.cpp
        debug.cpp
        value.cpp
        virtual_machine.cpp
        stack.h
        )


add_library(libangreal STATIC ${LIBANGREAL_SOURCE} ${ANGREAL_INCLUDES})
target_include_directories(libangreal PRIVATE ${CMAKE_CURRENT} ${ANGREAL_INCLUDE_DIR})
target_compile_definitions(libangreal PRIVATE DEBUG_TRACE_EXECUTION=${DEBUG_TRACE_EXECUTION})
