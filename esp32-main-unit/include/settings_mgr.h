#pragma once

#include <Arduino.h>

class SettingsMgr {
public:
	SettingsMgr();
	bool tischlampeManualMode;
	bool tischlampeManualStatus;
	int daytimeStartHour;
	int daytimeStartMinute;
	int daytimeEndHour;
	int daytimeEndMinute;
private:
};
