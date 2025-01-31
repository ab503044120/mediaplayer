//
// Created by 听见 on 2020/10/25.
//

#ifndef MUSICPLAYER_EGLTHREAD_H
#define MUSICPLAYER_EGLTHREAD_H

#include <android/native_window.h>
#include "Thread.h"
#include "EglCore.h"
#include "GLRender.h"
class EGLThread : public Thread {
 private:
  const char *TAG = "EGLRender";
  Mutex drawMutex;
  Condition drawCond;
  bool exit = false;
  bool hasSurface = false;
  bool hasGLContext = false;
  bool hasGlSurface = false;
  bool mSurfaceIsBad = false;
  bool mSizeChange = false;
  uint16_t height = 0;
  uint16_t width = 0;
  EglCore eglCore;
  EGLSurface mEglSurface = nullptr;
  ANativeWindow *mWindow = nullptr;
  GLRender *render = nullptr;


 public:
  EGLThread();

  void run() override;

  bool readyDraw();

  void surfaceCreate(ANativeWindow *nativeWindow);

  void setBitmap(uint8_t *bitmap, uint32_t width, uint32_t height, int32_t format);

  void surfaceChange(uint16_t height, uint16_t width);

  void surfaceDestory();

  virtual ~EGLThread();

};

#endif //MUSICPLAYER_EGLTHREAD_H
