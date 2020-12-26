//
// Created by huihui on 2019/10/4.
//

#include "Thread.h"

#include <utility>
void *fnThread(void *arg) {
  auto *thread = reinterpret_cast < Thread * > (arg);
  pthread_setname_np(pthread_self(), thread->name.c_str());
  thread->mutex.lock();
  thread->isRunning = true;
  thread->mutex.unLock();
  if (thread->runnable) {
    thread->runnable->run();
  } else {
    thread->run();
  }
  thread->mutex.lock();
  thread->isRunning = false;
  thread->mutex.unLock();
  return nullptr;
}
volatile int64_t Thread::threadIndex = 0;

Thread::Thread(std::string &&name, Runnable *runnable) : Thread(name, runnable) {}

Thread::Thread(std::string &name, Runnable *runnable) : name(name), runnable(runnable) {
}
Thread::Thread(Runnable *runnable) : Thread(std::string("thread-").append(std::to_string(threadIndex++)), runnable) {

}
void Thread::start() {
  autoLock(mutex);
  if (!isRunning) {
    pthread_create(&tid, nullptr, fnThread, this);
  }

}
int Thread::join() {
  return pthread_join(tid, nullptr);
}
void Thread::detach() {
  pthread_detach(tid);
}



