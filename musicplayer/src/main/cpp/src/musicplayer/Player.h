//
// Created by huihui on 2019/9/22.
//

#ifndef MUSICPLAYER_PLAYER_H
#define MUSICPLAYER_PLAYER_H

#include <cstdint>
extern "C" {
#include "libavformat/avformat.h"
}

class Player {
 private:
  const char *url;
  AVFormatContext *pFormatCtx;

 public:
  void setDataSource(const char *dataSource);

  void prepare();

  void start();

  void stop();

  void pause();

  void seek(int64_t msec);

  int64_t getPosition();

  int64_t getDuration();

  void reset();

  void release();

};

#endif //MUSICPLAYER_PLAYER_H
