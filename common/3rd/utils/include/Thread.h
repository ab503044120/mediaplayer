//
// Created by huihui on 2019/10/4.
//

#ifndef MUSICPLAYER_THREAD_H
#define MUSICPLAYER_THREAD_H

#include <cstdint>
#include <pthread.h>
#include "Mutex.h"
#include "Condition.h"

class Runnable {
 protected:
  friend void *fnThread(void *arg);
  virtual void run() = 0;
};

class Thread : public Runnable {

 private:
  static volatile int64_t threadIndex;
  std::string name;
  Mutex mutex;
  Condition cond;
  bool isRunning = false;
  pthread_t tid{};

  Runnable *runnable;

  friend void *fnThread(void *arg);

 public:
  Thread(std::string& name , Runnable *runnable = nullptr);
  Thread(std::string&& name , Runnable *runnable = nullptr);
  Thread( Runnable *runnable = nullptr);

  void run() override {}

  void start();

  int join();

  void detach();

};

#endif //MUSICPLAYER_THREAD_H
