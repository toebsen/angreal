set(VIRTUAL_MACHINE_SOURCE
        vm.cpp object.cpp object.h object.cpp literal_types.cpp type.h)

set(VIRTUAL_MACHINE_HEADERS_DIR
        ${CMAKE_CURRENT_SOURCE_DIR};
        PARENT_SCOPE
        literal_types.h
        vm.h
        object.h
        )

add_library(tb_vm STATIC ${VIRTUAL_MACHINE_SOURCE})
target_link_libraries(tb_vm bcrypt)