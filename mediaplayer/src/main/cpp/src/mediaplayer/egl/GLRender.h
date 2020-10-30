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
 public:

  virtual void onSurfaceCreate();

  virtual void onSurfaceChange(uint16_t height, uint16_t width);

  virtual void onDrawFrame();
  virtual void onSurfacedestroy() {};

};

#endif //MUSICPLAYER_GLRENDER_H
