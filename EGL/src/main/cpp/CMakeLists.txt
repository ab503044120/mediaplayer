cmake_minimum_required(VERSION 3.4.1)

set(COMMON_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../common")

include_directories(${COMMON_DIR}/3rd/utils/include)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/egl SRC_EGL)

add_subdirectory(${COMMON_DIR}/3rd/utils ${CMAKE_CURRENT_BINARY_DIR}/common)

#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd/ffmpeg/libs/${ANDROID_ABI})

add_library(egl SHARED ${SRC_EGL} egl_jni.cpp)

target_link_libraries(egl
        android
        log
        -lGLESv2
        -lGLESv3
        -ljnigraphics
        -lEGL
        utils)

