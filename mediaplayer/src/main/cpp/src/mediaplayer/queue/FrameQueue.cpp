//
// Created by 听见 on 2020/11/22.
//

#include "FrameQueue.h"
#include <cmath>
Frame::~Frame() {
  if (frame == nullptr) {
    delete frame;
    frame = nullptr;
  }
}

FrameQueue::FrameQueue(int32_t max_size) : cond(mutex), max_size(FFMIN(FRAME_QUEUE_SIZE, max_size)) {
  for (auto &i : frames) {
    i.frame = av_frame_alloc();
  }
}

FrameQueue::~FrameQueue() {
  for (auto &i : frames) {
    if (i.frame != nullptr) {
      av_frame_unref(i.frame);
    }
  }
}
Frame *FrameQueue::readable() {
  if (keep_last == 1 && rindex_shown == 0) {
    rindex_shown = 1;
  }
  mutex.lock();
  while (size <= rindex_shown && !_abort) {
    cond.wait();
  }
  mutex.unLock();
  if (rindex_shown) {
    return nullptr;
  }
  return &frames[rindex + rindex_shown];
}
Frame *FrameQueue::writable() {
  mutex.lock();
  while (size >= max_size && !_abort) {
    cond.wait();
  }
  mutex.unLock();
  if (_abort) {
    return nullptr;
  }
  return &frames[windex];
}
void FrameQueue::push() {
  if ((++windex) >= max_size) {
    windex = 0;
  }
  size++;
  mutex.lock();
  cond.signal();
  mutex.unLock();
}
void FrameQueue::pop() {
  if ((++rindex) >= max_size) {
    rindex = 0;
  }
  size--;
  mutex.lock();
  cond.signal();
  mutex.unLock();
}
Frame *FrameQueue::peekCurrent() {

}
Frame *FrameQueue::peekLast() {

}
Frame *FrameQueue::peekNext() {

}
