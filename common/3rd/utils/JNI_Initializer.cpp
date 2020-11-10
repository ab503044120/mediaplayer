//
// Created by 听见 on 2020/10/25.
//
#include "JNI_Initializer.h"

JavaVM *gJavaVM = nullptr;

static std::list<InitFun> *initList;

int registInitFun(InitFun fun) {
  if (initList == nullptr) {
    initList = new std::list<InitFun>();
  }
  initList->push_back(fun);
  return 1;
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
  gJavaVM = vm;
  auto begin = initList->begin();
  while (begin != initList->end()) {
    (**begin)(vm, reserved);
    begin++;
  }
  return JNI_VERSION_1_6;
}