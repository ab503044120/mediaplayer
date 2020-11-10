//
// Created by 听见 on 2020/11/4.
//

#ifndef MUSICPLAYER_TEXTRUESHADER_H
#define MUSICPLAYER_TEXTRUESHADER_H

#include <GLES2/gl2.h>
#include "GLRender.h"
class TextrueRender : public GLRender {
 private:
  const char *vertexShaderStr = "#version 300 es                            \n"
                                "layout(location = 0) in vec4 a_position;   \n"
                                "layout(location = 1) in vec2 a_texCoord;   \n"
                                "out vec2 v_texCoord;                       \n"
                                "void main()                                \n"
                                "{                                          \n"
                                "   gl_Position = a_position;               \n"
                                "   v_texCoord = a_texCoord;                \n"
                                "}                                          \n";
  const char *fragmentShaderStr = "#version 300 es                                     \n"
                                  "precision mediump float;                            \n"
                                  "in vec2 v_texCoord;                                 \n"
                                  "out vec4 outColor;                                  \n"
                                  "layout(location = 0)uniform sampler2D s_texture;    \n"
                                  "void main()                                         \n"
                                  "{                                                   \n"
                                  "   outColor = texture( s_texture, v_texCoord );     \n"
                                  "}                                                   \n";

  GLuint programId = 0;
  GLuint vertexShader = 0;
  GLuint fragmentShader = 0;
  enum VERTEX_ID { APOS_ID, ATEXCOORD_ID };
  enum FRAGMNET_ID { STEXTURE_ID };
  GLuint VAO = GL_NONE, VBO = GL_NONE,
      EBO = GL_NONE, TEXTRUE_ID = GL_NONE;
 public:
  virtual void onSurfaceCreate();
  virtual void onSurfaceChange(uint16_t height, uint16_t width);
  virtual void onDrawFrame();
  virtual void onSurfacedestroy();
};

#endif //MUSICPLAYER_TEXTRUESHADER_H
