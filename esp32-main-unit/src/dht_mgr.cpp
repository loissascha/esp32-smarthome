#include "dht_mgr.h"

DHTMgr::DHTMgr() : dht(DHTPIN, DHTTYPE) {
  lastCheckDht = 0;
  humidity = 0.0;
  tempcels = 0.0;
}

void DHTMgr::setup() { dht.begin(); }

void DHTMgr::loop() {
  // every 20 seconds
  if (millis() - lastCheckDht > 20000) {
    lastCheckDht = millis();
    float h = dht.readHumidity();
    float t = dht.readTemperature();     // Celsius
    float f = dht.readTemperature(true); // Fahrenheit

    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println(
          "Failed to read from DHT22 (check wiring + pull-up resistor)");
      return;
    }

    humidity = h;
    tempcels = t;

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %  |  Temp: ");
    Serial.print(t);
    Serial.print(" °C  (");
    Serial.print(f);
    Serial.println(" °F)");
  }
}

float DHTMgr::getHumidity() { return humidity; }
float DHTMgr::getTempCels() { return tempcels; }

String DHTMgr::getHumidityString() {
  float h = getHumidity();
  return String(h) + " %";
}

String DHTMgr::getTempCelsString() {
  float t = getTempCels();
  return String(t) + " C";
}
