//
// Created by 听见 on 2020/11/20.
//

#ifndef MUSICPLAYER_VIDEODECODER_H
#define MUSICPLAYER_VIDEODECODER_H

#include "Decoder.h"

class VideoDecoder: public Decoder {
  const char *TAG = "VideoDecoder";
 public:
  VideoDecoder(int32_t streamIndex, AVStream *avStream, AVCodecContext *codecContext, PlayerState *playerState);

  void run() override;

  void stop() override;

};

#endif //MUSICPLAYER_VIDEODECODER_H
