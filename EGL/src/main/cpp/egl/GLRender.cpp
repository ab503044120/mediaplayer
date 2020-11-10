//
// Created by 听见 on 2020/10/29.
//

#include <GLES2/gl2.h>
#include "GLRender.h"
#include "Log.h"
#include <ctime>
void GLRender::onDrawFrame() {
  clock_t start = clock();
  glClearColor(1.0f, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  LOGE(TAG, "swapBuffers: %lfms", double (clock() -start) * 1000 / CLOCKS_PER_SEC);
}
void GLRender::onSurfaceChange(uint16_t height, uint16_t width) {
  this->height = height;
  this->width = width;
  glViewport(0, 0, width, height);
}
void GLRender::onSurfaceCreate() {

}

void GLRender::setBitmap(uint8_t *bitmap, uint32_t width, uint32_t height, int32_t format) {
  this->bitmap = bitmap;
  this->bitmapHeight = height;
  this->bitmapWidth = width;
  this->format = format;
}
