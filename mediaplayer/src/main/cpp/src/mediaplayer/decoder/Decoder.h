//
// Created by huihui on 2019/10/5.
//

#ifndef MUSICPLAYER_DECODER_H
#define MUSICPLAYER_DECODER_H

#include <cstdint>
#include <queue/FrameQueue.h>
#include "Thread.h"
extern "C" {
#include "libavformat/avformat.h"
}

class VideoState;

class Decoder : public Runnable {
 protected:
  int32_t streamIndex;
  AVStream *avStream;
  AVCodecContext *codecContext;
  VideoState *videoState;
  Thread decodeThread;
  bool abortRequest = false;
 public:
  Decoder(const char *name, int32_t streamIndex,
          AVStream *avStream,
          AVCodecContext *codecContext,
          VideoState *videoState);

  void run() override;

  virtual void start();

  virtual void stop();

};

#endif //MUSICPLAYER_DECODER_H
