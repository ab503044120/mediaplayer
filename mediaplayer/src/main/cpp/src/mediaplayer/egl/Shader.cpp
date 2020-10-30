//
// Created by 听见 on 2020/10/29.
//

#include "Shader.h"
#include <GLES2/gl2.h>
#include "Log.h"
Shader::Shader(GLenum type) : type(type) {
}
int Shader::init(const char *program) {
  shaderId = glCreateShader(type);
  GLint ret = 0;
  if (shaderId != 0) {
    glShaderSource(shaderId, 1, &program, nullptr);
    glCompileShader(shaderId);
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &ret);
    if (!ret) {
      GLint length = 0;
      glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
      char log[length];
      glGetShaderInfoLog(shaderId,length, nullptr,log);
    }
  }
  return glGetError();
}

int Shader::release() {
  glDeleteShader(shaderId);
  shaderId = -1;
  return glGetError();
}
