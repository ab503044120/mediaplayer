//
// Created by 听见 on 2020/11/4.
//

#include <GLES3/gl3.h>
#include <Log.h>
#include "TextrueShader.h"
#include "GLUtils.h"
void TextrueRender::onSurfaceCreate() {
  programId = glCreateProgram();
  vertexShader = GLUtils::createShader(vertexShaderStr, GL_VERTEX_SHADER);
  fragmentShader = GLUtils::createShader(fragmentShaderStr, GL_FRAGMENT_SHADER);
  glAttachShader(programId, vertexShader);
  glAttachShader(programId, fragmentShader);
  GLUtils::linkProgram(programId);
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);


}
void TextrueRender::onSurfaceChange(uint16_t height, uint16_t width) {
  GLRender::onSurfaceChange(height, width);

  float ratio = bitmapWidth * 1.0f / (width * 1.0f ) * bitmapHeight / (height * 1.0f );
  LOGE("haha", " ratio %f", ratio);
  const GLfloat vertex[]{
      -1.0f, 1.0f * ratio, 0.0f, 0.0f, 0.0f,
      1.0f, 1.0f * ratio, 0.0f, 1.0f, 0.0f,
      1.0f, -1.0f * ratio, 0.0f, 1.0f, 1.0f,
      -1.0f, -1.0f * ratio, 0.0f, 0.0f, 1.0f,
  };

  const GLuint indecies[]{
      0, 1, 3, 2
  };

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), indecies, GL_STATIC_DRAW);

  glEnableVertexAttribArray(APOS_ID);
  glVertexAttribPointer(APOS_ID, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) 0);

  glEnableVertexAttribArray(ATEXCOORD_ID);
  glVertexAttribPointer(ATEXCOORD_ID, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) (sizeof(GLfloat) * 3));

  glBindVertexArray(0);
  glGenTextures(1, &TEXTRUE_ID);
  glBindTexture(GL_TEXTURE_2D, TEXTRUE_ID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmapWidth, bitmapHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
void TextrueRender::onDrawFrame() {
  glViewport(0, 0, width, height);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(programId);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, TEXTRUE_ID);
  glUniform1i(STEXTURE_ID, 0);

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (void *) 0);
  glUseProgram(0);
}
void TextrueRender::onSurfacedestroy() {

}
