set(LIBANGREAL_SOURCE
        chunk.cpp
        debug.cpp
        value.cpp
        virtual_machine.cpp
        )


add_library(libangreal STATIC ${LIBANGREAL_SOURCE} ${ANGREAL_INCLUDES})
target_include_directories(libangreal PRIVATE ${ANGREAL_INCLUDE_DIR})
target_compile_definitions(libangreal PRIVATE DEBUG_TRACE_EXECUTION=${DEBUG_TRACE_EXECUTION})
