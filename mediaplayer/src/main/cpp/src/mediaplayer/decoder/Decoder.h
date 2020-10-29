//
// Created by huihui on 2019/10/5.
//

#ifndef MUSICPLAYER_DECODER_H
#define MUSICPLAYER_DECODER_H

#include <cstdint>
#include "../Player.h"
extern "C" {
#include "libavformat/avformat.h"
};

class Decoder : public Runnable {
 private:
  int32_t streamIndex;
  AVStream *avStream;
  AVCodecContext *codecContext;
  VideoState *videoState;
  Thread DecodeThread;
  bool abortRequest;

 public:
  Decoder(const char *name, int32_t streamIndex,
          AVStream *avStream,
          AVCodecContext *codecContext,
          VideoState *videoState);

  virtual void start() = 0;

  virtual void stop() = 0;

};

#endif //MUSICPLAYER_DECODER_H
