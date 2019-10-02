//
// Created by huihui on 2019/8/19.
//
#include <pthread.h>
#include "utils.h"
#ifndef MUTEX_H
#define MUTEX_H

#define TAG "mutex"

#define debug true

#define pthread_log1(mutex)  if(debug) Log_e("%s Lock1: %ld,%s,%d",mutex, pthread_self(), __FUNCTION__, __LINE__)
#define pthread_log2(mutex)  if(debug) Log_e("%s Lock2: %ld,%s,%d",mutex, pthread_self(), __FUNCTION__, __LINE__)
#define pthread_log3(mutex)  if(debug) Log_e("%s UnLock: %ld,%s,%d",mutex,pthread_self(), __FUNCTION__, __LINE__)

#define pthreadLock(mutex)\
{\
  pthread_log1(mutex->name);\
  mutex->lock();\
  pthread_log2(mutex->name);\
}

#define pthreadUnLock(mutex)\
{\
  mutex->unLock();\
  pthread_log3(mutex->name);\
}

class Mutex {
 public:
  const char *name = "";
  pthread_mutex_t mutex;
  void lock();
  void unLock();
  Mutex();
  virtual ~Mutex();
  Mutex(const char *name);
};

#endif //MUTEX_H
