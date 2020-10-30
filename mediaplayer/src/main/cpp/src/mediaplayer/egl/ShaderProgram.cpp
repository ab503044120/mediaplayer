//
// Created by 听见 on 2020/10/29.
//

#include "ShaderProgram.h"
#include "Shader.h"

ShaderProgram::ShaderProgram() = default;

void ShaderProgram::init() {
  programId = glCreateProgram();
}
void ShaderProgram::detachShader(Shader &shader) {
  glDetachShader(programId, shader.shaderId);

}
void ShaderProgram::attachShader(Shader &shader) {
  glAttachShader(programId, shader.shaderId);
}
void ShaderProgram::linkProgram() {
  glLinkProgram(programId);
  GLint ret = 0;
  glGetProgramiv(programId, GL_LINK_STATUS, &ret);
  if (!ret) {
    GLint length = 0;
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
    char log[length];
    glGetProgramInfoLog(programId, length, nullptr, log);
  }
}
GLint ShaderProgram::getAttribLocation(const char *name) {
  return glGetAttribLocation(programId, name);
}

void ShaderProgram::useProgram() {
  glUseProgram(programId);
}
void ShaderProgram::release() {
  if (programId != -1) {
    glDeleteProgram(programId);
    programId = -1;
  }
}
