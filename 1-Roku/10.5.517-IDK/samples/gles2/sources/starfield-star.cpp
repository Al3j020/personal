// Copyright 2021 Roku, Inc.  All rights reserved.
#include "starfield-star.h"
#include "starfield-field.h"
StarRef
Star::Create(float x, float y, float z, GLuint t)
{
  return std::make_shared<Star>(x, y, z, t);
}

Star::Star(float x, float y, float z, GLuint tex)
  : pos(x, y, z)
  , texture(tex)
{}

void
Star::animate(Field* field, bool faster)
{
  pos.z += faster ? (38 * field->fSpeed) : field->fSpeed;
  if (pos.z > 0.1f) {
    pos.z = -38.8f;
  }
  matrixMVP = field->matrixVP * glm::translate(glm::mat4(1.0f), pos);
}

void
Star::render(ScreenRef GL, Field* field, const Key& key, bool textured)
{
  // This is odd?  glm::value_ptr(glm::mat4) gives back data in column-
  // major order, and if I dump the array I can see it is true.  However, unless
  // I transpose the MVP matrix, the output is wrong.  For example, if I perform
  // a translate, the array from glm::value_ptr(glm::mat4) reads column-major:
  //   1, 0, 0, 0
  //   0, 1, 0, 0
  //   0, 0, 1, 0
  //   x, y, z, 1
  // Without transposing MVP, the output isn't shifted by (x,y,z) but stayed in
  // center of screen.  Setting GL_TRUE to transpose the matrix will move output
  // as expected.
  glUniformMatrix4fv(GL->shader.uMVP, 1, GL_TRUE, glm::value_ptr(matrixMVP));

  if (textured) {
    glUniform1i(GL->shader.uTexColor, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
  }

  field->drawBox(key);
}
