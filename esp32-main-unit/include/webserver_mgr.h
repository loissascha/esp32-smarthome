#pragma once

#include <Arduino.h>
#include <WebServer.h>

class WebserverMgr {
public:
  WebserverMgr();
  void setup();
  void loop();

private:
  WebServer server;
  void handleRoot();
  void handleNotFound();
  void handlePostMessage();
};
