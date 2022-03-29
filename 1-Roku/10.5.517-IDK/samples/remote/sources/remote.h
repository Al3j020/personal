// Copyright 2021 Roku, Inc.  All rights reserved.
#ifndef IDK_SAMPLE_REMOTE_H
#define IDK_SAMPLE_REMOTE_H
#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <roku/RokuInput.h>
struct RemoteControl
{
  using ButtonCallback = std::function<void(uint32_t keycode, bool released)>;
  using Ref = std::shared_ptr<RemoteControl>;
  static Ref Create();

  void runEventLoop();
  void exitEventLoop();
  void setButtonCallback(ButtonCallback);
  void clearButtonCallback();

  RemoteControl();
  ~RemoteControl();

private:
  RokuInputContext* pContext;
  std::atomic<bool> bRunning;
  std::mutex mtxButton;
  ButtonCallback cbButton;
};
using RemoteControlRef = RemoteControl::Ref;
#endif //#ifndef IDK_SAMPLE_REMOTE_H
