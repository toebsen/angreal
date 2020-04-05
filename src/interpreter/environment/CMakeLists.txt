set(ENVIRONMENT_SOURCE
        environment.cpp object.cpp object.h object.cpp literal_types.cpp type.h unary_op.cpp unary_op.h binary_op.cpp binary_op.h)

set(ENVIRONMENT_HEADERS_DIR
        ${CMAKE_CURRENT_SOURCE_DIR};
        PARENT_SCOPE
        literal_types.h
        environment.h
        object.h
        )

add_library(tb_env STATIC ${ENVIRONMENT_SOURCE})
target_include_directories(tb_env PRIVATE ${PARSER_HEADERS_DIR})
target_link_libraries(tb_env bcrypt)