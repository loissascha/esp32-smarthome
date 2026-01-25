#include "oled_mgr.h"

// OLED
#define OLED_SCL 22
#define OLED_SDA 21
#define OLED_ADDR 0x3C

OledMgr::OledMgr(uint8_t width, uint8_t height, TimeMgr &time,
                 PCOnlineMgr &pconline, DHTMgr &dhtmgr,
                 SettingsMgr &settingsmgr)
    : display(width, height, &Wire, -1), timemgr(time), pconline(pconline),
      dhtmgr(dhtmgr), settingsmgr(settingsmgr) {}

void OledMgr::init(esp_chip_info_t *info) {
  Serial.println("Initializing OLED...");
  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("SSD1306 allocation failed");
    for (;;)
      ;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  Serial.println("ESP32 Chip Info:");
  Serial.printf("Cores: %d\n", info->cores);
  Serial.printf("WiFi: %s\n",
                (info->features & CHIP_FEATURE_WIFI_BGN) ? "yes" : "no");
  Serial.printf("Bluetooth: %s\n",
                (info->features & CHIP_FEATURE_BT) ? "yes" : "no");
  Serial.printf("BLE: %s\n",
                (info->features & CHIP_FEATURE_BLE) ? "yes" : "no");

  Serial.printf("Flash size: %d MB\n", ESP.getFlashChipSize() / (1024 * 1024));

  display.println("ESP32 Chip Info:");
  display.printf("Cores: %d\n", info->cores);
  display.printf("WiFi: %s\n",
                 (info->features & CHIP_FEATURE_WIFI_BGN) ? "yes" : "no");
  display.printf("Bluetooth: %s\n",
                 (info->features & CHIP_FEATURE_BT) ? "yes" : "no");
  display.printf("BLE: %s\n",
                 (info->features & CHIP_FEATURE_BLE) ? "yes" : "no");

  display.printf("Flash size: %d MB\n", ESP.getFlashChipSize() / (1024 * 1024));

  display.display();

  Serial.println("OLED initialized");
}

String OledMgr::getOledText() {
  String out;
  if (WiFi.status() == WL_CONNECTED) {
    out += "IP: " + WiFi.localIP().toString() + "\n";
    String time = timemgr.getCurrentTime();
    out += time + "\n";
    if (settingsmgr.tischlampeManualMode) {
      out += "Licht manuell\n";
    } else {
      out += "Licht automatisch\n";
    }

    if (pconline.getOnlineStatus()) {
      out += "PC on ";
    } else {
      out += "PC off";
    }
    out += " | ";
    if (timemgr.isDaytime()) {
      out += "DAY  ";
    } else {
      out += "NIGHT";
    }

    out += "\n\n";
    String humidity = dhtmgr.getHumidityString();
    String temp = dhtmgr.getTempCelsString();
    out += "Temp:       " + temp + "\n";
    out += "Humidity:   " + humidity + "\n";
    return out;
  }
  out += "Waiting for connection";
  return out;
}

void OledMgr::updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  String t = getOledText();
  display.print(t);
  display.display();
}
