//
// Created by 听见 on 2020/11/22.
//

#ifndef MUSICPLAYER_VIDEORENDER_H
#define MUSICPLAYER_VIDEORENDER_H

#include "IRender.h"
class VideoRender : public IRender {
 public:
  VideoRender(const char *name, PlayerState &playerState);
};

#endif //MUSICPLAYER_VIDEORENDER_H
