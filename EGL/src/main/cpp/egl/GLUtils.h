//
// Created by 听见 on 2020/11/2.
//

#ifndef MUSICPLAYER_GLUTILS_H
#define MUSICPLAYER_GLUTILS_H

#include <GLES2/gl2.h>
class GLUtils {
 public:
  static bool linkProgram(GLuint programId);

  static GLuint createShader(const char *shaderStr, GLenum type);

};

#endif //MUSICPLAYER_GLUTILS_H
