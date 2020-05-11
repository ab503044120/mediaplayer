//
// Created by huihui on 2019/10/4.
//

#include "Thread.h"
void *Thread::fnThread(void *arg) {
  auto *thread = reinterpret_cast < Thread * > (arg);
  pthread_setname_np(pthread_self(), thread->name);
  if (thread->runnable) {
    thread->runnable->run();
  } else {
    thread->run();
  }
  return 0;
}
Thread::Thread(const char *name, Runnable *runnable)
    : name(name), runnable(runnable) {}

int Thread::join() {
  return pthread_join(tid, nullptr);
}
void Thread::detach() {
  pthread_detach(tid);
}

void Thread::start() {
  pthread_create(&tid, nullptr, fnThread, this);
}
