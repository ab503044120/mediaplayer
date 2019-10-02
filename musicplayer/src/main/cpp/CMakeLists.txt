cmake_minimum_required(VERSION 3.4.1)

include_directories(3rd/utils/include)
include_directories(3rd/ffmpeg/include)

#add_subdirectory(3rd/ffmpeg)

add_subdirectory(3rd/utils)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/jni SRC_MUSIC_PLAYER_JNI)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/musicplayer SRC_MUSIC_PLAYER)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd/ffmpeg/libs/${ANDROID_ABI})

add_library(musicplayer SHARED ${SRC_MUSIC_PLAYER} ${SRC_MUSIC_PLAYER_JNI})

target_link_libraries(musicplayer
        android
        log
        utils
        avcodec
        avdevice
        avfilter
        avformat
        avutil
        postproc
        swresample
        swscale)

