//
// Created by 听见 on 2020/9/29.
//

#include <Player.h>
#include "AudioDecoder.h"
AudioDecoder::AudioDecoder(int32_t streamIndex,
                           AVStream *avStream,
                           AVCodecContext *codecContext,
                           VideoState *videoState) : Decoder("audioDecoder", streamIndex, avStream, codecContext, videoState) {}
void AudioDecoder::run() {
  int32_t ret = 0;
  AVPacket pkt;
  av_init_packet(&pkt);
  AVFrame *frame = av_frame_alloc();
  uint32_t serial = 0;
  while (!videoState->abort_req) {
    ret = 0;
    while (ret == 0 && !videoState->abort_req) {
      LOGE(TAG, "%s", " start take");
      ret = videoState->audioq.take(pkt, &serial);
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
      Frame *wf = nullptr;
      wf = videoState->audioFq.writable();
      if (wf) {
        av_frame_move_ref(wf->frame, frame);
        videoState->audioFq.push();
      }
    } else if (ret == AVERROR_EOF) {
      avcodec_flush_buffers(codecContext);

    }

  }

  av_packet_unref(&pkt);
}

void AudioDecoder::stop() {

}
