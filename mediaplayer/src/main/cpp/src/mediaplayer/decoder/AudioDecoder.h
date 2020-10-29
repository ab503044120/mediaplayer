//
// Created by 听见 on 2020/9/29.
//

#ifndef MUSICPLAYER_AUDIODECODER_H
#define MUSICPLAYER_AUDIODECODER_H
#include "Decoder.h"
class AudioDecoder : public Decoder {
 protected:
  void run() override;
 public:
  void start() override;
  void stop() override;

};

#endif //MUSICPLAYER_AUDIODECODER_H
