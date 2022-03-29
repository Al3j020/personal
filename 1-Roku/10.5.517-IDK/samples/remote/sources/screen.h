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
struct ShaderProgram
{
  GLuint id;
  GLuint uMVP;   // Uniform location for MVP matrix.
  GLuint uStuff; // Uniform to vary the color on-screen.
};

struct Screen : public ctpl::thread_pool
{
  using Ref = std::shared_ptr<Screen>;
  static Ref Create();

  ShaderProgram shader;

  // Use push() to call your lambdas in the same thread that initialized
  // GLES2 context.  It returns std::future and you can use it to wait for
  // results if operation must be synchronous.  The lambda you push() must
  // take an int, because ctpl::thread_pool tells you which thread has the
  // task.  Please see screen.cpp for examples.

  // Present new frame to graphics plane.
  virtual std::future<void> swap() = 0;

  Screen();
  virtual ~Screen();
};
using ScreenRef = Screen::Ref;
#endif //#ifndef IDK_SAMPLE_GL_SCREEN_H
