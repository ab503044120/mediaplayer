//
// Created by huihui on 2019/8/19.
//

#include "include/Mutex.h"
#include "include/utils.h"

Mutex::Mutex() {
  pthread_mutex_init(&mutex, nullptr);

}
Mutex::~Mutex() {
  pthread_mutex_destroy(&mutex);
}
void Mutex::lock() {
  pthread_mutex_lock(&mutex);
}
void Mutex::unLock() {
  pthread_mutex_unlock(&mutex);
}
Mutex::Mutex(const char *name) : name(name) {
  pthread_mutex_init(&mutex, nullptr);
}

