//
// Created by 听见 on 2020/9/29.
//

#ifndef MUSICPLAYER_AUDIODECODER_H
#define MUSICPLAYER_AUDIODECODER_H
#include "Decoder.h"
class AudioDecoder : public Decoder {
  const char *TAG = "AudioDecoder";
 public:
  AudioDecoder(int32_t streamIndex, AVStream *avStream, AVCodecContext *codecContext, VideoState *videoState);

  void run() override;

  void stop() override;

};

#endif //MUSICPLAYER_AUDIODECODER_H
