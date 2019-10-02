//
// Created by huihui on 2019/9/22.
//

#include "Player.h"
#include "utils.h"
void Player::setDataSource(const char *dataSource) {
  url = dataSource;
}

void Player::prepare() {
  if (avformat_open_input(&pFormatCtx, url, nullptr, nullptr) != 0) {
    Log_e("avformat_open_input fail");
    return;
  }
  if (avformat_find_stream_info(pFormatCtx, nullptr) < 0) {
    Log_e("avformat_find_stream_info fail");
    return;
  }
  av_dump_format(pFormatCtx, 0, nullptr, 0);
}

void Player::start() {

}

void Player::stop() {

}

void Player::pause() {

}

void Player::seek(int64_t msec) {

}

int64_t Player::getPosition() {
  return 0;
}

int64_t Player::getDuration() {
  return 0;
}

void Player::reset() {

}

void Player::release() {

}
