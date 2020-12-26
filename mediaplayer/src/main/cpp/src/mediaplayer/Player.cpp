//
// Created by huihui on 2019/9/22.
//

#include <decoder/AudioDecoder.h>
#include <decoder/VideoDecoder.h>
#include <unistd.h>
#include "Player.h"
#include "utils.h"
#include <netinet/ip_icmp.h>
#include <netinet/icmp6.h>
void Player::setDataSource(const char *dataSource) {
  url = dataSource;
}

void Player::prepare() {
  autoLock(mutex);
  audioRender = new AudioRender(*playerState, *this);
  audioRender->init();
  readThread = new Thread("read_thread", this);
  readThread->start();
}

void Player::start() {
  autoLock(mutex);
}

void Player::stop() {
  autoLock(mutex);
}

void Player::pause() {
  autoLock(mutex);
}

void Player::seek(int64_t msec) {
  autoLock(mutex);
}

int64_t Player::getPosition() {
  return 0;
}

int64_t Player::getDuration() {
  return 0;
}

void Player::reset() {
  autoLock(mutex);
}

void Player::release() {
  autoLock(mutex);

}
Player::Player() {
  playerState = new PlayerState;

}
Player::~Player() {
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
  int ret;
  if ((ret = avformat_open_input(&pFormatCtx, url, nullptr, nullptr)) < 0) {
    printError("avformat_open_input", ret);
    return;
  }
  if ((ret = avformat_find_stream_info(pFormatCtx, nullptr)) < 0) {
    printError("avformat_find_stream_info", ret);
    return;
  }

  av_dump_format(pFormatCtx, 0, nullptr, 0);

  audioStreamIndex =
      av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
  videoStreamIndex =
      av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
  if (audioStreamIndex < 0 && videoStreamIndex < 0) {
    LOGE(TAG, "%s", "not find audioStreamIndex and videoStreamIndex");
    return;
  }
  if (audioStreamIndex >= 0) {
    streamComponentOpen(audioStreamIndex);
  }
  if (videoStreamIndex >= 0) {
    streamComponentOpen(videoStreamIndex);
  }
  AVPacket pkt;
  while (true) {
    if (playerState->abort_req) {
      return;
    }
    if (audioDecoder->getPktTotalSize() + videoDecoder->getPktTotalSize() > 10 * 1000 * 1000 ||
        (audioDecoder->getPktNB() > 5000 && audioDecoder->getPktNB() > 5000)) {
      LOGE(TAG, "%s", "can be slow ");
      usleep(10 * 1000);
      continue;
    }

    ret = av_read_frame(pFormatCtx, &pkt);
    LOGE(TAG, "%s %d", " av_read_frame", ret);

    if (ret == 0) {
      if (pkt.stream_index == videoStreamIndex) {
        videoDecoder->putPkt(pkt, 0);
//        LOGE(TAG, "%s %d %d", " videoq", playerState->videoq.nb_packet, playerState->videoq.maxPackNb);
      } else if (pkt.stream_index == audioStreamIndex) {
        audioDecoder->putPkt(pkt, 0);
//        LOGE(TAG, "%s %d %d", " audioq", playerState->videoq.nb_packet, playerState->videoq.maxPackNb);
      } else {
        LOGE(TAG, "%s %d", "other", pkt.stream_index);
        av_packet_unref(&pkt);
      }
    } else if (ret == AVERROR_EOF) {
      break;
    } else {
      break;
    }
  }
}
int32_t Player::streamComponentOpen(int stream_index) {
  AVCodecContext *avctx;
  AVCodec *codec;
  AVDictionary *opts = nullptr;
  AVDictionaryEntry *t;
  int32_t sample_rate, nb_channels;
  int64_t channel_layout;
  int32_t ret;
  int32_t stream_lowres = 0;

  avctx = avcodec_alloc_context3(nullptr);
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
    case AVMEDIA_TYPE_AUDIO: {
      sample_rate = avctx->sample_rate;
      nb_channels = avctx->channels;
      channel_layout = avctx->channel_layout;
      AudioParam audioParam{};
      openAudioRender(channel_layout, nb_channels, sample_rate, audioParam);

      audioDecoder = new AudioDecoder(stream_index, pFormatCtx->streams[stream_index],
                                      avctx, playerState, audioParam);
      audioDecoder->start();
      break;
    }
    case AVMEDIA_TYPE_VIDEO:
      videoDecoder = new VideoDecoder(stream_index, pFormatCtx->streams[stream_index],
                                      avctx, playerState);
      videoDecoder->start();
      break;
    default :break;
  }
  goto out;

  fail:
  avcodec_free_context(&avctx);
  out:
  return ret;
}
int32_t Player::openAudioRender(int64_t wanted_channel_layout,
                                int32_t wanted_nb_channels,
                                int32_t wanted_sample_rate,
                                AudioParam &param) {

  int32_t bufferSize = 0;
  int next_sample_rate_idx = FF_ARRAY_ELEMS(next_sample_rates) - 1;

  if (!wanted_channel_layout || wanted_nb_channels != av_get_channel_layout_nb_channels(wanted_channel_layout)) {
    wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
    wanted_channel_layout &= ~AV_CH_LAYOUT_STEREO_DOWNMIX;
  }
  wanted_nb_channels = av_get_channel_layout_nb_channels(wanted_channel_layout);
  while (next_sample_rate_idx && next_sample_rates[next_sample_rate_idx] >= wanted_sample_rate)
    next_sample_rate_idx--;
  AudioRenderSpec wanted_spec{};
  wanted_spec.freq = wanted_sample_rate;
  wanted_spec.channels = wanted_nb_channels;
  wanted_spec.format = AV_SAMPLE_FMT_S16;

  while (audioRender->open(wanted_spec, bufferSize) < 0) {
    wanted_spec.channels = next_nb_channels[FFMIN(7, wanted_spec.channels)];
    if (!wanted_spec.channels) {
      wanted_spec.freq = next_sample_rates[next_sample_rate_idx--];
      wanted_spec.channels = wanted_nb_channels;
      if (!wanted_spec.freq) {
        av_log(nullptr, AV_LOG_ERROR, "No more combinations to try, audio open failed\n");
        return -1;
      }
    }
    wanted_channel_layout = av_get_default_channel_layout(wanted_spec.channels);
  }
  param.freq = wanted_spec.freq;
  param.fmt = wanted_spec.format;
  param.channel_layout = wanted_channel_layout;
  param.channels = wanted_spec.channels;
  param.frame_size = av_samples_get_buffer_size(nullptr, param.channels, 1, param.fmt, 1);
  param.bytes_per_sec = av_samples_get_buffer_size(nullptr, param.channels, param.freq, param.fmt, 1);

  return 0;
}
void Player::fetchAudioData(uint8_t *stream, int len) {
  audioDecoder->fetchData(stream, len);
}
