#pragma once

#include <Arduino.h>
#include <time.h>
#include "settings_mgr.h"

class TimeMgr {
public:
  TimeMgr(SettingsMgr &settingsmgr);
  void setup();
  String getCurrentTime();
  bool isDaytime();
private:
	SettingsMgr &settingsmgr;
};
