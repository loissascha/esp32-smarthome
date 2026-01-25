#pragma once

#include "dht_mgr.h"
#include "pconline_mgr.h"
#include "settings_mgr.h"
#include "time_mgr.h"
#include <Arduino.h>
#include <HTTPClient.h>

class LightcontrolMgr {
public:
  LightcontrolMgr(TimeMgr &timemgr, SettingsMgr &settingsmgr, DHTMgr &dhtmgr,
                  PCOnlineMgr &pconlinemgr);
  void setup();
  void loop();

private:
  TimeMgr &timemgr;
  SettingsMgr &settingsmgr;
  DHTMgr &dhtmgr;
  PCOnlineMgr &pconlinemgr;
  void turnOnTischlampe();
  void turnOffTischlampe();
  bool tischlampeStatus;
};
