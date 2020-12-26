//
// Created by huihui on 2019/9/22.
//

#ifndef MUSICPLAYER_PLAYER_H
#define MUSICPLAYER_PLAYER_H

#include <cstdint>
#include <Thread.h>
#include <Mutex.h>
#include <queue/PacketQueue.h>
#include <decoder/AudioDecoder.h>
#include <queue/FrameQueue.h>
#include <render/AudioRender.h>
#include <render/VideoRender.h>
extern "C" {
#include "libavformat/avformat.h"
}
struct AudioRenderSpec;

class PlayerState;

class Player : public Runnable, public AudioFetchCallback {
 private:
  constexpr static const int next_nb_channels[8] = {0, 0, 1, 6, 2, 6, 4, 6};
  constexpr static const int next_sample_rates[8] = {0, 44100, 48000};

  const char *url = nullptr;
  static constexpr const char *TAG = "Player";
  AVFormatContext *pFormatCtx;
  AVDictionary *codec_opts;
  Mutex mutex;
  PlayerState *playerState;
  Thread *readThread = nullptr;
  int32_t audioStreamIndex = -1;
  AudioDecoder *audioDecoder = nullptr;
  AudioRender *audioRender = nullptr;
  int32_t videoStreamIndex = -1;
  Decoder *videoDecoder = nullptr;
  VideoRender *videoRender = nullptr;

 public:

  Player();

  virtual ~Player();

  void setDataSource(const char *dataSource);

  int32_t streamComponentOpen(int stream_index);

  int32_t openAudioRender(int64_t wanted_channel_layout,
                          int32_t wanted_nb_channels,
                          int32_t wanted_sample_rate,
                          AudioParam &param);

  void fetchAudioData(uint8_t *stream, int len) override;

  void prepare();

  void start();

  void stop();

  void pause();

  void seek(int64_t msec);

  int64_t getPosition();

  int64_t getDuration();

  void reset();

  void release();

  void run() override;

};

class PlayerState {
 public:
  bool pause = false;
  bool last_pause = false;
  bool pause_req = false;

  bool prepared = false;

  volatile bool abort_req = false;

};

#endif //MUSICPLAYER_PLAYER_H
