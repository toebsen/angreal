set(SEM_ANALYZER_SOURCE
        semantic_analyzer.cpp resolver.cpp resolver.h)

set(SEM_ANALYZER_HEADERS_DIR
        ${CMAKE_CURRENT_SOURCE_DIR};
        PARENT_SCOPE
        )

add_library(angreal_sem_analyzer STATIC ${SEM_ANALYZER_SOURCE})
target_link_libraries(angreal_sem_analyzer angreal_parser angreal_interpreter)
target_include_directories(angreal_sem_analyzer PRIVATE ${SEM_ANALYZER_HEADERS_DIR} ${PARSER_HEADERS_DIR})