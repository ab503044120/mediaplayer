//
// Created by huihui on 2019/9/12.
//

#ifndef MUSICPLAYER_MUSIC_PAYER_JNI_H
#define MUSICPLAYER_MUSIC_PAYER_JNI_H

jclass gMusicPlayerClass = nullptr;

JavaVM *gJavaVM = nullptr;

static jlong player_init(JNIEnv *env, jobject thiz);

static void player_setDataSource( JNIEnv *env, jobject thiz,jlong nPlayer, jstring dataSource);

static void player_prepare( JNIEnv *env, jobject thiz,jlong nPlayer);

static void player_start( JNIEnv *env, jobject thiz,jlong nPlayer);

static void player_stop( JNIEnv *env, jobject thiz,jlong nPlayer);

static void player_pause( JNIEnv *env, jobject thiz,jlong nPlayer);

static void player_seek( JNIEnv *env, jobject thiz,jlong nPlayer, jlong msec);

static jlong player_getPosition( JNIEnv *env, jobject thiz,jlong nPlayer);

static jlong player_getDuration( JNIEnv *env, jobject thiz,jlong nPlayer);

static void player_reset( JNIEnv *env, jobject thiz,jlong nPlayer);

static void player_release( JNIEnv *env, jobject thiz,jlong nPlayer);

#endif //MUSICPLAYER_MUSIC_PAYER_JNI_H
