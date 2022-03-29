// Copyright 2021 Roku, Inc.  All rights reserved.
#ifndef IDK_SAMPLE_GL_SCREEN_H
#define IDK_SAMPLE_GL_SCREEN_H
#define GLM_FORCE_CXX17
#define GLM_FORCE_EXPLICIT_CTOR
#define GLM_FORCE_INLINE
#define GLM_FORCE_PURE
#define GLM_FORCE_SIZE_T_LENGTH
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ctpl_stl.h"
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <memory>
#include <string>
extern void
CheckGLErrorInternal(const char*, int);
#define CheckGLError(x)                                                        \
  {                                                                            \
    CheckGLErrorInternal(__FILE__, __LINE__);                                  \
  }

#define LOCATION_VPOS 0
#define LOCATION_VCOLOR 1
#define LOCATION_VTEX 2
struct ShaderProgram
{
  GLuint id;
  GLuint uMVP;       // Uniform location for MVP matrix.
  GLuint uCtrlBlur;  // Enable render path for blurring.
  GLuint uCtrlMask;  // Enable render path for rendering with alpha mask.
  GLuint uCtrlFinal; // Enable render path for final compositing.
  GLuint uTexMask;   // Holds texture for alpha mask.
  GLuint uTexBlur;   // Texture for blurred frame.
  GLuint uTexColor;  // Texture for full resolution frame.
  GLuint uBufDepth;  // Depth buffer from full resolution frame.
};

struct Screen : public ctpl::thread_pool
{
  using Ref = std::shared_ptr<Screen>;
  static Ref Create(int /* width */, int /* height */);

  ShaderProgram shader;

  // Use push() to call your lambdas in the same thread that initialized
  // GLES2 context.  It returns std::future and you can use it to wait for
  // results if operation must be synchronous.  The lambda you push() must
  // take an int, because ctpl::thread_pool tells you which thread has the
  // task.  Please see screen.cpp for examples.

  // Present new frame to graphics plane.
  virtual std::future<void> swap() = 0;

  // Returns texture ID of a 32-bit texture; upload pixel data from file or
  // buffer with 32-bit pixel data.  If buffer is nullptr, allocate texture
  // memory.
  virtual GLuint getPNGTexture(const std::string& file) = 0;
  virtual GLuint createTexture(int width,
                               int height,
                               std::shared_ptr<uint8_t> buffer) = 0;

  Screen(int /* width */, int /* height */);
  virtual ~Screen();
};
using ScreenRef = Screen::Ref;
#endif //#ifndef IDK_SAMPLE_GL_SCREEN_H
