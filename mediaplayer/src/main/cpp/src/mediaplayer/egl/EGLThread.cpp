//
// Created by 听见 on 2020/10/25.
//

#include "EGLThread.h"
#include <ctime>
#include "GLES2/gl2.h"
void EGLThread::run() {
  clock_t start = 0;
  bool lostEglContext = false;
  while (!exit) {
    drawMutex.lock();
    while (!exit) {
      if (lostEglContext) {
        eglCore.destroySurface(mEglSurface);
        hasGlSurface = false;
        mEglSurface = nullptr;
        eglCore.destroy();
        hasGLContext = false;
        lostEglContext = true;
      }
      if (!hasSurface) {
        if (hasGlSurface) {
          eglCore.destroySurface(mEglSurface);
          mEglSurface = nullptr;
          hasGlSurface = false;
        }
        mSurfaceIsBad = false;
      }

      if (readyDraw()) {
        if (!eglCore.hasEGLContext()) {
          eglCore.init(nullptr, 0);
        }
        if (!hasGlSurface) {
          hasGlSurface = true;
          mEglSurface = eglCore.createWindowSurface(mWindow);
          if (mEglSurface == nullptr) {
            mSurfaceIsBad = true;
          } else {
            eglCore.makeCurrent(mEglSurface);
          }
        }
        start = clock();
        glViewport(0, 0, width, height);
        glClearColor(1.0f, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        break;
      } else {
        drawCond.wait();
      }
    }
    eglCore.swapBuffers(mEglSurface);
    EGLint error = eglGetError();
    switch (error) {
      case EGL_SUCCESS:break;
      case EGL_CONTEXT_LOST:lostEglContext = true;
        break;
      default:mSurfaceIsBad = true;
        break;
    }

    LOGE(TAG, "swapBuffers: %lfms %x", double (clock() -start)*1000  / CLOCKS_PER_SEC, error);
    drawMutex.unLock();
  }
}

bool EGLThread::readyDraw() {
  return hasSurface && !mSurfaceIsBad && width != 0 && height != 0;
}

EGLThread::EGLThread() : Thread("EGLRenderThread", nullptr), drawMutex(Mutex()), drawCond(drawMutex) {
  start();
}

void EGLThread::surfaceCreate(ANativeWindow *nativeWindow) {
  autoLock(drawMutex);
  this->mWindow = nativeWindow;
  this->hasSurface = true;
  this->drawCond.broadcast();
}

void EGLThread::surfaceChange(uint16_t height, uint16_t width) {
  autoLock(drawMutex);
  this->height = height;
  this->width = width;
  this->drawCond.broadcast();

}

void EGLThread::surfaceDestory() {
  autoLock(drawMutex);
  mWindow = nullptr;
  hasSurface = false;
  this->drawCond.broadcast();
}

EGLThread::~EGLThread() {
  join();
}

