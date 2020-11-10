//
// Created by 听见 on 2020/10/25.
//

#include "EGLThread.h"
#include <ctime>
#include <unistd.h>
#include "GLES2/gl2.h"
#include "ShapeRender.h"
#include "VAOShader.h"
#include "TextrueShader.h"
#include <chrono>
void EGLThread::run() {
  bool lostEglContext = false;
  while (!exit) {
    drawMutex.lock();
    while (!exit) {
      if (lostEglContext) {
        if (render) {
          render->onSurfacedestroy();
        }
        eglCore.destroySurface(mEglSurface);
        hasGlSurface = false;
        mEglSurface = nullptr;
        eglCore.destroy();
        hasGLContext = false;
        lostEglContext = true;
      }
      if (!hasSurface) {
        if (hasGlSurface) {
          if (!render) {
            render->onSurfacedestroy();
          }
          eglCore.destroySurface(mEglSurface);
          mEglSurface = nullptr;
          hasGlSurface = false;
        }
        mSurfaceIsBad = false;
      }

      if (readyDraw()) {
        if (!eglCore.hasEGLContext()) {
          eglCore.init(nullptr, 0);
          hasGLContext = true;
        }

        break;
      } else {
        drawCond.wait();
      }
    }
    if (!hasGlSurface) {
      mEglSurface = eglCore.createWindowSurface(mWindow);
      eglCore.makeCurrent(mEglSurface);
      hasGlSurface = true;
      if (mEglSurface != nullptr) {
        if (render) {
          render->onSurfaceCreate();
        }
      } else {
        mSurfaceIsBad = true;
      }
    }
    if (mSizeChange) {
      mSizeChange = false;
      if (render) {
        render->onSurfaceChange(height, width);
      }
    }
//    auto start = currentTime();
    if (render) {
      render->onDrawFrame();
    }
    eglCore.swapBuffers(mEglSurface);
//    LOGE("haha", "%ld", ((currentTime() - start) / 1000000));

    EGLint error = eglGetError();
    switch (error) {
      case EGL_SUCCESS:break;
      case EGL_CONTEXT_LOST:lostEglContext = true;
        break;
      default:mSurfaceIsBad = true;
        break;
    }
    drawMutex.unLock();
  }
}

bool EGLThread::readyDraw() {
  return hasSurface && !mSurfaceIsBad && width != 0 && height != 0;
}

EGLThread::EGLThread() : Thread("EGLRenderThread", nullptr),
                         render(new TextrueRender()),
                         drawMutex(Mutex()),
                         drawCond(drawMutex) {
  start();
}

void EGLThread::setBitmap(uint8_t *bitmap, uint32_t width, uint32_t height, int32_t format) {
  render->setBitmap(bitmap, width, height, format);
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
  mSizeChange = true;
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


