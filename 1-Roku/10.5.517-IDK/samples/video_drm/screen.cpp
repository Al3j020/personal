// Copyright 2021 Roku, Inc.  All rights reserved.
#include "screen.h"
#include <cstdio>
namespace {
void
CheckGLErrorInternal(int line)
{
  switch (glGetError()) {
    case GL_NO_ERROR:
      break;
    case GL_INVALID_ENUM:
      fprintf(stderr, "unacceptable value for enum at: %d\n", line);
      break;
    case GL_INVALID_VALUE:
      fprintf(stderr, "argument out of range at: %d\n", line);
      break;
    case GL_INVALID_OPERATION:
      fprintf(stderr, "operation not allowed for current state at: %d\n", line);
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      fprintf(stderr, "framebuffer object incomplete at: %d\n", line);
      break;
    case GL_OUT_OF_MEMORY:
      fprintf(stderr, "not enough memory at: %d\n", line);
      break;
#if 0
    case GL_STACK_UNDERFLOW:
      fprintf(stderr, "internal stack underflow at: %d\n", line);
      break;
    case GL_STACK_OVERFLOW:
      fprintf(stderr, "internal stack overflow at: %d\n", line);
      break;
#endif
  }
}
} // anonymous namespace
#define CheckGLError(x)                                                        \
  {                                                                            \
    CheckGLErrorInternal(__LINE__);                                            \
  }

Screen::Screen()
{
  EGLint major, minor, nconfigs;
  pDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if (pDisplay == EGL_NO_DISPLAY) {
    fprintf(stderr, "unable to create display\n");
  } else {
    eglInitialize(pDisplay, &major, &minor);

    EGLint attributes[] = { EGL_RENDERABLE_TYPE,
                            EGL_OPENGL_ES2_BIT,
                            EGL_ALPHA_SIZE,
                            8,
                            EGL_RED_SIZE,
                            8,
                            EGL_GREEN_SIZE,
                            8,
                            EGL_BLUE_SIZE,
                            8,
                            EGL_NONE };
    EGLConfig pConfig;
    eglChooseConfig(pDisplay, attributes, &pConfig, 1, &nconfigs);

    pContext = eglCreateContext(pDisplay, pConfig, nullptr, nullptr);
    if (pContext == EGL_NO_CONTEXT) {
      fprintf(stderr, "unable to create context\n");
    } else {
      // Create default 720p framebuffer.  See gles2 sample if you need
      // different dimensions.
      pSurface = eglCreateWindowSurface(
        pDisplay, pConfig, (EGLNativeWindowType) nullptr, nullptr);
      if (pSurface == EGL_NO_SURFACE) {
        fprintf(stderr, "unable to create surface\n");
      } else {
        eglMakeCurrent(pDisplay, pSurface, pSurface, pContext);
        fillScreen();
        CheckGLError();
        fprintf(stderr, "GLES2 ready\n");
      }
    }
  }
  fflush(stderr);
}

Screen::~Screen()
{
  eglMakeCurrent(pDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
  if (pSurface != EGL_NO_SURFACE) {
    eglDestroySurface(pDisplay, pSurface);
  }
  if (pContext != EGL_NO_CONTEXT) {
    eglDestroyContext(pDisplay, pContext);
  }
  if (pDisplay != EGL_NO_DISPLAY) {
    eglTerminate(pDisplay);
  }
  fprintf(stderr, "GLES2 released\n");
  fflush(stderr);
}

void
Screen::fillScreen(float r, float g, float b, float a)
{
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT);
  eglSwapBuffers(pDisplay, pSurface);
}

void
MakeGraphicsTransparentToShowVideo()
{
  // For this sample application, we just need to clear framebuffer, so we
  // don't need to keep GLES2 around.  In fact, the context we created will
  // not compile shaders.  See GLES2 sample for something that will really
  // drive graphics.
  Screen screen;
  screen.fillScreen();
}
