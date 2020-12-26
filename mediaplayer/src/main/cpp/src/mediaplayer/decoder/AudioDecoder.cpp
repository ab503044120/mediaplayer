//
// Created by 听见 on 2020/9/29.
//

#include <Player.h>
#include "AudioDecoder.h"
AudioDecoder::AudioDecoder(int32_t streamIndex,
                           AVStream *avStream,
                           AVCodecContext *codecContext,
                           PlayerState *playerState,
                           AudioParam &param)
    : Decoder("audioDecoder", streamIndex, avStream, codecContext, playerState), targetParam(param), curParam(param) {}

void AudioDecoder::fetchData(uint8_t *outBuffer, int32_t bufferSize) {

}

void AudioDecoder::run() {
  int32_t ret;
  AVPacket pkt;
  av_init_packet(&pkt);
  AVFrame *frame = av_frame_alloc();
  uint32_t serial = 0;
  while (!playerState->abort_req) {
    ret = 0;
    while (ret == 0 && !playerState->abort_req) {
      LOGE(TAG, "%s", " start take");
      ret = pktQ.take(pkt, &serial);
    }
    if (ret < 0) {
      return;
    }
    LOGE(TAG, "%s %d", "take", ret);
    ret = avcodec_send_packet(codecContext, &pkt);
    LOGE(TAG, "%s %d", "avcodec_send_packet", ret);
    if (ret == 0) {
      av_packet_unref(&pkt);
    }
    ret = avcodec_receive_frame(codecContext, frame);
    LOGE(TAG, "%s %d", "avcodec_receive_frame", ret);
    if (ret == 0) {
      Frame *wf;
      wf = frameQ.writable();
      if (wf) {
        av_frame_move_ref(wf->frame, frame);
        frameQ.push();
      }
    } else if (ret == AVERROR_EOF) {
      avcodec_flush_buffers(codecContext);

    }

  }
  av_packet_unref(&pkt);
}

void AudioDecoder::stop() {

}

