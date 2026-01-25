#pragma once

#include <Arduino.h>
#include <WebServer.h>
#include "dht_mgr.h"

class WebserverMgr {
public:
  WebserverMgr(DHTMgr &dhtmgr);
  void setup();
  void loop();

private:
  WebServer server;
	DHTMgr &dhtmgr;
  void handleRoot();
  void handleSensors();
  void handleNotFound();
  void handlePostMessage();
};
