//
// Created by huihui on 2019/8/19.
//

#include <pthread.h>
#include "include/Condition.h"

#undef NANOSEC
#define NANOSEC ((uint64_t) 1e9)

Condition::Condition() {
  pthread_condattr_t condattr;
#if defined(__ANDROID_API__) && __ANDROID_API__ < 21
  //do nothing
   pthread_cond_init(&cond, nullptr);
#else
  pthread_condattr_init(&condattr);
  pthread_condattr_setclock(&condattr, CLOCK_MONOTONIC);
  pthread_cond_init(&cond, &condattr);
  pthread_condattr_destroy(&condattr);
#endif

}

Condition::~Condition() {
  pthread_cond_destroy(&cond);
}

void Condition::wait(Mutex *mutex) {
  pthread_cond_wait(&cond, &mutex->mutex);
}

void Condition::wait(int64_t timeout, Mutex *mutex) {
  struct timespec tv{};
  tv.tv_sec = timeout / NANOSEC;
  tv.tv_nsec = timeout % NANOSEC;
//#if defined(__ANDROID_API__) && __ANDROID_API__ < 21
//  /*
// * The bionic pthread implementation doesn't support CLOCK_MONOTONIC,
// * but has this alternative function instead.
// */
//  pthread_cond_timedwait_monotonic_np(&cond,  &mutex->mutex, &tv);
#if defined(HAVE_PTHREAD_COND_TIMEDWAIT_RELATIVE)
    return -pthread_cond_timedwait_relative_np(&cond,&mutex->mutex, &tv);
#else
  pthread_cond_timedwait(&this->cond,  &mutex->mutex, &tv);

#endif
}

void Condition::signal() {
  pthread_cond_signal(&cond);
}
void Condition::broadcast() {
  pthread_cond_broadcast(&cond);
}

