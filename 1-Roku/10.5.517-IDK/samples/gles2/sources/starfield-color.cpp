// Copyright 2021 Roku, Inc.  All rights reserved.
#include "starfield-color.h"
#include "starfield-field.h"
ColorRef
Color::Create(const std::initializer_list<glm::vec4>& list)
{
  return std::make_shared<Color>(list);
}

Color::Color(const std::initializer_list<glm::vec4>& list)
  : w(256)
  , h(256)
{
  int i = 0;
  for (auto&& color : list) {
    colors[(4 * i)] = color.r;
    colors[(4 * i) + 1] = color.g;
    colors[(4 * i) + 2] = color.b;
    colors[(4 * i) + 3] = color.a;
    ++i;
  }
  calculateCenter();

  for (int j = 0; j < 20; ++j) {
    deltas[j] = 0.038;
  }

  glGenBuffers(1, &cbo);
  glBindBuffer(GL_ARRAY_BUFFER, cbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glFramebufferTexture2D(
    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

  auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    fprintf(stderr, "FBO incomplete status: 0x%04x\n", status);
    CheckGLError();
  }
}

Color::~Color()
{
  glDeleteBuffers(1, &cbo);
  glDeleteTextures(1, &texture);
  glDeleteFramebuffers(1, &fbo);
}

void
Color::calculateCenter()
{
  colors[16] = (colors[0] + colors[4] + colors[8] + colors[12]) * 0.25f;
  colors[17] = (colors[1] + colors[5] + colors[9] + colors[13]) * 0.25f;
  colors[18] = (colors[2] + colors[6] + colors[10] + colors[14]) * 0.25f;
  colors[19] = (colors[3] + colors[7] + colors[11] + colors[15]) * 0.25f;
}

#define MODULATE(s, d)                                                         \
  {                                                                            \
    s = s + d;                                                                 \
    if (s > 1.0f) {                                                            \
      s = 1.0f;                                                                \
      d = -d;                                                                  \
    } else if (s < 0.0f) {                                                     \
      s = 0.0f;                                                                \
      d = -d;                                                                  \
    }                                                                          \
  }

void
Color::animate(Field*)
{
  for (int i = 0; i < 5; ++i) {
    MODULATE(colors[(4 * i)], deltas[(4 * i)]);
    MODULATE(colors[(4 * i) + 1], deltas[(4 * i) + 1]);
    MODULATE(colors[(4 * i) + 2], deltas[(4 * i) + 2]);
  }
}

void
Color::render(ScreenRef GL, Field* field, const Key& key)
{
  glViewport(0, 0, w, h);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glClear(GL_COLOR_BUFFER_BIT);

  glUniform1i(GL->shader.uTexMask, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, field->texMask);

  glBindBuffer(GL_ARRAY_BUFFER, cbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors), colors);

  field->drawBox(key, true, cbo);
}
