#include "dht_mgr.h"
#include "oled_mgr.h"
#include "pconline_mgr.h"
#include "secrets.h"
#include "settings_mgr.h"
#include "time_mgr.h"
#include "webserver_mgr.h"
#include "wifi_mgr.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include "lightcontrol_mgr.h"

// LEDs
#define LED_GREEN 23

SettingsMgr settingsmgr;
WifiMgr wifimgr;
TimeMgr timemgr;
PCOnlineMgr pconlinemgr;
WebserverMgr webserv;
DHTMgr dhtmgr;
LightcontrolMgr lightmgr(timemgr, settingsmgr, dhtmgr, pconlinemgr);
OledMgr oled(128, 64, timemgr, pconlinemgr, dhtmgr, settingsmgr);

// pin modes (input output)
void setPinModes() { pinMode(LED_GREEN, OUTPUT); }

void setup() {
  Serial.begin(115200);
  delay(500);

  setPinModes();
  dhtmgr.setup();

  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  oled.init(&chip_info);

  delay(1000);

  oled.updateDisplay();
  wifimgr.connectToWifi(WIFI_SSID, WIFI_PASS);

  timemgr.setup();
  webserv.setup();
	lightmgr.setup();
}

void loop() {
  pconlinemgr.loop();
  dhtmgr.loop();
  webserv.loop();
	lightmgr.loop();
  oled.updateDisplay();
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("lost wifi connection... trying to reconnect.");
    digitalWrite(LED_GREEN, LOW);
    wifimgr.connectToWifi(WIFI_SSID, WIFI_PASS);
    return;
  }
  digitalWrite(LED_GREEN, HIGH);
}
