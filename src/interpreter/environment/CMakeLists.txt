set(ENVIRONMENT_SOURCE
        environment.cpp
        object.cpp
        callable.cpp
        literal_types.cpp
        unary_op.cpp
        binary_op.cpp

        object.h
        type.h
        unary_op.h
        binary_op.h
        callable.h)

set(ENVIRONMENT_HEADERS_DIR
        ${CMAKE_CURRENT_SOURCE_DIR};
        PARENT_SCOPE
        literal_types.h
        environment.h
        object.h
        )

add_library(angreal_env STATIC ${ENVIRONMENT_SOURCE})
target_include_directories(angreal_env PRIVATE ${PARSER_HEADERS_DIR})
target_link_libraries(angreal_env bcrypt)