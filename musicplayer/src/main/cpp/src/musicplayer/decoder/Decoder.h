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

class Decoder {
 private:
  int32_t streamIndex;
  AVStream *avStream;
  AVCodecContext *codecContext;
  VideoState *videoState;

  bool abortRequest;

 public:
  Decoder(int32_t streamIndex,
          AVStream *avStream,
          AVCodecContext *codecContext,
          VideoState *videoState);

  void start();

  void stop();

};

#endif //MUSICPLAYER_DECODER_H
