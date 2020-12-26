//
// Created by 听见 on 2020/11/22.
//

#include "AudioRender.h"
#include "Player.h"
static bool checkSuccess(SLresult value) {
  return SL_RESULT_SUCCESS != value;
}

AudioRender::AudioRender(PlayerState &playerState, AudioFetchCallback &callback) : IRender("AudioRender", playerState),
                                                                                   callback(callback) {

}

void AudioRender::run() {
  //init 初始化
  uint8_t *next_buffer = nullptr;
  int next_buffer_index = 0;
  (*slPlayItf)->SetPlayState(slPlayItf, SL_PLAYSTATE_PLAYING);
  while (!abort) {
    SLAndroidSimpleBufferQueueState slState = {0};
    SLresult slRet = (*slBufferQueueItf)->GetState(slBufferQueueItf, &slState);
    LOGE(TAG, "(*slBufferQueueItf)->GetState %d %d", slRet, slState.count);
    mutex.lock();
    if (!abort && slState.count >= OPENSLES_BUFFERS) {
      while (!abort && slState.count >= OPENSLES_BUFFERS) {
        (*slPlayItf)->SetPlayState(slPlayItf, SL_PLAYSTATE_PLAYING);
        cond.wait(mutex, 100 * 1000 * 1000);
        slRet = (*slBufferQueueItf)->GetState(slBufferQueueItf, &slState);
        LOGE(TAG, "(*slBufferQueueItf)->GetState1 %d %d", slRet, slState.count);
//        if (slRet != SL_RESULT_SUCCESS) {
//          SDL_UnlockMutex(opaque->wakeup_mutex);
//        }
//
//        if (opaque->pause_on)
//          (*slPlayItf)->SetPlayState(slPlayItf, SL_PLAYSTATE_PAUSED);
      }
      if (!abort) {
        (*slPlayItf)->SetPlayState(slPlayItf, SL_PLAYSTATE_PLAYING);
      }
    }
    mutex.unLock();

    next_buffer = playBuffer + next_buffer_index * bytes_per_buffer;
    next_buffer_index = (next_buffer_index + 1) % OPENSLES_BUFFERS;
//    playerState.audioDecoder->fetchData(playBuffer, bytes_per_buffer);
    slRet = (*slBufferQueueItf)->Enqueue(slBufferQueueItf, next_buffer, bytes_per_buffer);
    LOGE(TAG, "(*slBufferQueueItf)->Enqueue %d", slRet);
  }

  abort = true;
}

int32_t AudioRender::init() {
  SLEngineItf slEngine = nullptr;
  SLObjectItf slOutputMixObject = nullptr;
  SLObjectItf slObject = nullptr;

  SLresult ret = slCreateEngine(&slObject, 0, NULL, 0, NULL, NULL);
  CHECK_FAIL_LOGE(TAG, ret, checkSuccess, "%s", "slCreateEngine  fail")
  this->slObject = slObject;
  ret = (*slObject)->Realize(slObject, SL_BOOLEAN_FALSE);
  CHECK_FAIL_LOGE(TAG, ret, checkSuccess, "%s", "slObject Realize fail")

  ret = (*slObject)->GetInterface(slObject, SL_IID_ENGINE, &slEngine);
  CHECK_FAIL_LOGE(TAG, ret, checkSuccess, "%s", "slObject GetInterface fail")
  this->slEngine = slEngine;

  {
    const SLInterfaceID ids1[] = {SL_IID_VOLUME};
    const SLboolean req1[] = {SL_BOOLEAN_FALSE};
    ret = (*slEngine)->CreateOutputMix(slEngine, &slOutputMixObject, 1, ids1, req1);
    CHECK_FAIL_LOGE(TAG, ret, checkSuccess, "%s", "CreateOutputMix fail")
  }

  this->slOutputMixObject = slOutputMixObject;

  ret = (*slOutputMixObject)->Realize(slOutputMixObject, SL_BOOLEAN_FALSE);
  CHECK_FAIL_LOGE(TAG, ret, checkSuccess, "%s", "slOutputMixObject Realize fail")

  fail:
  return ret;
}

void openslesCallback(SLAndroidSimpleBufferQueueItf caller, void *pContext) {
  auto *ctx = static_cast<AudioRender *>(pContext);
  ctx->mutex.lock();
  ctx->cond.signal();
  ctx->mutex.unLock();

}
int32_t AudioRender::open(AudioRenderSpec &wantSpec, int32_t &bufferSize) {
  SLresult ret;
  format_pcm.formatType = SL_DATAFORMAT_PCM;
  format_pcm.numChannels = wantSpec.channels;
  format_pcm.samplesPerSec = wantSpec.freq * 1000; // milli Hz 采样率 毫秒
  format_pcm.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
  format_pcm.containerSize = SL_PCMSAMPLEFORMAT_FIXED_16;
  switch (wantSpec.channels) {
    case 2:format_pcm.channelMask = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
      break;
    case 1:format_pcm.channelMask = SL_SPEAKER_FRONT_CENTER;
      break;
    default:
      // todo 待添加错误处理
      break;
  }
  format_pcm.endianness = SL_BYTEORDER_LITTLEENDIAN;
  SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {
      SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
      OPENSLES_BUFFERS
  };
  SLDataSource audio_source = {&loc_bufq, &format_pcm};

  // config audio sink
  SLDataLocator_OutputMix loc_outmix = {
      SL_DATALOCATOR_OUTPUTMIX,
      slOutputMixObject
  };
  SLDataSink audio_sink = {&loc_outmix, nullptr};

  SLObjectItf slPlayerObject = nullptr;
  const SLInterfaceID ids2[] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE, SL_IID_VOLUME, SL_IID_PLAY};
  static const SLboolean req2[] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
  ret = (*slEngine)->CreateAudioPlayer(slEngine, &slPlayerObject, &audio_source,
                                       &audio_sink, sizeof(ids2) / sizeof(*ids2),
                                       ids2, req2);
  CHECK_FAIL_LOGE(TAG, ret, checkSuccess, "%s", "(*slEngine)->CreateAudioPlayer fail")
  this->slPlayerObject = slPlayerObject;

  ret = (*slPlayerObject)->Realize(slPlayerObject, SL_BOOLEAN_FALSE);
  CHECK_FAIL_LOGE(TAG, ret, checkSuccess, "%s", "slPlayerObject->Realize() failed")

  ret = (*slPlayerObject)->GetInterface(slPlayerObject, SL_IID_PLAY, &slPlayItf);
  CHECK_FAIL_LOGE(TAG, ret, checkSuccess, "%s", "slPlayerObject->GetInterface(SL_IID_PLAY) failed")

  ret = (*slPlayerObject)->GetInterface(slPlayerObject, SL_IID_VOLUME, &slVolumeItf);
  CHECK_FAIL_LOGE(TAG, ret, checkSuccess, "%s", "slPlayerObject->GetInterface(SL_IID_VOLUME) failed")

  ret = (*slPlayerObject)->GetInterface(slPlayerObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &slBufferQueueItf);
  CHECK_FAIL_LOGE(TAG, ret, checkSuccess, "%s", "slPlayerObject->GetInterface(SL_IID_ANDROIDSIMPLEBUFFERQUEUE) failed")

  ret = (*slBufferQueueItf)->RegisterCallback(slBufferQueueItf, openslesCallback, (void *) this);
  CHECK_FAIL_LOGE(TAG, ret, checkSuccess, "%s", "slBufferQueueItf->RegisterCallback() failed")

  bytes_per_frame = format_pcm.numChannels * format_pcm.bitsPerSample / 8;    //每帧多少个字节
  milli_per_buffer = OPENSLES_BUFLEN;                                         //一帧多少ms
  frames_per_buffer = milli_per_buffer * format_pcm.samplesPerSec / 1000000;  // 每个buffer 多少帧
  bytes_per_buffer = bytes_per_frame * frames_per_buffer;                     // 没个buffer多少个字节
  playBufferSize = OPENSLES_BUFFERS * bytes_per_buffer;                        //总buffer数
  playBuffer = new uint8_t[playBufferSize];
  for (int i = 0; i < playBufferSize; ++i) {
    ret = (*slBufferQueueItf)->Enqueue(slBufferQueueItf, playBuffer + i * bytes_per_buffer, bytes_per_buffer);
    CHECK_FAIL_LOGE(TAG, ret, checkSuccess, "%s", "slBufferQueueItf->Enqueue() failed")
  }
  start();
  fail:
  //todo 添加打开失败处理
  bufferSize = bytes_per_buffer;
  return 0;
}

void AudioRender::stop() {

}
int32_t AudioRender::getLatency() {
  SLAndroidSimpleBufferQueueState slState = {0};
  SLresult slRet = (*slBufferQueueItf)->GetState(slBufferQueueItf, &slState);
  if (slRet != SL_RESULT_SUCCESS) {
    return OPENSLES_BUFFERS * milli_per_buffer;
  }
  return slState.count * OPENSLES_BUFFERS * milli_per_buffer;
}
