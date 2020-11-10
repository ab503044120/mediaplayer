//
// Created by 听见 on 2020/11/2.
//

#ifndef MUSICPLAYER_VAOSHADER_H
#define MUSICPLAYER_VAOSHADER_H

#include "GLRender.h"
#include <GLES2/gl2.h>
class VAOShader : public GLRender {
 private:
  const char *vertexShaderStr = "#version 300 es\n"
                                "layout(location = 0)in vec3 a_pos;\n"
                                "layout(location = 1)in vec3 a_color;\n"
                                "out vec3 v_color;\n"
                                "void main(){\n"
                                " gl_Position = vec4(a_pos,1.0);\n"
                                " v_color = a_color;\n"
                                "}\n";
  const char *fragmentShaderStr = "#version 300 es\n"
                                  "precision mediump float;\n"
                                  "in vec3 v_color;\n"
                                  "out vec4 FragColor;\n"
                                  "void main(){\n"
                                  " FragColor = vec4(v_color,1.0);\n"
                                  "}\n";

  GLuint programId = 0;
  GLuint vertexShader = 0;
  GLuint fragmentShader = 0;
  enum VERTEX_ID { APOS_ID, ACOLOR_ID };
  GLuint VAO = GL_NONE, VBO[2] = {GL_NONE},
      EBO = GL_NONE;
 public:
  void onSurfaceCreate() override;
  void onSurfaceChange(uint16_t height, uint16_t width) override;
  void onDrawFrame() override;
  void onSurfacedestroy() override;
};

#endif //MUSICPLAYER_VAOSHADER_H
