//
// Created by huihui on 2019/9/22.
//

#ifndef MUSICPLAYER_PLAYER_H
#define MUSICPLAYER_PLAYER_H

#include <cstdint>
#include <Thread.h>
#include <Mutex.h>
extern "C" {
#include "libavformat/avformat.h"
}

class VideoState;

class Player : public Runnable {
 private:
  const char *url;
  static const char* TAG;
  AVFormatContext *pFormatCtx;
  AVDictionary *codec_opts;
  Mutex *mMutext;
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
  bool pause;
  bool last_pause;
  bool pause_req;

  bool prepared;

  bool abort_req;

  Thread *readThread;

  int32_t audioStreamIndex = -1;
  int32_t videoStreamIndex = -1;

};
#endif //MUSICPLAYER_PLAYER_H
