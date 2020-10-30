//
// Created by 听见 on 2020/10/29.
//

#ifndef MUSICPLAYER_SHADER_H
#define MUSICPLAYER_SHADER_H
#include <GLES2/gl2.h>
class Shader {

 private:
  friend class ShaderProgram;
  GLuint shaderId = 0;
  GLenum type;

 public:
  Shader(GLenum type);

  int init(const char *program);

  int release();

};

#endif //MUSICPLAYER_SHADER_H
