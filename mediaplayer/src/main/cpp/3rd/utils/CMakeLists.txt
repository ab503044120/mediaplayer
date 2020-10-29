cmake_minimum_required(VERSION 3.4.1)

include_directories(include)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_UTILS)

add_library(utils STATIC ${SRC_UTILS})

target_link_libraries(utils android log)
