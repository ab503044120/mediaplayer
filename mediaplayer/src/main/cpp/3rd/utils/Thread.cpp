//
// Created by huihui on 2019/10/4.
//

#include "Thread.h"
void *fnThread(void *arg) {
  auto *thread = reinterpret_cast < Thread * > (arg);
  pthread_setname_np(pthread_self(), thread->name);
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

Thread::Thread(const char *name, Runnable *runnable)
    : mutex(Mutex()), cond(mutex), name(name), runnable(runnable) {}

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


