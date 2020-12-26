//
// Created by huihui on 2019/10/5.
//

#include "Decoder.h"
Decoder::Decoder(const char *name, int32_t streamIndex,
                 AVStream *avStream,
                 AVCodecContext *codecContext,
                 PlayerState *playerState)
    : streamIndex(streamIndex),
      avStream(avStream),
      codecContext(codecContext),
      playerState(playerState), decodeThread(Thread(name, this)) {

}
void Decoder::putPkt(AVPacket &packet, uint32_t serial) {
  pktQ.put(packet, serial);
}
uint32_t Decoder::getPktNB() const {
  return pktQ.nb_packet;
}
uint32_t Decoder::getPktTotalSize() const {
  return pktQ.size;
}

void Decoder::start() {
  decodeThread.start();
}

void Decoder::stop() {
  decodeThread.join();
  avcodec_free_context(&codecContext);
}

