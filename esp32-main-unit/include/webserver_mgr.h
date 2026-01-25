#pragma once

#include <Arduino.h>
#include <WebServer.h>
#include "dht_mgr.h"
#include "time_mgr.h"
#include "pconline_mgr.h"
#include "settings_mgr.h"
#include "lightcontrol_mgr.h"

class WebserverMgr {
public:
  WebserverMgr(DHTMgr &dhtmgr, TimeMgr &timemgr, PCOnlineMgr &pconlinemgr, SettingsMgr &settingsmgr, LightcontrolMgr &lightcontrolmgr);
  void setup();
  void loop();

private:
  WebServer server;
	DHTMgr &dhtmgr;
	TimeMgr &timemgr;
	PCOnlineMgr &pconlinemgr;
	SettingsMgr &settingsmgr;
	LightcontrolMgr &lightcontrolmgr;
  void handleRoot();
  void handleSensors();
  void handleNotFound();
  void handlePostMessage();
};
