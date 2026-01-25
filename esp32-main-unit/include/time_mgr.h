#pragma once

#include <Arduino.h>
#include <time.h>

class TimeMgr {
public:
  TimeMgr();
  void setup();
  String getCurrentTime();
  bool isDaytime();
};
