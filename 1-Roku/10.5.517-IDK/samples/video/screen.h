// Copyright 2021 Roku, Inc.  All rights reserved.
#ifndef IDK_SAMPLE_CLEAR_SCREEN_H
#define IDK_SAMPLE_CLEAR_SCREEN_H
#include <EGL/egl.h>
#include <GLES2/gl2.h>
struct Screen
{
  void fillScreen(float r = 0.0f,
                  float g = 0.0f,
                  float b = 0.0f,
                  float a = 0.0f);

  Screen();
  ~Screen();

private:
  EGLConfig pContext;
  EGLDisplay pDisplay;
  EGLSurface pSurface;
};

void
MakeGraphicsTransparentToShowVideo();
#endif //#ifndef IDK_SAMPLE_CLEAR_SCREEN_H
