//
// Created by huihui on 2019/10/5.
//

#include <Player.h>
#include "Decoder.h"
Decoder::Decoder(const char *name, int32_t streamIndex,
                 AVStream *avStream,
                 AVCodecContext *codecContext,
                 VideoState *videoState)
    : streamIndex(streamIndex),
      avStream(avStream),
      codecContext(codecContext),
      videoState(videoState), decodeThread(Thread(name, this)) {

}
void Decoder::start() {
  decodeThread.start();
}

void Decoder::stop() {
  decodeThread.join();
  avcodec_free_context(&codecContext);
}
void Decoder::run() {


}

