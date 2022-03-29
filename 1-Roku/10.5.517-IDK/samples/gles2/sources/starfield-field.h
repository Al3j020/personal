// Copyright 2021 Roku, Inc.  All rights reserved.
#ifndef IDK_SAMPLE_STARFIELD_FIELD_H
#define IDK_SAMPLE_STARFIELD_FIELD_H
#include "screen.h"
#include "starfield.h"
#include <vector>
struct Star;
struct Color;
class Key;
struct Field : public Starfield
{
  const int width, height;
  ScreenRef GL;
  std::vector<std::shared_ptr<Star>> vecStars;
  std::vector<std::shared_ptr<Color>> vecColors;
  GLuint fboColor;
  GLuint texColor;
  GLuint fboBlurry;
  GLuint texBlurry;
  GLuint texMask;
  GLuint vbo_fs;
  GLuint vbo;
  GLuint ibo_fs;
  GLuint ibo;
  GLuint uv;
  glm::mat4 matrixVP;

  using Ref = std::shared_ptr<Field>;
  static Ref Create(int width, int height);

  // Use one set of buffers to do fullscreen drawing when rendering to texture,
  // and switch to another set when I am drawing individual stars, this function
  // does it all.
  void drawBox(const Key&, bool fullscreen = false, GLuint cbo = 0);
  void setup();
  void animate(bool);
  std::future<void> render();

  Field(int, int);
  ~Field();
};

class Key
{
  friend struct Field;
  Key() {}
};
using FieldRef = Field::Ref;
#endif //#ifndef IDK_SAMPLE_STARFIELD_FIELD_H
