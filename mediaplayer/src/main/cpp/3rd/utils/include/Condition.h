//
// Created by huihui on 2019/8/19.
//
#include <pthread.h>
#include "Mutex.h"
#ifndef CONDITION_H
#define CONDITION_H

class Condition {
 private:
  pthread_cond_t cond{};
  Mutex &mutex;
 public:
  void wait();
  void wait(int64_t time);
  void signal();
  void broadcast();
  Condition(Mutex &mutex);
  virtual ~Condition();
};

#endif //QLAUNCHER_CONDITION_H
