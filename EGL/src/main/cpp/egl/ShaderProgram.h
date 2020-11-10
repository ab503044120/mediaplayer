//
// Created by 听见 on 2020/10/29.
//

#ifndef MUSICPLAYER_SHADERPROGRAM_H
#define MUSICPLAYER_SHADERPROGRAM_H
#include <GLES2/gl2.h>
#include "Shader.h"
class ShaderProgram {
 private:
  GLuint programId = 0;
 public:
  ShaderProgram();

  void init();
  void release();

  bool linkProgram();
  GLint getAttribLocation(const char* name);
  void useProgram();
  void unuseProgram();
  void detachShader(Shader &shader);
  void attachShader(Shader &shader);
};

#endif //MUSICPLAYER_SHADERPROGRAM_H
