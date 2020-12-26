//
// Created by 听见 on 2020/11/22.
//

#include "IRender.h"
void IRender::start() {
  thread.start();
}
IRender::IRender(const char *name, PlayerState &playerState) : playerState(playerState), thread(Thread(name, this)) {}
