#pragma once

#include <Arduino.h>
#include <HTTPClient.h>

class PCOnlineMgr {
public:
  PCOnlineMgr();
  void loop();
  bool getOnlineStatus();

private:
  bool onlineStatus;
  unsigned long lastPCStatusCheckTime;
};
