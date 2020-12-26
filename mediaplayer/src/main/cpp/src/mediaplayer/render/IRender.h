//
// Created by 听见 on 2020/11/22.
//

#ifndef MUSICPLAYER_IRENDER_H
#define MUSICPLAYER_IRENDER_H

#include <Thread.h>
extern "C" {
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
}
class PlayerState;

class IRender : public Runnable {
 protected:
  Thread thread;
  PlayerState &playerState;
 public:
  IRender(const char *name, PlayerState &playerState);
 public:
  virtual void start();

  virtual void stop() = 0;
};

#endif //MUSICPLAYER_IRENDER_H
