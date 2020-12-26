//
// Created by 听见 on 2020/11/20.
//

#include <Player.h>
#include "VideoDecoder.h"

VideoDecoder::VideoDecoder(int32_t streamIndex,
                           AVStream *avStream,
                           AVCodecContext *codecContext,
                           PlayerState *playerState) : Decoder("videoDecoder", streamIndex, avStream, codecContext, playerState) {

}
void VideoDecoder::run() {
  int32_t ret;
  AVPacket pkt;
  av_init_packet(&pkt);
  AVFrame *frame = av_frame_alloc();
  uint32_t serial = 0;
  while (!playerState->abort_req) {
    ret = 0;
    while (ret == 0 && !playerState->abort_req) {
      LOGE(TAG, "%s", " start take");
      ret =pktQ.take(pkt, &serial);
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
    } else if (ret == AVERROR(EAGAIN)) {
      LOGE(TAG, "%s ", "try again");
    } else if (ret == AVERROR(EINVAL)) {
      LOGE(TAG, "%s ", "codec not opened, or it is an encoder");
    } else if (ret == AVERROR_EOF) {
      avcodec_flush_buffers(codecContext);
    }

  }
  av_packet_unref(&pkt);
}
void VideoDecoder::stop() {

}