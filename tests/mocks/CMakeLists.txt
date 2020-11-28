set(ANGREAL_MOCKS_SOURCE

        mock_debug_tracer.h

        PARENT_SCOPE
        )

SET(ANGREAL_MOCKS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)

add_library(angreal_mocks INTERFACE ${ANGREAL_MOCKS_SOURCE} ${ANGREAL_INCLUDES})
target_include_directories(angreal_mocks INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
