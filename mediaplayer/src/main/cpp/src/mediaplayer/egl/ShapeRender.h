//
// Created by 听见 on 2020/10/29.
//

#ifndef MUSICPLAYER_SHAPERENDER_H
#define MUSICPLAYER_SHAPERENDER_H

#include "GLRender.h"
#include "ShaderProgram.h"
class ShapeRender : public GLRender {
 private:
  const GLint COORDS_PER_VERTEX = 3;
  const GLint vertexStride = COORDS_PER_VERTEX * 4;
  const char *vertexShaderStr =
      "#version 300 es\n"
      "layout(location = 0) in vec4 a_position;\n"
      "layout(location = 1) in vec4 a_color;\n"
      "out vec4 v_color;"
      "void main()\n"
      "{\n"
      "   gl_Position = a_position;\n"
      "   v_color = a_color;\n"
      "}\n";

  const char *fragmentShaderStr =
      "#version 300 es\n"
      "precision mediump float;\n"
      "in vec4 v_color;\n"
      "out vec4 fragColor;\n"
      "void main()\n"
      "{\n"
      "   fragColor = v_color;\n"
      "}\n";
  ShaderProgram shaderProgram;
  Shader vetexShader;
  Shader fragmentShader;
  GLint positionHandle = -1;
  GLint colorHandle = -1;
 public:
  ShapeRender();
  void onSurfaceCreate() override;
  void onSurfaceChange(uint16_t height, uint16_t width) override;
  void onDrawFrame() override;
  void onSurfacedestroy() override ;
};

#endif //MUSICPLAYER_SHAPERENDER_H
