// Copyright 2021 Roku, Inc.  All rights reserved.
#include "remote.h"
#include <chrono>
#include <cstdio>
#include <thread>
using namespace std::chrono_literals;

RemoteControlRef
RemoteControl::Create()
{
  return std::make_shared<RemoteControl>();
}

RemoteControl::RemoteControl()
  : pContext(nullptr)
{
  if (RokuInputContext_create(&pContext) == 0) {
    fprintf(stderr, "remote context created\n");
  } else {
    pContext = nullptr;
  }
}

RemoteControl::~RemoteControl()
{
  if (pContext) {
    RokuInputContext_release(pContext);
    fprintf(stderr, "remote context released\n");
  }
}

void
RemoteControl::runEventLoop()
{
  // Set this variable to true to print event type information to console.
  // Since IDK does not support gaming remotes, you will not get the motion
  // and pointer events.
  const bool printToConsole = false;

  bRunning = true;
  while (bRunning) {
    if (RokuInputContext_hasEvents(pContext) == 1) {
      RokuInputEvent* pEvent = nullptr;
      while (RokuInputContext_getEvent(pContext, &pEvent) == 1) {
        switch (RokuInputEvent_getType(pEvent)) {
          case ROKU_INPUT_EVENT_TYPE_INVALID:
            if (printToConsole) {
              fprintf(stderr, "input event type INVALID\n");
            }
            break;
          case ROKU_INPUT_EVENT_TYPE_BUTTON_RELEASED:
            if (printToConsole) {
              fprintf(stderr,
                      "input event type BUTTON RELEASED %08x\n",
                      RokuInputEvent_getButton(pEvent));
            }
            {
              std::lock_guard<std::mutex> lock(mtxButton);
              if (cbButton) {
                cbButton(RokuInputEvent_getButton(pEvent), true);
              }
            }
            break;
          case ROKU_INPUT_EVENT_TYPE_BUTTON_PRESSED:
            if (printToConsole) {
              fprintf(stderr,
                      "input event type BUTTON PRESSED %08x\n",
                      RokuInputEvent_getButton(pEvent));
            }
            {
              std::lock_guard<std::mutex> lock(mtxButton);
              if (cbButton) {
                cbButton(RokuInputEvent_getButton(pEvent), false);
              }
            }
            break;
          case ROKU_INPUT_EVENT_TYPE_POINTER:
            if (printToConsole) {
              fprintf(stderr,
                      "input event type POINTER (%d,%d) (%d,%d)\n",
                      RokuInputEvent_getPointerX(pEvent),
                      RokuInputEvent_getPointerY(pEvent),
                      RokuInputEvent_getDeltaX(pEvent),
                      RokuInputEvent_getDeltaY(pEvent));
            }
            break;
          case ROKU_INPUT_EVENT_TYPE_MOTION:
            if (printToConsole) {
              fprintf(stderr, "input event type MOTION\n");
            }
            break;
          case ROKU_INPUT_EVENT_TYPE_KEY_RELEASED:
            if (printToConsole) {
              fprintf(stderr,
                      "input event type KEY RELEASED %08x\n",
                      RokuInputEvent_getKey(pEvent));
            }
            break;
          case ROKU_INPUT_EVENT_TYPE_KEY_PRESSED:
            if (printToConsole) {
              fprintf(stderr,
                      "input event type KEY PRESSED %08x\n",
                      RokuInputEvent_getKey(pEvent));
            }
            break;
        }
        RokuInputEvent_release(pEvent);
      }
    } else {
      std::this_thread::sleep_for(16ms);
    }
  }
}

void
RemoteControl::exitEventLoop()
{
  bRunning = false;
}

void
RemoteControl::setButtonCallback(ButtonCallback callback)
{
  std::lock_guard<std::mutex> lock(mtxButton);
  cbButton = callback;
}

void
RemoteControl::clearButtonCallback()
{
  std::lock_guard<std::mutex> lock(mtxButton);
  cbButton = nullptr;
}
