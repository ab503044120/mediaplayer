//
// Created by 听见 on 2020/9/29.
//

#ifndef MUSICPLAYER_AUDIODECODER_H
#define MUSICPLAYER_AUDIODECODER_H
#include "Decoder.h"
class Resampler {
 private:
  SwrContext *swr_ctx;

 public:

  void init();

  void destory();

};
struct AudioParam {
  int freq;
  int channels;
  int64_t channel_layout;
  enum AVSampleFormat fmt;
  int frame_size;
  int bytes_per_sec;
};

class AudioDecoder : public Decoder {
 private:
  const char *TAG = "AudioDecoder";
  AudioParam targetParam;
  AudioParam curParam;
  Resampler resampler{};
 public:
  AudioDecoder(int32_t streamIndex,
               AVStream *avStream,
               AVCodecContext *codecContext,
               PlayerState *playerState,
               AudioParam &param);

  void fetchData(uint8_t *outBuffer, int32_t bufferSize);

  void run() override;

  void stop() override;

};

#endif //MUSICPLAYER_AUDIODECODER_H
