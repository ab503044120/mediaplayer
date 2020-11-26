//
// Created by huihui on 2019/9/22.
//

#ifndef MUSICPLAYER_PLAYER_H
#define MUSICPLAYER_PLAYER_H

#include <cstdint>
#include <Thread.h>
#include <Mutex.h>
#include <queue/PacketQueue.h>
#include <decoder/Decoder.h>
#include <queue/FrameQueue.h>
extern "C" {
#include "libavformat/avformat.h"
}

class VideoState;

class Player : public Runnable {
 private:
  const char *url;
  static const char *TAG;
  AVFormatContext *pFormatCtx;
  AVDictionary *codec_opts;
  Mutex mutex;
  VideoState *videoState;

 public:
  Player();

  virtual ~Player();

  void setDataSource(const char *dataSource);

  int32_t streamComponentOpen(int stream_index);

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

 private:

};

class VideoState {
 public:
  bool pause = false;
  bool last_pause = false;
  bool pause_req = false;

  bool prepared = false;

  volatile bool abort_req = false;

  Thread *readThread = nullptr;

  int32_t audioStreamIndex = -1;
  PacketQueue audioq;
  FrameQueue audioFq;
  Decoder *audioDecoder = nullptr;

  int32_t videoStreamIndex = -1;
  PacketQueue videoq;
  FrameQueue videoFq;
  Decoder *videoDecoder = nullptr;

};
#endif //MUSICPLAYER_PLAYER_H
