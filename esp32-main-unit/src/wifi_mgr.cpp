#include "wifi_mgr.h"

WifiMgr::WifiMgr() {}

const char *WifiMgr::wlStatusToStr(wl_status_t s) {
  switch (s) {
  case WL_NO_SHIELD:
    return "WL_NO_SHIELD";
  case WL_IDLE_STATUS:
    return "WL_IDLE_STATUS";
  case WL_NO_SSID_AVAIL:
    return "WL_NO_SSID_AVAIL";
  case WL_SCAN_COMPLETED:
    return "WL_SCAN_COMPLETED";
  case WL_CONNECTED:
    return "WL_CONNECTED";
  case WL_CONNECT_FAILED:
    return "WL_CONNECT_FAILED";
  case WL_CONNECTION_LOST:
    return "WL_CONNECTION_LOST";
  case WL_DISCONNECTED:
    return "WL_DISCONNECTED";
  default:
    return "WL_UNKNOWN";
  }
}

void WifiMgr::connectToWifi(const char *ssid, const char *pass) {
  Serial.println("Scanning WiFi networks...");

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("Scan done");

  if (n == 0) {
    Serial.println("No networks found");
  } else {
    Serial.printf("%d networks found:\n", n);
    for (int i = 0; i < n; ++i) {
      Serial.printf(
          "%d: %s (%d dBm) %s\n", i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i),
          (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "open" : "secured");
      delay(10);
    }
  }

  Serial.printf("Connecting to %s (%s)\n", ssid, pass);
  // updateOledDisplay();
  WiFi.begin(ssid, pass);

  const unsigned long timeoutMs = 30000; // 30s
  unsigned long start = millis();
  wl_status_t last = WL_IDLE_STATUS;

  while (WiFi.status() != WL_CONNECTED && (millis() - start) < timeoutMs) {
    wl_status_t s = WiFi.status();
    if (s != last) {
      Serial.printf("status=%d (%s)\n", (int)s, wlStatusToStr(s));
      last = s;
    }
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected!");

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.print("RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");

    // updateOledDisplay();
  } else {
    Serial.println("Failed to connect within timeout.");
  }
}
