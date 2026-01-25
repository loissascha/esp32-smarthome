#include "pconline_mgr.h"

PCOnlineMgr::PCOnlineMgr() {
  onlineStatus = false;
  lastPCStatusCheckTime = 0;
}

bool PCOnlineMgr::getOnlineStatus() { return onlineStatus; }

void PCOnlineMgr::loop() {
  // check pc up every 5 seconds
  if (millis() - lastPCStatusCheckTime > 5000) {
    lastPCStatusCheckTime = millis();
    HTTPClient http;
    http.begin("http://192.168.0.215:4444/ping");
    int code = http.GET();

    if (code > 0) {
      Serial.printf("HTTP Code: %d\n", code);

      if (code == HTTP_CODE_OK) {
        String body = http.getString();
        Serial.println("Response body:");
        Serial.println(body);
        if (body == "pong") {
          onlineStatus = true;
        } else {
          onlineStatus = false;
        }
      }
    } else {
      Serial.printf("HTTP GET failed: %s\n", http.errorToString(code).c_str());
      onlineStatus = false;
    }
  }
}
