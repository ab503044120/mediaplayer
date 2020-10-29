//
// Created by huihui on 2019/9/22.
//

#include "Player.h"
#include "utils.h"
const char *Player::TAG = "Player";

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

void printError(const char *preFix, int err) {
  char errbuf[128];
  const char *errbuf_ptr = errbuf;

  if (av_strerror(err, errbuf, sizeof(errbuf)) < 0) {
    errbuf_ptr = strerror(AVUNERROR(err));
  }
  LOGE("%s %s", preFix, errbuf_ptr);
}

void Player::run() {
  pFormatCtx = avformat_alloc_context();
  int ret = 0;
  if ((ret = avformat_open_input(&pFormatCtx, url, nullptr, nullptr)) < 0) {
    printError("avformat_open_input", ret);
    return;
  }
  if ((ret = avformat_find_stream_info(pFormatCtx, nullptr)) < 0) {
    printError("avformat_find_stream_info", ret);
    return;
  }

  av_dump_format(pFormatCtx, 0, nullptr, 0);

  videoState->audioStreamIndex =
      av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
  videoState->videoStreamIndex =
      av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
  if (videoState->audioStreamIndex < 0 && videoState->videoStreamIndex < 0) {
    LOGE(TAG, "%s", "not find audioStreamIndex and videoStreamIndex");
    return;
  }
  if (videoState->audioStreamIndex > 0) {
    streamComponentOpen(videoState->audioStreamIndex);
  }
  if (videoState->videoStreamIndex > 0) {
    streamComponentOpen(videoState->videoStreamIndex);
  }

}
int32_t Player::streamComponentOpen(int stream_index) {
  AVCodecContext *avctx;
  AVCodec *codec;
  AVDictionary *opts = NULL;
  AVDictionaryEntry *t = NULL;
  int32_t sample_rate = AVMEDIA_TYPE_UNKNOWN, nb_channels;
  int64_t channel_layout;
  int32_t ret = 0;
  int32_t stream_lowres = 0;

  avctx = avcodec_alloc_context3(NULL);
  if (!avctx)
    return AVERROR(ENOMEM);
  ret = avcodec_parameters_to_context(avctx, pFormatCtx->streams[stream_index]->codecpar);
  if (ret < 0) {
    goto fail;
  }
  avctx->pkt_timebase = pFormatCtx->streams[stream_index]->time_base;
  codec = avcodec_find_decoder(avctx->codec_id);
  if (!codec) {
    ret = AVERROR(EINVAL);
    goto fail;
  }
  avctx->codec_id = codec->id;
  if (stream_lowres > codec->max_lowres) {
    av_log(avctx, AV_LOG_WARNING, "The maximum value for lowres supported by the decoder is %d\n",
           codec->max_lowres);
    stream_lowres = codec->max_lowres;
  }
  avctx->lowres = stream_lowres;

  if ((ret = avcodec_open2(avctx, codec, &opts)) < 0) {
    goto fail;
  }

  pFormatCtx->streams[stream_index]->discard = AVDISCARD_DEFAULT;
  switch (avctx->codec_type) {
    case AVMEDIA_TYPE_AUDIO:sample_rate = avctx->sample_rate;
      nb_channels = avctx->channels;
      channel_layout = avctx->channel_layout;
      break;
    case AVMEDIA_TYPE_VIDEO:

      break;
    default :break;
  }

  fail:
  avcodec_free_context(&avctx);
  return 0;
}
