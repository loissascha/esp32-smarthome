#include "time_mgr.h"

TimeMgr::TimeMgr(SettingsMgr &settingsmgr): settingsmgr(settingsmgr) {}

void TimeMgr::setup() {
  const long gmtOffset_sec = 3600;
  const int daylightOffset_sec = 3600;

  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org",
             "time.nist.gov");

  Serial.println("Waiting for NTP time sync...");
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Time synchronized");
}

String TimeMgr::getCurrentTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "Failed to obtain time";
  }

  char buf[32];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(buf);
}

bool TimeMgr::isDaytime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return false;
  }
  int hour = timeinfo.tm_hour;
	int minute = timeinfo.tm_minute;
	// hour matches start hour -> check if minute is in range
	if (hour == settingsmgr.daytimeStartHour) {
		if (minute >= settingsmgr.daytimeStartMinute) {
			return true;
		}
		return false;
	}
	// hour matches end hour -> check if current minute is before
	if (hour == settingsmgr.daytimeEndHour) {
		if (minute <= settingsmgr.daytimeEndMinute) {
			return true;
		}
		return false;
	}
	return hour > settingsmgr.daytimeStartHour && hour < settingsmgr.daytimeEndHour;
}
