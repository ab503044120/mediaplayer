//
// Created by 听见 on 2020/11/22.
//

#ifndef MUSICPLAYER_FRAMEQUEUE_H
#define MUSICPLAYER_FRAMEQUEUE_H
#include "PacketQueue.h"
extern "C" {
#include <libavutil/frame.h>
#include <libavcodec/avcodec.h>
}
#define FRAME_QUEUE_SIZE 3

class Frame {
 public:
  AVFrame *frame = nullptr;
  AVSubtitle sub{};
  int serial = 0;
  double pts = 0;           /* presentation timestamp for the frame */
  double duration = 0;      /* estimated duration of the frame */
  int64_t pos = 0;          /* byte position of the frame in the input file */
  int width = 0;
  int height = 0;
  int format = 0;
  AVRational sar{};
  virtual ~Frame();

};

class FrameQueue {
  Frame frames[FRAME_QUEUE_SIZE]{};
  int32_t rindex = 0;
  int32_t windex = 0;
  int32_t size = 0;
  int32_t max_size = 0;
  int32_t keep_last = 0;
  int32_t rindex_shown = 0;
  bool _abort = false;
  Mutex mutex;
  Condition cond;
 public:
  FrameQueue(int32_t max_size = FRAME_QUEUE_SIZE);
  virtual ~FrameQueue();

  void push();

  void pop();

  Frame *peekCurrent();

  Frame *peekLast();

  Frame *peekNext();

  Frame *readable();

  Frame *writable();
};

#endif //MUSICPLAYER_FRAMEQUEUE_H
