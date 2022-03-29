// Copyright 2021 Roku, Inc.  All rights reserved.
#ifndef IDK_SAMPLE_STARFIELD_COLOR_H
#define IDK_SAMPLE_STARFIELD_COLOR_H
#include "screen.h"
#include <initializer_list>
struct Field;
class Key;
struct Color
{
  using Ref = std::shared_ptr<Color>;
  static Ref Create(const std::initializer_list<glm::vec4>&);

  const int w, h;
  GLuint fbo;
  GLuint texture;
  GLuint cbo;
  float colors[4 * 5];
  float deltas[4 * 5];

  void calculateCenter();
  void animate(Field*);
  void render(ScreenRef, Field*, const Key&);

  Color(const std::initializer_list<glm::vec4>&);
  ~Color();
};
using ColorRef = Color::Ref;
#endif //#ifndef IDK_SAMPLE_STARFIELD_COLOR_H
