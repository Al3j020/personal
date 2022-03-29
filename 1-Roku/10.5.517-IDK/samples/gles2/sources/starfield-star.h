// Copyright 2021 Roku, Inc.  All rights reserved.
#ifndef IDK_SAMPLE_STARFIELD_STAR_H
#define IDK_SAMPLE_STARFIELD_STAR_H
#include "screen.h"
struct Field;
class Key;
struct Star
{
  using Ref = std::shared_ptr<Star>;
  static Ref Create(float x, float y, float z, GLuint t);

  glm::vec3 pos;
  GLuint texture;
  glm::mat4 matrixMVP;

  void animate(Field*, bool faster);
  void render(ScreenRef, Field*, const Key&, bool textured = true);

  Star(float x, float y, float z, GLuint texutre);
};
using StarRef = Star::Ref;
#endif //#ifndef IDK_SAMPLE_STARFIELD_STAR_H
