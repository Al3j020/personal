// Copyright 2021 Roku, Inc.  All rights reserved.
#include "starfield-color.h"
#include "starfield-field.h"
#include "starfield-star.h"
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cinttypes>
#include <mutex>
#include <random>
#include <thread>
using namespace std::chrono_literals;
namespace {
// Set this to true when demo starts, false to exit.
std::atomic<bool> bRunning = { false };

// Don't want to render field::vecStars when it is being sorted for z-order.
std::mutex mtxStars;

std::mt19937 RNG;

float
floatRandom(float min = 0.0f, float max = 1.0f)
{
  std::uniform_real_distribution<float> distribution(min, max);
  return distribution(RNG);
}

float
RandomX(float limit)
{
  return floatRandom(-limit, limit);
}

float
RandomY(float limit)
{
  return floatRandom(-limit, limit);
}

int
intRandom(int min = 0, int max = 100)
{
  std::uniform_int_distribution<int> distribution(min, max);
  return distribution(RNG);
}

int
RandomColor(int max)
{
  return intRandom(0, max);
}

void
InitializeRNG()
{
  std::array<int, 624> seed_data;
  std::random_device rdev;
  std::generate_n(seed_data.data(), seed_data.size(), std::ref(rdev));
  std::seed_seq seq(seed_data.begin(), seed_data.end());
  RNG = std::mt19937(seq);
}

void
RenderTextures(ScreenRef GL, Field* field, const Key& key)
{
  // This function renders to textures color cycling clipped by a mask, which
  // is then used to texture individual stars.
  GL->push([=](int) {
    glm::mat4 identity(1.0f);
    glUniformMatrix4fv(GL->shader.uMVP, 1, GL_TRUE, glm::value_ptr(identity));

    // I load one shader that does everything, these boolean variables control
    // the GLSL code path in fragment shader.
    glUniform1i(GL->shader.uCtrlMask, 1);
    glUniform1i(GL->shader.uCtrlBlur, 0);
    glUniform1i(GL->shader.uCtrlFinal, 0);

    for (auto&& color : field->vecColors) {
      color->render(GL, field, key);
    }
  });
}

void
RenderColor(ScreenRef GL, Field* field, const Key& key)
{
  // This function renders full-color star field, using texture from above
  // RenderTextures().
  GL->push([=](int) {
    glUniform1i(GL->shader.uCtrlMask, 0);
    glUniform1i(GL->shader.uCtrlBlur, 0);
    glUniform1i(GL->shader.uCtrlFinal, 0);

    glViewport(0, 0, field->width, field->height);
    glBindFramebuffer(GL_FRAMEBUFFER, field->fboColor);
    glClear(GL_COLOR_BUFFER_BIT);

    std::lock_guard<std::mutex> lock(mtxStars);
    for (auto&& star : field->vecStars) {
      star->render(GL, field, key);
    }
  });
}

void
RenderBlurry(ScreenRef GL, Field* field, const Key& key)
{
  // Make a blurry version of image produced in RenderColor().
  GL->push([=](int) {
    glm::mat4 identity(1.0f);
    glUniformMatrix4fv(GL->shader.uMVP, 1, GL_TRUE, glm::value_ptr(identity));

    glUniform1i(GL->shader.uCtrlMask, 0);
    glUniform1i(GL->shader.uCtrlBlur, 1);
    glUniform1i(GL->shader.uCtrlFinal, 0);

    glViewport(0, 0, field->width, field->height);
    glBindFramebuffer(GL_FRAMEBUFFER, field->fboBlurry);
    glClear(GL_COLOR_BUFFER_BIT);

    glUniform1i(GL->shader.uTexColor, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, field->texColor);

    field->drawBox(key, true);
  });
}

std::future<void>
RenderAndPresent(ScreenRef GL, Field* field, const Key& key)
{
  GL->push([=](int) {
    // Composite every thing together, make pixels along the outer edge more
    // fuzzy.
    glm::mat4 identity(1.0f);
    glUniformMatrix4fv(GL->shader.uMVP, 1, GL_TRUE, glm::value_ptr(identity));

    glUniform1i(GL->shader.uCtrlMask, 0);
    glUniform1i(GL->shader.uCtrlBlur, 0);
    glUniform1i(GL->shader.uCtrlFinal, 1);

    glUniform1i(GL->shader.uTexColor, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, field->texColor);
    glUniform1i(GL->shader.uTexBlur, 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, field->texBlurry);
    glUniform1i(GL->shader.uTexMask, 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, field->texMask);

    glViewport(0, 0, field->width, field->height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    field->drawBox(key, true);
  });
  return GL->swap();
}
} // anonymous namespace

FieldRef
Field::Create(int w, int h)
{
  return std::make_shared<Field>(w, h);
}

Field::Field(int w, int h)
  : Starfield()
  , width(w)
  , height(h)
  , GL(Screen::Create(w, h))
{
  // Allocate buffers and textures in setup().
  fprintf(stderr, "resolution %dx%d\n", w, h);
}

Field::~Field()
{
  GL->push([this](int) {
    glDeleteFramebuffers(1, &fboColor);
    glDeleteFramebuffers(1, &fboBlurry);
    glDeleteTextures(1, &texColor);
    glDeleteTextures(1, &texBlurry);
    glDeleteTextures(1, &texMask);
    glDeleteBuffers(1, &vbo_fs);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo_fs);
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &uv);

    std::lock_guard<std::mutex> lock(mtxStars);
    vecColors.clear();
    vecStars.clear();
  });
}

void
Field::setup()
{
  // Texture APIs will wait, don't push them to GL thread to avoid deadlock.
  // Deleted in destructor.
  texColor = GL->createTexture(width, height, {});
  texBlurry = GL->createTexture(width, height, {});
  texMask = GL->getPNGTexture("/pkg:/source/mask.png");

  auto setup = GL->push([=](int) {
    // Internally I use 5 vertices for each quad:
    //   3 --- 2
    //   |  4  |
    //   0 --- 1
    glm::vec3 vertices_fs[5] = { { -1.0f, -1.0f, 0.0f },
                                 { 1.0f, -1.0f, 0.0f },
                                 { 1.0f, 1.0f, 0.0f },
                                 { -1.0f, 1.0f, 0.0f },
                                 { 0.0f, 0.0f, 0.0f } };
    glGenBuffers(1, &vbo_fs); // Deleted in destructor.
    glBindBuffer(GL_ARRAY_BUFFER, vbo_fs);
    glBufferData(
      GL_ARRAY_BUFFER, sizeof(glm::vec3) * 5, vertices_fs, GL_STATIC_DRAW);

    // This VBO is used with perspective projection and is corrected for
    // aspect ratio.
    float v = 0.1f;
    glm::vec3 vertices[4] = {
      { -v, -v, 0.0f }, { v, -v, 0.0f }, { v, v, 0.0f }, { -v, v, 0.0f }
    };
    glGenBuffers(1, &vbo); // Deleted in destructor.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
      GL_ARRAY_BUFFER, sizeof(glm::vec3) * 4, vertices, GL_STATIC_DRAW);

    glm::vec2 texcoords[5] = { { 0.0f, 0.0f },
                               { 1.0f, 0.0f },
                               { 1.0f, 1.0f },
                               { 0.0f, 1.0f },
                               { 0.5f, 0.5f } };
    glGenBuffers(1, &uv); // Deleted in destructor.
    glBindBuffer(GL_ARRAY_BUFFER, uv);
    glBufferData(
      GL_ARRAY_BUFFER, sizeof(glm::vec2) * 5, texcoords, GL_STATIC_DRAW);

    uint16_t indices_fs[] = { 0, 1, 4, 1, 2, 4, 2, 3, 4, 3, 0, 4 };
    glGenBuffers(1, &ibo_fs); // Deleted in destructor.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_fs);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(uint16_t) * 12,
                 indices_fs,
                 GL_STATIC_DRAW);

    uint16_t indices[] = { 0, 1, 2, 2, 3, 0 };
    glGenBuffers(1, &ibo); // Deleted in destructor.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 6, indices, GL_STATIC_DRAW);

    glGenFramebuffers(1, &fboColor); // Deleted in destructor.
    glBindFramebuffer(GL_FRAMEBUFFER, fboColor);

    glBindTexture(GL_TEXTURE_2D, texColor);
    glFramebufferTexture2D(
      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColor, 0);
    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
      fprintf(stderr, "FBO incomplete status: 0x%04x\n", status);
      CheckGLError();
    }

    glGenFramebuffers(1, &fboBlurry); // Deleted in destructor.
    glBindFramebuffer(GL_FRAMEBUFFER, fboBlurry);
    glBindTexture(GL_TEXTURE_2D, texBlurry);
    glFramebufferTexture2D(
      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texBlurry, 0);
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
      fprintf(stderr, "FBO incomplete status: 0x%04x\n", status);
      CheckGLError();
    }

    for (int i = 0; i < zStarColors; ++i) {
      vecColors.push_back(Color::Create(
        { { floatRandom(), 0.0f, 0.0f, 1.0f },
          { 0.0f, floatRandom(), 0.0f, 1.0f },
          { 0.0f, 0.0f, floatRandom(), 1.0f },
          { floatRandom(), floatRandom(), floatRandom(), 1.0f } }));
    }

    std::lock_guard<std::mutex> lock(mtxStars);
    const float limit_w = 3.0f;
    const float limit_h = 2.0f;
    for (int i = 0; i < zStars; ++i) {
      vecStars.push_back(
        Star::Create(RandomX(limit_w),
                     RandomY(limit_h),
                     floatRandom(-38.8f, -3.8),
                     vecColors[RandomColor(zStarColors - 1)]->texture));
    }

    // I picked the scale and frustum that look right to me, and there is
    // not a lot of logic behind the numbers.  They do show how easy it is to
    // set up MVP matrix with GLM.  Model matrix is calculated in function
    // Star::animate().
    const auto view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.8f), // eye
                                  glm::vec3(0.0f, 0.0f, 0.0f), // center
                                  glm::vec3(0.0f, 1.0f, 0.0f)  // up
    );

    const auto projection = glm::perspective(
      glm::radians(85.0f), float(width) / float(height), 1.0f, 38.8f);
    matrixVP = projection * view;
  });
  setup.wait();
}

void
Field::animate(bool faster)
{
  for (int i = 0; i < zStarColors; ++i) {
    vecColors[i]->animate(this);
  }

  for (int i = 0; i < zStars; ++i) {
    vecStars[i]->animate(this, faster);
  }

  std::lock_guard<std::mutex> lock(mtxStars);
  std::sort(
    vecStars.begin(), vecStars.end(), [](const StarRef& a, const StarRef& b) {
      return a->pos.z < b->pos.z;
    });
}

std::future<void>
Field::render()
{
  RenderTextures(GL, this, Key());
  RenderColor(GL, this, Key());
  RenderBlurry(GL, this, Key());
  return RenderAndPresent(GL, this, Key());
}

void
Field::drawBox(const Key&, bool fullscreen, GLuint cbo)
{
  // This function is only called from thread that initialized context.
  glBindBuffer(GL_ARRAY_BUFFER, uv);
  glEnableVertexAttribArray(LOCATION_VTEX);
  glVertexAttribPointer(LOCATION_VTEX, 2, GL_FLOAT, GL_FALSE, 0, 0);

  if (!fullscreen) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(LOCATION_VPOS);
    glVertexAttribPointer(LOCATION_VPOS, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
  } else {
    glBindBuffer(GL_ARRAY_BUFFER, vbo_fs);
    glEnableVertexAttribArray(LOCATION_VPOS);
    glVertexAttribPointer(LOCATION_VPOS, 3, GL_FLOAT, GL_FALSE, 0, 0);

    if (cbo != 0) {
      glBindBuffer(GL_ARRAY_BUFFER, cbo);
      glEnableVertexAttribArray(LOCATION_VCOLOR);
      glVertexAttribPointer(LOCATION_VCOLOR, 4, GL_FLOAT, GL_FALSE, 0, 0);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_fs);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, nullptr);
  }
}

void
RunStarfieldDemo(char** argv)
{
  InitializeRNG();
  bRunning = true;
  auto field = Field::Create(std::stoi(argv[1]), std::stoi(argv[2]));
  field->setup();
  auto timeLastHyperspeedJump = std::chrono::steady_clock::now();
  auto timeLastFPSReport = std::chrono::steady_clock::now();
  bool bHyperspeed = false;

  std::chrono::steady_clock::duration total_elapsed;
  int frame_count = 0;
  field->animate(false);
  while (bRunning) {
    const auto now = std::chrono::steady_clock::now();

    if (bHyperspeed) {
      if ((now - timeLastHyperspeedJump) > field->msHyperspeedInterval) {
        bHyperspeed = false;
        timeLastHyperspeedJump = now;
        fprintf(stderr, "exiting hyperspeed\n");
      }
    } else if ((now - timeLastHyperspeedJump) > field->msHyperspeedInterval) {
      bHyperspeed = true;
      timeLastHyperspeedJump = now;
      fprintf(stderr, "entering hyperspeed\n");
    }

    if ((now - timeLastFPSReport) > 5s) {
      timeLastFPSReport = now;
      const auto total = total_elapsed / 1ms;
      fprintf(stderr,
              "frames: %d total: %" PRId64 "ms average: %0.2fms\n",
              frame_count,
              total,
              total / float(frame_count));
      frame_count = 0;
      total_elapsed = 0ms;
    }

    // All drawing commands are queued on another thread, the sync future object
    // is returned by Screen::swap().  Start animating stars in current thread,
    // and once animations are done, wait for drawing to finish.
    auto sync = field->render();
    field->animate(bHyperspeed);
    sync.wait();

    auto elapsed = std::chrono::steady_clock::now() - now;
    total_elapsed = total_elapsed + elapsed;
    ++frame_count;

    if (elapsed < 16ms) {
      std::this_thread::sleep_for(16ms - elapsed);
    }
  }

  fprintf(stderr, "exiting...\n");
  fflush(stderr);
  field.reset();
}

void
ExitStarfieldDemo()
{
  bRunning = false;
}
