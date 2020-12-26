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
#include "libswresample/swresample.h"
}

class PlayerState;

class Decoder : public Runnable {
 protected:
  int32_t streamIndex;
  AVStream *avStream;
  AVCodecContext *codecContext;
  PacketQueue pktQ;
  FrameQueue frameQ;
  PlayerState *playerState;
  Thread decodeThread;
  bool abortRequest = false;
 public:
  Decoder(const char *name, int32_t streamIndex,
          AVStream *avStream,
          AVCodecContext *codecContext,
          PlayerState *playerState);

  void putPkt(AVPacket &packet, uint32_t serial);

  uint32_t getPktNB() const;

  uint32_t getPktTotalSize() const;

  virtual void start();

  virtual void stop();

};

#endif //MUSICPLAYER_DECODER_H
