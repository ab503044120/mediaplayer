cmake_minimum_required(VERSION 3.4.1)

include_directories(3rd/utils/include)
include_directories(3rd/ffmpeg/include)

#add_subdirectory(3rd/ffmpeg)

add_subdirectory(3rd/utils)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/jni SRC_MUSIC_PLAYER_JNI)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/musicplayer SRC_MUSIC_PLAYER)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/musicplayer/decoder SRC_MUSIC_PLAYER_DECODER)

#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd/ffmpeg/libs/${ANDROID_ABI})

add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec
        PROPERTIES IMPORTED_LOCATION
        ${Project_SOURCE_DIR}/3rd/ffmpeg/libs/${ANDROID_ABI}/libavcodec.so)

add_library(avdevice SHARED IMPORTED)
set_target_properties(avdevice
        PROPERTIES IMPORTED_LOCATION
        ${Project_SOURCE_DIR}/3rd/ffmpeg/libs/${ANDROID_ABI}/libavdevice.so)

add_library(avfilter SHARED IMPORTED)
set_target_properties(avfilter
        PROPERTIES IMPORTED_LOCATION
        ${Project_SOURCE_DIR}/3rd/ffmpeg/libs/${ANDROID_ABI}/libavfilter.so)

add_library(avformat SHARED IMPORTED)
set_target_properties(avformat
        PROPERTIES IMPORTED_LOCATION
        ${Project_SOURCE_DIR}/3rd/ffmpeg/libs/${ANDROID_ABI}/libavformat.so)

add_library(avutil SHARED IMPORTED)
set_target_properties(avutil
        PROPERTIES IMPORTED_LOCATION
        ${Project_SOURCE_DIR}/3rd/ffmpeg/libs/${ANDROID_ABI}/libavutil.so)

add_library(postproc SHARED IMPORTED)
set_target_properties(postproc
        PROPERTIES IMPORTED_LOCATION
        ${Project_SOURCE_DIR}/3rd/ffmpeg/libs/${ANDROID_ABI}/libpostproc.so)

add_library(swresample SHARED IMPORTED)
set_target_properties(swresample
        PROPERTIES IMPORTED_LOCATION
        ${Project_SOURCE_DIR}/3rd/ffmpeg/libs/${ANDROID_ABI}/libswresample.so)


add_library(swscale SHARED IMPORTED)
set_target_properties(swscale
        PROPERTIES IMPORTED_LOCATION
        ${Project_SOURCE_DIR}/3rd/ffmpeg/libs/${ANDROID_ABI}/libswscale.so)





add_library(musicplayer SHARED ${SRC_MUSIC_PLAYER} ${SRC_MUSIC_PLAYER_JNI} ${SRC_MUSIC_PLAYER_DECODER})

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

