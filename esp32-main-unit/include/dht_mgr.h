#pragma once

#include <Arduino.h>
#include <DHT.h>

// Temperature and Humidity sensor
#define DHTPIN 4
#define DHTTYPE DHT22

class DHTMgr {
public:
  DHTMgr();
  void setup();
  void loop();
  float getHumidity();
  float getTempCels();
  String getHumidityString();
  String getTempCelsString();

private:
  DHT dht;
  unsigned long lastCheckDht;
  float humidity;
  float tempcels;
};
