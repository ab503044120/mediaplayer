//
// Created by 听见 on 2020/10/29.
//

#ifndef MUSICPLAYER_GLRENDER_H
#define MUSICPLAYER_GLRENDER_H

#include <android/native_window.h>
class GLRender {
 private:
  const char *TAG = "GLRender";
 protected:
  int32_t height = 0;
  int32_t width = 0;

  uint8_t *bitmap = nullptr;
  uint32_t bitmapWidth = 0;
  uint32_t bitmapHeight = 0;
  int32_t format = 0;
  
 public:
  void setBitmap(uint8_t *bitmap, uint32_t width, uint32_t height, int32_t format);

  virtual void onSurfaceCreate();

  virtual void onSurfaceChange(uint16_t height, uint16_t width);

  virtual void onDrawFrame();
  virtual void onSurfacedestroy() {};
};

#endif //MUSICPLAYER_GLRENDER_H
