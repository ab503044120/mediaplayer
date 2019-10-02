//
// Created by huihui on 2019/8/21.
//
#include "android/log.h"
#ifndef ANDROIDUTILS_H
#define ANDROIDUTILS_H
#define TAG "cLog"

#define  Log_i(...)  __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
#define  Log_e(...)  __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

#define VLOG(level, LTAG,...)    ((void)__android_log_vprint(level, LTAG, __VA_ARGS__))
#define VLOGV(...)  VLOG(ANDROID_LOG_VERBOSE,   __VA_ARGS__)
#define VLOGD(...)  VLOG(ANDROID_LOG_DEBUG,     __VA_ARGS__)
#define VLOGI(...)  VLOG(ANDROID_LOG_INFO,      __VA_ARGS__)
#define VLOGW(...)  VLOG(ANDROID_LOG_WARN,      __VA_ARGS__)
#define VLOGE(...)  VLOG(ANDROID_LOG_ERROR,     __VA_ARGS__)

#define ALOG(level,LTAG, ...)    ((void)__android_log_print(level, LTAG, __VA_ARGS__))
#define ALOGV(...)  ALOG(ANDROID_LOG_VERBOSE,   __VA_ARGS__)
#define ALOGD(...)  ALOG(ANDROID_LOG_DEBUG,     __VA_ARGS__)
#define ALOGI(...)  ALOG(ANDROID_LOG_INFO,      __VA_ARGS__)
#define ALOGW(...)  ALOG(ANDROID_LOG_WARN,      __VA_ARGS__)
#define ALOGE(...)  ALOG(ANDROID_LOG_ERROR,     __VA_ARGS__)

#define LOGE(format, ...)  __android_log_print(ANDROID_LOG_ERROR, format, ##__VA_ARGS__)
#define LOGI(format, ...)  __android_log_print(ANDROID_LOG_INFO,  format, ##__VA_ARGS__)

#endif
