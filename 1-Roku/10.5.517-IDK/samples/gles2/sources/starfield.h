// Copyright 2021 Roku, Inc.  All rights reserved.
#ifndef IDK_SAMPLE_STARFIELD_DEMO_H
#define IDK_SAMPLE_STARFIELD_DEMO_H
#include <chrono>
using namespace std::chrono_literals;
struct Starfield
{
  Starfield()
    : zStars(888)                   // How many stars on screen at once.
    , zStarColors(3)                // How many variants for the stars.
    , msHyperspeedInterval(38888ms) // How often to go fast.
    , fSpeed(0.0088f)               // Nominal star speed.
  {}

  const int zStars;
  const int zStarColors;
  const std::chrono::milliseconds msHyperspeedInterval;
  const float fSpeed;
};

void
RunStarfieldDemo(char** argv);

void
ExitStarfieldDemo();
#endif //#ifndef IDK_SAMPLE_STARFIELD_DEMO_H
