//
// Created by huihui on 2019/10/4.
//

#ifndef MUSICPLAYER_THREAD_H
#define MUSICPLAYER_THREAD_H

#include <cstdint>
#include <pthread.h>

class Runnable {
 public:
  virtual void run() = 0;
};

class Thread : public Runnable {

 private:
  const char *name;

  Runnable *runnable;

  pthread_t tid;

  static void *fnThread(void *arg);

 public:
  Thread(const char *name, Runnable *runnable);

  void run() override {}

  void start();

  int join();

  void detach();

};

#endif //MUSICPLAYER_THREAD_H
