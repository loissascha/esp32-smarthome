#include "lightcontrol_mgr.h"

LightcontrolMgr::LightcontrolMgr(TimeMgr &timemgr, SettingsMgr &settingsmgr,
                                 DHTMgr &dhtmgr, PCOnlineMgr &pconlinemgr)
    : timemgr(timemgr), settingsmgr(settingsmgr), dhtmgr(dhtmgr),
      pconlinemgr(pconlinemgr) {
  tischlampeStatus = false;
}

void LightcontrolMgr::setup() {
  turnOffTischlampe();
  tischlampeStatus = false;
}

void LightcontrolMgr::loop() {	
	bool daytime = timemgr.isDaytime();
	bool tischlampeManualMode = settingsmgr.tischlampeManualMode;
	bool pconline = pconlinemgr.getOnlineStatus();

	bool newStatus = false;

	// tischlampe automatic mode
	if (!tischlampeManualMode) {
		if (!daytime && pconline) {
			newStatus = true;
		}
	}
	// TODO: tischlampe manual mode implementation

	// update tischlampe (send home assistant request) if status is different
	if (newStatus != tischlampeStatus) {
		tischlampeStatus = newStatus;
		if (newStatus) {
			turnOnTischlampe();
		} else {
			turnOffTischlampe();
		}
	}
}

void LightcontrolMgr::turnOnTischlampe() {
  HTTPClient http;
  http.begin("http://192.168.0.96:8123/api/webhook/-rPs1a5lV1u48t_Pl9twvFonH");
  int code = http.POST("ok");
}

void LightcontrolMgr::turnOffTischlampe() {
  HTTPClient http;
  http.begin("http://192.168.0.96:8123/api/webhook/-oEHY9OBuaU2bJ5PHoWEvpsHp");
  int code = http.POST("ok");
}
