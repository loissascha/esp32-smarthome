#pragma once

#include <Arduino.h>
#include <WiFi.h>

class WifiMgr {
public:
  WifiMgr();
  void connectToWifi(const char *ssid, const char *pass);

private:
  static const char *wlStatusToStr(wl_status_t s);
};
