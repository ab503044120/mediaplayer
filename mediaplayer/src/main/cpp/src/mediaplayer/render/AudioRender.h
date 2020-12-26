//
// Created by 听见 on 2020/11/22.
//

#ifndef MUSICPLAYER_AUDIORENDER_H
#define MUSICPLAYER_AUDIORENDER_H

#include <SLES/OpenSLES_Android.h>
#include "IRender.h"
extern "C" {
#include <libavutil/samplefmt.h>

}

#define OPENSLES_BUFFERS 255
#define OPENSLES_BUFLEN 10

struct AudioRenderSpec {
  int freq;                   // 采样率
  AVSampleFormat format;      // 音频采样格式
  uint8_t channels;           // 声道
  uint16_t samples;           // 采样大小
  uint32_t bufferSize;              // 总缓冲区大小
};

class AudioFetchCallback {
 public:
  virtual void fetchAudioData(uint8_t *stream, int len) = 0;
};

class AudioRender : public IRender {

 public:
  volatile bool abort = false;
  AudioRender(PlayerState &playerState, AudioFetchCallback &callback);
  int32_t init();

  int32_t open(AudioRenderSpec &wantSpec, int32_t &bufferSize);

  void stop() override;

  int32_t getLatency();

 private:
  friend void openslesCallback(SLAndroidSimpleBufferQueueItf caller, void *pContext);
 protected:
  static constexpr char *TAG = "AudioRender";

  Mutex mutex;
  Condition cond;

  SLObjectItf slObject = nullptr;
  SLEngineItf slEngine = nullptr;
  SLObjectItf slOutputMixObject = nullptr;

  SLObjectItf slPlayerObject = nullptr;
  SLAndroidSimpleBufferQueueItf slBufferQueueItf = nullptr;
  SLVolumeItf slVolumeItf = nullptr;
  SLPlayItf slPlayItf = nullptr;

  SLDataFormat_PCM format_pcm{};

  uint8_t *playBuffer = nullptr;
  int64_t playBufferSize = 0;

  int32_t bytes_per_frame = 0;
  int32_t milli_per_buffer = 0;
  int32_t frames_per_buffer = 0;
  int32_t bytes_per_buffer = 0;

  uint8_t *queueBuffer = nullptr;
  int64_t queueBufferSize = 0;
  AudioFetchCallback &callback;
  void run() override;
};

#endif //MUSICPLAYER_AUDIORENDER_H
