// Copyright 2021 Roku, Inc.  All rights reserved.
#include "snake.h"
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
      ExitSnakeDemo();
      break;
    default:
      break;
  }
}

int
main(int, char**)
{
  struct sigaction old_handler;
  struct sigaction new_handler;
  memset(&new_handler, 0, sizeof(new_handler));
  new_handler.sa_sigaction = handler;
  sigemptyset(&new_handler.sa_mask);
  new_handler.sa_flags = SA_SIGINFO | SA_RESTART;
  sigaction(SIGTERM, &new_handler, &old_handler);

  RunSnakeDemo();

  sigaction(SIGTERM, &old_handler, nullptr);
  fflush(stderr);
  return 0;
}
