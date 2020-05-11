//
// Created by huihui on 2019/8/19.
//
#include <pthread.h>
#include "Mutex.h"
#ifndef CONDITION_H
#define CONDITION_H

class Condition {
 private:
  pthread_cond_t cond;
 public:
  void wait(Mutex *mutex);
  void wait(int64_t time, Mutex *);
  void signal();
  void broadcast();
  Condition();
  virtual ~Condition();
};

#endif //QLAUNCHER_CONDITION_H
