//
// Created by huihui on 2019/8/19.
//

#include <pthread.h>
#include "include/Condition.h"
#include <time.h>
#undef NANOSEC
#define NANOSEC ((uint64_t) 1e9)

Condition::Condition(Mutex &mutex) : mutex(mutex) {
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

void Condition::wait() {
  pthread_cond_wait(&cond, &mutex.mutex);
}

void Condition::wait(int64_t reltime) {
#if defined(HAVE_PTHREAD_COND_TIMEDWAIT_RELATIVE)
  struct timespec ts;
    ts.tv_sec  = reltime/1000000000;
    ts.tv_nsec = reltime%1000000000;
    pthread_cond_timedwait_relative_np(&cond, &mutex->mutex, &ts);
#else // HAVE_PTHREAD_COND_TIMEDWAIT_RELATIVE

  struct timespec ts;
#if defined(__linux__)
  clock_gettime(CLOCK_MONOTONIC, &ts);
#else // __APPLE__
  // we don't support the clocks here.
    struct timeval t;
    gettimeofday(&t, NULL);
    ts.tv_sec = t.tv_sec;
    ts.tv_nsec= t.tv_usec*1000;
#endif
  ts.tv_sec += reltime / 1000000000;
  ts.tv_nsec += reltime % 1000000000;
  if (ts.tv_nsec >= 1000000000) {
    ts.tv_nsec -= 1000000000;
    ts.tv_sec += 1;
  }
  pthread_cond_timedwait(&cond, &mutex.mutex, &ts);
#endif // HAVE_PTHREAD_COND_TIMEDWAIT_RELATIVE
}

void Condition::signal() {
  pthread_cond_signal(&cond);
}
void Condition::broadcast() {
  pthread_cond_broadcast(&cond);
}

