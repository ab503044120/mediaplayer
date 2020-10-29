//
// Created by 听见 on 2020/10/29.
//

#ifndef MUSICPLAYER_GLRENDER_H
#define MUSICPLAYER_GLRENDER_H

#include <android/native_window.h>
class GLRender {
  void onSurfaceCreate(ANativeWindow *nativeWindow);

  void onSurfaceChange(uint16_t height, uint16_t width);

  void onDrawFrame();
};

#endif //MUSICPLAYER_GLRENDER_H
