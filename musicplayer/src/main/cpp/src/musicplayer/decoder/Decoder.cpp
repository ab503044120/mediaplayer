//
// Created by huihui on 2019/10/5.
//

#include "Decoder.h"
Decoder::Decoder(int32_t streamIndex,
                 AVStream *avStream,
                 AVCodecContext *codecContext,
                 VideoState *videoState)
    : streamIndex(streamIndex),
      avStream(avStream),
      codecContext(codecContext),
      videoState(videoState) {}
