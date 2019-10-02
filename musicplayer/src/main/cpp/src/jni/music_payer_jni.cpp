//
// Created by huihui on 2019/9/12.
//
#include <jni.h>
#include "music_payer_jni.h"
#include "utils.h"
#include "../musicplayer/Player.h"
static jlong player_init(JNIEnv *env, jobject thiz) {
  return reinterpret_cast<jlong>(new Player());
}

static void player_setDataSource(JNIEnv *env, jobject thiz, jlong nPlayer, jstring dataSource) {
  const char *dataSourceChar = env->GetStringUTFChars(dataSource, nullptr);
  reinterpret_cast<Player *>(nPlayer)->setDataSource(dataSourceChar);
  env->ReleaseStringUTFChars(dataSource, dataSourceChar);
}
static void player_prepare(JNIEnv *env, jobject thiz, jlong nPlayer) {
  reinterpret_cast<Player *>(nPlayer)->prepare();
}

static void player_start(JNIEnv *env, jobject thiz, jlong nPlayer) {
  reinterpret_cast<Player *>(nPlayer)->start();
}
static void player_stop(JNIEnv *env, jobject thiz, jlong nPlayer) {
  reinterpret_cast<Player *>(nPlayer)->stop();
}
static void player_pause(JNIEnv *env, jobject thiz, jlong nPlayer) {
  reinterpret_cast<Player *>(nPlayer)->pause();
}
static void player_seek(JNIEnv *env, jobject thiz, jlong nPlayer, jlong msec) {
  reinterpret_cast<Player *>(nPlayer)->seek(msec);
}
static jlong player_getPosition(JNIEnv *env, jobject thiz, jlong nPlayer) {
  return reinterpret_cast<Player *>(nPlayer)->getPosition();
}
static jlong player_getDuration(JNIEnv *env, jobject thiz, jlong nPlayer) {
  return reinterpret_cast<Player *>(nPlayer)->getDuration();
}
static void player_reset(JNIEnv *env, jobject thiz, jlong nPlayer) {
  reinterpret_cast<Player *>(nPlayer)->reset();
}
static void player_release(JNIEnv *env, jobject thiz, jlong nPlayer) {
  reinterpret_cast<Player *>(nPlayer)->release();
}

static const JNINativeMethod gMethods[] = {
    {"nInit", "()J", reinterpret_cast<void *>(player_init)},
    {"nSetDataSource", "(JLjava/lang/String;)V", reinterpret_cast<void *>(player_setDataSource)},
    {"nPrepare", "(J)V", reinterpret_cast<void *>(player_prepare)},
    {"nStart", "(J)V", reinterpret_cast<void *>(player_start)},
    {"nStop", "(J)V", reinterpret_cast<void *>(player_stop)},
    {"nPause", "(J)V", reinterpret_cast<void *>(player_pause)},
    {"nSeek", "(JJ)V", reinterpret_cast<void *>(player_seek)},
    {"nGetPosition", "(J)J", reinterpret_cast<void *>(player_getPosition)},
    {"nGetDuration", "(J)J", reinterpret_cast<void *>(player_getDuration)},
    {"nReset", "(J)V", reinterpret_cast<void *>(player_reset)},
    {"nRelease", "(J)V", reinterpret_cast<void *>(player_release)},
};

#define  FF_LOG_TAG "ffmpeg"
/*belown printf info*/
static void ffp_log_callback_brief(void *ptr, int level, const char *fmt, va_list vl) {
  int ffplv;
  if (level <= AV_LOG_ERROR)
    ffplv = ANDROID_LOG_ERROR;
  else if (level <= AV_LOG_WARNING)
    ffplv = ANDROID_LOG_WARN;
  else if (level <= AV_LOG_INFO)
    ffplv = ANDROID_LOG_INFO;
  else if (level <= AV_LOG_VERBOSE)
    ffplv = ANDROID_LOG_VERBOSE;
  else
    ffplv = ANDROID_LOG_DEBUG;

  if (level <= AV_LOG_INFO)
    VLOG(ffplv, FF_LOG_TAG, fmt, vl);
}

static void ffp_log_callback_report(void *ptr, int level, const char *fmt, va_list vl) {
  int ffplv;
  if (level <= AV_LOG_ERROR)
    ffplv = ANDROID_LOG_ERROR;
  else if (level <= AV_LOG_WARNING)
    ffplv = ANDROID_LOG_WARN;
  else if (level <= AV_LOG_INFO)
    ffplv = ANDROID_LOG_INFO;
  else if (level <= AV_LOG_VERBOSE)
    ffplv = ANDROID_LOG_VERBOSE;
  else
    ffplv = ANDROID_LOG_DEBUG;
  va_list vl2;
  char line[1024];
  static int print_prefix = 1;
  va_copy(vl2, vl);
  // av_log_default_callback(ptr, level, fmt, vl);
  av_log_format_line(ptr, level, fmt, vl2, line, sizeof(line), &print_prefix);
  va_end(vl2);
  ALOG(ffplv, FF_LOG_TAG, "%s", line);
}

static void log_callback(void *ptr, int level, const char *fmt, va_list vl) {
  ffp_log_callback_report(ptr, level, fmt, vl);
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
  gJavaVM = vm;
  JNIEnv *env = nullptr;
  vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);
  jclass classMusicplayer = env->FindClass("org/huihui/musicplayer/MusicPlayer");
  gMusicPlayerClass = reinterpret_cast<jclass>(env->NewGlobalRef(classMusicplayer));
  env->RegisterNatives(gMusicPlayerClass, gMethods, NELEM(gMethods));
  av_log_set_callback(log_callback);
  return JNI_VERSION_1_6;
}