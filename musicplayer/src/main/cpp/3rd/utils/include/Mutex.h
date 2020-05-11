//
// Created by huihui on 2019/8/19.
//
#include <pthread.h>
#include "utils.h"
#ifndef MUTEX_H
#define MUTEX_H

#define TAG "mutex"

#define mutex_debug 1

#define pthread_log1(mutex)  if(mutex_debug) Log_e("%s Lock1: %ld,%s,%d",mutex, pthread_self(), __FUNCTION__, __LINE__)
#define pthread_log2(mutex)  if(mutex_debug) Log_e("%s Lock2: %ld,%s,%d",mutex, pthread_self(), __FUNCTION__, __LINE__)
#define pthread_log3(mutex)  if(mutex_debug) Log_e("%s UnLock: %ld,%s,%d",mutex,pthread_self(), __FUNCTION__, __LINE__)

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

class AutoLock {
 public:
  inline AutoLock(Mutex &mutex) : mLock(mutex) { mLock.lock(); }
  inline AutoLock(Mutex *mutex) : mLock(*mutex) { mLock.lock(); }
  inline ~AutoLock() { mLock.unLock(); }
 private:
  Mutex &mLock;
};

#define autoLock(mutex) AutoLock _l(mutex)

#endif //MUTEX_H
