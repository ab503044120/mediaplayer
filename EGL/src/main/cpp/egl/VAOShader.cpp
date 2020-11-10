//
// Created by 听见 on 2020/11/2.
//

#include "VAOShader.h"
#include "GLUtils.h"
#include <GLES3/gl3.h>
void VAOShader::onSurfaceCreate() {
  programId = glCreateProgram();
  vertexShader = GLUtils::createShader(vertexShaderStr, GL_VERTEX_SHADER);
  fragmentShader = GLUtils::createShader(fragmentShaderStr, GL_FRAGMENT_SHADER);
  glAttachShader(programId, vertexShader);
  glAttachShader(programId, fragmentShader);
  GLUtils::linkProgram(programId);
  glGenVertexArrays(1, &VAO);
  glGenBuffers(2, VBO);
  glGenBuffers(1, &EBO);
  const GLfloat vertex[]{
      -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
      0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
      0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
  };
  const GLfloat vertex1[]{
      -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
      0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
      0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
  };
  const GLuint indecies[]{
      0, 1, 2,
      0, 2, 3
  };
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), indecies, GL_STATIC_DRAW);

  // 这里使用的这个颜色
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex1), vertex1, GL_STATIC_DRAW);

  glEnableVertexAttribArray(APOS_ID);
  glVertexAttribPointer(APOS_ID, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *) 0);



  glEnableVertexAttribArray(ACOLOR_ID);
  glVertexAttribPointer(ACOLOR_ID, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));

  glBindVertexArray(0);

}
void VAOShader::onSurfaceChange(uint16_t height, uint16_t width) {
  GLRender::onSurfaceChange(height, width);

}
void VAOShader::onDrawFrame() {
  glUseProgram(programId);
  glViewport(0, 0, width, height);
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *) 0);
  glUseProgram(0);
}
void VAOShader::onSurfacedestroy() {
  glDetachShader(programId, vertexShader);
  glDetachShader(programId, fragmentShader);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteProgram(programId);

}
