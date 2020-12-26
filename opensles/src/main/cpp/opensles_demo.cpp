#include <jni.h>
#include <SLES/OpenSLES_Android.h>
#include <Log.h>
#include <fcntl.h>
#include <unistd.h>
#define BUF_SIZE 4096
#define BUF_COUNT 255

const char *TAG = "haha";
SLAndroidSimpleBufferQueueItf pcmBufferQueue = nullptr;
int32_t fd;
uint8_t buf[BUF_SIZE];
uint8_t playBuf[BUF_COUNT][BUF_SIZE];
uint8_t curIndex = 0;
void callback(
    SLAndroidSimpleBufferQueueItf caller,
    void *pContext
) {
  //这里测试可得如果不enqueue 那么就不会继续播放了
//  while (i--) {
//    SLAndroidSimpleBufferQueueState pState{};
//    (*pcmBufferQueue)->GetState(pcmBufferQueue, &pState);
//    LOGE("haha", "%d %d", pState.count, pState.index);
//    sleep(1);
//  }
  SLAndroidSimpleBufferQueueState pState{};
  (*pcmBufferQueue)->GetState(pcmBufferQueue, &pState);
  LOGE("haha", "%d %d", pState.count, pState.index);

  int32_t count = 0;
  memset(playBuf[curIndex], 0, BUF_SIZE);

  int32_t re = read(fd, buf, sizeof(buf));
  if (re > 0) {
    count += re;
    while (count < BUF_SIZE) {
      re = read(fd, buf + count, BUF_SIZE - count);
      if (re <= 0) {
        break;
      }
      count += re;
    }
  }
  if (re > 0) {
    memcpy(playBuf[curIndex], buf, count);
    (*caller)->Enqueue(caller, buf, count);

  }


  // Start queue callback
}

extern "C"
JNIEXPORT void JNICALL
Java_org_huihui_opensles_MainActivity_play(JNIEnv *env, jobject thiz, jstring path) {
  char *cPath = const_cast<char *>(env->GetStringUTFChars(path, nullptr));
  SLresult re;
  SLObjectItf engineObject;
  SLEngineItf slAudioEngine;
/* Effect interface for the output mix */
  SLEqualizerItf eqOutputItf;

  // 1. Create and init audio engine
  re = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
  if (re != SL_RESULT_SUCCESS) {
    LOGD(TAG, "%s", "slCreateEngine() failed");
  }
  re = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
  if (re != SL_RESULT_SUCCESS) {
    LOGD(TAG, "%s", "engineObject Realize failed");
  }
  re = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &slAudioEngine);
  if (re != SL_RESULT_SUCCESS) {
    LOGD(TAG, "%s", "engineObject GetInterface SL_IID_ENGINE failed");
  }

  // 2. Set output mix


  SLObjectItf outputMix;
  re = (*slAudioEngine)->CreateOutputMix(slAudioEngine, &outputMix, 0, nullptr, nullptr);
  if (re != SL_RESULT_SUCCESS) {
    LOGD(TAG, "%s", "CreateOutputMix() failed");
  }
  re = (*outputMix)->Realize(outputMix, SL_BOOLEAN_FALSE);
  if (re != SL_RESULT_SUCCESS) {
    LOGD(TAG, "%s", "outputMix Realize failed");
  }



  // 3. Configuring the input data source
  SLDataLocator_AndroidSimpleBufferQueue inputBuffQueueLocator = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, BUF_COUNT};
  SLDataFormat_PCM input_format_pcm = {
      SL_DATAFORMAT_PCM,                              // <<< 输入的音频格式,PCM
      2,                                              // <<< 输入的声道数，2(立体声)
      SL_SAMPLINGRATE_44_1,                           // <<< 输入的采样率，44BUF_COUNT0hz
      SL_PCMSAMPLEFORMAT_FIXED_16,                    // <<< 输入的采样位数，16bit
      SL_PCMSAMPLEFORMAT_FIXED_16,                    // <<< 容器大小，同上
      SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,   // <<< 声道标记，这里使用左前声道和右前声道
      SL_BYTEORDER_LITTLEENDIAN                       // <<< 输入的字节序,小端
  };
  SLDataSource dataSource = {&inputBuffQueueLocator, &input_format_pcm};

  SLDataLocator_OutputMix outputMixLocator = {SL_DATALOCATOR_OUTPUTMIX, outputMix};
  SLDataSink dataSink = {&outputMixLocator, 0};


  // 4. Create Audio Player
  SLObjectItf audioPlayer;
  SLPlayItf playInterface;
  SLInterfaceID audioPlayerInterfaceIDs[] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE, SL_IID_EQUALIZER};
  SLboolean audioPlayerInterfaceRequired[] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

  re = (*slAudioEngine)->CreateAudioPlayer(slAudioEngine,
                                           &audioPlayer,
                                           &dataSource,
                                           &dataSink,
                                           2,
                                           audioPlayerInterfaceIDs,
                                           audioPlayerInterfaceRequired);

  if (re != SL_RESULT_SUCCESS) {
    LOGD(TAG, "%s", "CreateAudioPlayer() failed");
  }

  re = (*audioPlayer)->Realize(audioPlayer, SL_BOOLEAN_FALSE);
  if (re != SL_RESULT_SUCCESS) {
    LOGD(TAG, "%s", "AudioPlayer Realize failed");
  }

  re = (*audioPlayer)->GetInterface(audioPlayer, SL_IID_EQUALIZER, (void *) &eqOutputItf);
  if (re != SL_RESULT_SUCCESS) {
    LOGD(TAG, "%s", "AudioPlayer GetInterface SL_IID_EQUALIZER failed");
  }
  SLuint16 pAmount = 0;
  re = (*eqOutputItf)->GetNumberOfBands(eqOutputItf, &pAmount);

  LOGD(TAG, " GetNumberOfBands %d %d", re, pAmount);

  SLmillibel min;
  SLmillibel max;
  re = (*eqOutputItf)->GetBandLevelRange(eqOutputItf, &min, &max);

  LOGD(TAG, " GetBandLevel %d %d %d", re, min, max);
  SLmilliHertz pCenter;
  for (int i = 0; i < pAmount; ++i) {
    re = (*eqOutputItf)->GetCenterFreq(eqOutputItf, i, &pCenter);
    LOGD(TAG, " GetCenterFreq %d %d %d", re, i, pCenter);
  }

  re = (*audioPlayer)->GetInterface(audioPlayer, SL_IID_PLAY, &playInterface);
  if (re != SL_RESULT_SUCCESS) {
    LOGD(TAG, "%s", "AudioPlayer GetInterface SL_IID_PLAY failed");
  }
  re = (*audioPlayer)->GetInterface(audioPlayer, SL_IID_BUFFERQUEUE, &pcmBufferQueue);
  if (re != SL_RESULT_SUCCESS) {
    LOGD(TAG, "%s", "AudioPlayer GetInterface SL_IID_BUFFERQUEUE failed");
  }

  fd = open(cPath, O_RDONLY);
  re = read(fd, buf, sizeof(buf));
  (*pcmBufferQueue)->RegisterCallback(pcmBufferQueue, callback, NULL);
  (*playInterface)->SetPlayState(playInterface, SL_PLAYSTATE_PLAYING);

  // Start queue callback
  for (auto &i : playBuf) {
    (*pcmBufferQueue)->Enqueue(pcmBufferQueue, i, re);
  }

}

