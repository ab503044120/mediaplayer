//
// Created by 听见 on 2020/11/2.
//

#include <Log.h>
#include "GLUtils.h"

bool GLUtils::linkProgram(GLuint programId) {
  glLinkProgram(programId);
  GLint ret = 0;
  glGetProgramiv(programId, GL_LINK_STATUS, &ret);
  if (!ret) {
    GLint length = 0;
    if (length > 0) {
      glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
      char log[length + 1];
      glGetProgramInfoLog(programId, length, nullptr, log);
      LOGE("GLUtils", "%s", log);
    } else {
      LOGE("GLUtils", "%s", "link error unknown");
    }
    return false;
  }
  return true;
}
GLuint GLUtils::createShader(const char *shaderStr, GLenum type) {
  GLuint shaderId = glCreateShader(type);
  GLint ret = 0;
  if (shaderId != 0) {
    glShaderSource(shaderId, 1, &shaderStr, nullptr);
    glCompileShader(shaderId);
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &ret);
    if (!ret) {
      GLint length = 0;
      glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
      char log[length];
      glGetShaderInfoLog(shaderId, length, nullptr, log);
      LOGE("GLUtils", "%s\n%s", shaderStr, log);
      glDeleteShader(shaderId);

      return 0;
    }
  }
  return shaderId;
}
