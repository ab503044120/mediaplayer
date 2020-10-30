//
// Created by 听见 on 2020/10/29.
//

#include "ShapeRender.h"
#include <ctime>
#include <Log.h>
ShapeRender::ShapeRender() : vetexShader(GL_VERTEX_SHADER), fragmentShader(GL_FRAGMENT_SHADER) {

}

void ShapeRender::onSurfaceCreate() {
  vetexShader.init(vertexShaderStr);
  fragmentShader.init(fragmentShaderStr);

  shaderProgram.init();
  shaderProgram.attachShader(vetexShader);
  shaderProgram.attachShader(fragmentShader);
  shaderProgram.linkProgram();
  positionHandle = shaderProgram.getAttribLocation("a_position");
  colorHandle = shaderProgram.getAttribLocation("a_color");
  shaderProgram.useProgram();
}
void ShapeRender::onSurfaceChange(uint16_t height, uint16_t width) {
  GLRender::onSurfaceChange(height, width);

}
void ShapeRender::onDrawFrame() {
//  uint64_t start = clock();
  GLfloat vertices[] = {
      0.0f, 0.5f, 0.0f,
      -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f
  };

  GLfloat color[] = {
      1.0f, 0.0f, 0.0f, 1.0f
  };
  GLint vertexCount = sizeof(vertices) / (sizeof(vertices[0]) * COORDS_PER_VERTEX);
  glViewport(0, 0, width, height);
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0, 1, 1, 1);
  glVertexAttribPointer(positionHandle, COORDS_PER_VERTEX, GL_FLOAT, GL_FALSE, vertexStride, vertices);
  glEnableVertexAttribArray(positionHandle);
  glVertexAttrib4fv(colorHandle, color);

  glDrawArrays(GL_TRIANGLES, 0, vertexCount);

  glDisableVertexAttribArray(positionHandle);
//  LOGE("haha", "%d", (clock() - start) / 1000);
}
void ShapeRender::onSurfacedestroy() {
  shaderProgram.detachShader(vetexShader);
  shaderProgram.detachShader(fragmentShader);
  vetexShader.release();
  fragmentShader.release();
  shaderProgram.release();
}
