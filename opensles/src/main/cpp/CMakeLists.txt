cmake_minimum_required(VERSION 3.10.2)
project(opensles_demo)
set(COMMON_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../common")

include_directories(${COMMON_DIR}/3rd/utils/include)

add_subdirectory(${COMMON_DIR}/3rd/utils ${CMAKE_CURRENT_BINARY_DIR}/common)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd/ffmpeg/libs/${ANDROID_ABI})

add_library(opensles_demo SHARED opensles_demo.cpp)

target_link_libraries(opensles_demo
        android
        log
        OpenSLES
        utils
        )

