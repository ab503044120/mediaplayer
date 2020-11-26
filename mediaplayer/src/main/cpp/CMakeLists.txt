cmake_minimum_required(VERSION 3.4.1)

set(COMMON_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../common")

include_directories(${COMMON_DIR}/3rd/utils/include)
include_directories(${COMMON_DIR}/3rd/ffmpeg/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/mediaplayer)

#add_subdirectory(3rd/ffmpeg)

add_subdirectory(${COMMON_DIR}/3rd/utils utils)
add_subdirectory(${COMMON_DIR}/3rd/ffmpeg ffmpeg)


aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/jni SRC_MEDIA_PLAYER_JNI)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/mediaplayer SRC_MEDIA_PLAYER)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/mediaplayer/decoder SRC_MEDIA_PLAYER_DECODER)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/mediaplayer/queue SRC_MEDIA_PLAYER_QUEUE)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/mediaplayer/render SRC_MEDIA_PLAYER_RENDER)


add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec
        PROPERTIES IMPORTED_LOCATION
        ${COMMON_DIR}/3rd/ffmpeg/libs/${ANDROID_ABI}/libavcodec.so)

add_library(avdevice SHARED IMPORTED)
set_target_properties(avdevice
        PROPERTIES IMPORTED_LOCATION
        ${COMMON_DIR}/3rd/ffmpeg/libs/${ANDROID_ABI}/libavdevice.so)

add_library(avfilter SHARED IMPORTED)
set_target_properties(avfilter
        PROPERTIES IMPORTED_LOCATION
        ${COMMON_DIR}/3rd/ffmpeg/libs/${ANDROID_ABI}/libavfilter.so)

add_library(avformat SHARED IMPORTED)
set_target_properties(avformat
        PROPERTIES IMPORTED_LOCATION
        ${COMMON_DIR}/3rd/ffmpeg/libs/${ANDROID_ABI}/libavformat.so)

add_library(avutil SHARED IMPORTED)
set_target_properties(avutil
        PROPERTIES IMPORTED_LOCATION
        ${COMMON_DIR}/3rd/ffmpeg/libs/${ANDROID_ABI}/libavutil.so)

add_library(postproc SHARED IMPORTED)
set_target_properties(postproc
        PROPERTIES IMPORTED_LOCATION
        ${COMMON_DIR}/3rd/ffmpeg/libs/${ANDROID_ABI}/libpostproc.so)

add_library(swresample SHARED IMPORTED)
set_target_properties(swresample
        PROPERTIES IMPORTED_LOCATION
        ${COMMON_DIR}/3rd/ffmpeg/libs/${ANDROID_ABI}/libswresample.so)


add_library(swscale SHARED IMPORTED)
set_target_properties(swscale
        PROPERTIES IMPORTED_LOCATION
        ${COMMON_DIR}/3rd/ffmpeg/libs/${ANDROID_ABI}/libswscale.so)


#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd/ffmpeg/libs/${ANDROID_ABI})
add_library(mediaplayer SHARED
        ${SRC_MEDIA_PLAYER_JNI}
        ${SRC_MEDIA_PLAYER}
        ${SRC_MEDIA_PLAYER_DECODER}
        ${SRC_MEDIA_PLAYER_QUEUE}
        ${SRC_MEDIA_PLAYER_RENDER})

target_link_libraries(mediaplayer
        android
        log
        -lGLESv2
        -lGLESv3
        -ljnigraphics
        -lEGL
        utils
        avcodec
        avdevice
        avfilter
        avformat
        avutil
        postproc
        swresample
        swscale)

