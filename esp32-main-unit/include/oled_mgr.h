#pragma once

#include "dht_mgr.h"
#include "pconline_mgr.h"
#include "settings_mgr.h"
#include "time_mgr.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>

class OledMgr {
public:
  OledMgr(uint8_t width, uint8_t height, TimeMgr &timemgr,
          PCOnlineMgr &pconline, DHTMgr &dhtmgr, SettingsMgr &settingsmgr);

  void init(esp_chip_info_t *info);
  void updateDisplay();

private:
  Adafruit_SSD1306 display;
  TimeMgr &timemgr;
  PCOnlineMgr &pconline;
  DHTMgr &dhtmgr;
  SettingsMgr &settingsmgr;
  String getOledText();
};
