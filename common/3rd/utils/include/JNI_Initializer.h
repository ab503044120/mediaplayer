//
// Created by 听见 on 2020/10/25.
//

#ifndef JNI_INITIALIZER_H
#define JNI_INITIALIZER_H
#include <jni.h>
#include <list>
typedef void (*InitFun)(JavaVM *, void *);

extern JavaVM *gJavaVM;

int registInitFun(InitFun fun);

#endif