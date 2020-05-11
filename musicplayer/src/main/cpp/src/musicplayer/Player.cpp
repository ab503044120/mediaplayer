//
// Created by huihui on 2019/9/22.
//

#include "Player.h"
#include "utils.h"

void Player::setDataSource(const char *dataSource) {
  url = dataSource;
}
void Player::prepare() {
  autoLock(mMutext);
  videoState->readThread = new Thread("read_thread", this);
  videoState->readThread->start();

}

void Player::start() {
  autoLock(mMutext);
}

void Player::stop() {
  autoLock(mMutext);
}

void Player::pause() {
  autoLock(mMutext);
}

void Player::seek(int64_t msec) {
  autoLock(mMutext);
}

int64_t Player::getPosition() {
  return 0;
}

int64_t Player::getDuration() {
  return 0;
}

void Player::reset() {
  autoLock(mMutext);
}

void Player::release() {
  autoLock(mMutext);

}
Player::Player() {
  mMutext = new Mutex;
  videoState = new VideoState;

}
Player::~Player() {
  SAFE_DELETE(mMutext)
}
void Player::run() {
  pFormatCtx = avformat_alloc_context();
  if (avformat_open_input(&pFormatCtx, url, nullptr, nullptr) != 0) {
    Log_e("avformat_open_input fail");
    return;
  }
  if (avformat_find_stream_info(pFormatCtx, nullptr) < 0) {
    Log_e("avformat_find_stream_info fail");
    return;
  }
  av_dump_format(pFormatCtx, 0, nullptr, 0);
  videoState->audioStreamIndex =
      av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
  if (videoState->audioStreamIndex < 0) {
    Log_e("not find audioStreamIndex");
    return;
  }

}
