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
#include "mic_volume_nb.h"

// LEDs
#define LED_GREEN 23

SettingsMgr settingsmgr;
WifiMgr wifimgr;
TimeMgr timemgr(settingsmgr);
PCOnlineMgr pconlinemgr;
DHTMgr dhtmgr;
MicVolumeNB::Config micCfg;
MicVolumeNB mic(micCfg);
LightcontrolMgr lightmgr(timemgr, settingsmgr, dhtmgr, pconlinemgr);
WebserverMgr webserv(dhtmgr, timemgr, pconlinemgr, settingsmgr, lightmgr);
OledMgr oled(128, 64, timemgr, pconlinemgr, dhtmgr, settingsmgr);

unsigned long lastPrintVolume = 0;

// pin modes (input output)
void setPinModes() { 
	pinMode(LED_GREEN, OUTPUT);
	//pinMode(MIC_PIN, INPUT);
	analogReadResolution(12);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  setPinModes();
  dhtmgr.setup();
	micCfg.pin = 35;    
	micCfg.sampleRateHz = 8000;
	micCfg.windowMs = 50;
	micCfg.smoothing = 0.25f;
	mic.begin();

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
	mic.update();
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

	if (millis() - lastPrintVolume >= 500) {
		lastPrintVolume = millis();
		Serial.print("level=");
		Serial.print(mic.level());
		Serial.print(" p2p=");
		Serial.print(mic.lastPeakToPeak());
		Serial.print(" noise=");
		Serial.println(mic.noiseFloor());
	}
}
