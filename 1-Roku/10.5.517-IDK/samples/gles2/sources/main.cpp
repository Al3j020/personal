// Copyright 2021 Roku, Inc.  All rights reserved.
#include "starfield.h"
#include <atomic>
#include <csignal>
#include <cstdio>
#include <cstring>
void
handler(int signal, siginfo_t*, void*)
{
  switch (signal) {
    case SIGTERM:
      fprintf(stderr, "received SIGTERM\n");
      ExitStarfieldDemo();
      break;
    default:
      break;
  }
}

int
main(int argc, char** argv)
{
  if (argc < 3) {
    fprintf(stderr, "usage: %s screen-width screen-height\n", argv[0]);
    fprintf(stderr, "   ex: %s 1920 1080\n", argv[0]);
  } else {
    struct sigaction old_handler;
    struct sigaction new_handler;
    memset(&new_handler, 0, sizeof(new_handler));
    new_handler.sa_sigaction = handler;
    sigemptyset(&new_handler.sa_mask);
    new_handler.sa_flags = SA_SIGINFO | SA_RESTART;
    sigaction(SIGTERM, &new_handler, &old_handler);

    RunStarfieldDemo(argv);

    sigaction(SIGTERM, &old_handler, nullptr);
  }
  fflush(stderr);
  return 0;
}
